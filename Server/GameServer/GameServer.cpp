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
#include "ServerPacketHandler.h"
#include <tchar.h>

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
	ServerPacketHandler::Init();

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

	//char sendData[1000] = "가"; // CP949 = KS-X-1001 (한글 2바이트) + KS-X-1003 (로마 1바이트)
	//char sendData[1000] = u8"가"; // UTF-8 = UNICODE (한글 3바이트, 로마 1바이트)
	//WCHAR sendData[1000] = L"가"; // UTF-16 = UNICODE (한글/로마 2바이트)
	//TCHAR sendData[1000] = _T("가"); 

	while (true)
	{
		Protocol::S_TEST pkt;
		pkt.set_id(1000);
		pkt.set_hp(100);
		pkt.set_attack(10);

		{
			Protocol::BuffData* data = pkt.add_buffs();
			data->set_buffid(100);
			data->set_remaintime(1.2f);
			data->add_victims(4000);
		}

		{
			Protocol::BuffData* data = pkt.add_buffs();
			data->set_buffid(200);
			data->set_remaintime(2.5f);
			data->add_victims(1000);
			data->add_victims(2000);
		}

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		GSessionManager.Broadcast(sendBuffer);

		this_thread::sleep_for(250ms);
	}

	GThreadManager->Join();

}
