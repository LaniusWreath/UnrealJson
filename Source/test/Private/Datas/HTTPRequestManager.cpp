// Fill out your copyright notice in the Description page of Project Settings.

// 블루프린트로 뽑은 뒤, 레벨에 따로 인스턴싱 해야함.
// 인스턴스마다 하나의 json url과 response json 객체를 저장함.

#include "Datas/HTTPRequestManager.h"

void UHTTPRequestManager::MakeGetRequest(const FString& Url)
{
    FHttpModule* Http = &FHttpModule::Get();
    if (!Http) return;

    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
    Request->SetURL(Url);
    Request->SetVerb(TEXT("GET"));

    // 기본 헤더 설정 (주석 해제)
    /*Request->SetHeader(TEXT("User-Agent"), TEXT("UnrealEngine/5.4"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Request->SetHeader(TEXT("Accept"), TEXT("application/json"));*/

    // 인증이 필요한 경우 추가 예시 (주석 해제)
    //Request->SetHeader(TEXT("Authorization"), TEXT("Bearer YOUR_ACCESS_TOKEN"));

    // 응답 처리 함수 델리게이트 바인딩
    Request->OnProcessRequestComplete().BindUObject(this, &UHTTPRequestManager::OnResponseReceived);

    // 요청 실행
    Request->ProcessRequest();
}

void UHTTPRequestManager::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (bWasSuccessful && Response.IsValid())
    {
        // 응답 데이터 확인
        FString ResponseString = Response->GetContentAsString();
        UE_LOG(LogTemp, Log, TEXT("Response: %s"), *ResponseString);

        // JSON 파싱
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseString);

        if (FJsonSerializer::Deserialize(Reader, JsonData)&& JsonData.IsValid())
        {
            // 대리자 호출
            OnJsonDataReady.ExecuteIfBound(JsonData);

            // FString으로 저장 (에디터에서 따로 결과 호출 가능하게끔)
            TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&ResultResponseString);
            UE_LOG(LogTemp, Log, TEXT("HTTPRequestManager : Json Response Saved"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("HTTP Request failed."));
    }
}
