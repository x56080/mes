
#ifndef __EVOLVE_UNIT_H__
#define __EVOLVE_UNIT_H__

#include "../../include/algorith_def.h"
#include "solution.h"

enum MUTATION_METHOD
{
    MUTATION_RAND       =   1,      //完全随机
    MUTATION_ADJUST     =   2       //微调
};

//进化单元
class CEvolveUnit : public IEvolveUnit
{
public:
    CEvolveUnit(IResouce *pResouce);
    ~CEvolveUnit();

public:
    virtual INT32   Init(IParamSet *pParamSet);
    
    virtual INT32   Evolve(ISolutionSet *pSolutionSet);

protected:
    
    //变异算子
    INT32   MutationSet(CSolutionSet *pSolutionSet);
    INT32   Mutation(CSolution *pSolution);

    //交叉遗传算子
    INT32   CrossOverSet(CSolutionSet *pSolutionSet);
    INT32   CrossOver(CSolution *pOne, CSolution *pTwo);

    //逆转算子
    INT32   ReverseSet(CSolutionSet *pSolutionSet);
    INT32   Reverse(CSolution *pSolution);
    INT32   Reverse(UINT32 uiBegin, UINT32 uiEnd, CGene *pGene, CSolution *pSolution);

private:
    
    //产生随机连续序列 [0, uiNum)
    INT32   RandSerail(UINT32 *pArray, UINT32 uiNum);

private:
    IResouce                *m_pResource;
    IParamSet               *m_pParamSet;

};

#endif //__EVOLVE_UNIT_H__
