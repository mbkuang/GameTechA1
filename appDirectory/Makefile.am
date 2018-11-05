ACLOCAL_AMFLAGS= -I m4
noinst_HEADERS= BaseApplication.h TutorialApplication.h

bin_PROGRAMS= A1
A1_CPPFLAGS= -I$(top_srcdir)
A1_SOURCES= BaseApplication.cpp TutorialApplication.cpp
A1_CXXFLAGS= -I/usr/include/OGRE -pthread $(OIS_CFLAGS)
A1_LDADD= -L/usr/lib/x86_64-linux-gnu/OGRE-1.9.0 -lOgreMain -lpthread $(OIS_LIBS)
A1_LDFLAGS= -lOgreOverlay -lboost_system

EXTRA_DIST= buildit makeit
AUTOMAKE_OPTIONS= foreign
