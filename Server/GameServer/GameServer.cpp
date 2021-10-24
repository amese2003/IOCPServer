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
#include "BufferWritter.h"
#include "Protocol.pb.h"


using namespace std;
#include "Service.h"
#include "Session.h"
#include "ClientPacketHandler.h"
#include <tchar.h>
#include "Job.h"
#include "Room.h"

enum
{
	WORKER_TICK = 64
};

void DoWorkerJob(ServerServiceRef& service)
{
	while (true)
	{
		LEndTickCount = ::GetTickCount64() + WORKER_TICK;

		// 네트워크 입출력 처리 -> 인게임 로직까지 (패킷 핸들러에 의해
		service->GetIocpCore()->Dispatch(100);

		// 예약된 일감 처리
		ThreadManager::DistributeReserveJobs();

		// 글로벌 큐
		ThreadManager::DoGloablQueueWork();
	}
}

int main()
{
	GRoom->DoTimer(1000, [] {cout << "Hello 1000" << endl; });
	GRoom->DoTimer(2000, [] {cout << "Hello 2000" << endl; });
	GRoom->DoTimer(3000, [] {cout << "Hello 3000" << endl; });
	ClientPacketHandler::Init();

	ServerServiceRef service = MakeShared<ServerService>(
		NetAddress(L"127.0.0.1", 7777),
		MakeShared<IocpCore>(),
		MakeShared<GameSession>, // TODO : SessionManager
		100);

	ASSERT_CRASH(service->Start());

	for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch([&service]()
			{

				while (true)
				{
					DoWorkerJob(service);
				}

			});
	}

	// main thread
	DoWorkerJob(service);


	GThreadManager->Join();

}
