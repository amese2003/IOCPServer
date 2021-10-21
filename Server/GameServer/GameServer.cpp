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

// 패킷 직렬화(Serialization)
void HealByValue(int64 target, int32 value)
{
	cout << target << "한테 힐" << value << "만큼 줌" << endl;
}

class Knight
{
public:
	void HealMe(int32 val)
	{
		cout << "HealMe! " << val << endl;
	}
};


int main()
{
	// Test JOB
	{
		FuncJob<void, int64, int32> job(HealByValue, 100, 10);
		job.Execute();
	}
	{
		Knight k1;
		MemberJob job2(&k1, &Knight::HealMe, 10);
		job2.Execute();
	}

	// job

	ClientPacketHandler::Init();

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

	while (true)
	{
		GRoom.FlushJob();
		this_thread::sleep_for(1s);
	}

	GThreadManager->Join();

}
