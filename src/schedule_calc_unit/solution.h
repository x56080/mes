
#ifndef __SOLUTION_H__
#define __SOLUTION_H__

#include "../../include/algorith_def.h"

class CDNA : public IDNA
{
public:
    CDNA();
    ~CDNA();

public:
    
    //获取产品信息
    virtual const SProductInfo*   GetProductInfo() const;
    
    //获取机器ID
    virtual UINT32          GetMachineID() const;
    
    //获取工序类型
    virtual UINT32          GetProcessType() const;
    //获取工序索引
    virtual UINT32          GetProcessIndex() const;

    //获取生产时间(单位：秒)
    virtual UINT32          GetProduceTime() const;
    
    //获取切换时间(单位：秒)
    virtual UINT32          GetSwitchTime() const;

    //空闲时间
    virtual UINT32          GetIdleTime() const;
    
    //获取启始时间
    virtual time_t          GetStartTime() const;
    
    //获取结束时间
    virtual time_t          GetEndTime() const;

public:
    
    INT32       Init();

    void        SetValue(DOUBLE dbCalcValue);
    DOUBLE      GetValue() const;

    void        SetMachineID(UINT32 uiMachineID);
    void        SetProcessType(UINT32 uiProcessType);
    void        SetProcessIndex(UINT32 uiProcessIndex);
    void        SetProductInfo(SProductInfo *pProductInfo);

    void        SetPosition(UINT32 uiLine, UINT32 uiColum);
    UINT32      GetPosLine() const;
    UINT32      GetPosColum() const;

    void        SetSwitchTime(UINT32 uiTime);
    void        SetIdleTime(UINT32 uiTime);
    void        SetProduceTime(UINT32 uiTime);
    void        SetStartTime(time_t lTime);

    IGene*      GetGene() const;
    void        AttachGene(IGene *pGene);

    //下道工序最早启动时间
    void        SetNStartOffTime(LONG lOffTime);
    LONG        GetNStartTime() const;

    CDNA&       operator=(const CDNA &right);
    bool        operator==(const CDNA &right) const;
    bool        operator!=(const CDNA &right) const;

private:

    UINT32              m_uiMachineID;
    UINT32              m_uiProcessType;
    UINT32              m_uiProcessIndex;

    time_t              m_tProduceTime;
    time_t              m_tSwitchTime;
    time_t              m_tIdleTime;
    time_t              m_tStartTime;
    time_t              m_tNextStartTime; //下道工序最早启动时间
    //time_t              m_tEndTime;

    const SProductInfo  *m_pProductInfo;
    IGene               *m_pGene;

    DOUBLE              m_dbCalcValue;

    UINT32              m_uiLine;
    UINT32              m_uiColum;

};

class CGene : public IGene
{
public:
    CGene(const VECTOR<UINT32> &vecMachine);
    virtual ~CGene();

public:

    virtual BOOL    IsIn(UINT32 uiDNAIndex) const;
    virtual BOOL    IsEqual(const VECTOR<UINT32> &vecMachineSet) const;
    
    virtual UINT32  GetDNANum() const;
    virtual IDNA*   GetDNA(UINT32 uiIndex, ISolution *pSolution) const;
    virtual INT32   GetDNAIndex(UINT32 uiIndex) const;
    
    virtual UINT32  CalcMachineID(UINT32 uiMachineIdx) const;

    virtual UINT32  GetMachineNum() const;

public:
    
    INT32           AddDNA(UINT32 uiDNAIndex);

private:

    //机器集
    VECTOR<UINT32>              m_vecMachine;
    //DNA Index集
    VECTOR<UINT32>              m_vecDNAIndex;

};

class CSolution : public ISolution
{
public:
    CSolution();
    ~CSolution();

public:
    
    //获取DNA个数
    virtual UINT32      GetDNANum() const;
    
    //获取基因个数
    virtual UINT32      GetGeneNum() const;
    virtual IGene*      GetGene(UINT32 uiIndex) const;
    
    //获取DNA
    virtual UINT32      GetDNASet(UINT32 uiLine, VECTOR<IDNA*> &vecDNASet) const;
    
    //获取行、列
    virtual UINT32      GetLine() const;
    virtual UINT32      GetColum() const;
    
    //获取DNA
    virtual IDNA*       GetDNA(UINT32 uiLine, UINT32 uiColum) const;
    virtual IDNA*       GetDNA(UINT32 uiIndex) const;

    //适应度
    virtual DOUBLE      GetFitness() const;
    virtual void        SetFitness(DOUBLE dbFitness);

    //库存
    virtual void        SetMaxStoreNum(UINT32 uiMaxStoreNum);
    virtual UINT32      GetMaxStoreNum() const;

public:
    
    INT32       Init(IResouce *pResource);

    //设置值
    BOOL        SetValue(UINT32 uiLine, UINT32 uiColum, DOUBLE dbValue);
    BOOL        SetValue(UINT32 uiIndex, DOUBLE dbValue);
    //获取值
    DOUBLE      GetValue(UINT32 uiLine, UINT32 uiColum);
    DOUBLE      GetValue(UINT32 uiIndex);

    CSolution&  operator=(const CSolution &right);
    bool        operator==(const CSolution &right) const;
    bool        operator!=(const CSolution &right) const;

protected:
    
    UINT32      CalcIndex(UINT32 uiLine, UINT32 uiColum) const;
    UINT32      CalcLine(UINT32 uiIndex) const;
    UINT32      CalcColum(UINT32 uiIndex) const;

    CGene*      FindGene(CDNA *pDNA, UINT32 uiDNAIndex);
    CGene*      AddToGene(CDNA *pDNA, UINT32 uiDNAIndex);

private:
    
    IResouce                *m_pResource;

    VECTOR<CDNA*>           m_vecDNA;
    static VECTOR<CGene*>   m_vecGene;
    static UINT32           m_uiReference;

    UINT32                  m_uiLine;
    UINT32                  m_uiColum;
    UINT32                  m_uiNum;

    DOUBLE                  m_dbFitness;
    UINT32                  m_uiMaxStoreNum;

};

class CSolutionSet : public ISolutionSet
{
public:
    CSolutionSet();
    ~CSolutionSet();

public:
    //获取解集个数
    virtual UINT32          GetNum() const;
    
    //获取解
    virtual ISolution*      GetSolution(UINT32 uiIndex) const;

    //获取解,带好坏评估
    virtual INT32           GetOrderFirstPos() const;
    virtual ISolution*      GetOrderNext(INT32 &iPos) const;

public:

    INT32       Init(UINT32 uiNum, IResouce *pResource);
    BOOL        FindLeft(CSolution *pSolution, UINT32 uiEndPos);

    INT32       MakeOrder();

protected:
    
    INT32       ClearOrder();

private:
    
    VECTOR<CSolution*>      m_vecSolution;
    INT32                   *m_pOrder;

};


#endif //__SOLUTION_H__
