// Fill out your copyright notice in the Description page of Project Settings.

#include "Datas/JCMDataManager.h"
#include "Datas/JCMJsonHandler.h"
#include "Datas/CSVHandler.h"
#include "Datas/JCMDataContainer.h"
#include "Serialization/JsonWriter.h"

// json 로컬 파일 직접 읽고 처리하는 루틴
UJCMDataContainer* UJCMDataManager::InstancingDataContainerFromLocalJson(const FString& FilePath)
{
	TSharedPtr<FJsonObject> Data = LoadDataFromJSON(FilePath);
	FDataInstancePair NewChartData = InstancingDataClass(Data);
	return NewChartData.DataInstance;
}

// json FString 읽어 데이터 컨테이너 반환해주는 
UJCMDataContainer* UJCMDataManager::InstancingDataContainerFromJsonString(const FString& JsonBody)
{
	TSharedPtr<FJsonObject> Data = DeserializeJsonStringToJsonObject(JsonBody);
	FDataInstancePair NewChartData = InstancingDataClass(Data);
	return NewChartData.DataInstance;
}

// FString으로 Serialize된 Json문자열 객체로 다시 변환
TSharedPtr<FJsonObject> UJCMDataManager::DeserializeJsonStringToJsonObject(const FString& JsonString)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

	// JSON 문자열을 FJsonObject로 파싱
	if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("Successfully deserialized JSON string to FJsonObject."));
		return JsonObject;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to deserialize JSON string."));
		return nullptr;
	}
}

// 탐색기 패스로 파일 읽기, 나중에 오버로딩 하던 뭘 하던 JSON만 넘겨받는 함수 만들 것
TSharedPtr<FJsonObject> UJCMDataManager::LoadDataFromJSON(const FString& FilePath)
{
	if (!JSONHandlerInstance)
	{
		JSONHandlerInstance = NewObject<UJCMJsonHandler>(this);
	}
	if (JSONHandlerInstance)
	{
		// JSONHandler에서 읽은 JSON 포인터 그대로 가져옴, 이거 받는 함수만 따로 분리
		const TSharedPtr<FJsonObject> ParsedData = JSONHandlerInstance->GetJsonObjectData(FilePath);

		DataString = SerializeJSONToString(ParsedData);
		return ParsedData;
	}
	else
	{
		return nullptr;
	}
}

void UJCMDataManager::LoadDataFromCSV(const FString& FilePath)
{
}

void UJCMDataManager::FetchDataFromHTTP(const FString& URL)
{
}

// Return JSON String Getter()
const FString& UJCMDataManager::GetJSONStringData() const
{
	if (DataString == "")
	{
		UE_LOG(LogTemp, Warning, TEXT("DataManager.cpp : DataString is null"));
	}
	return DataString;
}

// Create Emtpy BarType Data Container Instance
UJCMDataContainerBar* UJCMDataManager::CreateEmptyShapeChartDataInstance()
{
	return NewObject<UJCMDataContainerBar>();
}

// JSON -> FString
FString UJCMDataManager::SerializeJSONToString(const TSharedPtr<FJsonObject> JSONObject)
{
	FString JsonString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);

	if (FJsonSerializer::Serialize(JSONObject.ToSharedRef(), Writer))
	{
		UE_LOG(LogTemp, Log, TEXT("DataManager: JSONToString Serialized"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("DataManager: JSONToString Serialize Failed"));
	}

	return JsonString;
}

// 데이터 입력 받아 파싱, DataClass 객체 생성 -> Chart
FDataInstancePair UJCMDataManager::InstancingDataClass(const TSharedPtr<FJsonObject> Data)
{
	if (!Data.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("DataManager : InstancingDataClass : Input Data is invalid"));
		return FDataInstancePair();
	}

	FString ChartType = Data->GetStringField(TEXT("chartType"));
	int32 ChartTypeNumber = JCMDataTypes::JCMMapChartTypes[ChartType.ToUpper()];
	EJCMChartTypes CurChartTypeEnum = JCMDataTypes::JCMMapChartTypes[ChartType];
	FString ChartTitle = Data->GetStringField(TEXT("chartTitle"));

	FDataInstancePair DataPair;

	switch (CurChartTypeEnum)
	{
	case None:
		break;
	case BAR:
	{
		// 데이터 객체 생성
		UJCMDataContainerBar* NewChartBarClass = NewObject<UJCMDataContainerBar>(this);

		FString ClassName = NewChartBarClass->GetClass()->GetName();

		// X축 라벨 이름 추출
		const TSharedPtr<FJsonObject> XAxisObject = Data->GetObjectField(TEXT("xAxis"));
		FString XName = XAxisObject->GetStringField(TEXT("key"));

		// x축 데이터 배열 추출
		TArray<FString> XLabels;
		TArray<TSharedPtr<FJsonValue>> LabelArray = XAxisObject->GetArrayField(TEXT("data"));
		for (const TSharedPtr<FJsonValue>& Value : LabelArray)
		{
			XLabels.Add(Value->AsString());
		}

		// Y축 라벨 이름 추출
		const TSharedPtr<FJsonObject> YAxisObject = Data->GetObjectField(TEXT("yAxis"));
		FString YName = YAxisObject->GetStringField(TEXT("key"));

		// Y축 데이터 배열 추출
		TArray<float> YValues;
		TArray<TSharedPtr<FJsonValue>> ValueArray = YAxisObject->GetArrayField(TEXT("data"));
		for (const TSharedPtr<FJsonValue>& Value : ValueArray)
		{
			YValues.Add(Value->AsNumber());
		}
		NewChartBarClass->SetChartData(ChartTitle, ChartType, XName, XLabels, YName, YValues);

		DataPair.ClassName = ClassName;
		DataPair.DataInstance = NewChartBarClass;

		UE_LOG(LogTemp, Log, TEXT("DataManager : DataClass Instanced"));
		return DataPair;
	}
		
	case LINE:
		break;
	case PIE:
		break;
	case XY:
		break;
	case XYZ:
		break;
	case FREE:
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("DataManager.cpp : Instancing Data Class was failed"));
	}
	return DataPair;
}

