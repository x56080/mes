
#include "solution.h"
#include "../../include/tool.h"


CDNA::CDNA()
{
    m_uiMachineID = 0;
    m_uiProcessType = 0;
    m_uiProcessIndex = 0;
    m_dbCalcValue = 0.0;

    m_uiLine = 0;
    m_uiColum = 0;
    
    m_tProduceTime = 0;
    m_tSwitchTime = 0;
    m_tIdleTime = 0;
    m_tStartTime = 0;
    m_tNextStartTime = 0;
    //m_tEndTime = 0;

    m_pGene = NULL;
    m_pProductInfo = NULL;
}

CDNA::~CDNA()
{
    m_pGene = NULL;
    m_pProductInfo = NULL;
}

CDNA& CDNA::operator =(const CDNA &right)
{
    m_uiMachineID = right.GetMachineID();
    //m_uiProcessType = right.GetProcessType();
    //m_uiProcessIndex = right.GetProcessIndex();
    m_dbCalcValue = right.GetValue();
    
    //m_uiLine = right.GetPosLine();
    //m_uiColum = right.GetPosColum();

    m_tProduceTime = right.GetProduceTime();
    m_tSwitchTime = right.GetSwitchTime();
    m_tIdleTime = right.GetIdleTime();
    m_tStartTime = right.GetStartTime();
    m_tNextStartTime = right.GetNStartTime();

    //m_pGene = right.GetGene();
    //m_pProductInfo = right.GetProductInfo();

    return *this;
}

bool CDNA::operator ==(const CDNA &right) const
{
    if (fabs(m_dbCalcValue - right.GetValue()) < EPSILON)
    {
        return true;
    }

    return false;
}

bool CDNA::operator !=(const CDNA &right) const
{
    if (fabs(m_dbCalcValue - right.GetValue()) >= EPSILON)
    {
        return true;
    }

    return false;
}

INT32 CDNA::Init()
{
    return RET_SUC;
}

void CDNA::SetValue(DOUBLE dbCalcValue)
{
    m_dbCalcValue = dbCalcValue;
}

DOUBLE CDNA::GetValue() const
{
    return m_dbCalcValue;
}

void CDNA::SetProductInfo(SProductInfo *pProductInfo)
{
    m_pProductInfo = pProductInfo;
}

const SProductInfo* CDNA::GetProductInfo() const
{
    return m_pProductInfo;
}

void CDNA::SetMachineID(UINT32 uiMachineID)
{
    m_uiMachineID = uiMachineID;

    m_tProduceTime = 0;
    m_tSwitchTime = 0;
    m_tStartTime = 0;
    m_tIdleTime = 0;
    m_tNextStartTime = 0;
    //m_tEndTime = 0;
}

UINT32 CDNA::GetMachineID() const
{
    return m_uiMachineID;
}

void CDNA::SetProcessType(UINT32 uiProcessType)
{
    m_uiProcessType = uiProcessType;
}

UINT32 CDNA::GetProcessType() const
{
    return m_uiProcessType;
}

void CDNA::SetProcessIndex(UINT32 uiProcessIndex)
{
    m_uiProcessIndex = uiProcessIndex;
}

UINT32 CDNA::GetProcessIndex() const
{
    return m_uiProcessIndex;
}

UINT32 CDNA::GetProduceTime() const
{
    return m_tProduceTime;
}

UINT32 CDNA::GetSwitchTime() const
{
    return m_tSwitchTime;
}

UINT32 CDNA::GetIdleTime() const
{
    return m_tIdleTime;
}

time_t CDNA::GetStartTime() const
{
    return m_tStartTime;
}

time_t CDNA::GetEndTime() const
{
    return m_tStartTime + m_tProduceTime + m_tSwitchTime;
}

IGene* CDNA::GetGene() const
{
    return m_pGene;
}

void CDNA::AttachGene(IGene *pGene)
{
    m_pGene = pGene;
}

