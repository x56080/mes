
#include "product_mgr.h"
#include "../../include/param_macro.h"
#include "../../include/tool.h"

CProductMgr::CProductMgr()
{
}

CProductMgr::~CProductMgr()
{
    Clear();
}

INT32 CProductMgr::Analisize(const STRING &strIn, STRING &strLeft, STRING &strRight)
{
    size_t nPos1 = strIn.find('*');
    if (nPos1 == STRING::npos)
    {
        TRACE("CProductMgr::Analisize", "Error[%s]", strIn.c_str());
        return RET_ERR;
    }

    strLeft = strIn.substr(0, nPos1);
    strRight = strIn.substr(nPos1+1);
    
    if (!TOOL::IsDigit(strLeft.c_str()) || !TOOL::IsDigit(strRight.c_str())
        || strRight.empty())
    {
        TRACE("CProductMgr::Analisize", "Error[%s]", strIn.c_str());
        return RET_ERR;
    }

    return RET_SUC;
}

INT32 CProductMgr::AddProduct(IParamSet *pParamSet)
{
    UINT32 uiProductID = 0;
    STRING strSpec, strPCD;
    DOUBLE dbCB = 0.0;
    INT32 iET = 0;

    //{{
    RDX_Int(pParamSet, uiProductID, S_PARAM_TYPE);
    RDX_String(pParamSet, strSpec, S_PARAM_SPEC);
    RDX_String(pParamSet, strPCD, S_PARAM_PCD);
    RDX_Int(pParamSet, iET, S_PARAM_ET);
    RDX_Double(pParamSet, dbCB, S_PARAM_CB);
    //}}

    if (NULL != GetProductProperty(uiProductID))
    {
        TRACE("CProductMgr::AddProduct", "Product[ID=%d] already exist", uiProductID);
        return RET_ERR;
    }

    //解析Spec
    STRING strDiameter, strWidth;
    Analisize(strSpec, strDiameter, strWidth);
    CSpec spec(TOOL::String2Double(strWidth), TOOL::String2Double(strDiameter));

    //解析PCD
    STRING strNum;
    Analisize(strPCD, strNum, strDiameter);
    CPCD pcd(atoi(strNum.c_str()), TOOL::String2Double(strDiameter));

    CProductProperty *pProperty = new CProductProperty(uiProductID, spec, iET, dbCB, pcd);
    m_mapProducts[uiProductID] = pProperty;

    return RET_SUC;
}

INT32 CProductMgr::AddProductivity(UINT32 uiProductID, IParamSet *pParamSet)
{
    CProductProperty* pProperty = GetProductProperty(uiProductID);
    if (NULL == pProperty)
    {
        return RET_ERR;
    }

    UINT32 uiProcessType = 0;
    UINT32 uiValue = 0;
    //{{
    RDX_Int(pParamSet, uiProcessType, S_PARAM_PROCESS_TYPE);
    RDX_Int(pParamSet, uiValue, S_PARAM_VALUE);
    //}}
    pProperty->AddProductivity(uiProcessType, uiValue);

    return RET_SUC;
}

//根据产品型号获取产品属性
CProductProperty* CProductMgr::GetProductProperty(UINT32 uiProductID) const
{
	MAP<UINT32, CProductProperty*>::const_iterator it = m_mapProducts.find(uiProductID);
    if (it != m_mapProducts.end())
    {
        return it->second;
    }

	return NULL;
}

//获取产品的种类
UINT32 CProductMgr::GetProductKind() const
{
	return m_mapProducts.size();
}

void CProductMgr::Clear()
{
    MAP<UINT32, CProductProperty*>::iterator it = m_mapProducts.begin();
    while (it != m_mapProducts.end())
    {
        delete it->second;
        ++it;
    }
    m_mapProducts.clear();
}

