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
#include "Protocol.pb.h"


using namespace std;
#include "Service.h"
#include "Session.h"
#include "ClientPacketHandler.h"
#include <tchar.h>
#include "Job.h"
#include "Room.h"

// 패킷 직렬화(Serialization)

class Player
{
public:
	int32 hp = 0;
	int32 attack = 0;
	Player* target = nullptr;
	vector<int32> buffs;
};

int main()
{
	// Test JOB
	//{
	//	// [일감 의뢰 내용] : 1번 유저한테 10만큼 힐 '줘'!
	//	// 행동 : Heal
	//	// 인자 : 1번 유저, 10이라는 힐량
	//	HealJob healJob;
	//	healJob._target = 1;
	//	healJob._healValue = 10;

	//	// 나-중에
	//	healJob.Execute();
	//}

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
