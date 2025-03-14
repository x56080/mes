
#ifndef __PROCESS_H__
#define __PROCESS_H__

#include "../../include/interface.h"


class CProcess : public IProcess
{
public:
	CProcess();
	virtual ~CProcess();

	//初始化
    INT32               Init(IParamSet *pParamSet);

    BOOL                AddMachine(IMachine *pMachine);
    
public:
    //获取产品型号对应的机器数
    virtual UINT32      GetMachineNum(UINT32 uiProductID);
    //获取产品型号对应的机器ID集
    virtual UINT32      GetMachineSet(UINT32 uiProductID,
                                      VECTOR<UINT32> &vecMachineSet);
    
    //获取机器数
    virtual UINT32      GetMachineNum() const;
    //获取机器ID集
    virtual UINT32      GetMachineSet(VECTOR<UINT32> &vecMachineSet) const;
    
    //计算切换时间
    virtual time_t      CalcSwitchTime(UINT32 uiPrevProductID, 
                                       UINT32 uiCurProductID) const;
    
    virtual STRING      GetName() const;
    virtual UINT32      GetType() const;

private:
    //机器列表
    VECTOR<IMachine*>   m_vecMachine;
    //流程类型
    UINT32              m_uiProcessType;
    //流程名称
    STRING              m_strProcessName;
    //切换时间
    UINT32              m_uiSwtichTime;

};


class CProcessMgr : public IProcessMgr
{
public:
	CProcessMgr();

	virtual ~CProcessMgr();

    INT32                   AddProcess(IParamSet *pParamSet);

public:
    virtual IProcess*       GetProcessByType(UINT32 uiType) const;
    virtual IProcess*       GetProcess(UINT32 uiIndex) const;

    virtual UINT32          Type2Index(UINT32 uiType) const;
    virtual UINT32          Index2Type(UINT32 uiIndex) const;
    
    //获取流程个数
    virtual UINT32          GetProcessNum() const;

protected:
    
    void                    Clear();

private:

	//流程信息
	VECTOR<CProcess*>       m_vecProcesses;

};

#endif //__PROCESS_H__