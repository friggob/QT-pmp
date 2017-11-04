#ifndef CLI_GLOBAL_H
#define CLI_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CLI_LIBRARY)
#  define CLISHARED_EXPORT Q_DECL_EXPORT
#else
#  define CLISHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // CLI_GLOBAL_H
