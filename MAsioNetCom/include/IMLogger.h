#pragma once
#include <comdef.h>
interface __declspec(uuid("347C381D-C4AF-4660-AF94-E27F4B2502F4")) IMLogger : public IUnknown
{
	virtual void __stdcall EnableConsole(bool useConsole) = 0;
	virtual bool __stdcall SetLogFile(const char* filename) = 0;
	virtual void __stdcall CloseLogFile() = 0;
	virtual void __stdcall log(const char* str, ...) = 0;
	virtual void __stdcall Start() = 0;
	virtual void __stdcall Stop() = 0;
	virtual void __stdcall StopConsole() = 0;
	virtual void __stdcall StartConsole() = 0;
	virtual void __stdcall isNeedTime(bool isTime) = 0;
};

