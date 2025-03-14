
#include "../../include/export_dll.h"
#include "../../include/tool.h"
#include "trace.h"

CTrace::CTrace()
{
    m_pFile = NULL;
}

CTrace::~CTrace()
{
    if (NULL != m_pFile)
    {
        fclose(m_pFile);
        m_pFile = NULL;
    }
}

INT32 CTrace::Init(const char *pszDir, const char *pszApp)
{
    if (!pszDir || !pszApp)
    {
        return RET_ERR;
    }
    mkdir(pszDir);

    m_strFileName = pszDir;
    if (!TOOL::IsBacklashEnd(pszDir))
    {
        m_strFileName += ACE_DIRECTORY_SEPARATOR_STR;
    }

    m_strFileName += pszApp;
    m_strFileName += "_trace.txt";

    m_pFile = fopen(m_strFileName.c_str(), "a+");

    return (m_pFile ? RET_SUC : RET_ERR);
}

BOOL CTrace::Trace(const char *pszModule, const char *pszText)
{
    if (!m_pFile)
    {
        return FALSE;
    }

    ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, ace_mon, m_Mutex, FALSE)
    {
        //写行起始时间
        fprintf(m_pFile, "\n%s (%s): ", TOOL::GetAscTimeEx().c_str(), pszModule);

        //写内容
        fprintf(m_pFile, "%s", pszText);

        //刷文件
        fflush(m_pFile);
    }

    return TRUE;
}

