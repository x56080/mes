
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

    //�η�ӹ��ò�Ʒ
    virtual BOOL        CanProduct(UINT32 uiProductID) const = 0;

};

class IMachineMgr
{
public:
    IMachineMgr() {}
    virtual ~IMachineMgr() {}

public:
    
    //��ȡ����
    virtual IMachine*       GetMachine(UINT32 uiMachineID) const = 0;

    //��ȡ���л�������
    virtual UINT32          GetMachineNum() const = 0;

};

//����ӿ�
class IProcess
{
public:
    IProcess() {}
    virtual ~IProcess() {}

public:
    //��ȡ��Ʒ�ͺŶ�Ӧ�Ļ�����
    virtual UINT32      GetMachineNum(UINT32 uiProductID) = 0;
    //��ȡ��Ʒ�ͺŶ�Ӧ�Ļ���ID��
    virtual UINT32      GetMachineSet(UINT32 uiProductID,
                                      VECTOR<UINT32> &vecMachineSet) = 0;

    //��ȡ������
    virtual UINT32      GetMachineNum() const = 0;
    //��ȡ����ID��
    virtual UINT32      GetMachineSet(VECTOR<UINT32> &vecMachineSet) const = 0;

    //�����л�ʱ��
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
    
    //��ȡ���̸���
    virtual UINT32          GetProcessNum() const = 0;

};

class IProductMgr
{
public:
    IProductMgr() {}
    virtual ~IProductMgr() {}

public:

    //���ݲ�Ʒ�ͺŻ�ȡ��Ʒ����
    virtual CProductProperty* GetProductProperty(UINT32 uiProductID) const = 0;

    //��ȡ��Ʒ������
    virtual UINT32 GetProductKind() const = 0;
    
};

//////////////////////////////////////////////////////////////////////////

typedef struct tagProductInfo
{
    UINT32      uiProductID;        //��Ʒ�ͺ�
    UINT32      uiProductNum;       //��Ʒ����
    UINT32      uiOrderID;          //����ID    
}SProductInfo;

//����
class IOrder
{
public:
    IOrder() {}
    virtual ~IOrder() {}

public:
    
    //��ȡ����ID
    virtual UINT32      GetOrderID() const = 0;

    //��ȡ��Ʒ����
    virtual UINT32      GetProductKinds() const = 0;

    //��ȡ������Ʒ��Ϣ�б�
    virtual UINT32      GetProductInfo(VECTOR<SProductInfo> &vecProductList) const = 0;

};

//��������
class IOrderMgr
{
public:
    IOrderMgr() {}
    virtual ~IOrderMgr() {}

public:

    //��ȡ���ж�����Ʒ����
    virtual UINT32      GetProductKinds() const = 0;

    //��ȡ��������
    virtual UINT32      GetOrderNum() const = 0;

    //��ȡ����
    virtual IOrder*     GetOrder(UINT32 uiIndex) const = 0;

    //��ȡ���ж�����Ʒ��Ϣ�б�
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

//����
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
