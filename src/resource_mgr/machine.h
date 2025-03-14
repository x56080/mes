
#ifndef __MACHINE_H__
#define __MACHINE_H__

#include "../../include/interface.h"
#include "../../include/product.h"

//设备状态定义
enum MACHINE_STATUS
{
    STATUS_NORMAL           = 0,        //正常
    STATUS_FAILURE          = 1         //故障
};

//设备基类定义
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
    
    //参否加工该产品
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
    //设备编号
    UINT32              m_uiID;
    //设备名称
    STRING              m_strName;
    //设备状态
    UINT32              m_uiStatus;
    //能够生产产品的规格（直径）范围
    DOUBLE              m_dbDiameterMin;
    DOUBLE              m_dbDiameterMax;

};

class CMachineMgr : public IMachineMgr
{
public:
    CMachineMgr(IProductMgr *pProductMgr);
    ~CMachineMgr();

public:
    //获取机器
    virtual IMachine*       GetMachine(UINT32 uiMachineID) const;
    
    //获取所有机器数量
    virtual UINT32          GetMachineNum() const;

public:
    INT32                   AddMachine(IProcess *pProcess, UINT32 uiBaseID, 
                                       IParamSet *pParamSet);

protected:
    void                    Clear();

private:
    //设备MAP表
    MAP<UINT32, CMachine*>      m_mapMachines;
    //产品管理指针
    IProductMgr                 *m_pProductMgr;

};

#endif //__MACHINE_H__
