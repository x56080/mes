
#include "../../include/tool.h"
#include "plan_writer.h"

#define S_BEGIN_TIME            "开始时间"
#define S_END_TIME              "结束时间"
#define S_TOTAL_PRODUCT_TIME    "总生产时间(sec)"
#define S_TOTAL_SWITCH_TIME     "总切换时间(sec)"
#define S_TOTAL_IDLE_TIME       "总暂停时间(sec)"
#define S_MAX_STORE             "最大库存"
#define S_FITNESS               "Fitness"

const char * CPlanWriter::m_ColHeader[] = 
{
	"设备",
	"产品",
	"开始时间",
	"完成时间",
    "生产耗时(sec)",
    "切换时间(sec)",
    "暂停时间(sec)"
};

CPlanWriter::CPlanWriter(IResouce *pResource)
{
    m_pResource = pResource;
    m_pSolution = NULL;
    m_uiCurLine = 0;
    m_szCellBuf[0] = 0;
    m_szwCellBuf[0] = 0;
}

CPlanWriter::~CPlanWriter()
{
    m_pResource = NULL;
    m_pSolution = NULL;
}

INT32 CPlanWriter::MakeOrderByMachine(const ISolution* pSolution, UINT32 uiLine)
{
    //清空
    m_mapMachineToDNASet.clear();
    MAP< UINT32, LIST<IDNA*> >::iterator it;
    
    m_pSolution = pSolution;
    m_uiCurLine = uiLine;
    
    IDNA *pDNA = NULL;
    UINT32 uiIndex = 0;
    while (uiIndex < pSolution->GetColum())
    {
        pDNA = pSolution->GetDNA(uiLine, uiIndex);
        
        it = m_mapMachineToDNASet.find(pDNA->GetMachineID());
        if (it == m_mapMachineToDNASet.end())
        {
            LIST<IDNA*> lstDNA;
            lstDNA.push_back(pDNA);
            m_mapMachineToDNASet[pDNA->GetMachineID()] = lstDNA;
            uiIndex++;
            continue;
        }
        
        //对同一MachineID的DNA进行排序
        LIST<IDNA*> &lstDNA = it->second;
        LIST<IDNA*>::iterator itDNA = lstDNA.begin();
        IDNA *pDNAIndex = NULL;
        while (itDNA != lstDNA.end())
        {
            pDNAIndex = (*itDNA);

            if (pDNA->GetStartTime() < pDNAIndex->GetStartTime())
            {
                lstDNA.insert(itDNA, pDNA);
                break;
            }

            itDNA++;
        }
        
        //直到最后还没有插入
        if (itDNA == lstDNA.end())
        {
            lstDNA.push_back(pDNA);
        }
        
        uiIndex++;
    }
    
    return RET_SUC;
}

//将排程结果输出到文件中
BOOL CPlanWriter::Write(const STRING& strFileName, const ISolution* pSolution)
{
	//新建Excel
	BasicExcel planFile;
	planFile.New(3);

    //将Sheet1 修改为 total_info 表
    planFile.RenameWorksheet((size_t)0, "total_info");
	//将Sheet2 修改为 plan_by_machine 表
	planFile.RenameWorksheet((size_t)1, "plan_by_machine");
    //将Sheet3 修改为 plan_by_product 表
    planFile.RenameWorksheet((size_t)2, "plan_by_product");

    BasicExcelWorksheet* pSheet0 = planFile.GetWorksheet((size_t)0);
	BasicExcelWorksheet* pSheet1 = planFile.GetWorksheet((size_t)1);
    BasicExcelWorksheet* pSheet2 = planFile.GetWorksheet((size_t)2);

    if (WriteByMachine(pSheet1, pSolution) >= 0
        && WriteByProduct(pSheet2, pSolution) >= 0
        && WriteTotalInfo(pSheet0, pSolution) >= 0)
    {
        planFile.SaveAs(strFileName.c_str());
        return TRUE;
    }

    return FALSE;
}


