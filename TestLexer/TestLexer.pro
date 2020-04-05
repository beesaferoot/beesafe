QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle


SOURCES +=  tst_testlexer.cpp
INCLUDEPATH += "/home/beesafe/Documents/Source Code/Prj/Qt_Creator/BS/Lexer/Lexer"
DEPENDPATH += "/home/beesafe/Documents/Source Code/Prj/Qt_Creator/BS/Lexer/Lexer"
LIBS += -L"/home/beesafe/Documents/Source Code/Prj/Qt_Creator/BS/Lexer/build-Lexer-Qt-Debug" -lLexer
