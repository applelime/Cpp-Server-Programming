#pragma once
#include <Windows.h>

class CriticalSection
{
	CRITICAL_SECTION section;

public:
	CriticalSection();
	~CriticalSection();

	void Lock();
	void Unlock();
};

class CriticalSectionLock
{
	CriticalSection* criticalSection;

public:
	CriticalSectionLock(CriticalSection& critSec);
	~CriticalSectionLock();
};