INT32 CPlanWriter::WriteByMachine(BasicExcelWorksheet *pSheet, const ISolution* pSolution)
{
    //设置表头
    UINT32 uiIndex = 0;
    for (; uiIndex < sizeof(m_ColHeader) / sizeof(m_ColHeader[0]); ++uiIndex)
    {
        MultiByteToWideChar(CP_ACP, 0, m_ColHeader[uiIndex], MAX_PATH, m_szwCellBuf, MAX_PATH); 
        pSheet->Cell(0, uiIndex)->SetWString(m_szwCellBuf);
    }

    IProcessMgr *pProcessMgr = m_pResource->GetProcessMgr();
    IDNA *pDNA = NULL;
    uiIndex = 0;
    UINT32 uiWriteLine = 1;
    UINT32 uiWriteCol = 0;
    MAP< UINT32, LIST<IDNA*> >::iterator it;
    LIST<IDNA*>::iterator itDNA;
    while (uiIndex < pSolution->GetLine())
    {
        //按工序对机器排序
        MakeOrderByMachine(pSolution, uiIndex);

        it = m_mapMachineToDNASet.begin();
        while (it != m_mapMachineToDNASet.end())
        {
            uiWriteCol = 0;

            //输出机器信息
            _snprintf(m_szCellBuf, MAX_PATH-1, "%s:%u#", 
                pProcessMgr->GetProcess(uiIndex)->GetName().c_str(), it->first);
            m_szCellBuf[MAX_PATH-1] = 0;
            MultiByteToWideChar(CP_ACP, 0, m_szCellBuf, MAX_PATH, m_szwCellBuf, MAX_PATH); 
            pSheet->Cell(uiWriteLine, uiWriteCol++)->SetWString(m_szwCellBuf);

            //输出机器调度信息
            LIST<IDNA*> &lstDNA = it->second;
            itDNA = lstDNA.begin();
            while (itDNA != lstDNA.end())
            {
                pDNA = (*itDNA);
                uiWriteCol = 1;

                //产品信息
                _snprintf(m_szCellBuf, MAX_PATH-1, "订单%u#:型号%u:数量%u", pDNA->GetProductInfo()->uiOrderID,
                    pDNA->GetProductInfo()->uiProductID, pDNA->GetProductInfo()->uiProductNum);
                m_szCellBuf[MAX_PATH-1] = 0;
                MultiByteToWideChar(CP_ACP, 0, m_szCellBuf, MAX_PATH, m_szwCellBuf, MAX_PATH); 
                pSheet->Cell(uiWriteLine, uiWriteCol++)->SetWString(m_szwCellBuf);

                //开始时间
                pSheet->Cell(uiWriteLine, uiWriteCol++)->SetString(
                                TOOL::GetAscTime(pDNA->GetStartTime()).c_str());

                //完成时间
                pSheet->Cell(uiWriteLine, uiWriteCol++)->SetString(
                                TOOL::GetAscTime(pDNA->GetEndTime()).c_str());

                //生产耗时
                pSheet->Cell(uiWriteLine, uiWriteCol++)->SetInteger(pDNA->GetProduceTime());

                //切换时间
                pSheet->Cell(uiWriteLine, uiWriteCol++)->SetInteger(pDNA->GetSwitchTime());

                //暂停时间
                pSheet->Cell(uiWriteLine, uiWriteCol++)->SetInteger(pDNA->GetIdleTime());

                ++uiWriteLine;
                ++itDNA;
            }
            
            ++uiWriteLine;
            ++it;
        }
        
        uiIndex++;
    }

    return RET_SUC;
}

INT32 CPlanWriter::WriteByProduct(BasicExcelWorksheet *pSheet, const ISolution* pSolution)
{
    return RET_SUC;
}

