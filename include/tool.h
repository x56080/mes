
#ifndef __TOOL_H__
#define __TOOL_H__

#include "base_type.h"

namespace TOOL
{
    BOOL DLL_EXPORT InitTrace(const char *pszDir, const char *pszApp);
    BOOL DLL_EXPORT Trace(const char *pszModule, const char *pszFormat, ...);


    //时间类
    STRING DLL_EXPORT   GetAscTime();
    STRING DLL_EXPORT   GetAscTime(time_t tTime);
    STRING DLL_EXPORT   GetAscTimeEx();
    //把"%Y-%m-%d %H:%M:%S"的字符串时间转换成time_t时间
    time_t DLL_EXPORT   String2Time(const STRING &strAscTime);

    //字符处理类
    BOOL    DLL_EXPORT  IsBacklashEnd(const char *pszDir);
    //判断是否为数字（包含+-.)
    BOOL    DLL_EXPORT  IsDigit(const STRING &strNum);

    //字符与数字转换
    DOUBLE  DLL_EXPORT  String2Double(const STRING &strNum);
    STRING  DLL_EXPORT  Int2String(INT32 iNum);

    //随机函数
    UINT32  DLL_EXPORT  Random(UINT32 uiMaxNum, BOOL bIncludeMax);
    DOUBLE  DLL_EXPORT  Randomf(DOUBLE dbMaxNum, BOOL bIncludeMax);

};

#define TRACE       TOOL::Trace

#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))

//////////////////////////////////////////////////////////////////////////
#define MTS_RES_MGR                 "ResourceMgr"
#define MTS_CONFIG_LOADER           "ConfigLoader"
#define MTS_CALC_UNIT               "CalcUnit"
#define MTS_TOOL                    "Tool"

#endif //__TOOL_H__
