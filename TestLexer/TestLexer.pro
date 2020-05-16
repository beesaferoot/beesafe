QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle


SOURCES +=  tst_testlexer.cpp
INCLUDEPATH += "$$PWD/../Lexer/Lexer"
DEPENDPATH += "$$PWD/../Lexer/Lexer" #"/home/beesafe/Documents/Source Code/Prj/Qt_Creator/BS/Lexer/Lexer"
LIBS += -L "$$PWD/../Lexer/build-Lexer-Qt-Debug" -lLexer
