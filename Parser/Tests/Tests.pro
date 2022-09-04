QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

SOURCES +=  tst_testparser.cpp
INCLUDEPATH += "$$PWD/../"
INCLUDEPATH += "$$PWD/../../Lexer/Lexer"
INCLUDEPATH += "$$PWD/../../ast/ast"
INCLUDEPATH += "$$PWD/../../Symbols/Symbols"
DEPENDPATH += "$$PWD/../"
LIBS += -L"$$PWD/../../build-Parser-Desktop-Debug" -lParser
LIBS += -L"$$PWD/../../Lexer/build-Lexer-Desktop-Debug" -lLexer
LIBS += -L"$$PWD/../../Symbols/build-Symbols-Desktop-Debug" -lSymbols
LIBS += -L"$$PWD/../../ast/build-ast-Desktop-Debug" -l ast
