
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

    //根据产品型号获取产品属性
    virtual CProductProperty* GetProductProperty(UINT32 uiProductID) const;

    //获取产品的种类
    virtual UINT32 GetProductKind() const;

protected:
    
    void            Clear();

    INT32           Analisize(const STRING &strIn, STRING &strLeft, STRING &strRight);

private:

	//产品信息表，包含产能信息
	MAP<UINT32, CProductProperty*>           m_mapProducts;

};

#endif //__PRODUCT_MGR_H__