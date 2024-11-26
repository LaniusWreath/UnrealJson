// Fill out your copyright notice in the Description page of Project Settings.

#include "SFCommon/SFCWebSocketManager.h"
#include "SFCommon/SFCLog.h"
#include "WebSocketsModule.h"

// 웹소켓 연결 초기화 및 연결 함수 바인딩.
void USFCWebSocketManager::InitWebSocketManager(const FString& ServcerAddress)
{
	if (!FModuleManager::Get().IsModuleLoaded("WebSockets"))
	{
		FModuleManager::Get().LoadModule("WebSockets");
	}

	WebSocketObject = FWebSocketsModule::Get().CreateWebSocket("ws://localhost:8080");

	// 함수 바인딩.
	WebSocketObject->OnConnected().AddLambda([]() 
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "Successfully Connected");
		UE_LOG(SFClog, Log, TEXT("WebSocket connected to Pixel Streaming Server"));
	});

	WebSocketObject->OnConnectionError().AddLambda([](const FString& ErrorMessage) 
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, ErrorMessage);
		UE_LOG(SFClog, Error, TEXT("%s"), *ErrorMessage);
	});

	WebSocketObject->OnClosed().AddLambda([](int32 StatusCode, const FString& Reason, bool bWasCleen)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, bWasCleen ? FColor::Green : FColor::Red, "Connection Closed" + Reason);
	});

	WebSocketObject->OnMessage().AddLambda([](const FString& Message) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, "Received Message: " + Message);

		// 여기부터 메시지 다루는 함수 작성해서 바인딩할 것,.
	});

	WebSocketObject->OnMessageSent().AddLambda([](const FString& Message) 
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "Scent Message: " + Message);
	});

	WebSocketObject->Connect();
}

// 연결 해제.
void USFCWebSocketManager::ShutDownWebSocketManager()
{
	if (WebSocketObject->IsConnected())
	{
		WebSocketObject->Close();
	}
}

// 서버에 메시지 송신.
void USFCWebSocketManager::NotifyServerString(const FString& NotifyString)
{
	if (!WebSocketObject)
	{
		UE_LOG(SFClog, Error, TEXT("WebSocket not initialized"));
		return;
	}

	WebSocketObject->Send(NotifyString);
}


// 통신해야 하는 객체 이름 맵에 저장
void USFCWebSocketManager::RegisterObject(const FString& ObjectID, const FMessageDelegate& Callback)
{
	if (!RegisteredObjects.Contains(ObjectID))
	{
		RegisteredObjects.Add(ObjectID, Callback);
		UE_LOG(LogTemp, Log, TEXT("Object %s registered for WebSocket messages."), *ObjectID);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Object %s is already registered."), *ObjectID);
	}
}

// 등록했던 객체 이름 삭제
void USFCWebSocketManager::UnregisterObject(const FString& ObjectID)
{
	if (RegisteredObjects.Remove(ObjectID) > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("Object %s unregistered."), *ObjectID);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Object %s was not registered."), *ObjectID);
	}
}

// 메시지 받았을 때 바인딩
void USFCWebSocketManager::OnMessageReceived(const FString& Message)
{
	// Assuming messages are formatted as "ObjectID|Payload"
	FString ObjectID, Payload;
	if (Message.Split(TEXT("|"), &ObjectID, &Payload))
	{
		if (RegisteredObjects.Contains(ObjectID))
		{
			// 웹소켓 맵으로 묶어서 델리게이트 실행 가능
			RegisteredObjects[ObjectID].ExecuteIfBound(Payload);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No object registered for ID: %s"), *ObjectID);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid message format: %s"), *Message);
	}
}