void CDNA::SetPosition(UINT32 uiLine, UINT32 uiColum)
{
    m_uiLine = uiLine;
    m_uiColum = uiColum;
}

UINT32 CDNA::GetPosColum() const
{
    return m_uiColum;
}

UINT32 CDNA::GetPosLine() const
{
    return m_uiLine;
}

void CDNA::SetIdleTime(UINT32 uiTime)
{
    m_tIdleTime = uiTime;
}

void CDNA::SetStartTime(time_t lTime)
{
    m_tStartTime = lTime;
}

void CDNA::SetSwitchTime(UINT32 uiTime)
{
    m_tSwitchTime = uiTime;
}

void CDNA::SetProduceTime(UINT32 uiTime)
{
    m_tProduceTime = uiTime;
}

void CDNA::SetNStartOffTime(LONG lOffTime)
{
    m_tNextStartTime = m_tStartTime + m_tSwitchTime + lOffTime;
}

LONG CDNA::GetNStartTime() const
{
    return m_tNextStartTime;
}

//////////////////////////////////////////////////////////////////////////

CGene::CGene(const VECTOR<UINT32> &vecMachine)
{
    m_vecMachine = vecMachine;
}

CGene::~CGene()
{
    m_vecMachine.clear();
    m_vecDNAIndex.clear();
}

BOOL CGene::IsEqual(const VECTOR<UINT32> &vecMachineSet) const
{
    UINT32 uiCount = m_vecMachine.size();
    if (uiCount != vecMachineSet.size())
    {
        return FALSE;
    }

    UINT32 uiIndex = 0;
    for (; uiIndex < uiCount; uiIndex++)
    {
        if (m_vecMachine[uiIndex] != vecMachineSet[uiIndex])
        {
            return FALSE;
        }
    }

    return TRUE;
}

BOOL CGene::IsIn(UINT32 uiDNAIndex) const
{
    UINT32 uiCount = m_vecDNAIndex.size();
    UINT32 uiIndex = 0;

    for (; uiIndex < uiCount; uiIndex++)
    {
        if (m_vecDNAIndex[uiIndex] == uiDNAIndex)
        {
            return TRUE;
        }
    }

    return FALSE;
}

IDNA* CGene::GetDNA(UINT32 uiIndex, ISolution *pSolution) const
{
    if (uiIndex >= m_vecDNAIndex.size())
    {
        return NULL;
    }

    return pSolution->GetDNA(m_vecDNAIndex[uiIndex]);
}

INT32 CGene::GetDNAIndex(UINT32 uiIndex) const
{
    if (uiIndex >= m_vecDNAIndex.size())
    {
        return RET_ERR;
    }

    return m_vecDNAIndex[uiIndex];
}

UINT32 CGene::GetDNANum() const
{
    return m_vecDNAIndex.size();
}

UINT32 CGene::CalcMachineID(UINT32 uiMachineIdx) const
{
    UINT32 uiCount = m_vecMachine.size();
    if (uiMachineIdx >= uiCount)
    {
        return m_vecMachine[uiCount - 1];
    }

    return m_vecMachine[uiMachineIdx];
}

INT32 CGene::AddDNA(UINT32 uiDNAIndex)
{
    if (!IsIn(uiDNAIndex))
    {
        m_vecDNAIndex.push_back(uiDNAIndex);
    }

    //pDNA->AttachGene(this);

    return RET_SUC;
}

UINT32 CGene::GetMachineNum() const
{
    return m_vecMachine.size();
}

//////////////////////////////////////////////////////////////////////////

UINT32 CSolution::m_uiReference = 0;
VECTOR<CGene*> CSolution::m_vecGene;

CSolution::CSolution()
{
    m_uiColum = 0;
    m_uiLine = 0;
    m_uiNum = 0;
    m_dbFitness = 0.0;
    m_uiMaxStoreNum = 0;

    m_pResource = NULL;
}

