// Fill out your copyright notice in the Description page of Project Settings.

#include "Edu/Public/EduGameInstance.h"

void UEduGameInstance::Init()
{
	TCHAR LogArray[] = TEXT("Hello Unreal");
	UE_LOG(LogTemp, Warning, TEXT("%s"), LogArray);

	FString LogCharString = TEXT("Hello String Unreal");
	UE_LOG(LogTemp, Warning, TEXT("%s"), *LogCharString);
	
	// FString에서 TCHAR 가져오기
	const TCHAR* LogCharPointer = *LogCharString;
	TCHAR* LogCharDataPointer = LogCharString.GetCharArray().GetData();

	// C 스타일 문자열 복사
	TCHAR LogCharStringSize[100];
	FCString::Strcpy(LogCharStringSize, LogCharString.Len(), *LogCharString);
	
	// 문자열 자르기
	if (LogCharString.Contains(TEXT("unreal"), ESearchCase::IgnoreCase))
	{
		// 시작 인덱스 검색
		int32 Index = LogCharString.Find(TEXT("unreal"), ESearchCase::IgnoreCase);
		// 중간에서부터 끝 까지 자름
		FString EndString = LogCharString.Mid(Index);

		UE_LOG(LogTemp, Warning, TEXT("%s"), *EndString);
	}

	// 문자열 조합
	FString AppendedString = FString::Printf(TEXT("Int: %d, Float : %f"), 32, 3.231412f);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *AppendedString);

}
