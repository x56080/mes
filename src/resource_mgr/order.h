
#ifndef __ORDER_H__
#define __ORDER_H__

#include "../../include/interface.h"

class COrder : public IOrder
{
public:
    COrder(UINT32 uiOrderID);
    ~COrder();

    INT32               AddProductInfo(UINT32 uiProductID, UINT32 uiNum);

public:

    //获取订单ID
    virtual UINT32      GetOrderID() const;
    
    //获取产品种类
    virtual UINT32      GetProductKinds() const;
    
    //获取订单产品信息列表
    virtual UINT32      GetProductInfo(VECTOR<SProductInfo> &vecProductList) const;

protected:
    
    BOOL                FindProductInfo(UINT32 uiProductID);

private:
    
    //订单ID
    UINT32              m_uiOrderID;
    //产品信息
    VECTOR<SProductInfo>    m_vecProductInfo;

};

class COrderMgr : public IOrderMgr
{
public:
	COrderMgr();

	~COrderMgr();

    COrder*             AddOrder(UINT32 uiOrderID);
    INT32               AddProductInfo(UINT32 uiOrderID, UINT32 uiProductID, UINT32 uiNum);
    COrder*             GetOrderByID(UINT32 uiOrderID);
    
public:

    //获取所有订单产品种类
    virtual UINT32      GetProductKinds() const;
    
    //获取订单个数
    virtual UINT32      GetOrderNum() const;
    
    //获取订单
    virtual IOrder*     GetOrder(UINT32 uiIndex) const;
    
    //获取所有订单产品信息列表
    virtual UINT32      GetAllProductInfo(VECTOR<SProductInfo> &vecProductList) const;

protected:
    
    void                Clear();

private:

	//订单表
	VECTOR<COrder*>    m_vecOrders;

};


#endif //__ORDER_H__
