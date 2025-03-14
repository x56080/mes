
#include "../../include/export_dll.h"
#include "../../include/param_macro.h"
#include "../../include/tool.h"
#include "calc_unit.h"
#include "evolve_unit.h"
#include "evaluate_unit.h"

/*
CCalcMatrix::CCalcMatrix(UINT32 uiLine, UINT32 uiColum)
{
    m_uiLine = uiLine;
    m_uiColum = uiColum;
    m_uiNum = m_uiLine * m_uiColum;

    m_pMatrix = new DOUBLE[m_uiNum];
    for (UINT32 uiIndex = 0; uiIndex < m_uiNum; uiIndex++)
    {
        m_pMatrix[uiIndex] = 0.0;
    }
}

CCalcMatrix::~CCalcMatrix()
{
    if (NULL != m_pMatrix)
    {
        delete m_pMatrix, m_pMatrix = NULL;
    }
}

BOOL CCalcMatrix::SetValue(UINT32 uiIndex, DOUBLE dbValue)
{
    if (!m_pMatrix || uiIndex >= m_uiNum)
    {
        return FALSE;
    }

    m_pMatrix[uiIndex] = dbValue;

    return TRUE;
}

BOOL CCalcMatrix::SetValue(UINT32 uiLine, UINT32 uiColum, DOUBLE dbValue)
{
    return SetValue(CalcIndex(uiLine, uiColum), dbValue);
}

DOUBLE CCalcMatrix::GetValue(UINT32 uiIndex)
{
    if (!m_pMatrix || uiIndex >= m_uiNum)
    {
        return -1.0;
    }

    return m_pMatrix[uiIndex];
}

DOUBLE CCalcMatrix::GetValue(UINT32 uiLine, UINT32 uiColum)
{
    return GetValue(CalcIndex(uiLine, uiColum));
}

UINT32 CCalcMatrix::GetColum() const
{
    return m_uiColum;
}

UINT32 CCalcMatrix::GetLine() const
{
    return m_uiLine;
}

UINT32 CCalcMatrix::GetNum() const
{
    return m_uiNum;
}

UINT32 CCalcMatrix::CalcIndex(UINT32 uiLine, UINT32 uiColum)
{
    return uiLine * m_uiColum + uiColum;
}
*/

//////////////////////////////////////////////////////////////////////////

CCalcStrategy::CCalcStrategy(IResouce *pResouce, time_t tBastTime)
{
    m_pResource = pResouce;
    m_tBaseTime = tBastTime;
    m_pSolution = NULL;
    m_uiCurLine = 0;
}

CCalcStrategy::~CCalcStrategy()
{
    m_pResource = NULL;
}

