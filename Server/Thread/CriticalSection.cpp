#include "CriticalSection.h"

CriticalSection::CriticalSection()
{
	InitializeCriticalSectionEx(&section, 0, 0);
}

CriticalSection::~CriticalSection()
{
	DeleteCriticalSection(&section);
}

void CriticalSection::Lock()
{
	EnterCriticalSection(&section);
}

void CriticalSection::Unlock()
{
	LeaveCriticalSection(&section);
}

CriticalSectionLock::CriticalSectionLock(CriticalSection& critSec)
{
	criticalSection = &critSec;

	if (nullptr != criticalSection)
	{
		criticalSection->Lock();
	}
}

CriticalSectionLock::~CriticalSectionLock()
{
	if (nullptr != criticalSection)
	{
		criticalSection->Unlock();
	}
}