
#ifndef __PRODUCT_H__
#define __PRODUCT_H__

#include "base_type.h"

/*
汽车轮毂(英文名叫hub)规格和参数描述

规格--- 宽 * 直径, 单位为寸
PCD---- 螺栓孔中心节圆直径(Pitch Circle Diameter). 即轮胎螺丝所形成的圆的直径，单位为mm.
ET ---- 这个参数我们通常会用 ET (德语 E inpress T ieffe )或者 Offset 来描述. 是指车轮螺栓安装面
        到轮辋的中心线的距离. 如果螺栓安装面在轮辋的中心线靠近辐条(外)的一侧，这 ET 为正值. 
        单位为mm.
CB ---- 就是轮毂中心圆孔的直径大小.单位为mm. 是安装的定位孔，很重要.

*/

//轮毂规格
typedef struct tagSpec
{
    DOUBLE          m_dbWidth;          //宽
    DOUBLE          m_dbDiameter;       //直径
}SHubSpec;

class CSpec : public tagSpec
{
public:
    CSpec()
    {
        m_dbWidth = 0.0;
        m_dbDiameter = 0.0;
    }
    
    CSpec(DOUBLE dbWidth, DOUBLE dbDiameter)
    {
        m_dbWidth = dbWidth;
        m_dbDiameter = dbDiameter;
    }
    
    CSpec(const CSpec &right)
    {
        m_dbWidth = right.m_dbWidth;
        m_dbDiameter = right.m_dbDiameter;
    }

    ~CSpec() {}

    void    SetWidth(DOUBLE dbWidth);
    void    SetDiameter(DOUBLE dbDiameter);
    DOUBLE  GetWidth() const;
    DOUBLE  GetDiameter() const;

    bool    operator== (const CSpec &right) const;
    bool    operator!= (const CSpec &right) const;
    CSpec&  operator= (const CSpec &right);

};

//PCD
typedef struct tagPCD
{
    UINT32          m_uiNum;            //个数
    DOUBLE          m_dbDiameter;       //直径    
}SPCD;

class CPCD : public SPCD
{
public:
    CPCD()
    {
        m_uiNum = 0;
        m_dbDiameter = 0.0;
    }

    CPCD(UINT32 uiNum, DOUBLE dbDiameter)
    {
        m_uiNum = uiNum;
        m_dbDiameter = dbDiameter;
    }

    CPCD(const CPCD &right)
    {
        m_uiNum = right.m_uiNum;
        m_dbDiameter = right.m_dbDiameter;
    }

    ~CPCD() {}

    void    SetNum(UINT32 uiNum);
    void    SetDiameter(DOUBLE dbDiameter);
    UINT32  GetNum() const;
    DOUBLE  GetDiameter() const;

    bool    operator== (const CPCD &right) const;
    bool    operator!= (const CPCD &right) const;
    CPCD&   operator= (const CPCD &right);

};

//ET
typedef INT32           ET_TYPE;
//CB
typedef DOUBLE          CB_TYPE;
/*
inline bool operator== (const CB_TYPE &left, const CB_TYPE &right)
{
    if (fabs(left - right) < EPSILON)
    {
        return true;
    }

    return false;
}

inline bool operator!= (const CB_TYPE &left, const CB_TYPE &right)
{
    if (fabs(left - right) >= EPSILON)
    {
        return true;
    }

    return false;
}*/

//产品基类定义
class CProductProperty
{
public:
    CProductProperty(UINT32 uiProductID, CSpec &cSpec, ET_TYPE cET, 
                     CB_TYPE cCB,  CPCD &cPCD)
    {
        m_cSpec = cSpec;
        m_cCB = cCB;
        m_cET = cET;
        m_cPCD = cPCD;
        m_uiProductID = uiProductID;
    }
    
    CProductProperty(const CProductProperty &right)
    {
        m_cSpec = right.GetSpec();
        m_cCB = right.GetCB();
        m_cET = right.GetET();
        m_cPCD = right.GetPCD();
        m_uiProductID = right.GetProductID();
    }

    virtual ~CProductProperty() {}

public:
    CSpec       GetSpec() const;
    ET_TYPE     GetET() const;
    CB_TYPE     GetCB() const;
    CPCD        GetPCD() const;

    UINT32      GetProductivity(UINT32 uiProcessType) const;
    UINT32      GetProductID() const;

    bool        operator== (const CProductProperty &right) const;
    bool        operator!= (const CProductProperty &right) const;
    CProductProperty&   operator= (const CProductProperty &right);

public:

    void        AddProductivity(UINT32 uiProcessType, UINT32 uiValue);

private:
    CSpec       m_cSpec;
    ET_TYPE     m_cET;
    CB_TYPE     m_cCB;
    CPCD        m_cPCD;

