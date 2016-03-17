#include "categorie.h"
#include <QMessageBox>
#include <QtSql>
QString Categorie::getCode() const
{
    return code;
}


void Categorie::setCode(const QString &value)
{
    code = value;
}

QString Categorie::getTitre() const
{
    return titre;
}

void Categorie::setTitre(const QString &value)
{
    titre = value;
}

list<shared_ptr<Categorie>> CategorieC::getSousCates() const
{
    return sousCates;
}

void CategorieC::setSousCates(const list<shared_ptr<Categorie>>& value)
{
    sousCates = value;
}

void CategorieC::ajouteSousCate(shared_ptr<Categorie> sousCate)
{
    sousCates.push_back(sousCate);
}

void CategorieC::supSousCates(shared_ptr<Categorie> sousCate)
{
    sousCates.remove(sousCate);
}



categorieManager::categorieManager():categories(0),modification(false),file(""){
}
QString getLastExecutedQuery(const QSqlQuery& query)
{
    QString sql = query.executedQuery();
    const int nbBindValues = query.boundValues().size();

    for(int i = 0, j = 0; j < nbBindValues; ++j)
    {
        i = sql.indexOf(QLatin1Char('?'), i);
        if (i <= 0)
        {
            break;
        }
        const QVariant &var = query.boundValue(j);
        QSqlField field(QLatin1String(""), var.type());
        if (var.isNull())
        {
            field.clear();
        }
        else
        {
            field.setValue(var);
        }
        QString formatV = query.driver()->formatValue(field);
        sql.replace(i, 1, formatV);
        i += formatV.length();
    }

    return sql;
}


void categorieManager::load(const QString &f){
    QSqlDatabase db=QSqlDatabase::database("BDbase");
    if(!db.open()){
        throw UTProfilerException("Database Error"+db.lastError().text());
    }
    QSqlQuery query(db);
    //Inilisation à faire une fois pour creer la BD
    /*if (!query.exec("create table categorie(code varchar,titre varchar,parent varchar)"))
        throw UTProfilerException("SQL Query failed: " + query.executedQuery());
    for(auto it=categories.begin(); it!=categories.end();it++){
        query.prepare("insert into categorie values(:code,:titre,:parent)");
        query.bindValue(":code",(*it)->getCode());
        query.bindValue(":titre",(*it)->getTitre());
        query.bindValue(":parent","");
        if (!query.exec()) {
            throw UTProfilerException("SQL Query failed: " + query.executedQuery());
        }
    }*/
    if (!query.exec("select * from categorie")) {
        throw UTProfilerException("SQL Query failed: " + query.executedQuery());
    }
    map<shared_ptr<Categorie>,QString> mapParentTemp;
    while(query.next()){
        ajouterCategorie(query.value(0).toString(),query.value(1).toString());
    }
    //gestion pour pattern composite
    for(auto it=mapParentTemp.begin();it!=mapParentTemp.end();it++){
        if ((*it).second!=""){
            auto it2=categories.begin();
            while( it2!=categories.end() and (*it2)->getCode()!=(*it).second){
                it2++;
            }
            if (it2!=categories.end()){
                (*it2)=make_shared<CategorieC>((*it).second,this->getCategorie((*it).second)->getTitre());
                (*it2)->ajouteSousCate((*it).first);
            }
        }
    }
    db.close();
}

void categorieManager::save(const QString &f, QString code){
    shared_ptr<Categorie> cat = categorieManager::getInstance().getCategorie(code);

    QSqlDatabase db=QSqlDatabase::database("BDbase");
    if(!db.open()){
        throw UTProfilerException("Database Error"+db.lastError().text());
    }
    QSqlQuery query(db);



    query.prepare("select * from categorie where code=:code");
    query.bindValue(":code", cat->getCode());
    if (!query.exec()) {
        throw UTProfilerException("SQL Query failed: " + query.executedQuery());
    }
    if(query.next()) {
        //QMessageBox::critical(0,"asd", query.value(0).toString());
        query.prepare("UPDATE categorie "
                      "SET titre=:titre, parent=:parent  "
                      "WHERE code=:code");

    }else{
        query.prepare("insert into categorie values(:code,:titre,:parent)");
    }
    query.bindValue(":code",cat->getCode());
    query.bindValue(":titre",cat->getTitre());
    query.bindValue(":parent","");
    if (!query.exec()) {
        throw UTProfilerException("Query failed: " + getLastExecutedQuery(query));
    }//QMessageBox::critical(0,"asd",query.executedQuery());

    db.close();

}

void categorieManager::remove(const QString &code)
{
    auto it= categories.begin();
    while (it!= categories.end() and (*it)->getCode()!=code )
        it++;
    if(it!= categories.end())
        categories.erase(it);
    QSqlDatabase db=QSqlDatabase::database("BDbase");
    if(!db.open()){
        throw UTProfilerException("Database Error"+db.lastError().text());
    }
    QSqlQuery query(db);
    query.prepare("delete from categorie where code=:code");
    query.bindValue(":code", code);
    if (!query.exec()) {
        throw UTProfilerException("SQL Query failed: " + query.executedQuery());
    }
}



vector<shared_ptr<Categorie>>& categorieManager::getCategories()
{
    return categories;
}

void categorieManager::setCategories(const vector<shared_ptr<Categorie>> &value)
{
    categories = value;
}
void categorieManager::addItem(shared_ptr<Categorie> categorie){
    categories.push_back(categorie);
}



categorieManager::Handler categorieManager::handler=Handler();

categorieManager& categorieManager::getInstance(){
    if (!handler.instance) handler.instance = new categorieManager; /* instance créée une seule fois lors de la première utilisation*/
    return *handler.instance;
}

void categorieManager::libererInstance(){
    if (handler.instance) { delete handler.instance; handler.instance=0; }
}

shared_ptr<Categorie> categorieManager::initCategorie(const QString& c, const QString& t, shared_ptr<CategorieC> parent){
    auto newCategorie= make_shared<CategorieF>(c,t);
    if(parent!=0){
        parent->ajouteSousCate(newCategorie);
    }
    addItem(newCategorie);
    modification=true;
    return newCategorie;
}

shared_ptr<Categorie> categorieManager::ajouterCategorie(const QString& c, const QString& t, shared_ptr<CategorieC> parent){
    if (trouverCategorie(c)) {
        trouverCategorie(c)->setTitre(t);
        return trouverCategorie(c);
    }else{
        return initCategorie(c,t,parent);
    }
}


shared_ptr<Categorie> categorieManager::trouverCategorie(const QString& c){
    for(auto it=categories.begin(); it!=categories.end(); it++){
        if ((*it)->getCode()==c ) return *it;
    }
    return NULL;
}


shared_ptr<Categorie> categorieManager::getCategorie(const QString& code){
    auto categorie=trouverCategorie(code);
    if (!categorie) throw UTProfilerException("erreur, categorieManager, Categorie inexistante");
    return categorie;
}

bool categorieManager::existeCat(const QString &c)
{
    if (trouverCategorie(c)) return 1;
    else return 0;
}


shared_ptr<const Categorie> categorieManager::getCategorie(const QString& code)const{
    return const_cast<categorieManager*>(this)->getCategorie(code);
        // on peut aussi dupliquer le code de la méthode non-const
}
