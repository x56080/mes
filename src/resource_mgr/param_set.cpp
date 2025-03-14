
#include "param_set.h"
#include "../../include/tool.h"

CParamSet::CParamSet()
{
}

CParamSet::~CParamSet()
{
    m_mapParam.clear();
}

BOOL CParamSet::AddParam(const char *pszAttr, const char *pszValue)
{
    if (HasParam(pszAttr))
    {
        TRACE("CParamSet::AddParam", "Param[%s] has alread exist, will update", pszAttr);
    }

    m_mapParam[pszAttr] = pszValue;

    return TRUE;
}

UINT32 CParamSet::GetUint(const char *pszAttr) const
{
    MAP<STRING, STRING>::const_iterator it = m_mapParam.find(STRING(pszAttr));
    if (it == m_mapParam.end())
    {
        TRACE("CParamSet::GetUint", "Param[%s] not exist", pszAttr);
        return RET_ERR;
    }
    
    if (!TOOL::IsDigit(it->second))
    {
        TRACE("CParamSet::GetUint", "Param[%s] value[%s] not digit", pszAttr, it->second.c_str());
        return RET_ERR;
    }

    return atoi(it->second.c_str());
}

STRING CParamSet::GetString(const char *pszAttr) const
{
    MAP<STRING, STRING>::const_iterator it = m_mapParam.find(STRING(pszAttr));
    if (it == m_mapParam.end())
    {
        TRACE("CParamSet::GetString", "Param[%s] not exist", pszAttr);
        return "";
    }

    return it->second;
}

DOUBLE CParamSet::GetDouble(const char *pszAttr) const
{
    MAP<STRING, STRING>::const_iterator it = m_mapParam.find(STRING(pszAttr));
    if (it == m_mapParam.end())
    {
        TRACE("CParamSet::GetDouble", "Param[%s] not exist", pszAttr);
        return RET_ERR;
    }
    
    if (!TOOL::IsDigit(it->second))
    {
        TRACE("CParamSet::GetDouble", "Param[%s] value[%s] not digit", pszAttr, it->second.c_str());
        return RET_ERR;
    }
    
    return TOOL::String2Double(it->second.c_str());
}

BOOL CParamSet::HasParam(const char *pszAttr) const
{
    MAP<STRING, STRING>::const_iterator it = m_mapParam.find(STRING(pszAttr));
    if (it == m_mapParam.end())
    {
        return FALSE;
    }

    return TRUE;
}

void CParamSet::Clear()
{
    m_mapParam.clear();
}

