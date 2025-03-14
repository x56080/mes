
#include "../../include/export_dll.h"
#include "resouce_mgr.h"

IResouce* CreateResouce(const char *pszConfigFile)
{
    CResouceMgr *pResouceMgr = new CResouceMgr();
    if (!pResouceMgr->Init(pszConfigFile))
    {
        delete pResouceMgr, pResouceMgr = NULL;
    }

    return pResouceMgr;
}

BOOL ReleaseResouce(IResouce *pResouce)
{
    if (pResouce)
    {
        delete pResouce, pResouce = NULL;
    }

    return TRUE;
}

