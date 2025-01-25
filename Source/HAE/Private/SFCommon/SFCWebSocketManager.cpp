// Fill out your copyright notice in the Description page of Project Settings.

#include "SFCommon/SFCWebSocketManager.h"
#include "SFCommon/SFCLog.h"
#include "WebSocketsModule.h"

USFCWebSocketManager::USFCWebSocketManager()
{
	if (!FModuleManager::Get().IsModuleLoaded("WebSockets"))
	{
		FModuleManager::Get().LoadModule("WebSockets");
	}
}

// ������ �Ŵ��� �ν��Ͻ� ����
USFCWebSocketManager* USFCWebSocketManager::CreateWebSocketManagerInstance(UObject* Outer, TSubclassOf<USFCWebSocketManager> ManagerClass)
{
	// Ŭ���� ���� ���� ���, CPP��� StaticClass
	if (!ManagerClass)
	{
		return NewObject<USFCWebSocketManager>(Outer, USFCWebSocketManager::StaticClass());
	}
	// Ŭ���� ���� �� ���, BP ��� ��� Ŭ����
	return NewObject<USFCWebSocketManager>(Outer, ManagerClass);
}

// ������ ���� �ʱ�ȭ �� ���� �Լ� ���ε�.
void USFCWebSocketManager::Connect(const FString& ServcerAddress)
{
	if (WebSocket && WebSocket->IsConnected())
	{
		Disconnect();
	}

	WebSocket = FWebSocketsModule::Get().CreateWebSocket(ServcerAddress);

	// �Լ� ���ε�.
	WebSocket->OnConnected().AddUObject(this, &USFCWebSocketManager::OnConnected);
	WebSocket->OnConnectionError().AddUObject(this, &USFCWebSocketManager::OnError);
	WebSocket->OnClosed().AddUObject(this, &USFCWebSocketManager::OnClosed);
	WebSocket->OnMessage().AddUObject(this, &USFCWebSocketManager::OnMessageReceived);

	WebSocket->Connect();
}

// ���� ����.
void USFCWebSocketManager::Disconnect()
{
	if (!WebSocket)
	{
		return;
	}

	if (WebSocket->IsConnected())
	{
		WebSocket->Close();
		WebSocket.Reset();
	}
}

void USFCWebSocketManager::OnConnected()
{
	UE_LOG(SFClog, Log, TEXT("WebSocket connected."));
}

void USFCWebSocketManager::OnError(const FString& ErrorMessage)
{
	UE_LOG(SFClog, Error, TEXT("WebSocket error: %s"), *ErrorMessage);
}

void USFCWebSocketManager::OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean)
{
	UE_LOG(SFClog, Log, TEXT("WebSocket closed. Reason: %s"), *Reason);
}

// �޽��� �޾��� �� ���ε� 
void USFCWebSocketManager::OnMessageReceived(const FString& Message)
{
	UE_LOG(LogTemp, Log, TEXT("WebSocketManager: Message received: %s"), *Message);

	// ��������Ʈ�� ���ε��Ǿ� ������ �޽��� ����
	if (OnMessageReceivedDelegate.IsBound())
	{
		OnMessageReceivedDelegate.Execute(Message);
	}
	// ���̳��� ��������Ʈ�� ������ ��ε�ĳ��Ʈ
	OnMessageReceivedEvent.Broadcast(Message);
}

// ������ �޽��� �۽�.
void USFCWebSocketManager::SendMessage(const FString& Message)
{
	if (!WebSocket)
	{
		UE_LOG(SFClog, Error, TEXT("WebSocket not initialized"));
		return;
	}
	if (!WebSocket->IsConnected())
	{
		UE_LOG(SFClog, Error, TEXT("WebSocket server is not connected"));
		return;
	}

	WebSocket->Send(Message);
}