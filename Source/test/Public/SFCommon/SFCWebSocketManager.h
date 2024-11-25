// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "IWebSocket.h"
#include "SFCWebSocketManager.generated.h"

/**
 * 
 */
UCLASS()
class TEST_API USFCWebSocketManager : public UObject
{
	GENERATED_BODY()

private:

protected:
	virtual void InitWebSocketManager(const FString& ServcerAddress);
	virtual void ShutDownWebSocketManager();

	TSharedPtr<IWebSocket> WebSocketObject;

	virtual void NotifyServerString(const FString& NotifyString);
	
	virtual void ReturnReceiveMessage();

public:

};
