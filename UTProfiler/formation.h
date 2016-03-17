#ifndef FORMATION_H
#define FORMATION_H

#include <QTextStream>
#include <QString>
#include <list>
#include <memory>
#include "UTProfiler.h"
using namespace std;

class Formation{
protected:
    QString code;
    QString titre;
public:
    Formation(QString code, QString titre): code(code), titre(titre){}
    virtual QString getCode() const;
    virtual void setCode(const QString &value);
    virtual QString getTitre() const;
    virtual void setTitre(const QString &value);
    virtual void ajouteSousFormation(shared_ptr<Formation> sousF){sousF->getCode();/*rien*/}
};

class FormationF : public Formation{

public:
    FormationF(QString code, QString titre):Formation(code,titre){}
};


class FormationC : public Formation{
protected:
    list<shared_ptr<Formation>> sousFormation; //List des composants
public:
    FormationC(QString code, QString titre):Formation(code,titre){}
    list<shared_ptr<Formation>> getSousFormation() const;
    void setSousFormation(const list<shared_ptr<Formation>> &value);

    void ajouteSousFormation(shared_ptr<Formation> sousF){
        sousFormation.push_back(sousF);
    }
    void supSousFormation(shared_ptr<Formation> sousF){
        sousFormation.remove(sousF);
    }


};

/*!
 *\class FormationManager(singleton)
 * \brief classe pour gérer ensemble des formations.
 */

class FormationManager{
    list<shared_ptr<Formation>> formations;
    void addItem(shared_ptr<Formation> cat);
    bool modification;
    shared_ptr<Formation> trouverFormation(const QString& c);
    FormationManager(const FormationManager& um);
    FormationManager& operator=(const FormationManager& um);
    FormationManager();
    QString file;
    friend struct Handler;
    struct Handler{
        FormationManager* instance;
        Handler():instance(0){}
        ~Handler(){ if (instance) delete instance; instance=0; }
    };
    static Handler handler;

public:

    void load(const QString& f);
    void save(const QString& f, const QString &code);
    void remove(const QString &code);
    static FormationManager& getInstance();
    static void libererInstance();
    shared_ptr<Formation> initFormation(const QString& c, const QString& t, shared_ptr<FormationC> parent=NULL);
    /*!
     * \brief Crée une nouvelle formation
     * \param c : Code de la formation
     * \param t : Titre complet de la formation
     * \param parent : Formation parent (ex : relation branche/filière)
     */
    shared_ptr<Formation> ajouterFormation(const QString& c, const QString& t, shared_ptr<FormationC> parent=NULL); // renvoyer une ref vers la formation !
    const shared_ptr<Formation> getFormation(const QString& code) const;
    shared_ptr<Formation> getFormation(const QString& code);

    list<shared_ptr<Formation>>::iterator begin() { return formations.begin(); }
    list<shared_ptr<Formation>>::iterator end() { return formations.end(); }

    list<shared_ptr<Formation>>& getFormations();
    void setFormations(const list<shared_ptr<Formation>> &value);
};
inline QTextStream& operator>>(QTextStream& f, shared_ptr<Formation> form){
    QString str;
    f>>str;
    form = FormationManager::getInstance().getFormation(str);
    return f;
}
inline QTextStream& operator<<(QTextStream& f,const Formation& cat){
    return f<<cat.getCode();
}

#endif // FORMATION_H
