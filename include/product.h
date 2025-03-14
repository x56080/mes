
#ifndef __PRODUCT_H__
#define __PRODUCT_H__

#include "base_type.h"

/*
汽车轮毂(英文名叫hub)规格和参数描述

规格--- 宽 * 直径, 单位为寸
PCD---- 螺栓孔中心节圆直径(Pitch Circle Diameter). 即轮胎螺丝所形成的圆的直径，单位为mm.
ET ---- 这个参数我们通常会用 ET (德语 E inpress T ieffe )或者 Offset 来描述. 是指车轮螺栓安装面
        到轮辋的中心线的距离. 如果螺栓安装面在轮辋的中心线靠近辐条(外)的一侧，这 ET 为正值. 
        单位为mm.
CB ---- 就是轮毂中心圆孔的直径大小.单位为mm. 是安装的定位孔，很重要.

*/

//轮毂规格
typedef struct tagSpec
{
    DOUBLE          m_dbWidth;          //宽
    DOUBLE          m_dbDiameter;       //直径
}SHubSpec;

class DLL_EXPORT CSpec : public tagSpec
{
public:
    CSpec();
    CSpec(DOUBLE dbWidth, DOUBLE dbDiameter);
    CSpec(const CSpec &right);
    ~CSpec();

    void    SetWidth(DOUBLE dbWidth);
    void    SetDiameter(DOUBLE dbDiameter);
    DOUBLE  GetWidth() const;
    DOUBLE  GetDiameter() const;

    bool    operator== (const CSpec &right) const;
    bool    operator!= (const CSpec &right) const;
    CSpec&  operator= (const CSpec &right);

};

//PCD
typedef struct tagPCD
{
    UINT32          m_uiNum;            //个数
    DOUBLE          m_dbDiameter;       //直径    
}SPCD;

class DLL_EXPORT CPCD : public SPCD
{
public:
    CPCD();
    CPCD(UINT32 uiNum, DOUBLE dbDiameter);
    CPCD(const CPCD &right);
    ~CPCD();

    void    SetNum(UINT32 uiNum);
    void    SetDiameter(DOUBLE dbDiameter);
    UINT32  GetNum() const;
    DOUBLE  GetDiameter() const;

    bool    operator== (const CPCD &right) const;
    bool    operator!= (const CPCD &right) const;
    CPCD&   operator= (const CPCD &right);

};

//ET
typedef INT32           ET_TYPE;
//CB
typedef DOUBLE          CB_TYPE;

//产品基类定义
class DLL_EXPORT CProductProperty
{
public:
    CProductProperty(UINT32 uiProductID, CSpec &cSpec, ET_TYPE cET, 
                     CB_TYPE cCB,  CPCD &cPCD);    
    CProductProperty(const CProductProperty &right);
    virtual ~CProductProperty();

public:
    CSpec       GetSpec() const;
    ET_TYPE     GetET() const;
    CB_TYPE     GetCB() const;
    CPCD        GetPCD() const;

    UINT32      GetProductivity(UINT32 uiProcessType) const;
    UINT32      GetProductID() const;

    bool        operator== (const CProductProperty &right) const;
    bool        operator!= (const CProductProperty &right) const;
    CProductProperty&   operator= (const CProductProperty &right);

public:

    void        AddProductivity(UINT32 uiProcessType, UINT32 uiValue);

private:
    CSpec       m_cSpec;
    ET_TYPE     m_cET;
    CB_TYPE     m_cCB;
    CPCD        m_cPCD;

    UINT32      m_uiProductID;
    MAP<UINT32, UINT32>         m_mapProductivity;

};

#endif //__PRODUCT_H__
