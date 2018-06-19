#ifndef COS_UPDATE_LIB_GLOBAL_H
#define COS_UPDATE_LIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(COS_UPDATE_LIB_LIBRARY)
#  define COS_UPDATE_LIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define COS_UPDATE_LIBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // COS_UPDATE_LIB_GLOBAL_H
