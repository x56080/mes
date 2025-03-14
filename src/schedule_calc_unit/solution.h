
#ifndef __SOLUTION_H__
#define __SOLUTION_H__

#include "../../include/algorith_def.h"

class CDNA : public IDNA
{
public:
    CDNA();
    ~CDNA();

public:
    
    //��ȡ��Ʒ��Ϣ
    virtual const SProductInfo*   GetProductInfo() const;
    
    //��ȡ����ID
    virtual UINT32          GetMachineID() const;
    
    //��ȡ��������
    virtual UINT32          GetProcessType() const;
    //��ȡ��������
    virtual UINT32          GetProcessIndex() const;

    //��ȡ����ʱ��(��λ����)
    virtual UINT32          GetProduceTime() const;
    
    //��ȡ�л�ʱ��(��λ����)
    virtual UINT32          GetSwitchTime() const;

    //����ʱ��
    virtual UINT32          GetIdleTime() const;
    
    //��ȡ��ʼʱ��
    virtual time_t          GetStartTime() const;
    
    //��ȡ����ʱ��
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

    //�µ�������������ʱ��
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
    time_t              m_tNextStartTime; //�µ�������������ʱ��
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

    //������
    VECTOR<UINT32>              m_vecMachine;
    //DNA Index��
    VECTOR<UINT32>              m_vecDNAIndex;

};

class CSolution : public ISolution
{
public:
    CSolution();
    ~CSolution();

public:
    
    //��ȡDNA����
    virtual UINT32      GetDNANum() const;
    
    //��ȡ�������
    virtual UINT32      GetGeneNum() const;
    virtual IGene*      GetGene(UINT32 uiIndex) const;
    
    //��ȡDNA
    virtual UINT32      GetDNASet(UINT32 uiLine, VECTOR<IDNA*> &vecDNASet) const;
    
    //��ȡ�С���
    virtual UINT32      GetLine() const;
    virtual UINT32      GetColum() const;
    
    //��ȡDNA
    virtual IDNA*       GetDNA(UINT32 uiLine, UINT32 uiColum) const;
    virtual IDNA*       GetDNA(UINT32 uiIndex) const;

    //��Ӧ��
    virtual DOUBLE      GetFitness() const;
    virtual void        SetFitness(DOUBLE dbFitness);

    //���
    virtual void        SetMaxStoreNum(UINT32 uiMaxStoreNum);
    virtual UINT32      GetMaxStoreNum() const;

public:
    
    INT32       Init(IResouce *pResource);

    //����ֵ
    BOOL        SetValue(UINT32 uiLine, UINT32 uiColum, DOUBLE dbValue);
    BOOL        SetValue(UINT32 uiIndex, DOUBLE dbValue);
    //��ȡֵ
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
    //��ȡ�⼯����
    virtual UINT32          GetNum() const;
    
    //��ȡ��
    virtual ISolution*      GetSolution(UINT32 uiIndex) const;

    //��ȡ��,���û�����
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
