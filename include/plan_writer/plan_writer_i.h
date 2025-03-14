
#ifndef __PLAN_WRITER_I_H__
#define __PLAN_WRITER_I_H__

#include "../interface.h"
#include "../algorith_def.h"


//���ų̽������Ľӿ�
class IPlanWriter
{
public:
	IPlanWriter() {}

	virtual ~IPlanWriter() {}

public:
    
	//���ų̽��������ļ���
	virtual BOOL Write(const STRING& strFileName, const ISolution* pSolution) = 0; 

};

DLL_EXPORT IPlanWriter * CreatePlanWriter(IResouce *pResource);
DLL_EXPORT BOOL          ReleasePlanWriter(IPlanWriter *pWriter);

#endif  //__PLAN_WRITER_I_H__