INT32 CPlanWriter::WriteTotalInfo(BasicExcelWorksheet *pSheet, const ISolution* pSolution)
{
    time_t tBeginTime = pSolution->GetDNA(0)->GetStartTime();
    time_t tEndTime = tBeginTime;
    UINT32 uiTotalProductTime = 0;
    UINT32 uiTotalSwitchTime = 0;
    UINT32 uiTotalIdleTime = 0;

    UINT32 uiIndex = 0;
    IDNA *pDNA = NULL;
    while (uiIndex < pSolution->GetDNANum())
    {
        pDNA = pSolution->GetDNA(uiIndex++);
        if (pDNA->GetStartTime() < tBeginTime)
        {
            tBeginTime = pDNA->GetStartTime();
        }

        if (pDNA->GetEndTime() > tEndTime)
        {
            tEndTime = pDNA->GetEndTime();
        }

        uiTotalProductTime += pDNA->GetProduceTime();
        uiTotalSwitchTime += pDNA->GetSwitchTime();
        uiTotalIdleTime += pDNA->GetIdleTime();
    }

    UINT32 uiWriteRow = 0;

    //开始时间
    MultiByteToWideChar(CP_ACP, 0, S_BEGIN_TIME, MAX_PATH, m_szwCellBuf, MAX_PATH);
    pSheet->Cell(uiWriteRow, 0)->SetWString(m_szwCellBuf);
    pSheet->Cell(uiWriteRow, 1)->SetString(TOOL::GetAscTime(tBeginTime).c_str());
    uiWriteRow += 1;

    //结束时间
    MultiByteToWideChar(CP_ACP, 0, S_END_TIME, MAX_PATH, m_szwCellBuf, MAX_PATH);
    pSheet->Cell(uiWriteRow, 0)->SetWString(m_szwCellBuf);
    pSheet->Cell(uiWriteRow, 1)->SetString(TOOL::GetAscTime(tEndTime).c_str());
    uiWriteRow += 1;

    //总生产时间
    MultiByteToWideChar(CP_ACP, 0, S_TOTAL_PRODUCT_TIME, MAX_PATH, m_szwCellBuf, MAX_PATH);
    pSheet->Cell(uiWriteRow, 0)->SetWString(m_szwCellBuf);
    pSheet->Cell(uiWriteRow, 1)->SetInteger(uiTotalProductTime);
    uiWriteRow += 1;

    //总切换时间
    MultiByteToWideChar(CP_ACP, 0, S_TOTAL_SWITCH_TIME, MAX_PATH, m_szwCellBuf, MAX_PATH);
    pSheet->Cell(uiWriteRow, 0)->SetWString(m_szwCellBuf);
    pSheet->Cell(uiWriteRow, 1)->SetInteger(uiTotalSwitchTime);
    uiWriteRow += 1;

    //总暂停时间
    MultiByteToWideChar(CP_ACP, 0, S_TOTAL_IDLE_TIME, MAX_PATH, m_szwCellBuf, MAX_PATH);
    pSheet->Cell(uiWriteRow, 0)->SetWString(m_szwCellBuf);
    pSheet->Cell(uiWriteRow, 1)->SetInteger(uiTotalIdleTime);
    uiWriteRow += 1;

    //最大库存
    MultiByteToWideChar(CP_ACP, 0, S_MAX_STORE, MAX_PATH, m_szwCellBuf, MAX_PATH);
    pSheet->Cell(uiWriteRow, 0)->SetWString(m_szwCellBuf);
    pSheet->Cell(uiWriteRow, 1)->SetInteger(pSolution->GetMaxStoreNum());
    uiWriteRow += 1;

    //Fitness
    MultiByteToWideChar(CP_ACP, 0, S_FITNESS, MAX_PATH, m_szwCellBuf, MAX_PATH);
    pSheet->Cell(uiWriteRow, 0)->SetWString(m_szwCellBuf);
    pSheet->Cell(uiWriteRow, 1)->SetDouble(pSolution->GetFitness());
    
    return RET_SUC;
}


//////////////////////////////////////////////////////////////////////////

IPlanWriter * CreatePlanWriter(IResouce *pResource)
{
	return new CPlanWriter(pResource);
}

BOOL ReleasePlanWriter(IPlanWriter *pWriter)
{
    if (pWriter)
    {
        delete pWriter, pWriter = NULL;
    }
    return TRUE;
}

