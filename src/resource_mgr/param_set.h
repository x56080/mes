
#ifndef __PARAM_SET_H__
#define __PARAM_SET_H__

#include "../../include/interface.h"

class CParamSet : public IParamSet
{
public:
    CParamSet();
    ~CParamSet();

public:
    
    virtual UINT32      GetUint(const char *pszAttr) const;
    
    virtual STRING      GetString(const char *pszAttr) const;
    
    virtual BOOL        HasParam(const char *pszAttr) const;
    
    virtual DOUBLE      GetDouble(const char *pszAttr) const;

public:

    BOOL                AddParam(const char *pszAttr, const char *pszValue);

    void                Clear();

private:
    
    MAP<STRING, STRING> m_mapParam;

};

	
#endif //__PARAM_SET_H__