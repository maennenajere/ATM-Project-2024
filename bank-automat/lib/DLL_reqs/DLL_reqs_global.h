#ifndef DLL_REQS_GLOBAL_H
#define DLL_REQS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(DLL_REQS_LIBRARY)
#define DLL_REQS_EXPORT Q_DECL_EXPORT
#else
#define DLL_REQS_EXPORT Q_DECL_IMPORT
#endif

#endif // DLL_REQS_GLOBAL_H
