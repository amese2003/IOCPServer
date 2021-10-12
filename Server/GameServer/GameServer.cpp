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
#include "ServerPacketHandler.h"



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
		vector<BuffData> buffs{ BuffData {100, 1.5f}, BuffData{200, 2.3f}, BuffData {300, 0.7f } };
		SendBufferRef sendBuffer = ServerPacketHandler::Make_S_TEST(1001, 100, 10, buffs);
		GSessionManager.Broadcast(sendBuffer);

		this_thread::sleep_for(250ms);
	}

	GThreadManager->Join();

}
