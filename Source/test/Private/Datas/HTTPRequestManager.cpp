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

    // 응답 함수 델리게이트 바인딩
    Request->OnProcessRequestComplete().BindUObject(this, &UHTTPRequestManager::OnResponseReceived);

	// 응답 처리 함수 델리게이트 바인딩
	//OnJsonDataReady.AddUObject(this, &UHTTPRequestManager::ExecuteCustomFucntion);

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

		TSharedPtr<FJsonObject> JsonData;

        // JSON 파싱
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseString);
        if (FJsonSerializer::Deserialize(Reader, JsonData)&& JsonData.IsValid())
        {
			// 파싱 실행 함수 호출
			ExecuteCustomParseFucntion(JsonData);
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

// 데이터 형식에 맞는 커스텀 파싱 함수 호출.
void UHTTPRequestManager::ExecuteCustomParseFucntion(TSharedPtr<FJsonObject> OriginJsonObject)
{
	ParsedJsonData = ParseRequestBody(OriginJsonObject);
	if (ParsedJsonData)
	{
		UE_LOG(LogTemp, Log, TEXT("HTTPRequestManager : DataParsing Complete"));
		OnParsedDataReady.Broadcast(ParsedJsonData);
	}
}

TSharedPtr<FJsonObject> UHTTPRequestManager::ParseRequestBody(TSharedPtr<FJsonObject> RequestBody)
{
	const TArray<TSharedPtr<FJsonValue>>* DataArray;
	const TArray<TSharedPtr<FJsonObject>> ResultContainer;

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
}

