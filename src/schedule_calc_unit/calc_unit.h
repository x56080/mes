
#ifndef __CALC_UNIT_H__
#define __CALC_UNIT_H__

#include "../../include/algorith_def.h"
#include "solution.h"

//�������
/*class CCalcMatrix
{
public:
    CCalcMatrix(UINT32 uiLine, UINT32 uiColum);
    ~CCalcMatrix();

public:
    
    //����ֵ
    BOOL        SetValue(UINT32 uiLine, UINT32 uiColum, DOUBLE dbValue);
    BOOL        SetValue(UINT32 uiIndex, DOUBLE dbValue);
    //��ȡֵ
    DOUBLE      GetValue(UINT32 uiLine, UINT32 uiColum);
    DOUBLE      GetValue(UINT32 uiIndex);
    
    UINT32      GetLine() const;
    UINT32      GetColum() const;
    UINT32      GetNum() const;

protected:
    
    UINT32      CalcIndex(UINT32 uiLine, UINT32 uiColum);

private:
    
    UINT32              m_uiLine;           //��--- ��Ʒ����
    UINT32              m_uiColum;          //��--- ��������
    UINT32              m_uiNum;
    DOUBLE              *m_pMatrix;         //����

};*/


//�������
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
    
    //����ID��DNA��
    MAP< UINT32, LIST<CDNA*> >              m_mapMachineToDNASet;
    //��Դ
    IResouce                                *m_pResource;
    ISolution                               *m_pSolution;
    UINT32                                  m_uiCurLine;
    //BaseTime
    time_t                                  m_tBaseTime;
    
};

//���㵥Ԫ
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

    //����һ�������
    INT32       RandSolution(CSolution *pSolution);
    //����һ������⼯
    INT32       RandSolutionSet(CSolutionSet *pSolutionSet, UINT32 uiIndex = 0);
    //������Ӧ��
    INT32       CalcFitness(CSolutionSet *pSolutionSet);
    //����
    INT32       EvolveSet(CSolutionSet *pSolutionSet);

    //��������Ľ�
    INT32       CalcSolution(CSolution *pSolution);
    //��������Ľ⼯
    INT32       CalcSolutionSet(CSolutionSet *pSolutionSet);

    //ѡ����һ��
    INT32       Select(CSolutionSet *pParent, CSolutionSet *pSon, UINT32 uiNum);

    //�γ��µļ���⼯
    INT32       MakeCalcSolutionSet(CSolutionSet *pNewSet, CSolutionSet *pSet,
                                    CSolutionSet *pExchangeSet);

private:
    //��Դ����
    IResouce                *m_pResouce;
    IParamSet               *m_pParamSet;
    IProductMgr             *m_pProductMgr;
    IProcessMgr             *m_pProcessMgr;
    IMachineMgr             *m_pMachineMgr;
    IOrderMgr               *m_pOrderMgr;

    //������Ԫ
    IEvaluateUint           *m_pEvaluateUnit;
    //������Ԫ
    IEvolveUnit             *m_pEvolveUnit;

    //�⼯
    CSolutionSet            *m_pSolutionSet;
    //��һ��ѡ��
    CSolutionSet            *m_pNextGSolutionSet;
    //������
    CSolutionSet            *m_pExchangeSet;

    //�⼯��С
    UINT32                  m_uiSolutionSize;
    //ѡ�񼯴�С
    UINT32                  m_uiExchangeSize;
    //BaseTime
    time_t                  m_tBaseTime;

//���̼߳����������
private:
    CALC_CALLBACK_FUNC      m_pCalcCallback;
    ACE_Thread_Manager      m_ThdMgr;
    BOOL                    m_bStopFlag;
    BOOL                    m_bCalcFlag;

};

#endif //__CALC_UNIT_H__

