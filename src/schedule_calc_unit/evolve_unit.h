
#ifndef __EVOLVE_UNIT_H__
#define __EVOLVE_UNIT_H__

#include "../../include/algorith_def.h"
#include "solution.h"

enum MUTATION_METHOD
{
    MUTATION_RAND       =   1,      //��ȫ���
    MUTATION_ADJUST     =   2       //΢��
};

//������Ԫ
class CEvolveUnit : public IEvolveUnit
{
public:
    CEvolveUnit(IResouce *pResouce);
    ~CEvolveUnit();

public:
    virtual INT32   Init(IParamSet *pParamSet);
    
    virtual INT32   Evolve(ISolutionSet *pSolutionSet);

protected:
    
    //��������
    INT32   MutationSet(CSolutionSet *pSolutionSet);
    INT32   Mutation(CSolution *pSolution);

    //�����Ŵ�����
    INT32   CrossOverSet(CSolutionSet *pSolutionSet);
    INT32   CrossOver(CSolution *pOne, CSolution *pTwo);

    //��ת����
    INT32   ReverseSet(CSolutionSet *pSolutionSet);
    INT32   Reverse(CSolution *pSolution);
    INT32   Reverse(UINT32 uiBegin, UINT32 uiEnd, CGene *pGene, CSolution *pSolution);

private:
    
    //��������������� [0, uiNum)
    INT32   RandSerail(UINT32 *pArray, UINT32 uiNum);

private:
    IResouce                *m_pResource;
    IParamSet               *m_pParamSet;

};

#endif //__EVOLVE_UNIT_H__
