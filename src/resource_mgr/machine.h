
#ifndef __MACHINE_H__
#define __MACHINE_H__

#include "../../include/interface.h"
#include "../../include/product.h"

//�豸״̬����
enum MACHINE_STATUS
{
    STATUS_NORMAL           = 0,        //����
    STATUS_FAILURE          = 1         //����
};

//�豸���ඨ��
class CMachine : public IMachine
{
public:
    CMachine(IProductMgr *pProductMgr);
    CMachine(IProductMgr *pProductMgr, UINT32 uiID, IProcess *pProcess, 
             const STRING &strName, DOUBLE dbDiaMin, DOUBLE dbDiaMax);
    virtual ~CMachine();

    INT32               Init(IProcess *pProcess, UINT32 uiBaseID, 
                             IParamSet *pParamSet);

public:
    virtual UINT32      GetMachineID() const;
    virtual STRING      GetMachineName() const;
    virtual UINT32      GetProcessType() const;
    
    //�η�ӹ��ò�Ʒ
    virtual BOOL        CanProduct(UINT32 uiProductID) const;

public:
    UINT32              GetStatus() const;

    void                SetStatus(UINT32 uiStatus);
    void                SetName(const STRING &strName);
    void                SetMachineID(UINT32 uiID);
    void                SetProcess(IProcess *pProcess);
    void                SetProductSpec(DOUBLE dbDiaMin, DOUBLE dbDiaMax);

protected:
    IProductMgr         *m_pProductMgr;
    IProcess            *m_pProcess;
    //�豸���
    UINT32              m_uiID;
    //�豸����
    STRING              m_strName;
    //�豸״̬
    UINT32              m_uiStatus;
    //�ܹ�������Ʒ�Ĺ��ֱ������Χ
    DOUBLE              m_dbDiameterMin;
    DOUBLE              m_dbDiameterMax;

};

class CMachineMgr : public IMachineMgr
{
public:
    CMachineMgr(IProductMgr *pProductMgr);
    ~CMachineMgr();

public:
    //��ȡ����
    virtual IMachine*       GetMachine(UINT32 uiMachineID) const;
    
    //��ȡ���л�������
    virtual UINT32          GetMachineNum() const;

public:
    INT32                   AddMachine(IProcess *pProcess, UINT32 uiBaseID, 
                                       IParamSet *pParamSet);

protected:
    void                    Clear();

private:
    //�豸MAP��
    MAP<UINT32, CMachine*>      m_mapMachines;
    //��Ʒ����ָ��
    IProductMgr                 *m_pProductMgr;

};

#endif //__MACHINE_H__
