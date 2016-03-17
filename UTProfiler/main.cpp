#include "mainwindow.h"
#include <QApplication>
#include <QtSql>
#include <QMessageBox>
#include "uv.h"

int main(int argc, char *argv[])
{


    QApplication a(argc, argv);

/*
    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE", "BDbase");
    db.setDatabaseName("../BDbase.db");
    if(!db.open()){
        QMessageBox::critical(0,"Database Error",db.lastError().text());
    }

    UVManager::getInstance().getInstance().load(db);

    query.exec("create table student(id varchar,name varchar)");
    query.exec("insert into student values(1,'éasdas')");
    query.exec("insert into student values(2,'éafsdas')");
    query.exec("insert into student values(3,'éafasdas')");
    query.exec("insert into student values(3,'éasasdas')");
    query.exec("insert into student values(4,'éaasfsdas')");
    query.exec("insert into student values(5,'éasssdas')");
    query.exec("insert into student values(6,'éaasdas')");
    query.exec("select id,name from student where id>=1");
    while(query.next()){
        QMessageBox::critical(0,"asd", query.value(1).toString());
    }*/
    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE", "BDbase");
    db.setDatabaseName("../BDbase.db");
    try {
        FormationManager::getInstance().load("../BDbase.db");
    }catch(UTProfilerException& e){
        QMessageBox::warning(NULL, "Chargement Catalogue",e.getInfo());
    }
    try {
        categorieManager::getInstance().load("../BDbase.db");
    }catch(UTProfilerException& e){
        QMessageBox::warning(NULL, "Chargement Catalogue",e.getInfo());
    }
    try {
        UVManager::getInstance().load("../BDbase.db");
        QMessageBox::information(NULL, "Chargement Catalogue", "Le catalogue d’UVs a été chargé.");
    }catch(exception& e){
        QMessageBox::warning(NULL, "Chargement Catalogue", "Erreur lors du chargement du fichier (non valide ?)");
    }

    MainWindow w;
    w.show();

    return a.exec();
}
