#ifndef TESTQTLIB_GLOBAL_H
#define TESTQTLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef TESTQTLIB_LIB
# define TESTQTLIB_EXPORT Q_DECL_EXPORT
#else
# define TESTQTLIB_EXPORT Q_DECL_IMPORT
#endif

#endif // TESTQTLIB_GLOBAL_H
