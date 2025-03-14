
#ifndef __PROCESS_H__
#define __PROCESS_H__

#include "../../include/interface.h"


class CProcess : public IProcess
{
public:
	CProcess();
	virtual ~CProcess();

	//��ʼ��
    INT32               Init(IParamSet *pParamSet);

    BOOL                AddMachine(IMachine *pMachine);
    
public:
    //��ȡ��Ʒ�ͺŶ�Ӧ�Ļ�����
    virtual UINT32      GetMachineNum(UINT32 uiProductID);
    //��ȡ��Ʒ�ͺŶ�Ӧ�Ļ���ID��
    virtual UINT32      GetMachineSet(UINT32 uiProductID,
                                      VECTOR<UINT32> &vecMachineSet);
    
    //��ȡ������
    virtual UINT32      GetMachineNum() const;
    //��ȡ����ID��
    virtual UINT32      GetMachineSet(VECTOR<UINT32> &vecMachineSet) const;
    
    //�����л�ʱ��
    virtual time_t      CalcSwitchTime(UINT32 uiPrevProductID, 
                                       UINT32 uiCurProductID) const;
    
    virtual STRING      GetName() const;
    virtual UINT32      GetType() const;

private:
    //�����б�
    VECTOR<IMachine*>   m_vecMachine;
    //��������
    UINT32              m_uiProcessType;
    //��������
    STRING              m_strProcessName;
    //�л�ʱ��
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
    
    //��ȡ���̸���
    virtual UINT32          GetProcessNum() const;

protected:
    
    void                    Clear();

private:

	//������Ϣ
	VECTOR<CProcess*>       m_vecProcesses;

};

#endif //__PROCESS_H__