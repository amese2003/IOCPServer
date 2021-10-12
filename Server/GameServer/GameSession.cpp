#include "pch.h"
#include "GameSession.h"
#include "GameSessionManager.h"


void GameSession::OnConnected()
{
	GSessionManager.Add(static_pointer_cast<GameSession>(shared_from_this()));
}

void GameSession::OnDisconnected()
{
	GSessionManager.Remove(static_pointer_cast<GameSession>(shared_from_this()));
}

int32 GameSession::OnRecvPacket(BYTE* buffer, int32 len)
{
	// Echo
	//cout << "OnRecv Len = " << len << endl;

	PacketHeader header = *((PacketHeader*)buffer);

	cout << "Packet ID : " << header.id << "Size :" << header.size << endl;

	
	//Send(sendBuffer);

	return len;
}

void GameSession::OnSend(int32 len)
{
	//cout << "OnSend Len = " << len << endl;
}