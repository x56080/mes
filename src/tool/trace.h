
#ifndef __TRACE_H__
#define __TRACE_H__

#include "../../include/base_type.h"

class CTrace
{
public:
    CTrace();
    ~CTrace();

public:

    INT32       Init(const char *pszDir, const char *pszApp);

    BOOL        Trace(const char *pszModule, const char *pszText);

private:
    
    ACE_Recursive_Thread_Mutex          m_Mutex;
    FILE                                *m_pFile;
    STRING                              m_strFileName;

};

#endif //__TRACE_H__