CSolution::~CSolution()
{
    while (m_vecDNA.size() > 0)
    {
        delete m_vecDNA.back();
        m_vecDNA.pop_back();
    }

    if (m_uiReference > 0)
    {
        m_uiReference--;
    }

    if (m_uiReference == 0)
    {
        while (m_vecGene.size() > 0)
        {
            delete m_vecGene.back();
            m_vecGene.pop_back();
        }
    }

    m_pResource = NULL;
}

CSolution& CSolution::operator =(const CSolution &right)
{
    m_dbFitness = right.GetFitness();
    m_uiMaxStoreNum = right.GetMaxStoreNum();

    //对所有DNA覆值
    UINT32 uiIndex = 0;
    while (uiIndex < GetDNANum())
    {
        *(m_vecDNA[uiIndex]) = *(static_cast<CDNA*>(right.GetDNA(uiIndex)));
        uiIndex++;
    }

    return *this;
}

bool CSolution::operator ==(const CSolution &right) const
{
    UINT32 uiIndex = 0;
    while (uiIndex < GetDNANum())
    {
        if (*(m_vecDNA[uiIndex]) != *(static_cast<CDNA*>(right.GetDNA(uiIndex))))
        {
            return false;
        }

        uiIndex++;
    }

    return true;
}

bool CSolution::operator !=(const CSolution &right) const
{
    UINT32 uiIndex = 0;
    while (uiIndex < GetDNANum())
    {
        if (*(m_vecDNA[uiIndex]) != *(static_cast<CDNA*>(right.GetDNA(uiIndex))))
        {
            return true;
        }
        
        uiIndex++;
    }
    
    return false;
}

INT32 CSolution::Init(IResouce *pResource)
{
    m_pResource = pResource;
    m_uiLine = pResource->GetProcessMgr()->GetProcessNum();
    m_uiColum = pResource->GetOrderMgr()->GetProductKinds();
    m_uiNum = m_uiLine * m_uiColum;

    //产品信息
    static BOOL s_bGet = FALSE;
    static VECTOR<SProductInfo> s_vecProductInfo;
    
    if (!s_bGet)
    {
        pResource->GetOrderMgr()->GetAllProductInfo(s_vecProductInfo);
        s_bGet = TRUE;
    }

    UINT32 uiIndex = 0;
    CDNA *pDNA = NULL;
    CGene *pGene = NULL;

    for (; uiIndex < m_uiNum; uiIndex++)
    {
        pDNA = new CDNA();
        m_vecDNA.push_back(pDNA);

        if (pDNA->Init() < 0)
        {
            return RET_ERR;
        }
        pDNA->SetPosition(CalcLine(uiIndex), CalcColum(uiIndex));        
        pDNA->SetProcessIndex(CalcLine(uiIndex));
        pDNA->SetProcessType(pResource->GetProcessMgr()->Index2Type(CalcLine(uiIndex)));
        pDNA->SetProductInfo(&s_vecProductInfo[CalcColum(uiIndex)]);

        //计算基因
        if (m_uiReference == 0)
        {
            pGene = AddToGene(pDNA, uiIndex);
        }
        else
        {
            pGene = FindGene(pDNA, uiIndex);
        }

        if (NULL == pGene)
        {
            return RET_ERR;
        }
        pDNA->AttachGene(pGene);
    }

    m_uiReference++;

    return RET_SUC;
}

CGene* CSolution::FindGene(CDNA *pDNA, UINT32 uiDNAIndex)
{
    CGene *pGene = NULL;
    UINT32 uiIndex = 0;
    
    for (; uiIndex < m_vecGene.size(); uiIndex++)
    {
        pGene = m_vecGene[uiIndex];
        if (pGene->IsIn(uiDNAIndex))
        {
            return pGene;
        }
    }

    return NULL;
}

