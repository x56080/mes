
#include "machine.h"
#include "process.h"
#include "../../include/param_macro.h"
#include "../../include/tool.h"

CMachine::CMachine(IProductMgr *pProductMgr)
{
    m_pProductMgr = pProductMgr;
    m_uiID = 0;
    m_pProcess = NULL;
    m_uiStatus = STATUS_NORMAL;
    m_dbDiameterMin = 0.0;
    m_dbDiameterMax = 0.0;
}

CMachine::CMachine(IProductMgr *pProductMgr, UINT32 uiID,  IProcess *pProcess, 
                   const STRING &strName, DOUBLE dbDiaMin, DOUBLE dbDiaMax)
{
    m_pProductMgr = pProductMgr;
    m_uiID = uiID;
    m_pProcess = pProcess;
    m_uiStatus = STATUS_NORMAL;
    m_strName = strName;
    m_dbDiameterMin = dbDiaMin;
    m_dbDiameterMax = dbDiaMax;
}

INT32 CMachine::Init(IProcess *pProcess, UINT32 uiBaseID,
                     IParamSet *pParamSet)
{
    m_pProcess = pProcess;
    if (NULL == m_pProcess)
    {
        return RET_ERR;
    }

    STRING strProductSpec;
    //{{
    RDX_Int(pParamSet, m_uiID, S_PARAM_ID);
    RDX_String(pParamSet, m_strName, S_PARAM_NAME);
    RDX_String(pParamSet, strProductSpec, S_PARAM_PRODUCT_SPEC);
    //}}
    m_uiID += uiBaseID;

    //解析产品规格（直径）范围
    size_t nPos = strProductSpec.find('-');
    if (nPos == STRING::npos)
    {
        TRACE("CMachine::Init", "Machine[ID=%u] ProductSpec format error[%s]",
              m_uiID, strProductSpec.c_str());
        return RET_ERR;
    }
    STRING strDiaMin = strProductSpec.substr(0, nPos);
    STRING strDiaMax = strProductSpec.substr(nPos + 1);

    if (!TOOL::IsDigit(strDiaMin.c_str()) || !TOOL::IsDigit(strDiaMax.c_str())
        || strDiaMax.empty())
    {
        TRACE("CMachine::Init", "Machine[ID=%u] ProductSpec format error[%s]",
              m_uiID, strProductSpec.c_str());
        return RET_ERR;
    }

    m_dbDiameterMin = TOOL::String2Double(strDiaMin);
    m_dbDiameterMax = TOOL::String2Double(strDiaMax);

    return RET_SUC;
}

CMachine::~CMachine()
{
    m_pProductMgr = NULL;
}

UINT32 CMachine::GetMachineID() const
{
    return m_uiID;
}

UINT32 CMachine::GetProcessType() const
{
    return m_pProcess->GetType();
}

STRING CMachine::GetMachineName() const
{
    return m_strName;
}

BOOL CMachine::CanProduct(UINT32 uiProductID) const
{
    CProductProperty *pProperty = m_pProductMgr->GetProductProperty(uiProductID);
    if (NULL == pProperty)
    {
        return FALSE;
    }

    DOUBLE dbDiameter = pProperty->GetSpec().GetDiameter();
    if (dbDiameter >= m_dbDiameterMin && dbDiameter <= m_dbDiameterMax)
    {
        return TRUE;
    }

    return FALSE;
}

UINT32 CMachine::GetStatus() const
{
    return m_uiStatus;
}

void CMachine::SetStatus(UINT32 uiStatus)
{
    m_uiStatus = uiStatus;
}

void CMachine::SetName(const STRING &strName)
{
    m_strName = strName;
}

void CMachine::SetMachineID(UINT32 uiID)
{
    m_uiID = uiID;
}

void CMachine::SetProcess(IProcess *pProcess)
{
    m_pProcess = pProcess;
}

void CMachine::SetProductSpec(DOUBLE dbDiaMin, DOUBLE dbDiaMax)
{
    m_dbDiameterMin = dbDiaMin;
    m_dbDiameterMax = dbDiaMax;
}

//////////////////////////////////////////////////////////////////////////

CMachineMgr::CMachineMgr(IProductMgr *pProductMgr)
{
    m_pProductMgr = pProductMgr;
}

CMachineMgr::~CMachineMgr()
{
    Clear();
    
    m_pProductMgr = NULL;
}

IMachine* CMachineMgr::GetMachine(UINT32 uiMachineID) const
{
    MAP<UINT32, CMachine*>::const_iterator it = m_mapMachines.find(uiMachineID);
    if (it == m_mapMachines.end())
    {
        return NULL;
    }

    return it->second;
}

UINT32 CMachineMgr::GetMachineNum() const
{
    return m_mapMachines.size();
}

void CMachineMgr::Clear()
{
    MAP<UINT32, CMachine*>::iterator it = m_mapMachines.begin();
    while (it != m_mapMachines.end())
    {
        delete it->second;
        it++;
    }
    m_mapMachines.clear();
}

INT32 CMachineMgr::AddMachine(IProcess *pProcess, UINT32 uiBaseID,
                              IParamSet *pParamSet)
{
    CMachine *pMachine = new CMachine(m_pProductMgr);
    if (pMachine->Init(pProcess, uiBaseID, pParamSet) < 0)
    {
        delete pMachine;
        return RET_ERR;
    }
    
    //如果存在相同时机，报错
    if (NULL != GetMachine(pMachine->GetMachineID()))
    {
        TRACE("CMachineMgr::AddMachine", "Machine[ID=%d] already exist", pMachine->GetMachineID());
        delete pMachine;
        return RET_ERR;
    }

    CProcess *pCProcess = static_cast<CProcess*>(pProcess);
    if (pCProcess)
    {
        pCProcess->AddMachine(pMachine);
    }

    m_mapMachines[pMachine->GetMachineID()] = pMachine;

    return RET_SUC;
}

