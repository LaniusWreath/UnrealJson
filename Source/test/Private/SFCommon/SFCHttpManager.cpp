// Fill out your copyright notice in the Description page of Project Settings.


#include "SFCommon/SFCHttpManager.h"

void USFCHttpManager::MakeGetRequest(const FString& Url, const bool GetResultWithFString)
{
	FHttpModule* Http = &FHttpModule::Get();
	if (!Http) return;

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
	Request->SetURL(Url);
	Request->SetVerb(TEXT("GET"));

	// 응답 함수 델리게이트 바인딩
	if (GetResultWithFString)
	{
		Request->OnProcessRequestComplete().BindUObject(this, &USFCHttpManager::OnResponseReceived);
	}
	else
	{
		Request->OnProcessRequestComplete().BindUObject(this, &USFCHttpManager::OnResponseReceivedWithPtr);
	}

	// 요청 실행
	Request->ProcessRequest();
}

void USFCHttpManager::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{


}

void USFCHttpManager::OnResponseReceivedWithPtr(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
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
			ParsedJsonData = JsonData;
		}
	}
}
