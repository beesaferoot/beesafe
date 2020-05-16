QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

SOURCES +=  tst_testparser.cpp
INCLUDEPATH += "$$PWD/../"
INCLUDEPATH += "$$PWD/../../Lexer/Lexer" #"/home/beesafe/Documents/Source Code/Prj/Qt_Creator/BS/Lexer/Lexer"
INCLUDEPATH += "$$PWD/../../ast/ast" #"/home/beesafe/Documents/Source Code/Prj/Qt_Creator/BS/ast/ast"
INCLUDEPATH += "$$PWD/../../Symbols/Symbols" #"/home/beesafe/Documents/Source Code/Prj/Qt_Creator/BS/Symbols/Symbols"
DEPENDPATH += "$$PWD/../" #/home/beesafe/Documents/Source Code/Prj/Qt_Creator/BS/Parser"
LIBS += -L"$$PWD/../../build-Parser-Qt-Debug" -lParser #"/home/beesafe/Documents/Source Code/Prj/Qt_Creator/BS/build-Parser-Qt-Debug" -lParser
LIBS += -L"$$PWD/../../Lexer/build-Lexer-Qt-Debug" -lLexer #"/home/beesafe/Documents/Source Code/Prj/Qt_Creator/BS/Lexer/build-Lexer-Qt-Debug" -lLexer
LIBS += -L"$$PWD/../../Symbols/build-Symbols-Qt-Debug" -lSymbols#"/home/beesafe/Documents/Source Code/Prj/Qt_Creator/BS/Symbols/build-Symbols-Qt-Debug" -lSymbols
LIBS += -L"$$PWD/../../ast/build-ast-Qt-Debug" -l ast #"/home/beesafe/Documents/Source Code/Prj/Qt_Creator/BS/ast/build-ast-Qt-Debug" -l ast
