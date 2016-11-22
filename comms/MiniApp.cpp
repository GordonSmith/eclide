#include "StdAfx.h"

#include "MiniApp.h"
#include <global.h>
#include <cache.h>
#include <UtilFilesystem.h>

namespace algo = boost::algorithm;

class CMiniApp : public IMiniApp, public clib::CLockableUnknown
{
protected:
    boost::filesystem::wpath m_indexPath;
    std::_tstring m_indexUri;
    boost::filesystem::wpath m_typeFolder;
    boost::filesystem::wpath m_appFolder;

    std::_tstring m_name;

public:
    BEGIN_CLOCKABLEUNKNOWN
        //IMPLEMENT_INTERFACE(IMiniApp)
    END_CUNKNOWN(clib::CLockableUnknown)

    CMiniApp(const std::_tstring & indexPath) : m_indexPath(indexPath)
    {
        DWORD bufferLen = indexPath.length() * 3 + 8;
        std::vector<TCHAR> buffer(bufferLen);
        HRESULT hr = UrlCreateFromPath(m_indexPath.c_str(), &buffer[0], &bufferLen, NULL);
        m_indexUri = &buffer[0];
        m_typeFolder = m_indexPath.parent_path();
        m_appFolder = m_typeFolder.parent_path();
        m_name = m_appFolder.leaf().c_str();
    }

    const TCHAR * GetCacheID() const
    {
        clib::recursive_mutex::scoped_lock proc(m_mutex);
        return m_indexPath.c_str();
    }

    const TCHAR * Path() {
        return m_indexPath.c_str();
    }

    const TCHAR * Name() {
        return m_name.c_str();
    }

    const TCHAR * IndexURI() {
        return m_indexUri.c_str();
    }
};
typedef StlLinked<CMiniApp> CMiniAppAdapt;
typedef std::vector<CMiniAppAdapt> CMiniAppVector;

class CGlobalMiniApp : public CMiniApp, public IGlobalMiniApp
{
public:
    CGlobalMiniApp(const std::_tstring & miniAppFolder) : CMiniApp(miniAppFolder)
    {
    }

    BEGIN_CLOCKABLEUNKNOWN
        IMPLEMENT_INTERFACE(IGlobalMiniApp)
    END_CUNKNOWN(CMiniApp)

    const TCHAR * Name() {
        return CMiniApp::Name();
    }

    const TCHAR * IndexURI() {
        return CMiniApp::IndexURI();
    }
};

class CWorkunitMiniApp : public CMiniApp, public IWorkunitMiniApp
{
public:
    CWorkunitMiniApp(const std::_tstring & miniAppFolder) : CMiniApp(miniAppFolder)
    {
    }

    BEGIN_CLOCKABLEUNKNOWN
        IMPLEMENT_INTERFACE(IWorkunitMiniApp)
    END_CUNKNOWN(CMiniApp)

    const TCHAR * Name() {
        return CMiniApp::Name();
    }

    const TCHAR * IndexURI() {
        return CMiniApp::IndexURI();
    }
};

class CECLMiniApp : public IECLMiniApp, public CMiniApp
{
protected:
public:
    CECLMiniApp(const std::_tstring & miniAppFolder) : CMiniApp(miniAppFolder)
    {
    }

    BEGIN_CLOCKABLEUNKNOWN
        IMPLEMENT_INTERFACE(IECLMiniApp)
    END_CUNKNOWN(CMiniApp)

    const TCHAR * Name() {
        return CMiniApp::Name();
    }

    const TCHAR * IndexURI() {
        return CMiniApp::IndexURI();
    }
};

bool MiniAppCacheLoaded = false;
CacheT<std::_tstring, CMiniApp> MiniAppCache;
void ClearMiniAppSingletons()
{
    MiniAppCache.Clear();
    MiniAppCacheLoaded = false;
}

CMiniApp * CreateGlobalMiniApp(const std::_tstring & miniAppFolder)
{
    return MiniAppCache.Get(new CGlobalMiniApp(miniAppFolder));
}

CMiniApp * CreateWorkunitMiniApp(const std::_tstring & miniAppFolder)
{
    return MiniAppCache.Get(new CWorkunitMiniApp(miniAppFolder));
}

CMiniApp * CreateECLMiniApp(const std::_tstring & miniAppFolder)
{
    return MiniAppCache.Get(new CECLMiniApp(miniAppFolder));
}

unsigned int FindAllMiniApps(MINIAPP_TYPE type, CMiniAppVector & results)
{
    boost::filesystem::path folder, path;
    //  Check for sibling install  ---
    GetProgramFolder(folder);
    if (folder.has_parent_path())
    {
        folder = folder.parent_path();
        if (folder.has_parent_path())
        {
            folder = folder.parent_path();
            boost::filesystem::directory_iterator end_itr;
            for (boost::filesystem::directory_iterator itr(folder); itr != end_itr; ++itr)
            {
                switch (type) {
                    case MINIAPP_TYPE_GLOBAL: {
                        boost::filesystem::path miniappPath = *itr / "global" / "index.html";
                        if (exists(miniappPath)) {
                            results.push_back(CreateGlobalMiniApp(pathToWString(miniappPath)));
                        }
                        break;
                    }
                    case MINIAPP_TYPE_WORKUNIT: {
                        boost::filesystem::path miniappPath = *itr / "workunit" / "index.html";
                        if (exists(miniappPath)) {
                            results.push_back(CreateWorkunitMiniApp(pathToWString(miniappPath)));
                        }
                        break;
                    }
                    case MINIAPP_TYPE_ECL: {
                        boost::filesystem::path miniappPath = *itr / "ecl" / "index.html";
                        if (exists(miniappPath)) {
                            results.push_back(CreateECLMiniApp(pathToWString(miniappPath)));
                        }
                        break;
                    }
                }
            }
        }
    }
    return results.size();
}

COMMS_API unsigned int GlobalMiniApps(IGlobalMiniAppVector & results) {
    CMiniAppVector miniApps;
    FindAllMiniApps(MINIAPP_TYPE_GLOBAL, miniApps);
    for (CMiniAppVector::const_iterator itr = miniApps.begin(); itr != miniApps.end(); ++itr) {
        CComQIPtr<IGlobalMiniApp> miniApp = static_cast<IMiniApp *>(itr->get());
        if (miniApp) {
            results.push_back(static_cast<IGlobalMiniApp *>(miniApp));
        }
    }
    return results.size();
}

COMMS_API unsigned int WorkunitMiniApps(IWorkunitMiniAppVector & results) {
    CMiniAppVector miniApps;
    FindAllMiniApps(MINIAPP_TYPE_WORKUNIT, miniApps);
    for (CMiniAppVector::const_iterator itr = miniApps.begin(); itr != miniApps.end(); ++itr) {
        CComQIPtr<IWorkunitMiniApp> miniApp = static_cast<IMiniApp *>(itr->get());
        if (miniApp) {
            results.push_back(static_cast<IWorkunitMiniApp *>(miniApp));
        }
    }
    return results.size();
}

COMMS_API unsigned int ECLMiniApps(IECLMiniAppVector & results) {
    CMiniAppVector miniApps;
    FindAllMiniApps(MINIAPP_TYPE_ECL, miniApps);
    for (CMiniAppVector::const_iterator itr = miniApps.begin(); itr != miniApps.end(); ++itr) {
        CComQIPtr<IECLMiniApp> miniApp = static_cast<IMiniApp *>(itr->get());
        if (miniApp) {
            results.push_back(static_cast<IECLMiniApp *>(miniApp));
        }
    }
    return results.size();
}
