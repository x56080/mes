
#ifndef __RESOUCE_MGR_H__
#define __RESOUCE_MGR_H__

#include "../../include/interface.h"

#include "order.h"
#include "process.h"
#include "product_mgr.h"
#include "machine.h"
#include "param_set.h"

#include "../../include/config_loader/basic_excel.hpp"
using namespace YExcel;

class CResouceMgr : public IResouce
{
public:
	CResouceMgr();

	~CResouceMgr();

	BOOL Init(const char *pszConfigFile);

public:
    
    virtual IMachineMgr*        GetMachineMgr() const;

    virtual IProcessMgr*        GetProcessMgr() const;

    virtual IOrderMgr*          GetOrderMgr() const;

    virtual IProductMgr*        GetProductMgr() const;

    virtual IParamSet*          GetConfigSet() const;

protected:
    
    BOOL        InitProduct(BasicExcelWorksheet *pSheet);
    BOOL        InitProcess(BasicExcelWorksheet *pSheet);
    BOOL        InitMachine(BasicExcelWorksheet *pSheet);
    BOOL        InitLocalParamSet(BasicExcelWorksheet *pSheet);
    BOOL        InitOrder(BasicExcelWorksheet *pSheet);

private:

    CProductMgr         *m_pProductMgr;
    CProcessMgr         *m_pProcessMgr;
	CMachineMgr         *m_pMachineMgr;
	COrderMgr           *m_pOrderMgr;

    CParamSet           *m_pLocalParamSet;
	
};


#endif //__RESOUCE_MGR_H__

