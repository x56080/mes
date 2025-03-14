
#include "../../include/config_loader/excel_func.h"
#include "../../include/tool.h"


STRING GetStringFromCell(BasicExcelCell *cell)
{
    char szTmp[MAX_PATH] = {0};
    
    switch (cell->Type())
    {
    case BasicExcelCell::UNDEFINED:
        TRACE(MTS_CONFIG_LOADER, "Excel cell type UNDEFINED is error, expect string");
        return "";
        
    case BasicExcelCell::INT:
        {
            itoa(cell->GetInteger(),szTmp, 10);
            return szTmp;
        }
        
    case BasicExcelCell::eDOUBLE:
        {
            _snprintf(szTmp, MAX_PATH-1, "%f", cell->GetDouble());
            return szTmp;
        }
        
    case BasicExcelCell::eSTRING:
        return cell->GetString();
        
    case BasicExcelCell::WSTRING:
        {
            TRACE(MTS_CONFIG_LOADER, "Excel cell type WSTRING is error, expect string");
            return "";
        }
    default:
        TRACE(MTS_CONFIG_LOADER, "Excel cell type Unknow is error, expect string");
        return "";
    }
}

STRING GetStringFromCell(BasicExcelWorksheet *pSheet, UINT32 uiRow, UINT32 uiCol)
{
    if (!pSheet)
    {
        return "";
    }

    BasicExcelCell *pCell = pSheet->Cell(uiRow, uiCol);
    if (!pCell)
    {
        return "";
    }

    return GetStringFromCell(pCell);
}

INT32 GetIntFromCell(BasicExcelCell *cell)
{
    switch (cell->Type())
    {
    case BasicExcelCell::UNDEFINED:
        TRACE(MTS_CONFIG_LOADER, "Excel cell type UNDEFINED is error, expect integer");
        return RET_ERR;
        
    case BasicExcelCell::INT:
        return cell->GetInteger();
        
    case BasicExcelCell::eDOUBLE:
        return cell->GetDouble();
        return RET_ERR;

    case BasicExcelCell::eSTRING:
        return atoi(cell->GetString());
        
    case BasicExcelCell::WSTRING:
        TRACE(MTS_CONFIG_LOADER, "Excel cell type WSTRING is error, expect integer");
        return RET_ERR;
        
    default:
        break;
    }
    
    return RET_ERR;
}

INT32 GetIntFromCell(BasicExcelWorksheet *pSheet, UINT32 uiRow, UINT32 uiCol)
{
    if (!pSheet)
    {
        return RET_ERR;
    }
    
    BasicExcelCell *pCell = pSheet->Cell(uiRow, uiCol);
    if (!pCell)
    {
        return RET_ERR;
    }
    
    return GetIntFromCell(pCell);
}

DOUBLE GetDoubleFromCell(BasicExcelCell *cell)
{
    switch (cell->Type())
    {
    case BasicExcelCell::UNDEFINED:
        TRACE(MTS_CONFIG_LOADER, "Excel cell type UNDEFINED is error, expect double");
        return -1.0;
        
    case BasicExcelCell::INT:
         return cell->GetInteger();
        
    case BasicExcelCell::eDOUBLE:
         return cell->GetDouble();        
        
    case BasicExcelCell::eSTRING:
        return TOOL::String2Double(cell->GetString());
        
    case BasicExcelCell::WSTRING:
        TRACE(MTS_CONFIG_LOADER, "Excel cell type WSTRING is error, expect double");
        return -1.0;

    default:
        TRACE(MTS_CONFIG_LOADER, "Excel cell type Unknow is error, expect double");
        return -1.0;
    }
}

DOUBLE GetDoubleFromCell(BasicExcelWorksheet *pSheet, UINT32 uiRow, UINT32 uiCol)
{
    if (!pSheet)
    {
        return -1.0;
    }
    
    BasicExcelCell *pCell = pSheet->Cell(uiRow, uiCol);
    if (!pCell)
    {
        return -1.0;
    }
    
    return GetDoubleFromCell(pCell);
}
