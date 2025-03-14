
#include "order.h"
#include "../../include/tool.h"

COrder::COrder(UINT32 uiOrderID)
{
    m_uiOrderID = uiOrderID;
}

COrder::~COrder()
{
}

INT32 COrder::AddProductInfo(UINT32 uiProductID, UINT32 uiNum)
{
    if (FindProductInfo(uiProductID))
    {
        TRACE("COrder::AddProductInfo", "ProductInfo[ID=%d] already exist", uiProductID);
        return RET_ERR;
    }

    SProductInfo info;
    info.uiOrderID = m_uiOrderID;
    info.uiProductID = uiProductID;
    info.uiProductNum = uiNum;

    m_vecProductInfo.push_back(info);

    return RET_SUC;
}

BOOL COrder::FindProductInfo(UINT32 uiProductID)
{
    UINT32 uiIndex = 0;
    while (uiIndex < m_vecProductInfo.size())
    {
        SProductInfo &info = m_vecProductInfo[uiIndex++];
        if (info.uiProductID == uiProductID)
        {
            return TRUE;
        }
    }

    return FALSE;
}

UINT32 COrder::GetOrderID() const
{
    return m_uiOrderID;
}

UINT32 COrder::GetProductInfo(VECTOR<SProductInfo> &vecProductList) const
{
    UINT32 uiIndex = 0;
    while (uiIndex < m_vecProductInfo.size())
    {
        vecProductList.push_back(m_vecProductInfo[uiIndex++]);
    }

    return vecProductList.size();
}

UINT32 COrder::GetProductKinds() const
{
    return m_vecProductInfo.size();
}

//////////////////////////////////////////////////////////////////////////

COrderMgr::COrderMgr()
{
}

COrderMgr::~COrderMgr()
{
    Clear();
}

void COrderMgr::Clear()
{
    //释放订单数据
    UINT32 uiIndex = 0;
    while (uiIndex < m_vecOrders.size())
    {
        delete m_vecOrders[uiIndex++];
    }
    m_vecOrders.clear();
}

//获取所有订单产品种类
UINT32 COrderMgr::GetProductKinds() const
{
    UINT32 uiProductKinds = 0;
    COrder *pOrder = NULL;

    UINT32 uiIndex = 0;
    while (uiIndex < m_vecOrders.size())
    {
        pOrder = m_vecOrders[uiIndex++];
        uiProductKinds += pOrder->GetProductKinds();
    }

	return uiProductKinds;
}

 //获取订单个数
UINT32 COrderMgr::GetOrderNum() const
{
	return m_vecOrders.size();
}

IOrder* COrderMgr::GetOrder(UINT32 uiIndex) const
{
    if (uiIndex < m_vecOrders.size())
    {
        return m_vecOrders[uiIndex];
    }
    
    return NULL;
}

//获取所有订单产品信息列表
UINT32 COrderMgr::GetAllProductInfo(VECTOR<SProductInfo> &vecProductList) const
{
    vecProductList.clear();
    COrder *pOrder = NULL;

    UINT32 uiIndex = 0;
    while (uiIndex < m_vecOrders.size())
    {
        pOrder = m_vecOrders[uiIndex++];
        pOrder->GetProductInfo(vecProductList);
    }

    return vecProductList.size();
}

COrder* COrderMgr::AddOrder(UINT32 uiOrderID)
{
    if (GetOrderByID(uiOrderID))
    {
        TRACE("COrderMgr::AddOrder", "Order[ID=%d] already exist", uiOrderID);
        return NULL;
    }
    
    COrder *pOrder = new COrder(uiOrderID);
    m_vecOrders.push_back(pOrder);

    return pOrder;
}

INT32 COrderMgr::AddProductInfo(UINT32 uiOrderID, UINT32 uiProductID, UINT32 uiNum)
{
    COrder *pOrder = GetOrderByID(uiOrderID);
    if (pOrder)
    {
        return pOrder->AddProductInfo(uiProductID, uiNum);
    }

    return RET_ERR;
}

COrder* COrderMgr::GetOrderByID(UINT32 uiOrderID)
{
    UINT32 uiIndex = 0;
    COrder *pOrder = NULL;
    while (uiIndex < m_vecOrders.size())
    {
        pOrder = m_vecOrders[uiIndex++];
        if (pOrder->GetOrderID() == uiOrderID)
        {
            return pOrder;
        }
    }

    return NULL;
}
