
#ifndef __INTERFACE_H__
#define __INTERFACE_H__

#include "base_type.h"
#include "product.h"

class IMachine
{
public:
    IMachine() {}
    virtual ~IMachine() {}

public:
    virtual UINT32      GetMachineID() const = 0;
    virtual STRING      GetMachineName() const = 0;
    virtual UINT32      GetProcessType() const = 0;

    //参否加工该产品
    virtual BOOL        CanProduct(UINT32 uiProductID) const = 0;

};

class IMachineMgr
{
public:
    IMachineMgr() {}
    virtual ~IMachineMgr() {}

public:
    
    //获取机器
    virtual IMachine*       GetMachine(UINT32 uiMachineID) const = 0;

    //获取所有机器数量
    virtual UINT32          GetMachineNum() const = 0;

};

//工序接口
class IProcess
{
public:
    IProcess() {}
    virtual ~IProcess() {}

public:
    //获取产品型号对应的机器数
    virtual UINT32      GetMachineNum(UINT32 uiProductID) = 0;
    //获取产品型号对应的机器ID集
    virtual UINT32      GetMachineSet(UINT32 uiProductID,
                                      VECTOR<UINT32> &vecMachineSet) = 0;

    //获取机器数
    virtual UINT32      GetMachineNum() const = 0;
    //获取机器ID集
    virtual UINT32      GetMachineSet(VECTOR<UINT32> &vecMachineSet) const = 0;

    //计算切换时间
    virtual time_t      CalcSwitchTime(UINT32 uiPrevProductID, 
                                       UINT32 uiCurProductID) const = 0;

    virtual STRING      GetName() const = 0;
    virtual UINT32      GetType() const = 0;

};

class IProcessMgr
{
public:
    IProcessMgr() {}
    virtual ~IProcessMgr() {}

public:
    
    virtual IProcess*       GetProcessByType(UINT32 uiType) const = 0;
    virtual IProcess*       GetProcess(UINT32 uiIndex) const = 0;

    virtual UINT32          Type2Index(UINT32 uiType) const = 0;
    virtual UINT32          Index2Type(UINT32 uiIndex) const = 0;
    
    //获取流程个数
    virtual UINT32          GetProcessNum() const = 0;

};

class IProductMgr
{
public:
    IProductMgr() {}
    virtual ~IProductMgr() {}

public:

    //根据产品型号获取产品属性
    virtual CProductProperty* GetProductProperty(UINT32 uiProductID) const = 0;

    //获取产品的种类
    virtual UINT32 GetProductKind() const = 0;
    
};

//////////////////////////////////////////////////////////////////////////

typedef struct tagProductInfo
{
    UINT32      uiProductID;        //产品型号
    UINT32      uiProductNum;       //产品数量
    UINT32      uiOrderID;          //订单ID    
}SProductInfo;

//订单
class IOrder
{
public:
    IOrder() {}
    virtual ~IOrder() {}

public:
    
    //获取订单ID
    virtual UINT32      GetOrderID() const = 0;

    //获取产品种类
    virtual UINT32      GetProductKinds() const = 0;

    //获取订单产品信息列表
    virtual UINT32      GetProductInfo(VECTOR<SProductInfo> &vecProductList) const = 0;

};

//订单管理
class IOrderMgr
{
public:
    IOrderMgr() {}
    virtual ~IOrderMgr() {}

public:

    //获取所有订单产品种类
    virtual UINT32      GetProductKinds() const = 0;

    //获取订单个数
    virtual UINT32      GetOrderNum() const = 0;

    //获取订单
    virtual IOrder*     GetOrder(UINT32 uiIndex) const = 0;

    //获取所有订单产品信息列表
    virtual UINT32      GetAllProductInfo(VECTOR<SProductInfo> &vecProductList) const = 0;

};

class IParamSet;
//Resouce
class IResouce
{
public:
    IResouce() {}
    virtual ~IResouce() {}

public:
    
    virtual IMachineMgr*        GetMachineMgr() const = 0;

    virtual IProcessMgr*        GetProcessMgr() const = 0;

    virtual IOrderMgr*          GetOrderMgr() const = 0;

    virtual IProductMgr*        GetProductMgr() const = 0;

    virtual IParamSet*          GetConfigSet() const = 0;

};

//配置
class IParamSet
{
public:
    IParamSet() {}
    virtual ~IParamSet() {}

public:

    virtual UINT32      GetUint(const char *pszAttr) const = 0;

    virtual STRING      GetString(const char *pszAttr) const = 0;

    virtual BOOL        HasParam(const char *pszAttr) const = 0;

    virtual DOUBLE      GetDouble(const char *pszAttr) const = 0;

};

#define RDX_Int(pParamSet, nValue, pszAttr) \
    if (!pParamSet->HasParam(pszAttr)) \
    { \
        TRACE("CONFIG", "Param [%s] not exist", pszAttr); \
        return RET_ERR; \
    } \
    nValue = pParamSet->GetUint(pszAttr);

#define RDX_String(pParamSet, strValue, pszAttr) \
    if (!pParamSet->HasParam(pszAttr)) \
    { \
        TRACE("CONFIG", "Param [%s] not exist", pszAttr); \
        return RET_ERR; \
    } \
    strValue = pParamSet->GetString(pszAttr);

#define RDX_Double(pParamSet, dbValue, pszAttr) \
    if (!pParamSet->HasParam(pszAttr)) \
    { \
        TRACE("CONFIG", "Param [%s] not exist", pszAttr); \
        return RET_ERR; \
    } \
    dbValue = pParamSet->GetDouble(pszAttr);

//////////////////////////////////////////////////////////////////////////

DLL_EXPORT IResouce* CreateResouce(const char *pszConfigFile);
DLL_EXPORT BOOL      ReleaseResouce(IResouce *pResouce);

#endif //__INTERFACE_H__
