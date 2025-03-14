
#ifndef __CALC_UNIT_H__
#define __CALC_UNIT_H__

#include "../../include/algorith_def.h"
#include "solution.h"

//计算矩阵
/*class CCalcMatrix
{
public:
    CCalcMatrix(UINT32 uiLine, UINT32 uiColum);
    ~CCalcMatrix();

public:
    
    //设置值
    BOOL        SetValue(UINT32 uiLine, UINT32 uiColum, DOUBLE dbValue);
    BOOL        SetValue(UINT32 uiIndex, DOUBLE dbValue);
    //获取值
    DOUBLE      GetValue(UINT32 uiLine, UINT32 uiColum);
    DOUBLE      GetValue(UINT32 uiIndex);
    
    UINT32      GetLine() const;
    UINT32      GetColum() const;
    UINT32      GetNum() const;

protected:
    
    UINT32      CalcIndex(UINT32 uiLine, UINT32 uiColum);

private:
    
    UINT32              m_uiLine;           //行--- 产品种类
    UINT32              m_uiColum;          //列--- 工序数量
    UINT32              m_uiNum;
    DOUBLE              *m_pMatrix;         //矩阵

};*/


//计算策略
class CCalcStrategy
{
public:
    CCalcStrategy(IResouce *pResouce, time_t tBastTime);
    ~CCalcStrategy();
    
public:
    
    INT32   MakeOrderMap(CSolution *pSolution, UINT32 uiLine);
    INT32   Calc();
    INT32   Adjust();

protected:
    

private:
    
    //机器ID与DNA集
    MAP< UINT32, LIST<CDNA*> >              m_mapMachineToDNASet;
    //资源
    IResouce                                *m_pResource;
    ISolution                               *m_pSolution;
    UINT32                                  m_uiCurLine;
    //BaseTime
    time_t                                  m_tBaseTime;
    
};

//计算单元
class CCalcUnit : public ICalcUint
{
public:
    CCalcUnit(IResouce *pResouce);
    ~CCalcUnit();

public:
    
    INT32           Init(IParamSet *pParamSet);
    
    INT32           Start(CALC_CALLBACK_FUNC pCallback = NULL);
    BOOL            IsFinish() const;
    INT32           Stop();
    INT32           Wait(UINT32 uiTimeout = 0);
    
    ISolutionSet*   GetSolutionSet();
    ISolution*      GetBestSolution();

protected:
    INT32           ProductSolution();
    static void     ThreadFunc(CCalcUnit *pCalcUnit);

protected:

    //产生一个随机解
    INT32       RandSolution(CSolution *pSolution);
    //产生一个随机解集
    INT32       RandSolutionSet(CSolutionSet *pSolutionSet, UINT32 uiIndex = 0);
    //计算适应度
    INT32       CalcFitness(CSolutionSet *pSolutionSet);
    //进化
    INT32       EvolveSet(CSolutionSet *pSolutionSet);

    //计算合理化的解
    INT32       CalcSolution(CSolution *pSolution);
    //计算合理化的解集
    INT32       CalcSolutionSet(CSolutionSet *pSolutionSet);

    //选择下一代
    INT32       Select(CSolutionSet *pParent, CSolutionSet *pSon, UINT32 uiNum);

    //形成新的计算解集
    INT32       MakeCalcSolutionSet(CSolutionSet *pNewSet, CSolutionSet *pSet,
                                    CSolutionSet *pExchangeSet);

private:
    //资源对象
    IResouce                *m_pResouce;
    IParamSet               *m_pParamSet;
    IProductMgr             *m_pProductMgr;
    IProcessMgr             *m_pProcessMgr;
    IMachineMgr             *m_pMachineMgr;
    IOrderMgr               *m_pOrderMgr;

    //评估单元
    IEvaluateUint           *m_pEvaluateUnit;
    //进化单元
    IEvolveUnit             *m_pEvolveUnit;

    //解集
    CSolutionSet            *m_pSolutionSet;
    //下一代选择集
    CSolutionSet            *m_pNextGSolutionSet;
    //交换集
    CSolutionSet            *m_pExchangeSet;

    //解集大小
    UINT32                  m_uiSolutionSize;
    //选择集大小
    UINT32                  m_uiExchangeSize;
    //BaseTime
    time_t                  m_tBaseTime;

//多线程计算引入参数
private:
    CALC_CALLBACK_FUNC      m_pCalcCallback;
    ACE_Thread_Manager      m_ThdMgr;
    BOOL                    m_bStopFlag;
    BOOL                    m_bCalcFlag;

};

#endif //__CALC_UNIT_H__

