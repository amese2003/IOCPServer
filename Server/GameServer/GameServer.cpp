﻿#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"
#include "GameSession.h"
#include "GameSessionManager.h"


using namespace std;
#include "Service.h"
#include "Session.h"



int main()
{
	ServerServiceRef service = MakeShared<ServerService>(
		NetAddress(L"127.0.0.1", 7777),
		MakeShared<IocpCore>(),
		MakeShared<GameSession>, // TODO : SessionManager
		100);

	ASSERT_CRASH(service->Start());

	for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch([=]() 
		{
		
			while (true)
			{
				service->GetIocpCore()->Dispatch();
			}
			
		});
	}

	cout << "Client Connected!" << endl;

	char sendData[] = "Hello World";

	while (true)
	{
		SendBufferRef sendBuffer = GSendBufferManager->Open(4096);
		
		BYTE* buffer = sendBuffer->Buffer();

		((PacketHeader*)buffer)->size = (sizeof(sendData) + sizeof(PacketHeader));
		((PacketHeader*)buffer)->id = 1;
		::memcpy(&buffer[4], sendData, sizeof(sendData));
		sendBuffer->Close((sizeof(sendData) + sizeof(PacketHeader)));

		GSessionManager.Broadcast(sendBuffer);

		this_thread::sleep_for(250ms);
	}

	GThreadManager->Join();

}
