#include "uv.h"
#include "categorie.h"
#include <sstream>
#include <QFile>
#include <QTextCodec>
#include <QtXml>
#include <QMessageBox>
#include <QtSql>
#include <algorithm>

using namespace std;

QTextStream& operator<<(QTextStream& f, const UV& uv){
    return f<<uv.getCode()<<", "<<*(uv.getCategorie())<<", "<<uv.getNbCredits()<<" credits, "<<uv.getTitre();
}

UVManager::UVManager():modification(false),file(""){
}


/* construction fichier... à enlever */
bool isInFile(QString code, QString file){
    QFile fin(file);
    if (!fin.open(QIODevice::ReadOnly | QIODevice::Text)) throw UTProfilerException(QString("erreur ouverture fichier ")+file);
    QTextStream flux(&fin);
     while (!flux.atEnd()){
        QString codeuv=flux.readLine();
        if (code==codeuv) return true;
    }
    fin.close();
    return false;
}

void UVManager::load(const QString& f){
// Version Sqlite
    QSqlDatabase db=QSqlDatabase::database("BDbase");

    if(!db.open()){
        throw UTProfilerException("Database Error"+db.lastError().text());
    }
    QSqlQuery query(db);
    //query.exec("create table categorie(code varchar,titre varchar, parent varchar)");
    if (!query.exec("select * from UV")) {
        throw UTProfilerException("SQL Query failed: " + query.executedQuery());
    }

    while(query.next()){
        ajouterUV(query.value(0).toString(),query.value(1).toString(),query.value(2).toInt(),
                  categorieManager::getInstance().getCategorie(query.value(3).toString()),query.value(4).toBool(),query.value(5).toBool());
    }
    db.close();
}



void UVManager::save(const QString& f, QString code){
    shared_ptr<UV> uv = UVManager::getInstance().getUV(code);

    QSqlDatabase db=QSqlDatabase::database("BDbase");
    if(!db.open()){
        throw UTProfilerException("Database Error"+db.lastError().text());
    }
    QSqlQuery query(db);
    query.prepare("select * from UV where code=:code");
    query.bindValue(":code", uv->getCode());
    if (!query.exec()) {
        throw UTProfilerException("SQL Query failed: " + query.executedQuery());
    }
    if(query.next()) {
        //QMessageBox::critical(0,"asd", query.value(0).toString());
        query.prepare("UPDATE UV "
                      "SET titre=:titre, nbCredits=:nbCredits, cat=:cat, automne=:automne, printemps=:printemps "
                      "WHERE code=:code");

    }else{
        query.prepare("insert into UV values(:code,:titre,:nbCredits,:cat,:automne,:printemps)");
    }
    query.bindValue(":code",uv->getCode());
    query.bindValue(":titre",uv->getTitre());
    query.bindValue(":nbCredits",uv->getNbCredits());
    query.bindValue(":cat",uv->getCategorie()->getCode());
    query.bindValue(":automne",uv->ouvertureAutomne());
    query.bindValue(":printemps",uv->ouverturePrintemps());
    if (!query.exec()) {
        throw UTProfilerException("Query failed: " + query.executedQuery());
    }//QMessageBox::critical(0,"asd",query.executedQuery());
    db.close();
}

void UVManager::remove(const QString &code)
{
    for(auto it= UVManager::getInstance().getUvs().begin(); it!= UVManager::getInstance().getUvs().end();it++ )
        if ((*it).second->getCode()==code)
            UVManager::getInstance().getUvs().erase(it);
    QSqlDatabase db=QSqlDatabase::database("BDbase");
    if(!db.open()){
        throw UTProfilerException("Database Error"+db.lastError().text());
    }
    QSqlQuery query(db);
    query.prepare("delete from UV where code=:code");
    query.bindValue(":code", code);
    if (!query.exec()) {
        throw UTProfilerException("SQL Query failed: " + query.executedQuery());
    }
}

map<QString, shared_ptr<UV>>& UVManager::getUvs()
{
    return uvs;
}

void UVManager::setUvs(const map<QString, shared_ptr<UV>> &value)
{
    uvs = value;
}
void UVManager::addItem(shared_ptr<UV> uv){
    uvs[uv->getCode()]=uv;
}

shared_ptr<UV> UVManager::ajouterUV(const QString& c, const QString& t, unsigned int nbc, shared_ptr<Categorie> cat, bool a, bool p){
    if (trouverUV(c)) {
        throw UTProfilerException(QString("erreur, UVManager, UV ")+c+QString("déja existante"));
    }else{
        auto newuv = make_shared<UV>(c,t,nbc,cat,a,p);
        uvs.insert(beginUvs(), pair<QString, shared_ptr<UV>>(c, newuv));
        modification=true;
        return newuv;
    }
}

shared_ptr<UV> UVManager::trouverUV(const QString& c) const {
    auto it = uvs.find(c);
    if (it == endUvs()) {
        return shared_ptr<UV>();
    } else {
        return it->second;
    }
}

shared_ptr<UV> UVManager::getUV(const QString& code) {
    auto uv = trouverUV(code);
    if (!uv) throw UTProfilerException("erreur, UVManager, UV inexistante");
    return uv;
}

bool UVManager::existeUV(const QString &code)
{
    auto uv = trouverUV(code);
    if (!uv) return false;
    return true;
}


shared_ptr<const UV> UVManager::getUV(const QString& code)const{
    return const_cast<UVManager*>(this)->getUV(code);
        // on peut aussi dupliquer le code de la méthode non-const
}

UVManager::Handler UVManager::handler=Handler();

UVManager& UVManager::getInstance(){
    if (!handler.instance) handler.instance = new UVManager; /* instance créée une seule fois lors de la première utilisation*/
    return *handler.instance;
}

void UVManager::libererInstance(){
    if (handler.instance) { delete handler.instance; handler.instance=0; }
}


UV::UV(const QString &c, const QString &t, bool automne, bool printemps) :
    ECTS(), code(c), titre(t), automne(automne), printemps(printemps)
{

}

UV::UV(const QString& c, const QString& t, unsigned int nbc, shared_ptr<Categorie> cat, bool automne, bool printemps) :
    ECTS(nbc, cat), code(c), titre(t), automne(automne), printemps(printemps)
{

}
