QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

SOURCES +=  tst_testparser.cpp
INCLUDEPATH += "/home/beesafe/Documents/Source Code/Prj/Qt_Creator/BS/Parser"
INCLUDEPATH += "/home/beesafe/Documents/Source Code/Prj/Qt_Creator/BS/Lexer/Lexer"
INCLUDEPATH += "/home/beesafe/Documents/Source Code/Prj/Qt_Creator/BS/ast/ast"
INCLUDEPATH += "/home/beesafe/Documents/Source Code/Prj/Qt_Creator/BS/Symbols/Symbols"
DEPENDPATH += "/home/beesafe/Documents/Source Code/Prj/Qt_Creator/BS/Parser"
LIBS += -L"/home/beesafe/Documents/Source Code/Prj/Qt_Creator/BS/build-Parser-Qt-Debug" -lParser
LIBS += -L"/home/beesafe/Documents/Source Code/Prj/Qt_Creator/BS/Lexer/build-Lexer-Qt-Debug" -lLexer
LIBS += -L"/home/beesafe/Documents/Source Code/Prj/Qt_Creator/BS/Symbols/build-Symbols-Qt-Debug" -lSymbols
LIBS += -L"/home/beesafe/Documents/Source Code/Prj/Qt_Creator/BS/ast/build-ast-Qt-Debug" -l ast
