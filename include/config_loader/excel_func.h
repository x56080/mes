
#ifndef __EXCEL_FUNC_H__
#define __EXCEL_FUNC_H__


#include "basic_excel.hpp"
using namespace YExcel;

#include "../base_type.h"

/*��Execel ��Ԫ���ж�ȡһ���ַ���
  ���󷵻�"" 
*/
STRING GetStringFromCell(BasicExcelCell *cell);
STRING GetStringFromCell(BasicExcelWorksheet *pSheet, UINT32 uiRow, UINT32 uiCol);

INT32 GetIntFromCell(BasicExcelCell *cell);
INT32 GetIntFromCell(BasicExcelWorksheet *pSheet, UINT32 uiRow, UINT32 uiCol);

DOUBLE GetDoubleFromCell(BasicExcelCell *cell);
DOUBLE GetDoubleFromCell(BasicExcelWorksheet *pSheet, UINT32 uiRow, UINT32 uiCol);

#endif //__EXCEL_FUNC_H__