    UINT32      m_uiProductID;
    MAP<UINT32, UINT32>         m_mapProductivity;

};


//////////////////////////////////////////////////////////////////////////
//inline实现
//////////////////////////////////////////////////////////////////////////

//CSpec实现

inline void CSpec::SetWidth(DOUBLE dbWidth)
{
    m_dbWidth = dbWidth;
}

inline void CSpec::SetDiameter(DOUBLE dbDiameter)
{
    m_dbDiameter = dbDiameter;
}

inline DOUBLE CSpec::GetWidth() const
{
    return m_dbWidth;
}

inline DOUBLE CSpec::GetDiameter() const
{
    return m_dbDiameter;
}

inline bool CSpec::operator ==(const CSpec &right) const
{
    if (fabs(m_dbWidth - right.GetWidth()) < EPSILON
        && fabs(m_dbDiameter - right.GetDiameter()) < EPSILON)
    {
        return true;
    }
    
    return false;
}

inline bool CSpec::operator !=(const CSpec &right) const
{
    if (fabs(m_dbWidth - right.GetWidth()) >= EPSILON
        || fabs(m_dbDiameter - right.GetDiameter()) >= EPSILON)
    {
        return true;
    }

    return false;
}

inline CSpec& CSpec::operator =(const CSpec &right)
{
    if (this != &right)
    {
        m_dbWidth = right.GetWidth();
        m_dbDiameter = right.GetDiameter();
    }
    
    return *this;
}

//////////////////////////////////////////////////////////////////////////

//CPCD实现

inline void CPCD::SetNum(UINT32 uiNum)
{
    m_uiNum = uiNum;
}

inline void CPCD::SetDiameter(DOUBLE dbDiameter)
{
    m_dbDiameter = dbDiameter;
}

inline UINT32 CPCD::GetNum() const
{
    return m_uiNum;
}

inline DOUBLE CPCD::GetDiameter() const
{
    return m_dbDiameter;
}

inline bool CPCD::operator ==(const CPCD &right) const
{
    if (m_uiNum == right.GetNum()
        && fabs(m_dbDiameter - right.GetDiameter()) < EPSILON)
    {
        return true;
    }
    
    return false;
}

inline bool CPCD::operator !=(const CPCD &right) const
{
    if (m_uiNum != right.GetNum()
        || fabs(m_dbDiameter - right.GetDiameter()) >= EPSILON)
    {
        return true;
    }

    return false;
}

inline CPCD& CPCD::operator =(const CPCD &right)
{
    if (this != &right)
    {
        m_uiNum = right.GetNum();
        m_dbDiameter = right.GetDiameter();
    }

    return *this;
}

//////////////////////////////////////////////////////////////////////////

inline CSpec CProductProperty::GetSpec() const
{
    return m_cSpec;
}

inline ET_TYPE CProductProperty::GetET() const
{
    return m_cET;
}

inline CB_TYPE CProductProperty::GetCB() const
{
    return m_cCB;
}

inline CPCD CProductProperty::GetPCD() const
{
    return m_cPCD;
}

inline UINT32 CProductProperty::GetProductivity(UINT32 uiProcessType) const
{
    MAP<UINT32, UINT32>::const_iterator it = m_mapProductivity.find(uiProcessType);
    if (it != m_mapProductivity.end())
    {
        return it->second;
    }

    return 0;
}

inline UINT32 CProductProperty::GetProductID() const
{
    return m_uiProductID;
}

inline bool CProductProperty::operator ==(const CProductProperty &right) const
{
    if (m_cSpec == right.GetSpec() && fabs(m_cCB - right.GetCB()) < EPSILON
        && m_cET == right.GetET() && m_cPCD == right.GetPCD())
    {
        return true;
    }
    
    return false;
}

inline bool CProductProperty::operator !=(const CProductProperty &right) const
{
    if (m_cSpec != right.GetSpec() || fabs(m_cCB - right.GetCB()) >= EPSILON
        || m_cET != right.GetET() || m_cPCD != right.GetPCD())
    {
        return true;
    }
    
    return false;
}

inline CProductProperty& CProductProperty::operator =(const CProductProperty &right)
{
    if (this != &right)
    {
        m_cSpec = right.GetSpec();
        m_cCB = right.GetCB();
        m_cET = right.GetET();
        m_cPCD = right.GetPCD();
        m_uiProductID = right.GetProductID();
    }
    
    return *this;
}

inline void CProductProperty::AddProductivity(UINT32 uiProcessType, UINT32 uiValue)
{
    m_mapProductivity[uiProcessType] = uiValue;
}

#endif //__PRODUCT_H__
