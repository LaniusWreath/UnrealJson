// Fill out your copyright notice in the Description page of Project Settings.


#include "SFCommon/SFCHttpManager.h"
#include "SFCommon/SFCLog.h"
#include "SFCommon/SFCDataManager.h"

USFCHttpManager* USFCHttpManager::CreateHttpManagerInstance(UObject* Outer)
{
	return NewObject<USFCHttpManager>(Outer, USFCHttpManager::StaticClass());
}

// ��� ���� request �Լ�
void USFCHttpManager::MakeGetRequest(const FString& Url, const bool GetResultWithFString)
{
	FHttpModule* Http = &FHttpModule::Get();
	if (!Http) return;

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
	Request->SetURL(Url);
	Request->SetVerb(TEXT("GET"));

	// ���� �Լ� ��������Ʈ ���ε�.
	if (GetResultWithFString)
	{
		Request->OnProcessRequestComplete().BindUObject(this, &USFCHttpManager::OnResponseReceivedWithString);
	}
	else
	{
		Request->OnProcessRequestComplete().BindUObject(this, &USFCHttpManager::OnResponseReceivedWithPtr);
	}

	// ��û ����.
	Request->ProcessRequest();
}

// ��� �����ؼ� request
void USFCHttpManager::MakeGetRequestWithHeader(const FString& Url, const TMap<FString, FString>& Headers, 
	const TMap<FString, FString>& Parameters, const bool GetResultWithFString)
{
	FHttpModule* Http = &FHttpModule::Get();
	if (!Http) return;

	// URL�� �Ķ���� �߰�
	FString FinalUrl = Url;
	if (Parameters.Num() > 0)
	{
		FinalUrl += TEXT("?");
		for (const TPair<FString, FString>& Param : Parameters)
		{
			FinalUrl += FString::Printf(TEXT("%s=%s&"), *Param.Key, *Param.Value);
		}
		// ������ '&' ����
		FinalUrl.RemoveFromEnd(TEXT("&"));
	}

	// ��û ����
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
	Request->SetURL(FinalUrl);
	Request->SetVerb(TEXT("GET"));

	// ��� ����
	for (const TPair<FString, FString>& Header : Headers)
	{
		Request->SetHeader(Header.Key, Header.Value);
	}

	// ���� �Լ� ��������Ʈ ���ε�
	if (GetResultWithFString)
	{
		Request->OnProcessRequestComplete().BindUObject(this, &USFCHttpManager::OnResponseReceivedWithString);
	}
	else
	{
		Request->OnProcessRequestComplete().BindUObject(this, &USFCHttpManager::OnResponseReceivedWithPtr);
	}

	// ��û ����
	Request->ProcessRequest();
}

//------------------------------------------------------------------------------------------------------------//

// Request ���� ���ε� �Լ� : ���ڿ��� ��û���� ���
void USFCHttpManager::OnResponseReceivedWithString(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	// ���� ������ Ȯ��.
	if (bWasSuccessful && Response.IsValid())
	{
		// ����� HttpHandler �ν��Ͻ��� ResultResponseString�� ����.
		FString TempResponseString = Response->GetContentAsString();

		// ��ü ��� �� data �ʵ忡 �ش��ϴ� ���� ��� ����
		ResultResponseString = USFCDataManager::ExtractDataFieldFromJsonString(TempResponseString);

		// ��������Ʈ�� ���ε��� �Լ��� �������� execute() : cpp ����
		if (OnRequestedJsonStringReady.IsBound())
		{
			OnRequestedJsonStringReady.Execute();
		}
		// ���̳��� ��������Ʈ�� ���� ���� Broadcast
		OnDynamicRequestingEvent.Broadcast();
	}
	else
	{
		UE_LOG(SFClog, Error, TEXT("%s : HTTP Request failed."), *this->GetName());
	}
}

// Request ���� ���ε� �Լ� : ��ü �����ͷ� ��û���� ��� : �������Ʈ�� ���÷��� ���� �����Ƿ�, cpp���� �߰� �۾� �䱸��.
void USFCHttpManager::OnResponseReceivedWithPtr(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful && Response.IsValid())
	{
		// ���� ������ Ȯ��
		ResultResponseString = Response->GetContentAsString();

		// JSON �Ľ�
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResultResponseString);
		if (FJsonSerializer::Deserialize(Reader, ParsedJsonData))
		{
			// �Ľ� ���� �Լ� ȣ��� ��ü �� �� ������ ���� ���°� default
			if (OnParsedJsonObjectPtrReady.IsBound())
			{
				OnParsedJsonObjectPtrReady.Execute(ParsedJsonData);
			}

			// �������Ʈ�� ���̳��� ��Ƽĳ��Ʈ ��������Ʈ
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

// Ŀ���� �Ľ� �Լ� ȣ�� �Լ�.
void USFCHttpManager::CallCustomParseFucntion(TSharedPtr<FJsonObject> OriginJsonObject)
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

// �⺻ �Ľ� �Լ�: Ŀ���� �Ľ��� ����Ͽ� ó������ �ٽ� �ۼ��� ��.
TSharedPtr<FJsonObject> USFCHttpManager::ParseRequestBody(TSharedPtr<FJsonObject> RequestBody)
{
	const TSharedPtr<FJsonObject> DataObject = RequestBody;

	if (DataObject.IsValid())
	{
		// JSON ��ü�� ���ڿ��� ���ڵ��Ͽ� JSON �������� ���/
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

// jsonObject�� map���� ��ȯ���ִ� ����ƽ �Լ� : cpp����
TMap<FString, FString> USFCHttpManager::ParseJsonObjToMap(const TSharedPtr<FJsonObject> OriginJsonObject)
{
	TMap<FString, FString> ParsedMap;
	for (auto& Elem : OriginJsonObject->Values)
	{
		// FJsonValue�� Ÿ�Կ� ���� ó��
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
			// ��ø�� JSON ��ü�� FString���� ��ȯ
			TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&ParsedMap.Add(Elem.Key));
			FJsonSerializer::Serialize(Elem.Value->AsObject().ToSharedRef(), Writer);
		}
		else if (Elem.Value->Type == EJson::Array)
		{
			// �迭�� FString���� ��ȯ
			TArray<TSharedPtr<FJsonValue>> ArrayValues = Elem.Value->AsArray();
			FString ArrayAsString;
			TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&ArrayAsString);
			FJsonSerializer::Serialize(ArrayValues, Writer);
			ParsedMap.Add(Elem.Key, ArrayAsString);
		}
		else
		{
			ParsedMap.Add(Elem.Key, TEXT("")); // ��Ÿ ��� �� ���ڿ�
		}
	}

	return ParsedMap;
}
