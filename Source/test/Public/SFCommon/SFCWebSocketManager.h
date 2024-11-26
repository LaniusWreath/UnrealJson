// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "IWebSocket.h"
#include "SFCWebSocketManager.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_DELEGATE_OneParam(FMessageDelegate, const FString&, Message);

UCLASS()
class TEST_API USFCWebSocketManager : public UObject
{
	GENERATED_BODY()

private:
	TSharedPtr<IWebSocket> WebSocketObject;
	TMap<FString, FMessageDelegate> RegisteredObjects; // Map of Object IDs to Delegates

protected:

	// Initialize socket object & binding function
	virtual void InitWebSocketManager(const FString& ServcerAddress);

	// Delete socket object
	virtual void ShutDownWebSocketManager();

	// Call function to send message
	virtual void NotifyServerString(const FString& NotifyString);

	// Add object which to communicate with socket server in map
	virtual void RegisterObject(const FString& ObjectID, const FMessageDelegate& Callback);

	// Unregister an object
	virtual void UnregisterObject(const FString& ObjectID);

	// WebSocket message handling
	virtual void OnMessageReceived(const FString& Message);

public:

};
