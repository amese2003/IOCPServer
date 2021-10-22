#pragma once
#include "Job.h"
#include "JobSerializer.h"

class Room : public JobSerializer
{
public:
	// �̱� ������ ȯ���� ����
	void Enter(PlayerRef player);
	void Leave(PlayerRef player);
	void Broadcast(SendBufferRef sendBuffer);

public:
	// ��Ƽ������ ȯ�濡���� �ϰ����� ����
	virtual void FlushJob() override;


private:
	map<uint64, PlayerRef> _players;
	//JobQueue _jobs;
};

extern shared_ptr<Room> GRoom;
