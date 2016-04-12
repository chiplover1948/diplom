AC_INIT([ODESolver], [1.0], [avlasov@itis.cs.msu.ru])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_ARG_ENABLE([javascript],
[  --enable-javasctipt    Turn on debugging],
[case "${enableval}" in
  yes) javascript=true ;;
  no)  javascript=false ;;
  *) AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
esac],[javascript=false])
AM_CONDITIONAL([JS], [test x$javascript = xtrue])
if test x$javascript = xtrue
then
	AC_SUBST([exeext], [.js])
	AC_SUBST([flags], ['-s RESERVED_FUNCTION_POINTERS=20 -s ALLOW_MEMORY_GROWTH=1 --memory-init-file 0'])
	AC_SUBST([bind], ['--bind'])
fi
AC_PROG_CXX
AM_PROG_AR
AC_PROG_RANLIB
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
 Makefile
 src/Makefile
 src/solver/Makefile
 src/examples/Makefile
])
AC_OUTPUT
