

#include "resouce_mgr.h"
#include "../../include/tool.h"
#include "../../include/param_macro.h"
#include "../../include/config_loader/excel_func.h"

#define S_SHEET_PRODUCT         "product_info"
#define S_SHEET_PROCESS         "process_info"
#define S_SHEET_MACHINE         "machine_info"
#define S_SHEET_PARAM           "param_info"
#define S_SHEET_ORDER           "order_info"

CResouceMgr::CResouceMgr()
{
    m_pProductMgr = new CProductMgr();
    m_pProcessMgr = new CProcessMgr();
    m_pMachineMgr = new CMachineMgr(m_pProductMgr);
    m_pOrderMgr = new COrderMgr();

    m_pLocalParamSet = new CParamSet();
}

CResouceMgr::~CResouceMgr()
{
    delete m_pOrderMgr, m_pOrderMgr = NULL;
    delete m_pMachineMgr, m_pMachineMgr = NULL;
    delete m_pProcessMgr, m_pProcessMgr = NULL;
    delete m_pProductMgr, m_pProductMgr = NULL;

    delete m_pLocalParamSet, m_pLocalParamSet = NULL;
}

BOOL CResouceMgr::Init(const char *pszConfigFile)
{
    if (!pszConfigFile)
    {
        return FALSE;
    }

    BasicExcel excel;
    if (!excel.Load(pszConfigFile))
    {
        TRACE("CResouceMgr::Init", "Load [%s] filed", pszConfigFile);
        return FALSE;
    }

    BasicExcelWorksheet *pProductSheet = excel.GetWorksheet(S_SHEET_PRODUCT);
    if (!pProductSheet)
    {
        TRACE("CResouceMgr::Init", "Get [%s] failed", S_SHEET_PRODUCT);
        return FALSE;
    }
    if (!InitProduct(pProductSheet))
    {
        return FALSE;
    }

    BasicExcelWorksheet *pProcessSheet = excel.GetWorksheet(S_SHEET_PROCESS);
    if (!pProcessSheet)
    {
        TRACE("CResouceMgr::Init", "Get [%s] failed", S_SHEET_PROCESS);
        return FALSE;
    }
    if (!InitProcess(pProcessSheet))
    {
        return FALSE;
    }

    BasicExcelWorksheet *pMachineSheet = excel.GetWorksheet(S_SHEET_MACHINE);
    if (!pMachineSheet)
    {
        TRACE("CResouceMgr::Init", "Get [%s] failed", S_SHEET_MACHINE);
        return FALSE;
    }
    if (!InitMachine(pMachineSheet))
    {
        return FALSE;
    }

    BasicExcelWorksheet *pParamSheet = excel.GetWorksheet(S_SHEET_PARAM);
    if (!pParamSheet)
    {
        TRACE("CResouceMgr::Init", "Get [%s] failed", S_SHEET_PARAM);
        return FALSE;
    }
    if (!InitLocalParamSet(pParamSheet))
    {
        return FALSE;
    }

    BasicExcelWorksheet *pOrderSheet = excel.GetWorksheet(S_SHEET_ORDER);
    if (!pOrderSheet)
    {
        TRACE("CResouceMgr::Init", "Get [%s] failed", S_SHEET_ORDER);
        return FALSE;
    }
    if (!InitOrder(pOrderSheet))
    {
        return FALSE;
    }

	return TRUE;
}

IMachineMgr* CResouceMgr::GetMachineMgr() const
{
	return m_pMachineMgr;
}

IProcessMgr* CResouceMgr::GetProcessMgr() const
{
	return m_pProcessMgr;
}

IOrderMgr* CResouceMgr::GetOrderMgr() const
{
	return m_pOrderMgr;
}

IProductMgr* CResouceMgr::GetProductMgr() const
{
	return m_pProductMgr;
}

IParamSet* CResouceMgr::GetConfigSet() const
{
    return m_pLocalParamSet;
}

BOOL CResouceMgr::InitProduct(BasicExcelWorksheet *pSheet)
{
    UINT32 uiMaxRow = pSheet->GetTotalRows();
    UINT32 uiMaxCol = pSheet->GetTotalCols();
    CParamSet paramSet;

    UINT32 uiProductID = 0;
    UINT32 uiRowIndex = 3;
    while (uiRowIndex < uiMaxRow)
    {
        if (GetStringFromCell(pSheet, uiRowIndex, 0).compare("END") == 0)
        {
            break;
        }

        UINT32 uiColIndex = 0;
        while (uiColIndex < uiMaxCol)
        {
            if (uiColIndex <= 4)
            {
                paramSet.AddParam(GetStringFromCell(pSheet, 1, uiColIndex).c_str(),
                                  GetStringFromCell(pSheet, uiRowIndex, uiColIndex).c_str());
                if (uiColIndex == 4)
                {
                    if (m_pProductMgr->AddProduct(&paramSet) < 0)
                    {
                        return FALSE;
                    }
                    uiProductID = paramSet.GetUint(S_PARAM_TYPE);
                    paramSet.Clear();
                }
            }
            else
            {
                if (GetStringFromCell(pSheet, uiRowIndex, uiColIndex).compare("END") == 0)
                {
                    break;
                }
                paramSet.AddParam(S_PARAM_PROCESS_TYPE, 
                                  GetStringFromCell(pSheet, 1, uiColIndex).c_str());
                paramSet.AddParam(S_PARAM_VALUE,
                                  GetStringFromCell(pSheet, uiRowIndex, uiColIndex).c_str());

                if (m_pProductMgr->AddProductivity(uiProductID, &paramSet) < 0)
                {
                    return FALSE;
                }
                paramSet.Clear();
            }

            uiColIndex++;
        }

        uiRowIndex++;
    }
    
    return TRUE;
}

