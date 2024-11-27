// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "IWebSocket.h"
#include "SFCWebSocketManager.generated.h"

/**
 * 
 */

DECLARE_DELEGATE_OneParam(FOnMessageReceivedDelegate, const FString&);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMessageReceivedDynamicDelegate);

UCLASS(BlueprintAble)
class TEST_API USFCWebSocketManager : public UObject
{
	GENERATED_BODY()

private:
	TSharedPtr<IWebSocket> WebSocket;
	FOnMessageReceivedDelegate OnMessageReceivedDelegate; // Map of Object IDs to Delegates

protected:

	// Initialize socket object & binding function
	virtual void Connect(const FString& ServcerAddress);

	// Delete socket object
	virtual void Disconnect();

	// Call function to send message
	virtual void SendMessage(const FString& NotifyString);
	

	// Each binding functions : can be overrided
	
	// Connection event binding
	virtual void OnConnected();

	// Closing event binding
	virtual void OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean);

	// WebSocket message handling
	virtual void OnMessageReceived(const FString& Message);

	virtual void OnError(const FString& ErrorMessage);

public:
	USFCWebSocketManager();

	UPROPERTY(BlueprintAssignable)
	FOnMessageReceivedDynamicDelegate OnMessageReceivedEvent;
};
