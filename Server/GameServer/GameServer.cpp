#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "BufferWritter.h"


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
		
		BufferWriter bw(sendBuffer->Buffer(), 4096);
		PacketHeader* header = bw.Reserve<PacketHeader>();

		// id(uint64), 체력(uint32), 공격력(uint16)

		bw << (uint64)1001 << (uint32)100 << (uint16)10;
		bw.Write(sendData, sizeof(sendData));

		header->size = bw.WriteSize();
		header->id = 1;

		sendBuffer->Close(bw.WriteSize());

		GSessionManager.Broadcast(sendBuffer);

		this_thread::sleep_for(250ms);
	}

	GThreadManager->Join();

}
