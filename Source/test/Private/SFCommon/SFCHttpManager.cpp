// Fill out your copyright notice in the Description page of Project Settings.


#include "SFCommon/SFCHttpManager.h"
#include "SFCommon/SFCLog.h"

// 메인 request 함수
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

// Request 응답 바인딩 함수 : 문자열.
void USFCHttpManager::OnResponseReceivedWithString(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	// 응답 데이터 확인.
	if (bWasSuccessful && Response.IsValid())
	{
		// 결과는 HttpHandler 인스턴스의 ResultResponseString에 저장.
		ResultResponseString = Response->GetContentAsString();
		OnRequestedJsonStringReady.Execute(true);
		OnRequestingProcessDone.Broadcast();
	}
	else
	{
		UE_LOG(SFClog, Error, TEXT("%s : HTTP Request failed."), *this->GetName());
	}
}

// Request 응답 바인딩 함수 : 객체 포인터 : 블루프린트에 리플렉션 되지 않으므로, cpp단에서 추가 작업 요구됨.
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
			// 파싱 실행 함수 호출
			OnParsedJsonObjectPtrReady.Execute(ParsedJsonData);
			OnRequestingProcessDone.Broadcast();
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

// 커스텀 파싱 함수 실행 델리게이트.
void USFCHttpManager::ExecuteCustomParseFucntion(TSharedPtr<FJsonObject> OriginJsonObject)
{
	ParsedJsonData = ParseRequestBody(OriginJsonObject);
	if (ParsedJsonData)
	{
		OnParsedJsonObjectPtrReady.Execute(ParsedJsonData);
		OnRequestingProcessDone.Broadcast();
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

		// 디버깅 출력.
		// UE_LOG(JCMlog, Log, TEXT("DataObject JSON: %s"), *JsonString);
	}
	else
	{
		UE_LOG(SFClog, Warning, TEXT("%s : DataObject is invalid"), *this->GetName());
	}
	return DataObject;
}

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
