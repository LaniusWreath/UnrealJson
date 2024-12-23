// Fill out your copyright notice in the Description page of Project Settings.


#include "SFCommon/SFCHttpManager.h"
#include "SFCommon/SFCLog.h"

// 헤더 없는 request 함수
void USFCHttpManager::MakeGetRequest(const FString& Url, const bool GetResultWithFString)
{
	FHttpModule* Http = &FHttpModule::Get();
	if (!Http) return;

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
	Request->SetURL(Url);
	Request->SetVerb(TEXT("GET"));

	// 응답 함수 델리게이트 바인딩.
	if (GetResultWithFString)
	{
		Request->OnProcessRequestComplete().BindUObject(this, &USFCHttpManager::OnResponseReceivedWithString);
	}
	else
	{
		Request->OnProcessRequestComplete().BindUObject(this, &USFCHttpManager::OnResponseReceivedWithPtr);
	}

	// 요청 실행.
	Request->ProcessRequest();
}

// 헤더 포함해서 request
void USFCHttpManager::MakeGetRequestWithHeader(const FString& Url, const TMap<FString, FString>& Headers, 
	const TMap<FString, FString>& Parameters, const bool GetResultWithFString)
{
	FHttpModule* Http = &FHttpModule::Get();
	if (!Http) return;

	// URL에 파라미터 추가
	FString FinalUrl = Url;
	if (Parameters.Num() > 0)
	{
		FinalUrl += TEXT("?");
		for (const TPair<FString, FString>& Param : Parameters)
		{
			FinalUrl += FString::Printf(TEXT("%s=%s&"), *Param.Key, *Param.Value);
		}
		// 마지막 '&' 제거
		FinalUrl.RemoveFromEnd(TEXT("&"));
	}

	// 요청 생성
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
	Request->SetURL(FinalUrl);
	Request->SetVerb(TEXT("GET"));

	// 헤더 설정
	for (const TPair<FString, FString>& Header : Headers)
	{
		Request->SetHeader(Header.Key, Header.Value);
	}

	// 응답 함수 델리게이트 바인딩
	if (GetResultWithFString)
	{
		Request->OnProcessRequestComplete().BindUObject(this, &USFCHttpManager::OnResponseReceivedWithString);
	}
	else
	{
		Request->OnProcessRequestComplete().BindUObject(this, &USFCHttpManager::OnResponseReceivedWithPtr);
	}

	// 요청 실행
	Request->ProcessRequest();
}

//------------------------------------------------------------------------------------------------------------//

// FString으로 입력받은 json에서 data 키에 해당하는 데이터만 추출하여 FString으로 리턴
FString USFCHttpManager::ExtractDataFieldFromJsonString(const FString& JsonString)
{
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);

	if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
	{
		// Check if the "data" field exists
		TSharedPtr<FJsonObject> DataObject = JsonObject->GetObjectField(TEXT("data"));

		if (DataObject.IsValid())
		{
			// Convert the "data" object back to a JSON string
			FString DataString;
			TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&DataString);

			if (FJsonSerializer::Serialize(DataObject.ToSharedRef(), JsonWriter))
			{
				return DataString;
			}
		}
	}

	return FString(); // Return an empty string if extraction fails
}

// Request 응답 바인딩 함수 : 문자열로 요청했을 경우
void USFCHttpManager::OnResponseReceivedWithString(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	// 응답 데이터 확인.
	if (bWasSuccessful && Response.IsValid())
	{
		// 결과는 HttpHandler 인스턴스의 ResultResponseString에 저장.
		FString TempResponseString = Response->GetContentAsString();

		// 전체 결과 중 data 필드에 해당하는 값만 떼어내 저장
		ResultResponseString = ExtractDataFieldFromJsonString(TempResponseString);

		// 델리게이트에 바인딩된 함수가 있을때만 execute() : cpp 전용
		if (OnRequestedJsonStringReady.IsBound())
		{
			OnRequestedJsonStringReady.Execute(true);
		}
		// 다이나믹 델리게이트는 조건 없이 Broadcast
		OnDynamicRequestingEvent.Broadcast();
	}
	else
	{
		UE_LOG(SFClog, Error, TEXT("%s : HTTP Request failed."), *this->GetName());
	}
}

