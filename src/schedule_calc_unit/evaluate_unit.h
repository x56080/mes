
#ifndef __EVALUATE_UNIT_H__
#define __EVALUATE_UNIT_H__

#include "../../include/algorith_def.h"


//订单统计类
class COrderStat
{
public:
    COrderStat();
    ~COrderStat();

public:
    
    INT32       Init(IOrder *pOrder);

    UINT32      GetProductsCount() const;

    BOOL        PacketProduct(UINT32 uiProductID);

    INT32       Reset();

private:
    UINT32                  m_uiProductsCount;
    UINT32                  m_uiOrderID;
    LIST<UINT32>            m_lstProductID;
    LIST<UINT32>            m_lstStatProductID;

};

class COrderStatMgr
{
public:
    COrderStatMgr();
    ~COrderStatMgr();

public:
    
    INT32       Init(IOrderMgr *pOrderMgr);

    UINT32      GetProductsCount(UINT32 uiOrderID) const;

    BOOL        PacketProduct(UINT32 uiOrderID, UINT32 uiProductID);

    INT32       Reset();

private:
    MAP<UINT32, COrderStat*>    m_mapOrderStat;
    
};

//评估单元
class CEvaluateUnit : public IEvaluateUint
{
public:
    CEvaluateUnit(IResouce *pResouce);
    ~CEvaluateUnit();

public:

    virtual INT32   Init(IParamSet *pParamSet);
    
    virtual INT32   Evaludate(ISolution *pSolution);

protected:
    
private:
    IResouce            *m_pResource;
    IParamSet           *m_pParamSet;

    COrderStatMgr       m_OrderStatMgr;
    UINT32              m_uiMaxStoreNum;

};

#endif //__EVALUATE_UNIT_H__

