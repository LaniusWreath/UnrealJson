// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AGVDataTypes.h"
#include "AGVDataManager.generated.h"

class USFCHttpManager;
class UAGVDataContainer;
class USFCWebSocketManager;
/**
 * 
 */
UCLASS(Blueprintable)
class TEST_API UAGVDataManager : public UObject
{
	GENERATED_BODY()

public:

	//--------------------------------- DataManagement --------------------------------------------

	//
	UFUNCTION(BlueprintCallable, Category = "AGV")
	static const UAGVDataManager* GetAGVDataManager();

	// Instancing AGVDataContainer from Struct input
	UFUNCTION(BlueprintCallable, Category = "AGV")
	static UAGVDataContainer* CreateDataContainer(UObject* Outer, const FAGVData& InputData);

	UFUNCTION(BlueprintCallable, Category = "AGV")
	static UAGVDataContainer* CreateEmptyDataContainer(UObject* Outer);

	// Get AGVDataStruct from jsonObject
	static FAGVData JsonObjectToAGVStruct(const TSharedPtr<FJsonObject> OriginObject);

	static FAGVData JsonStringToAGVStruct(const FString& OriginString);

	//--------------------------------- HTTP --------------------------------------------


	UFUNCTION(BlueprintCallable, Category = "AGV")
	USFCHttpManager* GetHttpHandler();

	UFUNCTION(BlueprintCallable, Category = "AGV")
	UAGVDataContainer* UpdateContainerwithHTTPData(UAGVDataContainer* TargetContainer);

	UFUNCTION(BlueprintCallable, Category = "AGV")
	void RequestJsonObject(const FString& URL);


	//--------------------------------- WebSocket --------------------------------------------

	UFUNCTION(BlueprintCallable, Category = "AGV")
	USFCWebSocketManager* GetWebSocketHandler();

	UFUNCTION(BlueprintCallable, Category = "AGV")
	const FString& GetLastWebSocketMessage() const { return ReceivedMessage; }

	UFUNCTION(BlueprintCallable, Category = "AGV")
	UAGVDataContainer* UpdateContainerwithSocketMessage(UAGVDataContainer* TargetContainer);

	UFUNCTION(BlueprintCallable, Category = "AGV")
	void ConnectWebSocketServer(const FString& ServerAddress);

private:

	static UAGVDataManager* AGVDataManagerInstance;

	//--------------------------------- Http --------------------------------------------
	
	// Http Instance
	UPROPERTY()
	USFCHttpManager* HttpHandler;

	// For Delegate Binding Result
	TSharedPtr<FJsonObject> TempHTTPJsonObject;

	void SetHTTPJsonObject(const TSharedPtr<FJsonObject> OriginJsonObject);

	USFCHttpManager* InitializeHttpHandler();


	//--------------------------------- WebSocket --------------------------------------------

	// WebSocket Instance
	UPROPERTY()
	USFCWebSocketManager* WebSocketHandler;

	// Result Message
	UPROPERTY()
	FString ReceivedMessage;

	TSharedPtr<FJsonObject> TempWebSocketJsonObject;

	void SetWebSocketMessage(const FString& Message);

	USFCWebSocketManager* InitializeWebSocketHandler();

};
