
#include "../../include/export_dll.h"
#include "../../include/tool.h"
#include "trace.h"

namespace TOOL
{
    CTrace& GetTrace()
    {
        static CTrace s_Trace;
        return s_Trace;
    }
#define g_Trace     GetTrace()

    BOOL InitTrace(const char *pszDir, const char *pszApp)
    {
        return g_Trace.Init(pszDir, pszApp);
    }

    BOOL Trace(const char *pszModule, const char *pszFormat, ...)
    {
        char szLine[N_LINE] = {0};
        
        va_list arglist;
        va_start(arglist, pszFormat);
        VSNPRINTF(szLine, N_LINE-1, pszFormat, arglist);
        va_end(arglist);

        return g_Trace.Trace(pszModule, szLine);
    }

    STRING GetAscTime(time_t tTime)
    {
        tm tmTime;
        ACE_OS::localtime_r(&tTime, &tmTime);

        char szBuf[TMP_PATH] = {0};
        strftime(szBuf, TMP_PATH-1, "%Y-%m-%d %H:%M:%S", &tmTime);

        return (STRING)szBuf;
    }

    STRING GetAscTime()
    {
        time_t tTime = time(NULL);
        return GetAscTime(tTime);
    }

    STRING GetAscTimeEx()
    {
        timeb tmTime;
        ftime(&tmTime);

        STRING strAscTime = GetAscTime(tmTime.time);
        char szTmp[10] = {0};
        SNPRINTF(szTmp, 9, ".%u", tmTime.millitm);
        strAscTime += szTmp;
        
        return strAscTime;
    }

    time_t String2Time(const STRING &strAscTime)
    {
        const char *pSeperators = "- :.";

        char szTime[30] = {0};
        INT32 iTmp = 0;
        strncpy(szTime, strAscTime.c_str(), 29);
        char *pLast = NULL;

        tm tmTime;
        UINT32 uiIdex = 0;
        char *pToken = ACE_OS::strtok_r(szTime, pSeperators, &pLast);
        while (pToken)
        {
            iTmp = atoi(pToken);

            if (uiIdex == 0) //年
            {
                if (iTmp < 1900)
                {
                    iTmp += 2000;
                }
                iTmp -= 1900;
                tmTime.tm_year = iTmp;
            }
            else if (uiIdex == 1) //月
            {
                iTmp -= 1;
                tmTime.tm_mon = iTmp;
            }
            else if (uiIdex == 2) //日
            {
                tmTime.tm_mday = iTmp;
            }
            else if (uiIdex == 3) //时
            {
                tmTime.tm_hour = iTmp;
            }
            else if (uiIdex == 4) //分
            {
                tmTime.tm_min = iTmp;
            }
            else if (uiIdex == 5)
            {
                tmTime.tm_sec = iTmp;
            }

            if (iTmp < 0)
            {
                return RET_ERR;
            }

            uiIdex++;
            pToken = ACE_OS::strtok_r(NULL, pSeperators, &pLast);
        }

        return mktime(&tmTime);
    }

    BOOL IsBacklashEnd(const char *pszDir)
    {
        if (!pszDir || !strlen(pszDir))
        {
            return FALSE;
        }

        int nLast = strlen(pszDir) - 1;
        if (pszDir[nLast] == '\\' || pszDir[nLast] == '/')
        {
            return TRUE;
        }

        return FALSE;
    }

    BOOL IsDigit(const STRING &strNum)
    {
        const char *s_Nums = "+-.0123456789";
        if (strspn(strNum.c_str(), s_Nums) == strNum.size())
        {
            return TRUE;
        }

        return FALSE;
    }

    DOUBLE String2Double(const STRING &strNum)
    {
        return atof(strNum.c_str());
    }

    STRING Int2String(INT32 iNum)
    {
        char szTmp[MAX_PATH] = {0};
        sprintf(szTmp, "%d", iNum);
        return szTmp;
    }

    UINT32 Random(UINT32 uiMaxNum, BOOL bIncludeMax)
    {
        return (UINT32)Randomf(uiMaxNum, bIncludeMax);
    }

    DOUBLE Randomf(DOUBLE dbMaxNum, BOOL bIncludeMax)
    {
        if (bIncludeMax)
        {
            return (DOUBLE)((DOUBLE)rand() / (DOUBLE)RAND_MAX * dbMaxNum);
        }
        
        return (DOUBLE)((DOUBLE)rand() / (DOUBLE)(RAND_MAX + 1) * dbMaxNum);       
    }

};
