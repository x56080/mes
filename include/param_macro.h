
#ifndef __PARAM_MACRO_H__
#define __PARAM_MACRO_H__

#include "base_type.h"

//////////////////////////////////////////////////////////////////////////
//算法参数宏定义
//////////////////////////////////////////////////////////////////////////

//解集规模大小
#define S_PARAM_SOLUTION_SIZE               "SolutionSize"

//交换集大小
#define S_PARAM_EXCHANGE_SIZE               "ExchangeSize"

//开始的基准时间, 格式为 "YYYY-MM-DD HH:MM:SS"
#define S_PARAM_BEGIN_BASE_TIME             "BeginBaseTime"

//最大库存大小
#define S_PARAM_MAX_STORE_NUM               "MaxStoreNum"

//遗传算法的代数
#define S_PARAM_GENERATION                  "Generation"

//变异开关
#define S_PARAM_USE_MUTATION                "UseMutation"
//变异的概率
#define S_PARAM_PM_VALUE                    "Pm"
//变异的算法(1:  完全随机 2: 微调)
#define S_PARAM_PM_METHOD                   "PmMethod"

//交叉开关
#define S_PARAM_USE_CROSSOVER               "UseCrossOver"
//交叉遗传概率
#define S_PARAM_PC_VALUE                    "Pc"

//逆转开关
#define S_PARAM_USE_REVERSE                 "UseReverse"
//逆转概率
#define S_PARAM_PR_VALUE                    "Pr"


//////////////////////////////////////////////////////////////////////////
//资源管理参数宏定义
//////////////////////////////////////////////////////////////////////////

//Machine定义
#define S_PARAM_ID                          "ID"
#define S_PARAM_NAME                        "Name"
#define S_PARAM_PRODUCT_SPEC                "ProductSpec"
#define S_PARAM_PROCESS_INDEX               "ProcessIndex"

//Process定义
#define S_PARAM_TYPE                        "Type"
#define S_PARAM_SWITCH_TIME                 "SwitchTime"

//Product定义
#define S_PARAM_SPEC                        "Spec"
#define S_PARAM_ET                          "ET"
#define S_PARAM_CB                          "CB"
#define S_PARAM_PCD                         "PCD"
#define S_PARAM_PROCESS_TYPE                "ProcessType"
#define S_PARAM_VALUE                       "Value"

#endif //__PARAM_MACRO_H__
