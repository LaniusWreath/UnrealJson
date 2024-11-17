// Fill out your copyright notice in the Description page of Project Settings.

// 블루프린트로 뽑은 뒤, 레벨에 따로 인스턴싱 해야함.
// 인스턴스마다 하나의 json url과 response json 객체를 저장함.

#include "Datas/JCMHttpHandler.h"
#include "Datas/JCMLog.h"

// Object / String 공통 Request 함수
void UJCMHttpHandler::MakeGetRequest(const FString& Url, const bool GetResultWithFString)
{
	Super::MakeGetRequest(Url, GetResultWithFString);
 //   FHttpModule* Http = &FHttpModule::Get();
 //   if (!Http) return;

 //   TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
 //   Request->SetURL(Url);
 //   Request->SetVerb(TEXT("GET"));

 //   // 응답 함수 델리게이트 바인딩
	//if (GetResultWithFString)
	//{
	//	Request->OnProcessRequestComplete().BindUObject(this, &UJCMHttpHandler::OnResponseReceived);
	//}
	//else
	//{
	//	Request->OnProcessRequestComplete().BindUObject(this, &UJCMHttpHandler::OnResponseReceivedWithPtr);
	//}

 //   // 요청 실행
 //   Request->ProcessRequest();
}

// Object Response 함수
void UJCMHttpHandler::OnResponseReceivedWithPtr(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful && Response.IsValid())
	{
		// 응답 데이터 확인
		ResultResponseString = Response->GetContentAsString();
		//UE_LOG(JCMlog, Log, TEXT("Response: %s"), *ResultResponseString);

		TSharedPtr<FJsonObject> JsonData;

		// JSON 파싱
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResultResponseString);
		if (FJsonSerializer::Deserialize(Reader, JsonData) && JsonData.IsValid())
		{
			// 파싱 실행 함수 호출
			ExecuteCustomParseFucntion(JsonData);
		}
		else
		{
			UE_LOG(JCMlog, Error, TEXT("%s : Failed to parse JSON."), *this->GetName());
		}
	}
	else
	{
		UE_LOG(JCMlog, Error, TEXT("%s : HTTP Request failed."), *this->GetName());
	}
}

// String Response 함수
void UJCMHttpHandler::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	// 응답 데이터 확인
	if (bWasSuccessful && Response.IsValid())
	{
		// 결과는 HttpHandler 인스턴스의 ResultResponseString에 저장
		ResultResponseString = Response->GetContentAsString();
		//UE_LOG(JCMlog, Log, TEXT("Response: %s"), *ResultResponseString);
		OnRequestedJsonStringReady.Execute(true);
		OnRequestingProcessDone.Broadcast();
	}
	else
	{
		UE_LOG(JCMlog, Error, TEXT("%s : HTTP Request failed."), *this->GetName());
	}
}

// 데이터 형식에 맞는 커스텀 파싱 함수 호출. (Object Response 전용)
void UJCMHttpHandler::ExecuteCustomParseFucntion(TSharedPtr<FJsonObject> OriginJsonObject)
{
	ParsedJsonData = ParseRequestBody(OriginJsonObject);
	if (ParsedJsonData)
	{
		//UE_LOG(JCMlog, Log, TEXT("%s : DataParsing Complete"), *this->GetName());
		OnParsedJsonObjectPtrReady.Execute(ParsedJsonData);
		OnRequestingProcessDone.Broadcast();
	}
}

// 실제 파싱 함수
TSharedPtr<FJsonObject> UJCMHttpHandler::ParseRequestBody(TSharedPtr<FJsonObject> RequestBody)
{
	const TSharedPtr<FJsonObject> DataObject = RequestBody->GetObjectField(TEXT("data"));

	if (DataObject.IsValid())
	{
		// JSON 객체를 문자열로 인코딩하여 JSON 형식으로 출력
		FString JsonString;
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
		FJsonSerializer::Serialize(DataObject.ToSharedRef(), Writer);

		// 디버깅 출력
		//UE_LOG(JCMlog, Log, TEXT("DataObject JSON: %s"), *JsonString);
	}
	else
	{
		UE_LOG(JCMlog, Warning, TEXT("%s : DataObject is invalid"), *this->GetName());
	}

	return DataObject;
}

// JsonString을 Map으로 반환하는 함수
TMap<FString, FString> UJCMHttpHandler::ParseJsonStringToMap(const FString& JsonString)
{
	TMap<FString, FString> ParsedMap;
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

	if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
	{
		for (auto& Elem : JsonObject->Values)
		{
			// FJsonValue의 타입에 따라 처리
			if (Elem.Value->Type == EJson::String)
			{
				ParsedMap.Add(Elem.Key, Elem.Value->AsString());
			}
			else if (Elem.Value->Type == EJson::Number)
			{
				ParsedMap.Add(Elem.Key, FString::SanitizeFloat(Elem.Value->AsNumber()));
			}
			else if (Elem.Value->Type == EJson::Object)
			{
				// 중첩된 JSON 객체는 FString으로 변환
				TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&ParsedMap.Add(Elem.Key));
				FJsonSerializer::Serialize(Elem.Value->AsObject().ToSharedRef(), Writer);
			}
			else if (Elem.Value->Type == EJson::Array)
			{
				// 배열은 FString으로 변환
				TArray<TSharedPtr<FJsonValue>> ArrayValues = Elem.Value->AsArray();
				FString ArrayAsString;
				TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&ArrayAsString);
				FJsonSerializer::Serialize(ArrayValues, Writer);
				ParsedMap.Add(Elem.Key, ArrayAsString);
			}
			else
			{
				ParsedMap.Add(Elem.Key, TEXT("")); // 기타 경우 빈 문자열
			}
		}
	}
	else
	{
		UE_LOG(JCMlog, Warning, TEXT("Failed Json Parsing"));
	}

	return ParsedMap;
}

