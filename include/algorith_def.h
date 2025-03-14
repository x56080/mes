
#ifndef __ALGORITH_DEF_H__
#define __ALGORITH_DEF_H__

#include "base_type.h"
#include "interface.h"

class ISolution;

//DNA
class IDNA
{
public:
    IDNA() {}
    virtual ~IDNA() {}
    
public:
    
    //��ȡ��Ʒ��Ϣ
    virtual const SProductInfo*   GetProductInfo() const = 0;
    
    //��ȡ����ID
    virtual UINT32          GetMachineID() const = 0;
    
    //��ȡ��������
    virtual UINT32          GetProcessType() const = 0;
    //��ȡ��������
    virtual UINT32          GetProcessIndex() const = 0;

    //��ȡ����ʱ��(��λ����)
    virtual UINT32          GetProduceTime() const = 0;
    
    //��ȡ�л�ʱ��(��λ����)
    virtual UINT32          GetSwitchTime() const = 0;

    //����ʱ��
    virtual UINT32          GetIdleTime() const = 0;

    //��ȡ��ʼʱ��
    virtual time_t          GetStartTime() const = 0;

    //��ȡ����ʱ��
    virtual time_t          GetEndTime() const = 0;

};

//����
class IGene
{
public:
    IGene() {}
    ~IGene() {}
    
public:
    
    virtual BOOL    IsIn(UINT32 uiDNAIndex) const = 0;
    virtual BOOL    IsEqual(const VECTOR<UINT32> &vecMachineSet) const = 0;

    virtual UINT32  GetDNANum() const = 0;

    virtual IDNA*   GetDNA(UINT32 uiIndex, ISolution *pSolution) const = 0;
    virtual INT32   GetDNAIndex(UINT32 uiIndex) const = 0;

    virtual UINT32  CalcMachineID(UINT32 uiMachineIdx) const = 0;

    virtual UINT32  GetMachineNum() const = 0;
    
};

//��
class ISolution
{
public:
    ISolution() {}
    ~ISolution() {}
    
public:
    
    //��ȡDNA����
    virtual UINT32      GetDNANum() const = 0;

    //��ȡ�������
    virtual UINT32      GetGeneNum() const = 0;
    virtual IGene*      GetGene(UINT32 uiIndex) const = 0;

    //��ȡDNA
    virtual UINT32      GetDNASet(UINT32 uiLine, VECTOR<IDNA*> &vecDNASet) const = 0;

    //��ȡ�С���
    virtual UINT32      GetLine() const = 0;
    virtual UINT32      GetColum() const = 0;

    //��ȡDNA
    virtual IDNA*       GetDNA(UINT32 uiLine, UINT32 uiColum) const = 0;
    virtual IDNA*       GetDNA(UINT32 uiIndex) const = 0;

    //��Ӧ��
    virtual DOUBLE      GetFitness() const = 0;
    virtual void        SetFitness(DOUBLE dbFitness) = 0;

    //���
    virtual void        SetMaxStoreNum(UINT32 uiMaxStoreNum) = 0;
    virtual UINT32      GetMaxStoreNum() const = 0;

};


//�⼯
class ISolutionSet
{
public:
    ISolutionSet() {}
    ~ISolutionSet() {}

public:
    //��ȡ�⼯����
    virtual UINT32          GetNum() const = 0;

    //��ȡ��
    virtual ISolution*      GetSolution(UINT32 uiIndex) const = 0;

    //��ȡ��,���û�����
    virtual INT32           GetOrderFirstPos() const = 0;
    virtual ISolution*      GetOrderNext(INT32 &iPos) const = 0;

};

//////////////////////////////////////////////////////////////////////////

//��������еĻص�����
typedef void (*CALC_CALLBACK_FUNC)(UINT32 uiGeneration, 
                                   ISolutionSet *pSolutionSet, 
                                   ISolution *pBestSolution);

//���㵥Ԫ
class ICalcUint
{
public:
    ICalcUint() {}
    virtual ~ICalcUint() {}

public:

    virtual INT32           Init(IParamSet *pParamSet) = 0;
    virtual INT32           Start(CALC_CALLBACK_FUNC pCallback = NULL) = 0;
    virtual BOOL            IsFinish() const = 0;
    virtual INT32           Stop() = 0;
    virtual INT32           Wait(UINT32 uiTimeout = 0) = 0;
    
    virtual ISolutionSet*   GetSolutionSet() = 0;
    virtual ISolution*      GetBestSolution() = 0;

};

//������Ԫ
class IEvaluateUint
{
public:
    IEvaluateUint() {}
    virtual ~IEvaluateUint() {}

public:

    virtual INT32   Init(IParamSet *pParamSet) = 0;
    
    virtual INT32   Evaludate(ISolution *pSolution) = 0;

};

//������Ԫ
class IEvolveUnit
{
public:
    IEvolveUnit() {}
    virtual ~IEvolveUnit() {}

public:

    virtual INT32   Init(IParamSet *pParamSet) = 0;

    virtual INT32   Evolve(ISolutionSet *pSolutionSet) = 0;
    
};

//////////////////////////////////////////////////////////////////////////

DLL_EXPORT ICalcUint* CreateCalcUnit(IResouce *pResouce);
DLL_EXPORT BOOL       ReleaseCalcUnit(ICalcUint *pCalcUnit);

#endif //__ALGORITH_DEF_H__

