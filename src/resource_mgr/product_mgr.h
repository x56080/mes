
#ifndef __PRODUCT_MGR_H__
#define __PRODUCT_MGR_H__

#include "../../include/interface.h"
#include "../../include/product.h"

class CProductMgr : public IProductMgr
{
public:
	CProductMgr();

	virtual ~CProductMgr();

    INT32       AddProduct(IParamSet *pParamSet);
    INT32       AddProductivity(UINT32 uiProductID, IParamSet *pParamSet);

public:

    //���ݲ�Ʒ�ͺŻ�ȡ��Ʒ����
    virtual CProductProperty* GetProductProperty(UINT32 uiProductID) const;

    //��ȡ��Ʒ������
    virtual UINT32 GetProductKind() const;

protected:
    
    void            Clear();

    INT32           Analisize(const STRING &strIn, STRING &strLeft, STRING &strRight);

private:

	//��Ʒ��Ϣ������������Ϣ
	MAP<UINT32, CProductProperty*>           m_mapProducts;

};

#endif //__PRODUCT_MGR_H__