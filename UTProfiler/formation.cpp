#include "formation.h"
#include <QtSql>


QString Formation::getCode() const
{
    return code;
}

void Formation::setCode(const QString &value)
{
    code = value;
}

QString Formation::getTitre() const
{
    return titre;
}

void Formation::setTitre(const QString &value)
{
    titre = value;
}



list<shared_ptr<Formation>> FormationC::getSousFormation() const
{
    return sousFormation;
}

void FormationC::setSousFormation(const list<shared_ptr<Formation>> &value)
{
    sousFormation = value;
}



void FormationManager::load(const QString &f)
{
    QSqlDatabase db=QSqlDatabase::database("BDbase");
    if(!db.open()){
        throw UTProfilerException("Database Error"+db.lastError().text());
    }
    QSqlQuery query(db);
    //Inilisation à faire une fois pour creer la BD
    /*
    if (!query.exec("create table formation(code varchar,titre varchar,parent varchar)"))
        throw UTProfilerException("SQL Query failed: " + query.executedQuery());
    for(auto it=formations.begin(); it!=formations.end();it++){
        query.prepare("insert into formation values(:code,:titre,:parent)");
        query.bindValue(":code",(*it)->getCode());
        query.bindValue(":titre",(*it)->getTitre());
        query.bindValue(":parent","");
        if (!query.exec()) {
            throw UTProfilerException("SQL Query failed: " + query.executedQuery());
        }
    }*/
    if (!query.exec("select * from formation")) {
        throw UTProfilerException("SQL Query failed: " + query.executedQuery());
    }
    map<shared_ptr<Formation>,QString> mapParentTemp;
    while(query.next()){
        ajouterFormation(query.value(0).toString(),query.value(1).toString());
    }
    //gestion pour pattern composite
    for(auto it=mapParentTemp.begin();it!=mapParentTemp.end();it++){
        if ((*it).second!=""){
            auto it2=formations.begin();
            while( it2!=formations.end() and (*it2)->getCode()!=(*it).second){
                it2++;
            }
            if (it2!=formations.end()){
                (*it2)=make_shared<FormationC>((*it).second,this->getFormation((*it).second)->getTitre());
                (*it2)->ajouteSousFormation((*it).first);
            }
        }
    }
    db.close();
}

void FormationManager::save(const QString &f,const QString &code )
{
    shared_ptr<Formation> form = FormationManager::getInstance().getFormation(code);

    QSqlDatabase db=QSqlDatabase::database("BDbase");
    if(!db.open()){
        throw UTProfilerException("Database Error"+db.lastError().text());
    }
    QSqlQuery query(db);



    query.prepare("select * from Formation where code=:code");
    query.bindValue(":code", form->getCode());
    if (!query.exec()) {
        throw UTProfilerException("SQL Query failed: " + query.executedQuery());
    }
    if(query.next()) {
        //QMessageBox::critical(0,"asd", query.value(0).toString());
        query.prepare("UPDATE Formation "
                      "SET titre=:titre, parent=:parent "
                      "WHERE code=:code ");

    }else{
        query.prepare("insert into Formation values(:code,:titre,:parent)");
    }
    query.bindValue(":code",form->getCode());
    query.bindValue(":titre",form->getTitre());
    query.bindValue(":parent","");
    if (!query.exec()) {
        throw UTProfilerException("Query failed: " + query.executedQuery());
    }//QMessageBox::critical(0,"asd",query.executedQuery());

    db.close();
}

void FormationManager::remove(const QString &code)
{
    auto it= formations.begin();
    while (it!= formations.end() and (*it)->getCode()!=code )
        it++;
    if(it!= formations.end())
        formations.erase(it);
    QSqlDatabase db=QSqlDatabase::database("BDbase");
    if(!db.open()){
        throw UTProfilerException("Database Error"+db.lastError().text());
    }
    QSqlQuery query(db);
    query.prepare("delete from formation where code=:code");
    query.bindValue(":code", code);
    if (!query.exec()) {
        throw UTProfilerException("SQL Query failed: " + query.executedQuery());
    }
}
FormationManager::FormationManager():formations(0),modification(false),file(""){
}





list<shared_ptr<Formation> > &FormationManager::getFormations()
{
    return formations;
}

void FormationManager::setFormations(const list<shared_ptr<Formation>> &value)
{
    formations = value;
}
void FormationManager::addItem(shared_ptr<Formation> formation){
    formations.push_back(formation);
}



FormationManager::Handler FormationManager::handler=Handler();

FormationManager& FormationManager::getInstance(){
    if (!handler.instance) handler.instance = new FormationManager; /* instance créée une seule fois lors de la première utilisation*/
    return *handler.instance;
}

void FormationManager::libererInstance(){
    if (handler.instance) { delete handler.instance; handler.instance=0; }
}

shared_ptr<Formation> FormationManager::initFormation(const QString& c, const QString& t, shared_ptr<FormationC> parent){
    shared_ptr<Formation> newFormation=make_shared <FormationF>(c,t);
    if(parent!=0){
        parent->ajouteSousFormation(newFormation);
    }
    addItem(newFormation);
    modification=true;
    return newFormation;
}

shared_ptr<Formation> FormationManager::ajouterFormation(const QString& c, const QString& t, shared_ptr<FormationC> parent){
    if (trouverFormation(c)) {
        trouverFormation(c)->setTitre(t);
        return trouverFormation(c);
    }else{
        return initFormation(c,t,parent);
    }
}


shared_ptr<Formation> FormationManager::trouverFormation(const QString& c){
    for(auto it=formations.begin(); it!=formations.end(); it++){
        if ((*it)->getCode()==c ) return *it;
    }
    /********************************************/
    return NULL;
}

shared_ptr<Formation> FormationManager::getFormation(const QString& code){
    shared_ptr<Formation> formation=trouverFormation(code);
    if (!formation) throw UTProfilerException("erreur, FormationManager, Formation inexistante");
    return formation;
}


const shared_ptr<Formation> FormationManager::getFormation(const QString& code)const{
    return const_cast<FormationManager*>(this)->getFormation(code);
        // on peut aussi dupliquer le code de la méthode non-const
}
