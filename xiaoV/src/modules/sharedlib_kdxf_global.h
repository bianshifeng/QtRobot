#ifndef SHAREDLIB_KDXF_GLOBAL_H
#define SHAREDLIB_KDXF_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SHAREDLIB_KDXF_LIBRARY)
#  define SHAREDLIB_KDXFSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SHAREDLIB_KDXFSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SHAREDLIB_KDXF_GLOBAL_H