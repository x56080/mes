
#ifndef __EXPORT_DLL_H
#define __EXPORT_DLL_H

#ifdef WIN32
#define DLL_EXPORT          __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

#endif //__EXPORT_DLL_H

