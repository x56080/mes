
#include "../../include/export_dll.h"
#include "../../include/product.h"


//CSpec实现
CSpec::CSpec()
{
    m_dbWidth = 0.0;
    m_dbDiameter = 0.0;
}

CSpec::CSpec(DOUBLE dbWidth, DOUBLE dbDiameter)
{
    m_dbWidth = dbWidth;
    m_dbDiameter = dbDiameter;
}

CSpec::CSpec(const CSpec &right)
{
    m_dbWidth = right.m_dbWidth;
    m_dbDiameter = right.m_dbDiameter;
}

CSpec::~CSpec()
{
}

void CSpec::SetWidth(DOUBLE dbWidth)
{
    m_dbWidth = dbWidth;
}

void CSpec::SetDiameter(DOUBLE dbDiameter)
{
    m_dbDiameter = dbDiameter;
}

DOUBLE CSpec::GetWidth() const
{
    return m_dbWidth;
}

DOUBLE CSpec::GetDiameter() const
{
    return m_dbDiameter;
}

bool CSpec::operator ==(const CSpec &right) const
{
    if (fabs(m_dbWidth - right.GetWidth()) < EPSILON
        && fabs(m_dbDiameter - right.GetDiameter()) < EPSILON)
    {
        return true;
    }
    
    return false;
}

bool CSpec::operator !=(const CSpec &right) const
{
    if (fabs(m_dbWidth - right.GetWidth()) >= EPSILON
        || fabs(m_dbDiameter - right.GetDiameter()) >= EPSILON)
    {
        return true;
    }

    return false;
}

CSpec& CSpec::operator =(const CSpec &right)
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

CPCD::CPCD()
{
    m_uiNum = 0;
    m_dbDiameter = 0.0;
}

CPCD::CPCD(UINT32 uiNum, DOUBLE dbDiameter)
{
    m_uiNum = uiNum;
    m_dbDiameter = dbDiameter;
}

CPCD::CPCD(const CPCD &right)
{
    m_uiNum = right.m_uiNum;
    m_dbDiameter = right.m_dbDiameter;
}

CPCD::~CPCD() {}

void CPCD::SetNum(UINT32 uiNum)
{
    m_uiNum = uiNum;
}

void CPCD::SetDiameter(DOUBLE dbDiameter)
{
    m_dbDiameter = dbDiameter;
}

UINT32 CPCD::GetNum() const
{
    return m_uiNum;
}

DOUBLE CPCD::GetDiameter() const
{
    return m_dbDiameter;
}

bool CPCD::operator ==(const CPCD &right) const
{
    if (m_uiNum == right.GetNum()
        && fabs(m_dbDiameter - right.GetDiameter()) < EPSILON)
    {
        return true;
    }
    
    return false;
}

bool CPCD::operator !=(const CPCD &right) const
{
    if (m_uiNum != right.GetNum()
        || fabs(m_dbDiameter - right.GetDiameter()) >= EPSILON)
    {
        return true;
    }

    return false;
}

CPCD& CPCD::operator =(const CPCD &right)
{
    if (this != &right)
    {
        m_uiNum = right.GetNum();
        m_dbDiameter = right.GetDiameter();
    }

    return *this;
}

//////////////////////////////////////////////////////////////////////////

CProductProperty::CProductProperty(UINT32 uiProductID, CSpec &cSpec, ET_TYPE cET, 
                                   CB_TYPE cCB,  CPCD &cPCD)
{
    m_cSpec = cSpec;
    m_cCB = cCB;
    m_cET = cET;
    m_cPCD = cPCD;
    m_uiProductID = uiProductID;
}

CProductProperty::CProductProperty(const CProductProperty &right)
{
    m_cSpec = right.GetSpec();
    m_cCB = right.GetCB();
    m_cET = right.GetET();
    m_cPCD = right.GetPCD();
    m_uiProductID = right.GetProductID();
}

CProductProperty::~CProductProperty() {}

CSpec CProductProperty::GetSpec() const
{
    return m_cSpec;
}

ET_TYPE CProductProperty::GetET() const
{
    return m_cET;
}

CB_TYPE CProductProperty::GetCB() const
{
    return m_cCB;
}

CPCD CProductProperty::GetPCD() const
{
    return m_cPCD;
}

UINT32 CProductProperty::GetProductivity(UINT32 uiProcessType) const
{
    MAP<UINT32, UINT32>::const_iterator it = m_mapProductivity.find(uiProcessType);
    if (it != m_mapProductivity.end())
    {
        return it->second;
    }

    return 0;
}

UINT32 CProductProperty::GetProductID() const
{
    return m_uiProductID;
}

bool CProductProperty::operator ==(const CProductProperty &right) const
{
    if (m_cSpec == right.GetSpec() && fabs(m_cCB - right.GetCB()) < EPSILON
        && m_cET == right.GetET() && m_cPCD == right.GetPCD())
    {
        return true;
    }
    
    return false;
}

bool CProductProperty::operator !=(const CProductProperty &right) const
{
    if (m_cSpec != right.GetSpec() || fabs(m_cCB - right.GetCB()) >= EPSILON
        || m_cET != right.GetET() || m_cPCD != right.GetPCD())
    {
        return true;
    }
    
    return false;
}

CProductProperty& CProductProperty::operator =(const CProductProperty &right)
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

void CProductProperty::AddProductivity(UINT32 uiProcessType, UINT32 uiValue)
{
    m_mapProductivity[uiProcessType] = uiValue;
}