CGene* CSolution::AddToGene(CDNA *pDNA, UINT32 uiDNAIndex)
{
    IProcess *pProcess = 
        m_pResource->GetProcessMgr()->GetProcess(pDNA->GetProcessIndex());

    if (NULL != pDNA->GetGene())
    {
        return RET_SUC;
    }

    //获取该DNA对应的机器列表, 再判断是否已存在基因
    //若存在基因,则加入; 若不存在基因,则创建,并加入
    UINT32 uiProductID = pDNA->GetProductInfo()->uiProductID;

    VECTOR<UINT32> vecMachine;
    UINT32 uiMachineCount = pProcess->GetMachineSet(uiProductID, vecMachine);
    if (uiMachineCount == 0)
    {
        TRACE("CSolution::AddToGene", "No machine produce product[ID=%u]", uiProductID);
        return NULL;
    }

    UINT32 uiIndex = 0;
    for (; uiIndex < m_vecGene.size(); uiIndex++)
    {
        if (m_vecGene[uiIndex]->IsEqual(vecMachine))
        {
            m_vecGene[uiIndex]->AddDNA(uiDNAIndex);
            return m_vecGene[uiIndex];
        }
    }

    //不存在
    CGene *pGene = new CGene(vecMachine);
    pGene->AddDNA(uiDNAIndex);
    m_vecGene.push_back(pGene);

    return pGene;
}

BOOL CSolution::SetValue(UINT32 uiIndex, DOUBLE dbValue)
{
    if (uiIndex >= m_uiNum)
    {
        return FALSE;
    }
    
    m_vecDNA[uiIndex]->SetValue(dbValue);
    
    return TRUE;
}

BOOL CSolution::SetValue(UINT32 uiLine, UINT32 uiColum, DOUBLE dbValue)
{
    return SetValue(CalcIndex(uiLine, uiColum), dbValue);
}

DOUBLE CSolution::GetValue(UINT32 uiIndex)
{
    if (uiIndex >= m_uiNum)
    {
        return -1.0;
    }
    
    return m_vecDNA[uiIndex]->GetValue();
}

DOUBLE CSolution::GetValue(UINT32 uiLine, UINT32 uiColum)
{
    return GetValue(CalcIndex(uiLine, uiColum));
}

UINT32 CSolution::GetDNANum() const
{
    return m_uiNum;
}

UINT32 CSolution::GetGeneNum() const
{
    return m_vecGene.size();
}

IGene* CSolution::GetGene(UINT32 uiIndex) const
{
    if (uiIndex >= m_vecGene.size())
    {
        return NULL;
    }

    return m_vecGene[uiIndex];
}

UINT32 CSolution::GetLine() const
{
    return m_uiLine;
}

UINT32 CSolution::GetColum() const
{
    return m_uiColum;
}

UINT32 CSolution::CalcIndex(UINT32 uiLine, UINT32 uiColum) const
{
    return uiLine * m_uiColum + uiColum;
}

UINT32 CSolution::CalcLine(UINT32 uiIndex) const
{
    return uiIndex / m_uiColum;
}

UINT32 CSolution::CalcColum(UINT32 uiIndex) const
{
    return uiIndex % m_uiColum;
}

IDNA* CSolution::GetDNA(UINT32 uiIndex) const
{
    if (uiIndex >= m_uiNum)
    {
        return NULL;
    }

    return m_vecDNA[uiIndex];
}

IDNA* CSolution::GetDNA(UINT32 uiLine, UINT32 uiColum) const
{
    return GetDNA(CalcIndex(uiLine, uiColum));
}

UINT32 CSolution::GetDNASet(UINT32 uiLine, VECTOR<IDNA*> &vecDNASet) const
{
    UINT32 uiColumIndex = 0;
    vecDNASet.clear();

    for (; uiColumIndex < m_uiColum; uiColumIndex++)
    {
        vecDNASet.push_back(m_vecDNA[CalcIndex(uiLine, uiColumIndex)]);
    }

    return uiColumIndex;
}

DOUBLE CSolution::GetFitness() const
{
    return m_dbFitness;
}

void CSolution::SetFitness(DOUBLE dbFitness)
{
    m_dbFitness = dbFitness;
}