INT32 CCalcStrategy::MakeOrderMap(CSolution *pSolution, UINT32 uiLine)
{
    //清空
    m_mapMachineToDNASet.clear();
    MAP< UINT32, LIST<CDNA*> >::iterator it;

    m_pSolution = pSolution;
    m_uiCurLine = uiLine;

    CDNA *pDNA = NULL;
    IGene *pGene = NULL;
    UINT32 uiIndex = 0;
    while (uiIndex < pSolution->GetColum())
    {
        pDNA = (CDNA*)pSolution->GetDNA(uiLine, uiIndex);
        pGene = pDNA->GetGene();
        pDNA->SetMachineID(pGene->CalcMachineID((UINT32)pDNA->GetValue()));

        it = m_mapMachineToDNASet.find(pDNA->GetMachineID());
        if (it == m_mapMachineToDNASet.end())
        {
            LIST<CDNA*> lstDNA;
            lstDNA.push_back(pDNA);
            m_mapMachineToDNASet[pDNA->GetMachineID()] = lstDNA;
            uiIndex++;
            continue;
        }

        //对同一MachineID的DNA进行排序
        LIST<CDNA*> &lstDNA = it->second;
        LIST<CDNA*>::iterator itDNA = lstDNA.begin();
        CDNA *pDNAIndex = NULL;
        CDNA *pDNAIndexPre = NULL;
        CDNA *pDNAPre = NULL;
        while (itDNA != lstDNA.end())
        {
            pDNAIndex = (*itDNA);

            //第一道工只要比较随机值
            if (uiLine == 0)
            {
                if (pDNA->GetValue() < pDNAIndex->GetValue())
                {
                    lstDNA.insert(itDNA, pDNA);
                    break;
                }
            }
            //其它需要根据前一工序结束时间判断
            else
            {
                pDNAIndexPre = (CDNA*)pSolution->GetDNA(uiLine - 1, pDNAIndex->GetPosColum());
                pDNAPre = (CDNA*)pSolution->GetDNA(uiLine - 1, uiIndex);

                //上道工序先结束的先开始
                //if (pDNAPre->GetEndTime() < pDNAIndexPre->GetEndTime())
                if (pDNAPre->GetNStartTime() < pDNAIndexPre->GetNStartTime())
                {
                    lstDNA.insert(itDNA, pDNA);
                    break;
                }
                //同时结束则比较值
                //if (pDNAPre->GetEndTime() == pDNAIndex->GetEndTime())
                if (pDNAPre->GetNStartTime() == pDNAIndex->GetNStartTime())
                {
                    if (pDNA->GetValue() < pDNAIndex->GetValue())
                    {
                        lstDNA.insert(itDNA, pDNA);
                        break;
                    }
                }
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

INT32 CCalcStrategy::Calc()
{
    if (m_mapMachineToDNASet.empty() || NULL == m_pSolution)
    {
        return RET_ERR;
    }

    //计算时间
    
    IProductMgr *pProductMgr = m_pResource->GetProductMgr();
    CProductProperty *pProperty = NULL;
    UINT32 uiProductivity = 0;  //产能相关变量

    IProcess *pProcess = m_pResource->GetProcessMgr()->GetProcess(m_uiCurLine);
    CDNA *pDNAPrev = NULL;
    CDNA *pDNACur = NULL;
    CDNA *pLast = NULL;
    time_t tBeginTime = 0;
    time_t tPrevEndTime = 0;  //上道工序结束时间
    time_t tSwitchTime = 0;
    time_t tProduceTime = 0;
    MAP< UINT32, LIST<CDNA*> >::iterator it = m_mapMachineToDNASet.begin();
    while (it != m_mapMachineToDNASet.end())
    {
        LIST<CDNA*> &lstDNA = it->second;

        //当工序不为第一道工序时，需要根据上道工序计算开始时间
        tBeginTime = m_tBaseTime;
        tPrevEndTime = m_tBaseTime;
        tSwitchTime = 0;
        tProduceTime = 0;
        pDNACur = NULL;
        pLast = NULL;
        pDNAPrev = NULL;
        LIST<CDNA*>::iterator itDNA = lstDNA.begin();
        while (itDNA != lstDNA.end())
        {
            pLast = pDNACur;
            pDNACur = *itDNA;
            if (m_uiCurLine > 0)
            {
                pDNAPrev = (CDNA*)m_pSolution->GetDNA(m_uiCurLine-1, pDNACur->GetPosColum());
            }

            //需要生产的时间 = 数量 * 产能
            pProperty = pProductMgr->GetProductProperty(pDNACur->GetProductInfo()->uiProductID);
            uiProductivity = pProperty->GetProductivity(pProcess->GetType());
            tProduceTime = pDNACur->GetProductInfo()->uiProductNum * uiProductivity;
            
            if (pDNAPrev)
            {
                //tPrevEndTime = pDNAPrev->GetEndTime();
                tPrevEndTime = pDNAPrev->GetNStartTime();
            }

            if (!pLast)
            {
                tBeginTime = tPrevEndTime;
                tSwitchTime = 0;
            }
            else
            {
                tBeginTime = pLast->GetEndTime();
                //设置切换时间
                tSwitchTime = pProcess->CalcSwitchTime(pLast->GetProductInfo()->uiProductID,
                                                       pDNACur->GetProductInfo()->uiProductID);
            }

            pDNACur->SetSwitchTime(tSwitchTime);
            
            //计算开始时间
            if (tBeginTime + tSwitchTime < tPrevEndTime)
            {
                pDNACur->SetIdleTime(tPrevEndTime - tBeginTime - tSwitchTime);
                tBeginTime = tPrevEndTime - tSwitchTime;
            }

            pDNACur->SetStartTime(tBeginTime);
            pDNACur->SetProduceTime(tProduceTime);

            //计算下一道工序的最早开始时间
            if (m_uiCurLine + 1 < m_pSolution->GetLine())
            {
                time_t tProduceTimeTmp = (pDNACur->GetProductInfo()->uiProductNum + 2) * uiProductivity;
                UINT32 uiNProductivity = pProperty->GetProductivity(
                    m_pResource->GetProcessMgr()->Index2Type(m_uiCurLine + 1));
                time_t tNProduceTimeTmp = pDNACur->GetProductInfo()->uiProductNum * uiNProductivity;

                //下道工序完成时间快
                if (tNProduceTimeTmp < tProduceTimeTmp)
                {
                    pDNACur->SetNStartOffTime(tProduceTimeTmp - tNProduceTimeTmp);
                }
                else
                {
                    pDNACur->SetNStartOffTime(0);
                }
            }

            ++itDNA;
        }
       
        ++it;
    }

    return RET_SUC;
}

INT32 CCalcStrategy::Adjust()
{
    IProcessMgr *pProcessMgr = m_pResource->GetProcessMgr();
    IProductMgr *pProductMgr = m_pResource->GetProductMgr();
    
    //if (m_uiCurLine == 0
    //    || TYPE_TEMPERING != pProcessMgr->Index2Type(m_uiCurLine))
    //{
        return RET_SUC;
    //}

    time_t tBeginTime = 0;
    //time_t tPrevEndTime = 0;
    CProductProperty *pProperty = NULL;
    CDNA *pCurDNA = NULL;
    CDNA *pLastDNA = NULL;
    CDNA *pPrevDNA = NULL;
    UINT32 uiProductNum = 0;
    UINT32 uiPrevProductivity = 0; //上道工序产能
    UINT32 uiProductivity = 0; //本工序产能
    UINT32 uiNumPerTime = 0; //热处理每斗装入该产品的个数

    MAP< UINT32, LIST<CDNA*> >::iterator it = m_mapMachineToDNASet.begin();
    while (it != m_mapMachineToDNASet.end())
    {
        LIST<CDNA*> &lstDNA = it->second;
        tBeginTime = m_tBaseTime;
        pCurDNA = NULL;
        pLastDNA = NULL;
        pPrevDNA = NULL;

        LIST<CDNA*>::iterator itDNA = lstDNA.begin();
        while (itDNA != lstDNA.end())
        {
            pLastDNA = pCurDNA;
            pCurDNA = *itDNA;
            pPrevDNA = (CDNA*)m_pSolution->GetDNA(m_uiCurLine-1, pCurDNA->GetPosColum());

            uiProductNum = pCurDNA->GetProductInfo()->uiProductNum;
            pProperty = pProductMgr->GetProductProperty(pCurDNA->GetProductInfo()->uiProductID);
            uiProductivity = pProperty->GetProductivity(pCurDNA->GetProcessType());
            uiPrevProductivity = pProperty->GetProductivity(pPrevDNA->GetProcessType());
            uiNumPerTime = 3600 / uiProductivity;

            if (!pLastDNA) //第1个产品
            {
                if (uiProductNum < uiNumPerTime)
                {
                    tBeginTime = pPrevDNA->GetEndTime();
                }
                else
                {
                    tBeginTime = pPrevDNA->GetStartTime() + uiNumPerTime * uiPrevProductivity;
                }
            }

            pCurDNA->SetIdleTime(0);
            pCurDNA->SetStartTime(tBeginTime);
            pCurDNA->SetSwitchTime(0);

            //计算结束时间
            
            pCurDNA->SetProduceTime((uiProductNum / uiNumPerTime + 1) * 9 *3600);
            pCurDNA->SetNStartOffTime(pCurDNA->GetProduceTime());

            ++itDNA;
        }        
        

        ++it;
    }

    return RET_SUC;
}

//////////////////////////////////////////////////////////////////////////

CCalcUnit::CCalcUnit(IResouce *pResouce)
{
    m_pResouce = pResouce;
    m_pOrderMgr = m_pResouce->GetOrderMgr();
    m_pProcessMgr = m_pResouce->GetProcessMgr();
    m_pProductMgr = m_pResouce->GetProductMgr();
    m_pMachineMgr = m_pResouce->GetMachineMgr();

    m_uiSolutionSize = 0;
    m_uiExchangeSize = 0;
    m_pSolutionSet = NULL;
    m_pNextGSolutionSet = NULL;
    m_pExchangeSet = NULL;
    m_pCalcCallback = NULL;

    m_pEvaluateUnit = new CEvaluateUnit(pResouce);
    m_pEvolveUnit = new CEvolveUnit(pResouce);
    m_pParamSet = NULL;

    m_bStopFlag = FALSE;
    m_bCalcFlag = FALSE;
}

CCalcUnit::~CCalcUnit()
{
    if (NULL != m_pSolutionSet)
    {
        delete m_pSolutionSet, m_pSolutionSet = NULL;
    }

    if (NULL != m_pNextGSolutionSet)
    {
        delete m_pNextGSolutionSet, m_pNextGSolutionSet = NULL;
    }

    if (NULL != m_pExchangeSet)
    {
        delete m_pExchangeSet, m_pExchangeSet = NULL;
    }

    if (NULL != m_pEvolveUnit)
    {
        delete m_pEvolveUnit, m_pEvolveUnit = NULL;
    }

    if (NULL != m_pEvaluateUnit)
    {
        delete m_pEvaluateUnit, m_pEvaluateUnit = NULL;
    }

    m_pOrderMgr = NULL;
    m_pProcessMgr = NULL;
    m_pProductMgr = NULL;
    m_pMachineMgr = NULL;
    m_pResouce = NULL;
    m_pParamSet = NULL;
    m_pCalcCallback = NULL;
    
    m_tBaseTime = 0;
}

INT32 CCalcUnit::Init(IParamSet *pParamSet)
{
    if (NULL == pParamSet)
    {
        return RET_ERR;
    }
    m_pParamSet = pParamSet;

    STRING strBaseTime;
    //从配置中加载参数
    // {{
    RDX_Int(pParamSet, m_uiSolutionSize, S_PARAM_SOLUTION_SIZE);
    RDX_Int(pParamSet, m_uiExchangeSize, S_PARAM_EXCHANGE_SIZE);
    RDX_String(pParamSet, strBaseTime, S_PARAM_BEGIN_BASE_TIME);
    // }}

    m_tBaseTime = TOOL::String2Time(strBaseTime);
    if (m_tBaseTime < 0)
    {
        TRACE("CCalcUnit::Init", "Param[%s] value[%s] config error", 
              S_PARAM_BEGIN_BASE_TIME, strBaseTime.c_str());
    }

    //初始化随机种子
    srand(time(NULL));
    
    //初始化解集
    m_pSolutionSet = new CSolutionSet();
    if (m_pSolutionSet->Init(m_uiSolutionSize + m_uiExchangeSize, m_pResouce) < 0)
    {
        return RET_ERR;
    }

    //初始化下一代解集
    m_pNextGSolutionSet = new CSolutionSet();
    if (m_pNextGSolutionSet->Init(m_uiSolutionSize, m_pResouce) < 0)
    {
        return RET_ERR;
    }
    
    //初始化交换集
    m_pExchangeSet = new CSolutionSet();
    if (m_pExchangeSet->Init(m_uiExchangeSize, m_pResouce) < 0)
    {
        return RET_ERR;
    }

    //初始化进化单元
    if (m_pEvolveUnit->Init(pParamSet) < 0)
    {
        TRACE("CCalcUnit::Init", "Init EvolveUnit failed");
        return RET_ERR;
    }

    //初始化评估单元
    if (m_pEvaluateUnit->Init(pParamSet) < 0)
    {
        TRACE("CCalcUnit::Init", "Init EvaluateUnit failed");
        return RET_ERR;
    }

    return RET_SUC;
}

INT32 CCalcUnit::ProductSolution()
{
    UINT32 uiGeneration = 0;

    //读取计算参数
    //{{
    RDX_Int(m_pParamSet, uiGeneration, S_PARAM_GENERATION);
    //}}

    UINT32 uiGenerationIndex = 0;
    m_bStopFlag = FALSE;

    //产生随机解
    RandSolutionSet(m_pSolutionSet, 0);

    while (uiGenerationIndex++ < uiGeneration && m_bStopFlag == FALSE)
    {
        //计算合理解
        CalcSolutionSet(m_pSolutionSet);
        //计算适应度
        CalcFitness(m_pSolutionSet);
        //排序
        m_pSolutionSet->MakeOrder();

        //进行回调
        if (m_pCalcCallback)
        {
            m_pCalcCallback(uiGenerationIndex, GetSolutionSet(), GetBestSolution());
        }
        
        if (uiGenerationIndex >= uiGeneration)
        {
            break;
        }

        //选择交换集
        Select(m_pSolutionSet, m_pExchangeSet, m_pExchangeSet->GetNum());
        
        //选择最优的N个解进入下一代
        Select(m_pSolutionSet, m_pNextGSolutionSet, m_pNextGSolutionSet->GetNum());
        //对下一代进行进化
        EvolveSet(m_pNextGSolutionSet);

        //形成新的计算解集
        MakeCalcSolutionSet(m_pSolutionSet, m_pNextGSolutionSet, m_pExchangeSet);
    }

    return RET_SUC;
}

INT32 CCalcUnit::RandSolution(CSolution *pSolution)
{
    UINT32 uiIndex = 0;
    CDNA *pDNA = NULL;
    IGene *pGene = NULL;
    UINT32 uiMachineNum = 0;

    //工序
    for (; uiIndex < pSolution->GetDNANum(); uiIndex++)
    {
        pDNA = static_cast<CDNA*>(pSolution->GetDNA(uiIndex));
        pGene = pDNA->GetGene();

        uiMachineNum = pGene->GetMachineNum();
        //产生随机数
        pDNA->SetValue(TOOL::Randomf(uiMachineNum, FALSE));        
    }

    return RET_SUC;
}

INT32 CCalcUnit::RandSolutionSet(CSolutionSet *pSolutionSet, UINT32 uiIndex)
{
    while (uiIndex < pSolutionSet->GetNum())
    {
        if (RandSolution(static_cast<CSolution*>(pSolutionSet->GetSolution(uiIndex))) < 0)
        {
            return RET_ERR;
        }
        uiIndex++;
    }

    return RET_SUC;
}

INT32 CCalcUnit::CalcSolution(CSolution *pSolution)
{
    if (!pSolution)
    {
        return RET_ERR;
    }

    CCalcStrategy strategy(m_pResouce, m_tBaseTime);
    UINT32 uiLineIndex = 0;

    while (uiLineIndex < pSolution->GetLine())
    {
        strategy.MakeOrderMap(pSolution, uiLineIndex);
        if (strategy.Calc() < 0 || strategy.Adjust() < 0)
        {
            TRACE("CCalcUnit::CalcSolution", "Calc solution error");
            return RET_ERR;
        }

        uiLineIndex++;
    }

    return RET_SUC;
}

INT32 CCalcUnit::CalcSolutionSet(CSolutionSet *pSolutionSet)
{
    UINT32 uiIndex = 0;
    while (uiIndex < pSolutionSet->GetNum())
    {
        if (CalcSolution(static_cast<CSolution*>(pSolutionSet->GetSolution(uiIndex))) < 0)
        {
            return RET_ERR;
        }
        uiIndex++;
    }
    
    return RET_SUC;
}

INT32 CCalcUnit::CalcFitness(CSolutionSet *pSolutionSet)
{
    if (!m_pEvaluateUnit)
    {
        return RET_ERR;
    }
    
    UINT32 uiIndex = 0;
    while (uiIndex < pSolutionSet->GetNum())
    {
        if (m_pEvaluateUnit->Evaludate(pSolutionSet->GetSolution(uiIndex)) < 0)
        {
            return RET_ERR;
        }
        uiIndex++;
    }
    
    return RET_SUC;
}

INT32 CCalcUnit::Select(CSolutionSet *pParent, CSolutionSet *pSon, UINT32 uiNum)
{
    UINT32 uiIndex = 0;
    if (uiNum > pSon->GetNum())
    {
        uiNum = pSon->GetNum();
    }

    INT32 iPos = pParent->GetOrderFirstPos();
    CSolution *pSolution = NULL;

    for (; uiIndex < uiNum; uiIndex++)
    {
        pSolution = static_cast<CSolution*>(pParent->GetOrderNext(iPos));
        if (!pSolution)
        {
            break;
        }

        *(static_cast<CSolution*>(pSon->GetSolution(uiIndex))) = *pSolution;
    }

    return uiIndex;
}

INT32 CCalcUnit::EvolveSet(CSolutionSet *pSolutionSet)
{
    if (!m_pEvolveUnit)
    {
        return RET_ERR;
    }

    return m_pEvolveUnit->Evolve(pSolutionSet);
}

INT32 CCalcUnit::MakeCalcSolutionSet(CSolutionSet *pNewSet, 
                                     CSolutionSet *pSet, 
                                     CSolutionSet *pExchangeSet)
{
    UINT32 uiIndex = 0;
    UINT32 uiNewIndex = 0;
    UINT32 uiSubIndex = 0;
    CSolutionSet *pSubSet = pSet;
    CSolution *pSolution = NULL;

    while (uiIndex < pNewSet->GetNum())
    {
        if (uiSubIndex >= pSet->GetNum())
        {
            if (pSubSet == pSet)
            {
                uiSubIndex = 0;
                pSubSet = pExchangeSet;
                continue;
            }
            else
            {
                break;
            }
        }

        pSolution = static_cast<CSolution*>(pSubSet->GetSolution(uiSubIndex));
        uiSubIndex++;
        uiIndex++;
        
        //找到相同的，则跳过
        if (FALSE == pNewSet->FindLeft(pSolution, uiNewIndex))
        {
            *(static_cast<CSolution*>(pNewSet->GetSolution(uiNewIndex))) = *pSolution;
            uiNewIndex++;
        }
    }

    //还有空的则随机生成
    return RandSolutionSet(pNewSet, uiNewIndex);
}

ISolutionSet* CCalcUnit::GetSolutionSet()
{
    return m_pSolutionSet;
}

ISolution* CCalcUnit::GetBestSolution()
{
    INT32 iPos = m_pSolutionSet->GetOrderFirstPos();
    return m_pSolutionSet->GetOrderNext(iPos);
}

INT32 CCalcUnit::Start(CALC_CALLBACK_FUNC pCallback /* = NULL */)
{
    //已经开始，则反回失败
    if (m_bCalcFlag)
    {
        TRACE("CCalcUnit::Start", "Alread Start calculate");
        return RET_ERR;
    }

    m_pCalcCallback = pCallback;

    //启动计算线程
    if (m_ThdMgr.spawn((ACE_THR_FUNC)ThreadFunc, this) == -1)
    {
        TRACE("CCalcUnit::Start", "Start calc thread failed");
        return RET_ERR;
    }

    m_bCalcFlag = TRUE;

    return RET_SUC;
}

void CCalcUnit::ThreadFunc(CCalcUnit *pCalcUnit)
{
    pCalcUnit->ProductSolution();
    pCalcUnit->m_bCalcFlag = FALSE;
}

BOOL CCalcUnit::IsFinish() const
{
    return (m_bCalcFlag == FALSE) ? TRUE : FALSE;
}

INT32 CCalcUnit::Stop()
{
    m_bStopFlag = TRUE;
    return Wait();
}

INT32 CCalcUnit::Wait(UINT32 uiTimeout /* = 0 */)
{
    if (uiTimeout == 0)
    {
        return m_ThdMgr.wait();
    }
    else
    {
        ACE_Time_Value val(uiTimeout, 0);
        val += ACE_OS::gettimeofday();
        return m_ThdMgr.wait(&val);
    }
}

//////////////////////////////////////////////////////////////////////////

ICalcUint* CreateCalcUnit(IResouce *pReousce)
{
    CCalcUnit *pCalcUnit = new CCalcUnit(pReousce);
    return pCalcUnit;
}

BOOL ReleaseCalcUnit(ICalcUint *pCalcUnit)
{
    if (pCalcUnit)
    {
        delete pCalcUnit, pCalcUnit = NULL;
    }

    return TRUE;
}
