#-------------------------------------------------
#
# Project created by QtCreator 2014-05-03T19:02:03
#
#-------------------------------------------------

QT       += core gui
QT += sql
QT += xml
greaterThan(QT_MAJOR_VERSION, 4):
QT += widgets


TARGET = projet
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        semestre.cpp \
        semestresmanager.cpp \
        etudiant.cpp \
        solution.cpp \
        ects.cpp \
        inscription.cpp \
        UVEditeur.cpp \
        UTProfiler.cpp \
    formation.cpp \
    uv.cpp \
    categorie.cpp \
    etudiantstorage.cpp \
    etudiantstoragexml.cpp \
    credits.cpp \
    equivalence.cpp \
    creeretudiantdialog.cpp \
    etudiantediteur.cpp \
    solutionediteur.cpp \
    creerinscriptiondialog.cpp \
    categorieediteur.cpp \
    formationediteur.cpp

HEADERS  += mainwindow.h \
    semestre.h \
    semestresmanager.h \
    etudiant.h \
    solution.h \
    ects.h \
    inscription.h \
    UVEditeur.h \
    UTProfiler.h \
    formation.h \
    uv.h \
    categorie.h \
    etudiantstorage.h \
    etudiantstoragexml.h \
    credits.h \
    equivalence.h \
    creeretudiantdialog.h \
    etudiantediteur.h \
    solutionediteur.h \
    creerinscriptiondialog.h \
    categorieediteur.h \
    formationediteur.h


FORMS    += mainwindow.ui \
    categorieediteur.ui \
    formationediteur.ui
OTHER_FILES += \
    UV_XML.xml \
    ../BDbase.db

QMAKE_CXXFLAGS += -std=c++11
QMAKE_CFLAGS += -std=c++11



