
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
    
    //获取产品信息
    virtual const SProductInfo*   GetProductInfo() const = 0;
    
    //获取机器ID
    virtual UINT32          GetMachineID() const = 0;
    
    //获取工序类型
    virtual UINT32          GetProcessType() const = 0;
    //获取工序索引
    virtual UINT32          GetProcessIndex() const = 0;

    //获取生产时间(单位：秒)
    virtual UINT32          GetProduceTime() const = 0;
    
    //获取切换时间(单位：秒)
    virtual UINT32          GetSwitchTime() const = 0;

    //空闲时间
    virtual UINT32          GetIdleTime() const = 0;

    //获取启始时间
    virtual time_t          GetStartTime() const = 0;

    //获取结束时间
    virtual time_t          GetEndTime() const = 0;

};

//基因
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

//解
class ISolution
{
public:
    ISolution() {}
    ~ISolution() {}
    
public:
    
    //获取DNA个数
    virtual UINT32      GetDNANum() const = 0;

    //获取基因个数
    virtual UINT32      GetGeneNum() const = 0;
    virtual IGene*      GetGene(UINT32 uiIndex) const = 0;

    //获取DNA
    virtual UINT32      GetDNASet(UINT32 uiLine, VECTOR<IDNA*> &vecDNASet) const = 0;

    //获取行、列
    virtual UINT32      GetLine() const = 0;
    virtual UINT32      GetColum() const = 0;

    //获取DNA
    virtual IDNA*       GetDNA(UINT32 uiLine, UINT32 uiColum) const = 0;
    virtual IDNA*       GetDNA(UINT32 uiIndex) const = 0;

    //适应度
    virtual DOUBLE      GetFitness() const = 0;
    virtual void        SetFitness(DOUBLE dbFitness) = 0;

    //库存
    virtual void        SetMaxStoreNum(UINT32 uiMaxStoreNum) = 0;
    virtual UINT32      GetMaxStoreNum() const = 0;

};


//解集
class ISolutionSet
{
public:
    ISolutionSet() {}
    ~ISolutionSet() {}

public:
    //获取解集个数
    virtual UINT32          GetNum() const = 0;

    //获取解
    virtual ISolution*      GetSolution(UINT32 uiIndex) const = 0;

    //获取解,带好坏评估
    virtual INT32           GetOrderFirstPos() const = 0;
    virtual ISolution*      GetOrderNext(INT32 &iPos) const = 0;

};

//////////////////////////////////////////////////////////////////////////

//运算过程中的回调函数
typedef void (*CALC_CALLBACK_FUNC)(UINT32 uiGeneration, 
                                   ISolutionSet *pSolutionSet, 
                                   ISolution *pBestSolution);

//计算单元
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

//评估单元
class IEvaluateUint
{
public:
    IEvaluateUint() {}
    virtual ~IEvaluateUint() {}

public:

    virtual INT32   Init(IParamSet *pParamSet) = 0;
    
    virtual INT32   Evaludate(ISolution *pSolution) = 0;

};

//进化单元
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

