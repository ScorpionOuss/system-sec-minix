PROG=	csc
SRCS=	main.c signcheck.c

DPADD += ${LIBSYS}
LDADD += -lsys

CPPFLAGS.signcheck.c+= -Dregcomp=_regcomp -Dregfree=_regfree
.include <minix.service.mk>
