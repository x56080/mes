

#include "../../include/interface.h"
#include "../../include/tool.h"
#include "../../include/algorith_def.h"
#include "../../include/plan_writer/plan_writer_i.h"


void PrintSolution(ISolution *pSolution)
{
    
}

int main(int argc, char *agrv[])
{
    TOOL::InitTrace("./trace", "resouce_mgr_test");
    TRACE("TEST", "Start test programme...");

    /* 	Test Load_Config  */
    IResouce *pResouce = CreateResouce("config/mes_config.xls");
    if (NULL == pResouce)
    {
        printf("test [%s] failed.\n", "Load_Config");
        return -1;
    }
    else
    {
        printf("test [%s] ok.\n", "Load_Config");
    }

    IPlanWriter *pWriter = CreatePlanWriter(pResouce);

    /*  Test CalcUnit */
    ICalcUint *pCalcUnit = CreateCalcUnit(pResouce);
    if (pCalcUnit->Init(pResouce->GetConfigSet()) < 0)
    {
        printf("test [%s] failed.\n", "CalcUnit");
    }
    else
    {
        pCalcUnit->Start();
        pCalcUnit->Wait();

        ISolution *pBest = pCalcUnit->GetBestSolution();
        PrintSolution(pBest);
        pWriter->Write("plan_result.xls", pBest);
        printf("test [%s] ok.\n", "CalcUnit");
    }

    ReleaseCalcUnit(pCalcUnit);
    ReleasePlanWriter(pWriter);
    ReleaseResouce(pResouce);

    TRACE("TEST", "End test programme.\n\n");
    return 0;
}

