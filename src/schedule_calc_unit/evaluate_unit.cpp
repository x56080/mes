
#include "../../include/tool.h"
#include "../../include/param_macro.h"
#include "evaluate_unit.h"


COrderStat::COrderStat()
{
    m_uiProductsCount = 0;
    m_uiOrderID = 0;
}

COrderStat::~COrderStat()
{
    m_lstStatProductID.clear();
    m_lstProductID.clear();
}

INT32 COrderStat::Init(IOrder *pOrder)
{
    if (NULL == pOrder)
    {
        return RET_ERR;
    }

    m_uiOrderID = pOrder->GetOrderID();

    VECTOR<SProductInfo> vecProductInfo;
    pOrder->GetProductInfo(vecProductInfo);

    VECTOR<SProductInfo>::iterator it = vecProductInfo.begin();
    for (; it != vecProductInfo.end(); it++)
    {
        m_uiProductsCount += it->uiProductNum;
        m_lstProductID.push_back(it->uiProductID);
    }

    return RET_SUC;
}

UINT32 COrderStat::GetProductsCount() const
{
    return m_uiProductsCount;
}

BOOL COrderStat::PacketProduct(UINT32 uiProductID)
{
    LIST<UINT32>::iterator itFind = std::find(m_lstStatProductID.begin(), 
                                              m_lstStatProductID.end(), 
                                              uiProductID);
    if (itFind == m_lstStatProductID.end())
    {
        TRACE("COrderStat::PacketProduct", "Product[ID=%u] is not in Order[ID=%u]",
              uiProductID, m_uiOrderID);
        return FALSE;
    }

    m_lstStatProductID.erase(itFind);
    
    return m_lstStatProductID.empty() ? TRUE : FALSE;
}

INT32 COrderStat::Reset()
{
    m_lstStatProductID.clear();
    LIST<UINT32>::iterator it = m_lstProductID.begin();
    for (; it != m_lstProductID.end(); ++it)
    {
        m_lstStatProductID.push_back(*it);
    }

    return RET_SUC;
}

COrderStatMgr::COrderStatMgr()
{
}

COrderStatMgr::~COrderStatMgr()
{
    MAP<UINT32, COrderStat*>::iterator it = m_mapOrderStat.begin();
    while (it != m_mapOrderStat.end())
    {
        delete it->second;
        ++it;
    }
    m_mapOrderStat.clear();
}

INT32 COrderStatMgr::Init(IOrderMgr *pOrderMgr)
{
    if (NULL == pOrderMgr)
    {
        return RET_ERR;
    }

    UINT32 uiOrderIndex = 0;
    IOrder *pOrder = NULL;
    COrderStat *pOrderStat = NULL;
    for (; uiOrderIndex < pOrderMgr->GetOrderNum(); uiOrderIndex++)
    {
        pOrder = pOrderMgr->GetOrder(uiOrderIndex);
        pOrderStat = new COrderStat();
        m_mapOrderStat[pOrder->GetOrderID()] = pOrderStat;

        if (pOrderStat->Init(pOrder) < 0)
        {
            TRACE("COrderStatMgr::Init", "Init OrderStat failed, OrderID=%u", pOrder->GetOrderID());
            return RET_ERR;
        }
    }

    return RET_SUC;
}

UINT32 COrderStatMgr::GetProductsCount(UINT32 uiOrderID) const
{
    MAP<UINT32, COrderStat*>::const_iterator itFind = m_mapOrderStat.find(uiOrderID);
    if (itFind == m_mapOrderStat.end())
    {
        TRACE("COrderStatMgr::GetProductsCount", "Not found OrderStat[OrderID=%u]", uiOrderID);
        return 0;
    }

    return itFind->second->GetProductsCount();
}

BOOL COrderStatMgr::PacketProduct(UINT32 uiOrderID, UINT32 uiProductID)
{
    MAP<UINT32, COrderStat*>::const_iterator itFind = m_mapOrderStat.find(uiOrderID);
    if (itFind == m_mapOrderStat.end())
    {
        TRACE("COrderStatMgr::GetProductsCount", "Not found OrderStat[OrderID=%u]", uiOrderID);
        return FALSE;
    }

    return itFind->second->PacketProduct(uiProductID);
}

