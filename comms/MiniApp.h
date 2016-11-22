#pragma once

#include "comms.h"
#include <global.h>
#include <RecursiveMutex.h>

enum MINIAPP_TYPE
{
    MINIAPP_TYPE_UNKNOWN = 0,
    MINIAPP_TYPE_GLOBAL,
    MINIAPP_TYPE_WORKUNIT,
    MINIAPP_TYPE_ECL,
    MINIAPP_TYPE_LAST
};


__interface __declspec(uuid("7C7A8C5D-E1B0-45DB-BDA8-99A2431E09B0")) IMiniApp : public clib::ILockableUnknown
{
    const TCHAR * Name();
    const TCHAR * IndexURI();
};

__interface __declspec(uuid("7C7A8C5D-E1B0-45DB-BDA8-99A2431E09BF")) IGlobalMiniApp : public IMiniApp
{
};
typedef StlLinked<IGlobalMiniApp> IGlobalMiniAppAdapt;
typedef std::vector<IGlobalMiniAppAdapt> IGlobalMiniAppVector;


__interface __declspec(uuid("9502EAB8-C57D-4FB8-8606-D1CA5594C225")) IWorkunitMiniApp : public IMiniApp
{
};
typedef StlLinked<IWorkunitMiniApp> IWorkunitMiniAppAdapt;
typedef std::vector<IWorkunitMiniAppAdapt> IWorkunitMiniAppVector;


__interface __declspec(uuid("978039D1-2019-4175-89EF-C1C9CB13ECB5")) IECLMiniApp : public IMiniApp
{
};
typedef StlLinked<IECLMiniApp> IECLMiniAppAdapt;
typedef std::vector<IECLMiniAppAdapt> IECLMiniAppVector;



COMMS_API void ClearMiniAppSingletons();
COMMS_API unsigned int GlobalMiniApps(IGlobalMiniAppVector & results);
COMMS_API unsigned int WorkunitMiniApps(IWorkunitMiniAppVector & results);
COMMS_API unsigned int ECLMiniApps(IECLMiniAppVector & results);
