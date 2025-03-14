
#ifndef __PLAN_WRITER_H__
#define __PLAN_WRITER_H__

#include "../../include/plan_writer/plan_writer_i.h"

#include "../../include/config_loader/basic_excel.hpp"
using namespace YExcel;

#define PLAN_TABLE_COLUM_NUM        (7)

class CPlanWriter : public IPlanWriter
{
public:
	CPlanWriter(IResouce *pResource);
	virtual ~CPlanWriter();

public:

	//将排程结果输出到文件中
	virtual BOOL Write(const STRING& strFileName, const ISolution* pSolution);

protected:

	//按机器将产品排序
	INT32 MakeOrderByMachine(const ISolution* pSolution, UINT32 uiLine);

    //以机器为主线
    INT32 WriteByMachine(BasicExcelWorksheet *pSheet, const ISolution* pSolution);

    //以产品为主线
    INT32 WriteByProduct(BasicExcelWorksheet *pSheet, const ISolution* pSolution);

    //汇总
    INT32 WriteTotalInfo(BasicExcelWorksheet *pSheet, const ISolution* pSolution);

private:

    IResouce                    *m_pResource;

    //机器ID与DNA集
    MAP< UINT32, LIST<IDNA*> >  m_mapMachineToDNASet;
    UINT32                      m_uiCurLine;
    const ISolution             *m_pSolution;
    char                        m_szCellBuf[MAX_PATH];
    wchar_t                     m_szwCellBuf[MAX_PATH];

	//排程表头
	static const char           *m_ColHeader[PLAN_TABLE_COLUM_NUM];
    
};



#endif //__PLAN_WRITER_H__