// Request 응답 바인딩 함수 : 객체 포인터로 요청했을 경우 : 블루프린트에 리플렉션 되지 않으므로, cpp에서 추가 작업 요구됨.
void USFCHttpManager::OnResponseReceivedWithPtr(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful && Response.IsValid())
	{
		// 응답 데이터 확인
		ResultResponseString = Response->GetContentAsString();

		// JSON 파싱
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResultResponseString);
		if (FJsonSerializer::Deserialize(Reader, ParsedJsonData))
		{
			// 파싱 실행 함수 호출로 대체 할 수 있지만 지금 형태가 default
			if (OnParsedJsonObjectPtrReady.IsBound())
			{
				OnParsedJsonObjectPtrReady.Execute(ParsedJsonData);
			}

			// 블루프린트용 다이나믹 멀티캐스트 델리게이트
			OnDynamicRequestingEvent.Broadcast();
		}
		else
		{
			UE_LOG(SFClog, Error, TEXT("%s : Failed to parse JSON."), *this->GetName());
		}
	}
	else
	{
		UE_LOG(SFClog, Error, TEXT("%s : HTTP Request failed."), *this->GetName());
	}
}

// 커스텀 파싱 함수 호출 함수.
void USFCHttpManager::ExecuteCustomParseFucntion(TSharedPtr<FJsonObject> OriginJsonObject)
{
	ParsedJsonData = ParseRequestBody(OriginJsonObject);
	if (ParsedJsonData)
	{
		if (OnParsedJsonObjectPtrReady.IsBound())
		{
			OnParsedJsonObjectPtrReady.Execute(ParsedJsonData);
		}
		OnDynamicRequestingEvent.Broadcast();
	}
}

// 기본 파싱 함수: 커스텀 파싱은 상속하여 처음부터 다시 작성할 것.
TSharedPtr<FJsonObject> USFCHttpManager::ParseRequestBody(TSharedPtr<FJsonObject> RequestBody)
{
	const TSharedPtr<FJsonObject> DataObject = RequestBody;

	if (DataObject.IsValid())
	{
		// JSON 객체를 문자열로 인코딩하여 JSON 형식으로 출력/
		FString JsonString;
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
		FJsonSerializer::Serialize(DataObject.ToSharedRef(), Writer);
	}
	else
	{
		UE_LOG(SFClog, Warning, TEXT("%s : DataObject is invalid"), *this->GetName());
	}
	return DataObject;
}

// JsonString을 Map으로 변환해주는 스태틱 함수
TMap<FString, FString> USFCHttpManager::ParseJsonStringToMap(const FString& JsonString)
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
		UE_LOG(SFClog, Warning, TEXT("Failed Json Parsing"));
	}

	return ParsedMap;
}

// "["가", "나", "다", "라"]" 같은 전체가 문자열로 처리된 문자열 배열을 cpp형식 {"가", "나", "다","라"}로 변환해주는 스태틱 함수
TArray<FString> USFCHttpManager::ParseStringToStringArray(const FString& ArrayString)
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

// "[1,2,3,4,5]" 같은 문자열로 처리된 전체가 문자열로 처리된 숫자 배열을 cpp형식 {1,2,3,4,5}로 변환해주는 스태틱 함수
TArray<float> USFCHttpManager::ParseStringToFloatArray(const FString& ArrayString)
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

// jsonObject를 map으로 변환해주는 스태틱 함수 : cpp전용
TMap<FString, FString> USFCHttpManager::ParseJsonObjToMap(const TSharedPtr<FJsonObject> OriginJsonObject)
{
	TMap<FString, FString> ParsedMap;
	for (auto& Elem : OriginJsonObject->Values)
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

	return ParsedMap;
}