UINT32 CSolution::GetMaxStoreNum() const
{
    return m_uiMaxStoreNum;
}

void CSolution::SetMaxStoreNum(UINT32 uiMaxStoreNum)
{
    m_uiMaxStoreNum = uiMaxStoreNum;
}

//////////////////////////////////////////////////////////////////////////
CSolutionSet::CSolutionSet()
{
    m_pOrder = NULL;
}

CSolutionSet::~CSolutionSet()
{
    while (m_vecSolution.size() > 0)
    {
        delete m_vecSolution.back();
        m_vecSolution.pop_back();
    }

    if (NULL != m_pOrder)
    {
        delete [] m_pOrder;
        m_pOrder = NULL;
    }
}

UINT32 CSolutionSet::GetNum() const
{
    return m_vecSolution.size();
}

ISolution* CSolutionSet::GetSolution(UINT32 uiIndex) const
{
    if (uiIndex >= m_vecSolution.size())
    {
        return NULL;
    }

    return m_vecSolution[uiIndex];
}

INT32 CSolutionSet::Init(UINT32 uiNum, IResouce *pResource)
{
    UINT32 uiIndex = 0;
    CSolution *pSolution = NULL;

    for (; uiIndex < uiNum; uiIndex++)
    {
        pSolution = new CSolution();
        m_vecSolution.push_back(pSolution);
        
        if (pSolution->Init(pResource) < 0)
        {
            return RET_ERR;
        }
    }

    m_pOrder = new INT32[uiNum];
    for (uiIndex = 0; uiIndex < uiNum; uiIndex++)
    {
        m_pOrder[uiIndex] = -1;
    }

    return RET_SUC;
}

INT32 CSolutionSet::ClearOrder()
{
    if (NULL == m_pOrder)
    {
        return RET_ERR;
    }

    UINT32 uiIndex = 0;
    for (; uiIndex < GetNum(); uiIndex++)
    {
        m_pOrder[uiIndex] = -1;
    }

    return RET_SUC;
}

INT32 CSolutionSet::MakeOrder()
{
    if (NULL == m_pOrder)
    {
        return RET_ERR;
    }

    //清空序列
    ClearOrder();

    INT32 iIndex = 0;
    INT32 iHasOrderNum = 0; //已排序的个数
    for (; iIndex < GetNum(); iIndex++)
    {
        INT32 iOrderIndex = iHasOrderNum - 1;
        //寻找插入位置
        for (; iOrderIndex >= 0; iOrderIndex--)
        {
            if (GetSolution(iIndex)->GetFitness() >
                GetSolution(m_pOrder[iOrderIndex])->GetFitness())
            {
                m_pOrder[iOrderIndex + 1] = m_pOrder[iOrderIndex];
            }
            else
            {
                break;
            }
        }
        
        //插入
        m_pOrder[iOrderIndex + 1] = iIndex;
        iHasOrderNum++;
    }

    return RET_SUC;
}

INT32 CSolutionSet::GetOrderFirstPos() const
{
    return 0;
}

ISolution* CSolutionSet::GetOrderNext(INT32 &iPos) const
{
    INT32 iPosTmp = iPos;
    iPos++;
    if (iPos >= GetNum())
    {
        iPos = -1;
    }

    if (m_pOrder && iPosTmp < GetNum() && m_pOrder[iPosTmp] != -1)
    {
        return GetSolution(m_pOrder[iPosTmp]);
    }

    return NULL;
}

BOOL CSolutionSet::FindLeft(CSolution *pSolution, UINT32 uiEndPos)
{
    UINT32 uiIndex = 0;
    if (uiEndPos >= GetNum())
    {
        uiEndPos = GetNum() - 1;
    }

    while (uiIndex < uiEndPos)
    {
        if (*pSolution == *m_vecSolution[uiIndex])
        {
            return TRUE;
        }

        uiIndex++;
    }

    return FALSE;
}

