QT += core gui opengl multimedia
#CONFIG += c++14 warn_off debug
CONFIG += c++14 warn_off release
TARGET = masala.bin
TEMPLATE = app


#QMAKE_CXX = clang++

QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE *= -O3


# Platform specific
macx {
    INCLUDEPATH += /usr/local/opt/readline/include
    LIBS += -L/usr/local/opt/readline/lib
    TARGET = masala
}


# Third Party stuff
# Chaiscript
CHAISCRIPT_DIR = ../third_party/ChaiScript-6.0.0
INCLUDEPATH += $${CHAISCRIPT_DIR}/include
#LIBS = -L$${CHAISCRIPT_DIR}/lib -lchaiscript_stdlib-6.0.0
LIBS += -ldl
LIBS += -lreadline


HEADERS  += masalawindow.h\
            mglwidget.h\
            mterminal.h\
            meventtypes.h\
            masalaapp.h\
            msprite.h\
            mtriangle.h\
            mframe.h\
            msequence.h\
            mchaiqt.h\
            mchai.h\
            mkeystate.h\
            mkeyboardstate.h\
            mupdatethread.h\
            mtexturestruct.h\
            mutil.h\
            mtilemap.h\
            mspritebatch.h\
            msoundeffect.h

SOURCES += main.cpp\
           masalawindow.cpp\
           mglwidget.cpp\
           mterminal.cpp\
           masalaapp.cpp\
           msprite.cpp\
           mtriangle.cpp\
           mframe.cpp\
           msequence.cpp\
           mchaiqt.cpp\
           mchai.cpp\
           mupdatethread.cpp\
           mutil.cpp\
           mtilemap.cpp\
           mspritebatch.cpp\
           msoundeffect.cpp

