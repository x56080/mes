
#ifndef __PRODUCT_H__
#define __PRODUCT_H__

#include "base_type.h"

/*
�������(Ӣ������hub)���Ͳ�������

���--- �� * ֱ��, ��λΪ��
PCD---- ��˨�����Ľ�Բֱ��(Pitch Circle Diameter). ����̥��˿���γɵ�Բ��ֱ������λΪmm.
ET ---- �����������ͨ������ ET (���� E inpress T ieffe )���� Offset ������. ��ָ������˨��װ��
        �������������ߵľ���. �����˨��װ���������������߿�������(��)��һ�࣬�� ET Ϊ��ֵ. 
        ��λΪmm.
CB ---- �����������Բ�׵�ֱ����С.��λΪmm. �ǰ�װ�Ķ�λ�ף�����Ҫ.

*/

//��챹��
typedef struct tagSpec
{
    DOUBLE          m_dbWidth;          //��
    DOUBLE          m_dbDiameter;       //ֱ��
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
    UINT32          m_uiNum;            //����
    DOUBLE          m_dbDiameter;       //ֱ��    
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

//��Ʒ���ඨ��
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