// "[1,2,3,4,5]" 형태 문자열 배열로 추출
TArray<float> UJCMHttpHandler::ParseStringToFloatArray(const FString& ArrayString)
{
	TArray<float> FloatArray;

	// 1. `[` 및 `]` 제거
	FString CleanString = ArrayString;
	CleanString.RemoveFromStart("[");
	CleanString.RemoveFromEnd("]");

	// 2. 쉼표를 기준으로 문자열을 분리
	TArray<FString> StringArray;
	CleanString.ParseIntoArray(StringArray, TEXT(","), true);

	// 3. 각 문자열 요소를 float로 변환하여 배열에 추가
	for (const FString& Str : StringArray)
	{
		FloatArray.Add(FCString::Atof(*Str));
	}

	return FloatArray;
}

// "[a,b,c,d,e]" 형태 문자열 배열로 추출
TArray<FString> UJCMHttpHandler::ParseStringToStringArray(const FString& ArrayString)
{
	TArray<FString> StringArray;

	// 1. `[` 및 `]` 제거
	FString CleanString = ArrayString;
	CleanString.RemoveFromStart("[");
	CleanString.RemoveFromEnd("]");

	// 2. 쉼표를 기준으로 문자열을 분리
	CleanString.ParseIntoArray(StringArray, TEXT(","), true);

	// 3. 각 요소의 앞뒤 공백 제거
	for (FString& Str : StringArray)
	{
		Str = Str.TrimStartAndEnd();
	}

	return StringArray;
}

// 공공데이터 url 파싱 함수
/*
TSharedPtr<FJsonObject> UJCMHttpHandler::ParseRequestBody(TSharedPtr<FJsonObject> RequestBody)
{
	const TArray<TSharedPtr<FJsonValue>>* DataArray;

	// data [] 값 가져오기
	if (!RequestBody->TryGetArrayField(TEXT("data"), DataArray))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get 'data' array"));
		return nullptr;
	}

	// 최종 출력 JSON 객체 생성
	TSharedPtr<FJsonObject> OutputJsonObject = MakeShareable(new FJsonObject);

	for (int i = 0; i < DataArray->Num(); i++)
	{
		const TSharedPtr<FJsonObject> CurrentDataObject = (*DataArray)[i]->AsObject();

		// 키와 값을 저장할 배열 생성
		TArray<FString> Regions;
		TArray<float> Values;
		FString ChartTitle;
		FString ChartType = "bar";

		// 키-값 쌍 순회하기
		for (const auto& Pair : CurrentDataObject->Values)
		{
			FString Key = Pair.Key;

			// "기준일"은 제외
			if (Key.Equals(TEXT("기준일")))
			{
				ChartTitle = Pair.Value->AsString();
				continue;
			}

			// 키 추가
			Regions.Add(Key);

			// 값 추가
			float Value = Pair.Value->AsNumber();
			Values.Add(Value);
		}

		// 차트 타입, 타이틀 초기화
		OutputJsonObject->SetStringField(TEXT("chartTitle"), ChartTitle);
		OutputJsonObject->SetStringField(TEXT("chartType"), ChartType);

		// "xAxis" 객체 생성
		TSharedPtr<FJsonObject> XAxisObject = MakeShareable(new FJsonObject);
		XAxisObject->SetStringField(TEXT("key"), TEXT("region"));

		// 지역 데이터를 JSON 배열로 변환
		TArray<TSharedPtr<FJsonValue>> RegionJsonArray;
		for (const FString& Region : Regions)
		{
			RegionJsonArray.Add(MakeShareable(new FJsonValueString(Region)));
		}
		XAxisObject->SetArrayField(TEXT("data"), RegionJsonArray);

		// "yAxis" 객체 생성
		TSharedPtr<FJsonObject> YAxisObject = MakeShareable(new FJsonObject);
		YAxisObject->SetStringField(TEXT("key"), TEXT("amount"));
		YAxisObject->SetStringField(TEXT("unit"), TEXT("car"));

		// 값 데이터를 JSON 배열로 변환
		TArray<TSharedPtr<FJsonValue>> ValueJsonArray;
		for (float Value : Values)
		{
			ValueJsonArray.Add(MakeShareable(new FJsonValueNumber(Value)));
		}
		YAxisObject->SetArrayField(TEXT("data"), ValueJsonArray);


		// 최종 JSON 객체에 "xAxis" 및 "yAxis" 추가
		OutputJsonObject->SetObjectField(TEXT("xAxis"), XAxisObject);
		OutputJsonObject->SetObjectField(TEXT("yAxis"), YAxisObject);
	}

	return OutputJsonObject;
}*/