BOOL CResouceMgr::InitProcess(BasicExcelWorksheet *pSheet)
{
    UINT32 uiMaxRow = pSheet->GetTotalRows();
    CParamSet paramSet;
    
    UINT32 uiRowIndex = 1;
    while (uiRowIndex < uiMaxRow)
    {
        if (GetStringFromCell(pSheet, uiRowIndex, 2).compare("END") == 0)
        {
            break;
        }
        
        paramSet.AddParam(S_PARAM_TYPE, 
                          GetStringFromCell(pSheet, uiRowIndex, 0).c_str());
        paramSet.AddParam(S_PARAM_NAME,
                          GetStringFromCell(pSheet, uiRowIndex, 2).c_str());
        paramSet.AddParam(S_PARAM_SWITCH_TIME,
                          GetStringFromCell(pSheet, uiRowIndex, 3).c_str());

        if (m_pProcessMgr->AddProcess(&paramSet) < 0)
        {
            return FALSE;
        }
        paramSet.Clear();
        
        uiRowIndex++;
    }
    
    return TRUE;
}

BOOL CResouceMgr::InitMachine(BasicExcelWorksheet *pSheet)
{
    UINT32 uiMaxRow = pSheet->GetTotalRows();
    UINT32 uiMaxCol = pSheet->GetTotalCols();
    CParamSet paramSet;
    
    UINT32 uiProcessIndex = 0;
    IProcess *pProcess = NULL;
    UINT32 uiRowIndex = 2;
    while (uiRowIndex < uiMaxRow)
    {
        if (GetStringFromCell(pSheet, uiRowIndex, 0).compare("END") == 0)
        {
            break;
        }
        
        UINT32 uiColIndex = 0;
        while (uiColIndex < uiMaxCol)
        {
            paramSet.AddParam(GetStringFromCell(pSheet, 0, uiColIndex).c_str(),
                              GetStringFromCell(pSheet, uiRowIndex, uiColIndex).c_str());
            uiColIndex++;
        }
        
        uiProcessIndex = paramSet.GetUint(S_PARAM_PROCESS_INDEX) - 1;
        pProcess = m_pProcessMgr->GetProcess(uiProcessIndex);
        if (!pProcess)
        {
            TRACE("InitMachine", "GetProcess [Index=%d] failed", uiProcessIndex);
            return FALSE;
        }

        if (m_pMachineMgr->AddMachine(pProcess, 0, &paramSet) < 0)
        {
            return FALSE;
        }
        paramSet.Clear();

        uiRowIndex++;
    }
    
    return TRUE;
}

BOOL CResouceMgr::InitLocalParamSet(BasicExcelWorksheet *pSheet)
{
    UINT32 uiMaxRow = pSheet->GetTotalRows();
    
    UINT32 uiRowIndex = 1;
    while (uiRowIndex < uiMaxRow)
    {
        if (GetStringFromCell(pSheet, uiRowIndex, 0).compare("END") == 0)
        {
            break;
        }
        
        m_pLocalParamSet->AddParam(GetStringFromCell(pSheet, uiRowIndex, 0).c_str(),
                                   GetStringFromCell(pSheet, uiRowIndex, 3).c_str());

        uiRowIndex++;
    }
    
    return TRUE;
}

BOOL CResouceMgr::InitOrder(BasicExcelWorksheet *pSheet)
{
    UINT32 uiMaxRow = pSheet->GetTotalRows();
    UINT32 uiOrderID = 0;
    UINT32 uiProductID = 0;
    UINT32 uiNumber = 0;
    COrder *pOrder = NULL;

    UINT32 uiRowIndex = 1;
    while (uiRowIndex < uiMaxRow)
    {
        if (GetStringFromCell(pSheet, uiRowIndex, 0).compare("END") == 0)
        {
            break;
        }
        
        uiOrderID = GetIntFromCell(pSheet, uiRowIndex, 0);
        
        pOrder = m_pOrderMgr->GetOrderByID(uiOrderID);
        if (NULL == pOrder)
        {
            pOrder = m_pOrderMgr->AddOrder(uiOrderID);
        }

        if (NULL == pOrder)
        {
            return FALSE;
        }

        uiProductID = GetIntFromCell(pSheet, uiRowIndex, 1);
        uiNumber = GetIntFromCell(pSheet, uiRowIndex, 2);

        if (pOrder->AddProductInfo(uiProductID, uiNumber) < 0)
        {
            return FALSE;
        }
        
        uiRowIndex++;
    }
    
    return TRUE;
}

