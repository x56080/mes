
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

    //��ȡ����ID
    virtual UINT32      GetOrderID() const;
    
    //��ȡ��Ʒ����
    virtual UINT32      GetProductKinds() const;
    
    //��ȡ������Ʒ��Ϣ�б�
    virtual UINT32      GetProductInfo(VECTOR<SProductInfo> &vecProductList) const;

protected:
    
    BOOL                FindProductInfo(UINT32 uiProductID);

private:
    
    //����ID
    UINT32              m_uiOrderID;
    //��Ʒ��Ϣ
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

    //��ȡ���ж�����Ʒ����
    virtual UINT32      GetProductKinds() const;
    
    //��ȡ��������
    virtual UINT32      GetOrderNum() const;
    
    //��ȡ����
    virtual IOrder*     GetOrder(UINT32 uiIndex) const;
    
    //��ȡ���ж�����Ʒ��Ϣ�б�
    virtual UINT32      GetAllProductInfo(VECTOR<SProductInfo> &vecProductList) const;

protected:
    
    void                Clear();

private:

	//������
	VECTOR<COrder*>    m_vecOrders;

};


#endif //__ORDER_H__
