
#ifndef __BASE_TYPE_H__
#define __BASE_TYPE_H__

//////////////////////////////////////////////////////////////////////////
//头文件定义
//////////////////////////////////////////////////////////////////////////

#pragma warning(disable : 4786)
#pragma warning(disable : 4251)   
#pragma warning(disable : 4273)   

//C库基本的头文件
#include <stdio.h>
#include <iostream>
#include <string>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <algorithm>

//ACE头文件
#include "ace/ace.h"
#include "ace/ace_export.h"
#include "ace/sock.h"
#include "ace/dirent.h"
#include "ace/os.h"
#include "ace/mutex.h"
#include "ace/thread_mutex.h"
#include "ace/event.h"
#include "ace/thread.h"
#include "ace/thread_manager.h"
#include "ace/task.h"
#include "ace/recursive_thread_mutex.h"

//STL属性
#include <LIST>
#include <MAP>
#include <deque>
#include <STRING>
#include <vector>
#include <set>

//////////////////////////////////////////////////////////////////////////
//基本类型重定义
//////////////////////////////////////////////////////////////////////////

typedef int                 INT32;
typedef unsigned int        UINT32;
typedef short               SHORT;
typedef unsigned short      USHORT;
typedef short               INT16;
typedef unsigned short      UINT16;
typedef char                CHAR;
typedef unsigned char       UCHAR;
//typedef char                INT8;
typedef unsigned char       UINT8;
typedef long                LONG;
typedef unsigned long       ULONG;
typedef float               FLOAT;
typedef double              DOUBLE;
typedef int                 BOOL;

#ifdef WIN32
typedef __int64             INT64;
#else
typedef long long           INT64;
#endif //WIN32

//////////////////////////////////////////////////////////////////////////
//基本宏定义
//////////////////////////////////////////////////////////////////////////

#define LIST                std::list
#define MAP                 std::map
#define VECTOR              std::vector
#define DEQUE               std::deque
#define STRING              std::string
#define SET                 std::set

#define TRUE                1
#define FALSE               0
#define NULL                0

#define EPSILON             (1e-6)
#define TMP_PATH            (256)
#undef  MAX_PATH
#define MAX_PATH            (256)
#define N_LINE              (1024)

#ifndef DLL_EXPORT
#ifdef WIN32
#define DLL_EXPORT          __declspec(dllimport)
#else
#define DLL_EXPORT
#endif //WIN32
#endif //DLL_EXPORT

#define RET_ERR             (-1)
#define RET_SUC             (0)

//////////////////////////////////////////////////////////////////////////
//函数宏定义
//////////////////////////////////////////////////////////////////////////

#ifdef WIN32
#define VSNPRINTF   _vsnprintf
#define SNPRINTF    _snprintf
#else
#define VSNPRINTF vsnprintf
#define SNPRINTF  snprintf
#endif //WIN32

//////////////////////////////////////////////////////////////////////////
//枚举定义
//////////////////////////////////////////////////////////////////////////

//工序类型定义
enum PROCESS_TYPE
{
    TYPE_CAST               = 1,        //铸造
    TYPE_COLLAPSE           = 2,        //切冒
    TYPE_PLAIN_MILLING      = 3,        //铣平面
    TYPE_TEMPERING          = 4         //热处理
};

#endif //__BASE_TYPE_H__

