#pragma once

/* ------------------------
	IOCP Object
---------------------------*/

class IocpObject
{
public:
	virtual HANDLE GetHandle() abstract;
	virtual void Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) abstract;
};


/*--------------------------
	IOCP Core
--------------------------*/

class IocpCore
{
public:
	IocpCore();
	~IocpCore();

	HANDLE	GetHandle() { return _iocpHandle; }

	bool	Register(class IocpObject* iocpObject);
	bool	Dispatch(uint32 timeoutMs = INFINITE);

public:
	HANDLE _iocpHandle;
};


// TEMP
extern IocpCore GIocpCore;

