
#include "../../include/tool.h"
#include "../../include/param_macro.h"
#include "evolve_unit.h"

CEvolveUnit::CEvolveUnit(IResouce *pResouce)
{
    m_pResource = pResouce;
    m_pParamSet = NULL;
}

CEvolveUnit::~CEvolveUnit()
{
    m_pResource = NULL;
    m_pParamSet = NULL;
}

INT32 CEvolveUnit::Init(IParamSet *pParamSet)
{
    if (NULL == pParamSet)
    {
        return RET_ERR;
    }
    m_pParamSet = pParamSet;

    return RET_SUC;
}

INT32 CEvolveUnit::Evolve(ISolutionSet *pSolutionSet)
{
    UINT32 uiUseMutation = 0;
    UINT32 uiUseCrossOver = 0;
    UINT32 uiUseReverse = 0;

    //{{
    RDX_Int(m_pParamSet, uiUseMutation, S_PARAM_USE_MUTATION);
    RDX_Int(m_pParamSet, uiUseCrossOver, S_PARAM_USE_CROSSOVER);
    RDX_Int(m_pParamSet, uiUseReverse, S_PARAM_USE_REVERSE);
    //}}

    if (1 == uiUseMutation)
    {
        MutationSet(static_cast<CSolutionSet*>(pSolutionSet));
    }

    if (1 == uiUseCrossOver)
    {
        CrossOverSet(static_cast<CSolutionSet*>(pSolutionSet));
    }

    if (1 == uiUseReverse)
    {
        ReverseSet(static_cast<CSolutionSet*>(pSolutionSet));
    }

    return RET_SUC;
}

INT32 CEvolveUnit::MutationSet(CSolutionSet *pSolutionSet)
{
    UINT32 uiIndex = 0;
    
    for (; uiIndex < pSolutionSet->GetNum(); uiIndex++)
    {
        Mutation(static_cast<CSolution*>(pSolutionSet->GetSolution(uiIndex)));
    }

    return RET_SUC;
}

INT32 CEvolveUnit::Mutation(CSolution *pSolution)
{
    DOUBLE dbPm = 0.0;      //变异概率
    UINT32 uiMethod = MUTATION_RAND;

    //{{
    RDX_Double(m_pParamSet, dbPm, S_PARAM_PM_VALUE);
    RDX_Int(m_pParamSet, uiMethod, S_PARAM_PM_METHOD);
    //}}

    UINT32 uiDNANum = pSolution->GetDNANum();
    UINT32 uiIndex = 0;
    DOUBLE dbRand = 0;   //是否变异的随机值
    CDNA   *pDNA = NULL;
    CGene  *pGene = NULL;
    UINT32 uiMachineNum = 0;
    DOUBLE dbNewValue = 0.0;  //新产生的值
    INT32  iSgn = 1;    //符号
    DOUBLE dbMaxRandNum = 0.0;  //最大随机值

    for (; uiIndex < uiDNANum; uiIndex++)
    {
        //计算随机值
        dbRand = TOOL::Randomf(1, TRUE);
        if (dbRand < dbPm)
        {
            pDNA = static_cast<CDNA*>(pSolution->GetDNA(uiIndex));
            pGene = static_cast<CGene*>(pDNA->GetGene());
            uiMachineNum = pGene->GetMachineNum();

            dbNewValue = pDNA->GetValue();
            if (MUTATION_RAND == uiMethod)
            {
                dbNewValue = TOOL::Randomf(uiMachineNum, FALSE);
            }
            else if (MUTATION_ADJUST == uiMethod)
            {
                if (TOOL::Randomf(1, TRUE) > 0.5)
                {
                    iSgn = 1;
                    dbMaxRandNum = uiMachineNum - pDNA->GetValue();
                }
                else
                {
                    iSgn = -1;
                    dbMaxRandNum = pDNA->GetValue();
                }
                dbNewValue = pDNA->GetValue() + (iSgn * TOOL::Randomf(dbMaxRandNum, FALSE));
            }
            
            //设置新的值
            pDNA->SetValue(dbNewValue);
        }
    }

    return RET_SUC;
}

INT32 CEvolveUnit::CrossOverSet(CSolutionSet *pSolutionSet)
{
    //两两分组
    UINT32 uiSolutionNum = pSolutionSet->GetNum();
    UINT32 *pCoupleArray = new UINT32[uiSolutionNum];
    RandSerail(pCoupleArray, uiSolutionNum);

    DOUBLE dbPc = 0.0;      //交叉概率

    //{{
    RDX_Double(m_pParamSet, dbPc, S_PARAM_PC_VALUE);
    //}}

    DOUBLE dbRand = 0.0;
    UINT32 uiIndex = 0;
    
    for (; uiIndex < uiSolutionNum / 2; uiIndex++)
    {
        dbRand = TOOL::Randomf(1, TRUE);
        if (dbRand < dbPc)
        {
            CrossOver(static_cast<CSolution*>(pSolutionSet->GetSolution(pCoupleArray[2 * uiIndex])),
                      static_cast<CSolution*>(pSolutionSet->GetSolution(pCoupleArray[2 * uiIndex + 1])));
        }
    }

    delete pCoupleArray, pCoupleArray = NULL;

    return RET_SUC;
}