INT32 COrderStatMgr::Reset()
{
    MAP<UINT32, COrderStat*>::iterator it = m_mapOrderStat.begin();
    while (it != m_mapOrderStat.end())
    {
        it->second->Reset();
        ++it;
    }

    return RET_SUC;
}

//////////////////////////////////////////////////////////////////////////

CEvaluateUnit::CEvaluateUnit(IResouce *pResouce)
{
    m_uiMaxStoreNum = 0;
    m_pResource = pResouce;
    m_pParamSet = NULL;
}

CEvaluateUnit::~CEvaluateUnit()
{
    m_pResource = NULL;
    m_pParamSet = NULL;
}

INT32 CEvaluateUnit::Init(IParamSet *pParamSet)
{
    if (NULL == pParamSet)
    {
        return RET_ERR;
    }

    if (m_OrderStatMgr.Init(m_pResource->GetOrderMgr()) < 0)
    {
        return RET_ERR;
    }
    
    m_pParamSet = pParamSet;

    //{{
    RDX_Int(m_pParamSet, m_uiMaxStoreNum, S_PARAM_MAX_STORE_NUM);
    //}}

    return RET_SUC;
}

//若库存>阈值, Fitness = 0.0
//Fitness = 1 / 总时间
INT32 CEvaluateUnit::Evaludate(ISolution *pSolution)
{
    UINT32 uiProcessNum = pSolution->GetLine();
    UINT32 uiProductNum = pSolution->GetColum();

    time_t tStartTime = pSolution->GetDNA(0)->GetStartTime();
    time_t tEndTime = tStartTime;
    
    //计算最早开始时间(只需要计算第0道工序)
    IDNA *pDNA = NULL;
    UINT32 uiIndex = 0;
    for (uiIndex = 1; uiIndex < uiProductNum; uiIndex++)
    {
        pDNA = pSolution->GetDNA(uiIndex);
        if (pDNA->GetStartTime() < tStartTime)
        {
            tStartTime = pDNA->GetStartTime();
        }
    }

    LIST<IDNA*> lstDNA;
    //计算最晚结束时间(只需要计算最后一道工序)
    for (uiIndex = 0; uiIndex < uiProductNum; uiIndex++)
    {
        pDNA = pSolution->GetDNA(uiProcessNum - 1, uiIndex);
        if (pDNA->GetEndTime() > tEndTime)
        {
            tEndTime = pDNA->GetEndTime();
        }

        LIST<IDNA*>::iterator itDNA = lstDNA.begin();
        while (itDNA != lstDNA.end())
        {
            if (pDNA->GetEndTime() < (*itDNA)->GetEndTime())
            {
                lstDNA.insert(itDNA, pDNA);
                break;
            }

            itDNA++;
        }

        //直到最后没有插入
        if (itDNA == lstDNA.end())
        {
            lstDNA.push_back(pDNA);
        }
    }

    //把秒放大到小时计算
    DOUBLE dbFitness = (DOUBLE)360000 / (DOUBLE)(tEndTime - tStartTime);

    //计算适应度
    pSolution->SetFitness(dbFitness);

    //计算库存
    UINT32 uiStoreNum = 0;
    UINT32 uiMaxStoreNum = 0;
    m_OrderStatMgr.Reset();
    LIST<IDNA*>::iterator itDNAIndex = lstDNA.begin();
    while (itDNAIndex != lstDNA.end())
    {
        pDNA = *itDNAIndex;
        uiStoreNum += pDNA->GetProductInfo()->uiProductNum;
        if (m_OrderStatMgr.PacketProduct(pDNA->GetProductInfo()->uiOrderID,
                                         pDNA->GetProductInfo()->uiProductID))
        {
            uiStoreNum -= m_OrderStatMgr.GetProductsCount(pDNA->GetProductInfo()->uiOrderID);
        }
        
        if (uiStoreNum > uiMaxStoreNum)
        {
            uiMaxStoreNum = uiStoreNum;
        }

        ++itDNAIndex;
    }

    pSolution->SetMaxStoreNum(uiMaxStoreNum);
    if (uiMaxStoreNum > m_uiMaxStoreNum)
    {
        pSolution->SetFitness(0.0);
    }

    return RET_SUC;
}

