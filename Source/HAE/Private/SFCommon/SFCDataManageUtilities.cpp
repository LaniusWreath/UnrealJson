// Fill out your copyright notice in the Description page of Project Settings.


#include "SFCommon/SFCDataManageUtilities.h"
#include "SFCommon/SFCLog.h"
#include "SFCommon/SFCDataContainer.h"
#include "JsonUtilities.h"

//--------------------------------- Data Management --------------------------------------------

// ����ü �Է� �޾� ������ �����̳� �ν��Ͻ�
USFCDataContainer* USFCDataManageUtilities::InstancingDataContainer(UObject* Outer, const FGenericData& InData)
{
	USFCDataContainer* NewContainer = NewObject<USFCDataContainer>(Outer);
	if (NewContainer)
	{
		NewContainer->SetData(InData);
	}
	return NewContainer;
}

// �� ������ �����̳� �ν��Ͻ�
USFCDataContainer* USFCDataManageUtilities::CreateEmptyDataContainer(UObject* Outer)
{
	USFCDataContainer* NewContainer = NewObject<USFCDataContainer>(Outer);

	return NewContainer;
}

USFCDataContainer* USFCDataManageUtilities::UpdateDataContainer(USFCDataContainer* InContainer, const FGenericData& InData)
{
	if (!InContainer)
	{
		UE_LOG(SFClog, Warning, TEXT("UpdateDataContainer: InDataContainer is invalid"));
		return nullptr;
	}
	InContainer->SetData(InData);
	return InContainer;
}

// FString�� FGenericData��, FString ���� json ������ key�� ��Ȯ�� �¾ƾ� ��
bool USFCDataManageUtilities::StringToSFCDataStruct(const FString& InString, FGenericData& InDataStruct)
{
    if (InString.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("ConvertJsonToGenericData: Input JSON string is empty."));
        return false;
    }

    // Convert FString to FGenericData using JsonObjectStringToUStruct
    if (FJsonObjectConverter::JsonObjectStringToUStruct<FGenericData>(InString, &InDataStruct))
    {
        UE_LOG(LogTemp, Log, TEXT("Successfully converted JSON to FGenericData."));
        return true;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to convert JSON to FGenericData."));
        return false;
    }
}

// JsonString�� Map����
TMap<FString, FString> USFCDataManageUtilities::ParseJsonStringToMap(const FString& JsonString)
{
	TMap<FString, FString> ParsedMap;
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

	if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
	{
		for (auto& Elem : JsonObject->Values)
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
	}
	else
	{
		UE_LOG(SFClog, Warning, TEXT("Failed Json Parsing"));
	}

	return ParsedMap;
}


TArray<FString> USFCDataManageUtilities::ParseStringToStringArray(const FString& ArrayString)
{
	TArray<FString> StringArray;

	// 1. `[` �� `]` ����
	FString CleanString = ArrayString;
	CleanString.RemoveFromStart("[");
	CleanString.RemoveFromEnd("]");

	// 2. ��ǥ�� �������� ���ڿ��� �и�
	CleanString.ParseIntoArray(StringArray, TEXT(","), true);

	// 3. �� ����� �յ� ���� ����
	for (FString& Str : StringArray)
	{
		Str = Str.TrimStartAndEnd();
	}

	return StringArray;
}

TArray<float> USFCDataManageUtilities::ParseStringToFloatArray(const FString& ArrayString)
{
	TArray<float> FloatArray;

	// 1. `[` �� `]` ����
	FString CleanString = ArrayString;
	CleanString.RemoveFromStart("[");
	CleanString.RemoveFromEnd("]");

	// 2. ��ǥ�� �������� ���ڿ��� �и�
	TArray<FString> StringArray;
	CleanString.ParseIntoArray(StringArray, TEXT(","), true);

	// 3. �� ���ڿ� ��Ҹ� float�� ��ȯ�Ͽ� �迭�� �߰�
	for (const FString& Str : StringArray)
	{
		FloatArray.Add(FCString::Atof(*Str));
	}

	return FloatArray;
}

FString USFCDataManageUtilities::ExtractDataFieldFromJsonString(const FString& JsonString)
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

// ---------------------------------------- Debugging ---------------------------------------------

// FString���� ������ Map ����� ��� ������ �Լ�
void USFCDataManageUtilities::PrintStringMapMembers(const TMap<FString, FString>& InMap, const float Duration,
	const FColor TextColor)
{
	for (const TPair<FString, FString>& Pair : InMap)
	{
		UE_LOG(SFClog, Log, TEXT("Key : %s, Value : %s"), *Pair.Key, *Pair.Value);

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, Duration, TextColor, FString::Printf(TEXT("Key : %s, Value : %s"), *Pair.Key,
				*Pair.Value));
		}
	}
}

