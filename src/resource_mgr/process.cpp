
#include "../../include/param_macro.h"
#include "../../include/tool.h"
#include "process.h"

CProcess::CProcess()
{
    m_uiProcessType = 0;
    m_uiSwtichTime = 0;
}

CProcess::~CProcess()
{
}

INT32 CProcess::Init(IParamSet *pParamSet)
{
    //{{
    RDX_Int(pParamSet, m_uiProcessType, S_PARAM_TYPE);
    RDX_Int(pParamSet, m_uiSwtichTime, S_PARAM_SWITCH_TIME);
    RDX_String(pParamSet, m_strProcessName, S_PARAM_NAME);
    //}}

    return RET_SUC;
}

//获取产品属性对应的机器数
UINT32 CProcess::GetMachineNum(UINT32 uiProductID)
{
    UINT32 uiCount = 0;
    INT32 iIndex = 0;
    while (iIndex < m_vecMachine.size())
    {
        if (m_vecMachine[iIndex]->CanProduct(uiProductID))
        {
            uiCount++;
        }
        iIndex++;
    }

    return uiCount;
}

UINT32 CProcess::GetMachineSet(UINT32 uiProductID, VECTOR<UINT32> &vecMachineSet)
{
    UINT32 uiCount = 0;
    INT32 iIndex = 0;
    vecMachineSet.clear();

    while (iIndex < m_vecMachine.size())
    {
        if (m_vecMachine[iIndex]->CanProduct(uiProductID))
        {
            uiCount++;
            vecMachineSet.push_back(m_vecMachine[iIndex]->GetMachineID());
        }
        iIndex++;
    }
    
    return uiCount;
}

//获取机器数
UINT32 CProcess::GetMachineNum() const
{
	return m_vecMachine.size();
}
    
//获取机器ID集
UINT32 CProcess::GetMachineSet(VECTOR<UINT32> &vecMachineSet) const
{
	vecMachineSet.clear();
	vecMachineSet.resize(m_vecMachine.size());

	for (INT32 i = 0; i < m_vecMachine.size(); ++i)
	{
		vecMachineSet[i] = m_vecMachine[i]->GetMachineID();
	}

    return m_vecMachine.size();
}

time_t CProcess::CalcSwitchTime(UINT32 uiPrevProductID, UINT32 uiCurProductID) const
{
    if (uiPrevProductID != uiCurProductID)
    {
        return m_uiSwtichTime;
    }

    return 0;
}

STRING CProcess::GetName() const
{
	return m_strProcessName;
}

UINT32 CProcess::GetType() const
{
	return m_uiProcessType;
}

BOOL CProcess::AddMachine(IMachine *pMachine)
{
    m_vecMachine.push_back(pMachine);

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

CProcessMgr::CProcessMgr()
{

}

CProcessMgr::~CProcessMgr()
{
    Clear();
}

IProcess* CProcessMgr::GetProcessByType(UINT32 uiType) const
{
    INT32 iIndex = 0;
    CProcess *pProcess = NULL;
    while (iIndex < m_vecProcesses.size())
    {
        pProcess = m_vecProcesses[iIndex++];
        if (pProcess->GetType() == uiType)
        {
            return pProcess;
        }
    }

    return NULL;
}

IProcess* CProcessMgr::GetProcess(UINT32 uiIndex) const
{
    if (uiIndex < m_vecProcesses.size())
    {
        return m_vecProcesses[uiIndex];
    }

    return NULL;
}

UINT32 CProcessMgr::Type2Index(UINT32 uiType) const
{
    CProcess *pProcess = NULL;
    INT32 iIndex = 0;

    while (iIndex < m_vecProcesses.size())
    {
        pProcess = m_vecProcesses[iIndex];
        if (pProcess->GetType() == uiType)
        {
            return iIndex;
        }
        iIndex++;
    }
    
    TRACE("CProcessMgr::Type2Index", "Not find Process Type:%d", uiType);
    return -1;
}

UINT32 CProcessMgr::Index2Type(UINT32 uiIndex) const
{
    if (uiIndex < m_vecProcesses.size())
    {
        return m_vecProcesses[uiIndex]->GetType();
    }

    TRACE("CProcessMgr::Index2Type", "Not find Process index:%d", uiIndex);
    return -1;
}
    
//获取流程个数
UINT32 CProcessMgr::GetProcessNum() const
{
	return m_vecProcesses.size();
}

INT32 CProcessMgr::AddProcess(IParamSet *pParamSet)
{
    CProcess *pProcess = new CProcess();
    if (pProcess->Init(pParamSet) != RET_SUC)
    {
        delete pProcess, pProcess = NULL;
    }

    //存在重复，报错
    if (NULL != GetProcessByType(pProcess->GetType()))
    {
        TRACE("CProcessMgr::AddProcess", "Process[Type=%d] already exist", pProcess->GetType());
        delete pProcess, pProcess = NULL;
        return RET_ERR;
    }

    m_vecProcesses.push_back(pProcess);

    return RET_SUC;
}

void CProcessMgr::Clear()
{
    INT32 iIndex = 0;
    while (iIndex < m_vecProcesses.size())
    {
        delete m_vecProcesses[iIndex];
        iIndex++;
    }

    m_vecProcesses.clear();
}