INT32 CEvolveUnit::CrossOver(CSolution *pOne, CSolution *pTwo)
{
    DOUBLE dbPc = 0.0;      //交叉概率
    UINT32 uiGeneNum = pOne->GetGeneNum();  //基因个数

    //{{
    RDX_Double(m_pParamSet, dbPc, S_PARAM_PC_VALUE);
    //}}

    //只能同基因交叉
    DOUBLE dbRand = 0.0;
    UINT32 uiGenIndex = 0;
    DOUBLE dbTemp = 0.0;
    UINT32 uiDNAIndex = 0;        //基因中DNA序号
    IGene  *pGene = NULL;
    UINT32 uiDNAInSolutinPos = 0; //DNA在Solution中的位置

    for (; uiGenIndex < uiGeneNum; uiGenIndex++)
    {
        dbRand = TOOL::Randomf(1, TRUE);
        if (dbRand < dbPc)
        {
            pGene = pOne->GetGene(uiGenIndex);
            for (uiDNAIndex = 0; uiDNAIndex < pGene->GetDNANum(); uiDNAIndex++)
            {
                uiDNAInSolutinPos = pGene->GetDNAIndex(uiDNAIndex);
                dbTemp = pOne->GetValue(uiDNAInSolutinPos);
                pOne->SetValue(uiDNAInSolutinPos, pTwo->GetValue(uiDNAInSolutinPos));
                pTwo->SetValue(uiDNAInSolutinPos, dbTemp);
            }
        }
    }
    
    return RET_SUC;
}

INT32 CEvolveUnit::RandSerail(UINT32 *pArray, UINT32 uiNum)
{
    UINT32 uiRand = 0;
    UINT32 uiTimes = 0;
    UINT32 uiIndex = 0;

    //初始化
    for (uiIndex = 0; uiIndex < uiNum; uiIndex++)
    {
        pArray[uiIndex] = uiNum;
    }

    for (uiIndex = 0; uiIndex < uiNum; uiIndex++)
    {
        uiRand = TOOL::Random(uiNum, FALSE);

        //找到一个空值
        while (pArray[uiRand] != uiNum)
        {
            if (uiTimes < 30)
            {
                uiRand = (uiRand + (uiRand + 13) * 3 / 7) % uiNum;
            }
            else
            {
                uiRand = (uiRand + 1) % uiNum;
            }

            uiTimes++;
        }

        pArray[uiRand] = uiIndex;
        uiTimes = 0;
    }

    return RET_SUC;
}

INT32 CEvolveUnit::Reverse(UINT32 uiBegin, UINT32 uiEnd, CGene *pGene, CSolution *pSolution)
{
    UINT32 uiDNANum = pGene->GetDNANum();
    UINT32 uiTimes = (uiBegin < uiEnd) ? (uiEnd - uiBegin + 1) : (uiDNANum - uiBegin + uiEnd + 1);
    uiTimes /= 2;

    UINT32 uiDNAPosBegin = 0;
    UINT32 uiDNAPosEnd = 0;
    DOUBLE dbTmp = 0.0;
    UINT32 uiIndex = 0;
    while (uiIndex++ < uiTimes)
    {
        uiDNAPosBegin = pGene->GetDNAIndex(uiBegin);
        uiDNAPosEnd = pGene->GetDNAIndex(uiEnd);
        dbTmp = pSolution->GetValue(uiDNAPosBegin);
        pSolution->SetValue(uiDNAPosBegin, pSolution->GetValue(uiDNAPosEnd));
        pSolution->SetValue(uiDNAPosEnd, dbTmp);

        uiBegin = (uiBegin + 1) % uiDNANum;
        uiEnd = (uiEnd - 1 + uiDNANum) % uiDNANum;
    }

    return RET_SUC;
}

INT32 CEvolveUnit::Reverse(CSolution *pSolution)
{
    //逆转只能在同基因中进行
    DOUBLE dbPr = 0.0;  //逆转概率

    //{{
    RDX_Double(m_pParamSet, dbPr, S_PARAM_PR_VALUE);
    //}}

    UINT32 uiGenNum = pSolution->GetGeneNum();
    DOUBLE dbRand = 0.0;
    UINT32 uiGenIndex = 0;
    UINT32 uiGenDNANum = 0;
    CGene *pGene = NULL;

    for (; uiGenIndex < uiGenNum; uiGenIndex++)
    {
        dbRand = TOOL::Randomf(1, TRUE);

        if (dbRand >= dbPr)
        {
            continue;
        }

        pGene = static_cast<CGene*>(pSolution->GetGene(uiGenIndex));
        uiGenDNANum = pGene->GetDNANum();

        Reverse(TOOL::Random(uiGenDNANum, FALSE), TOOL::Random(uiGenDNANum, FALSE),
                pGene, pSolution);
    }

    return RET_SUC;
}

INT32 CEvolveUnit::ReverseSet(CSolutionSet *pSolutionSet)
{
    DOUBLE dbPr = 0.0;

    //{{
    RDX_Double(m_pParamSet, dbPr, S_PARAM_PR_VALUE);
    //}}

    DOUBLE dbRand = 0.0;
    UINT32 uiIndex = 0;

    for (; uiIndex < pSolutionSet->GetNum(); uiIndex++)
    {
        if (TOOL::Randomf(1, TRUE) >= dbPr)
        {
            continue;
        }

        Reverse(static_cast<CSolution*>(pSolutionSet->GetSolution(uiIndex)));
    }

    return RET_SUC;
}

