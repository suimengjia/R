#ifndef CATEGORIE_H
#define CATEGORIE_H

#include <QTextStream>
#include "UTProfiler.h"
#include <list>
#include <memory>

using namespace std;
/*!
 *\class Categorie(abstraite)
 * \brief classe composant de categorie
 */
class Categorie{
protected:
    QString code;
    QString titre;
public:
    Categorie(QString code = QString(), QString titre = QString()): code(code), titre(titre){}
    virtual QString getCode() const;
    virtual void setCode(const QString &value);
    virtual QString getTitre() const;
    virtual void ajouteSousCate(shared_ptr<Categorie> sousCate) {sousCate->getCode();/*rien*/}
    virtual void setTitre(const QString &value);
    virtual ~Categorie(){}
    inline bool operator==(const Categorie& rhs){ return this->getCode()==rhs.getCode(); }
    inline bool operator!=(const Categorie& rhs){ return !(this->operator==(rhs));}

};

/*!
 * \brief Représente une Feuille de l'arbre des catégories
 */
class CategorieF : public Categorie{
public:
    CategorieF(QString code, QString titre):Categorie(code,titre){}
};



/*!
 * \brief Représente un noeud de l'arbre des catégories
 * Si un uv appartient à une sous-catégorie elle appartient aussi
 * à la catégorie parent, et ainsi de suite jusqu'à la racine
 */
class CategorieC : public Categorie{
protected:
    list<shared_ptr<Categorie>> sousCates; //Liste des composants
public:
    CategorieC(QString code, QString titre):Categorie(code,titre){}

    list<shared_ptr<Categorie>> getSousCates() const;
    void setSousCates(const list<shared_ptr<Categorie>> &value);
    void ajouteSousCate(shared_ptr<Categorie> sousCate);
    void supSousCates(shared_ptr<Categorie> sousCate);
};

/*!
 *\class categorieManager(singleton)
 * \brief classe pour gérer ensemble des categorie.
 */
class categorieManager{
    vector<shared_ptr<Categorie>> categories;
    void addItem(shared_ptr<Categorie> cat);
    bool modification;
    shared_ptr<Categorie> trouverCategorie(const QString& c);
    categorieManager(const categorieManager& um);
    categorieManager& operator=(const categorieManager& um);
    categorieManager();
    QString file;
    friend struct Handler;
    struct Handler{
        categorieManager* instance;
        Handler():instance(0){}
        ~Handler(){ if (instance) delete instance; instance=0; }
    };
    static Handler handler;

public:

    void load(const QString& f);
    void save(const QString& f, QString code);
    void remove(const QString &code);
    static categorieManager& getInstance();
    static void libererInstance();
    shared_ptr<Categorie> initCategorie(const QString& c, const QString& t, shared_ptr<CategorieC> parent = shared_ptr<CategorieC>());
    shared_ptr<Categorie> ajouterCategorie(const QString& c, const QString& t, shared_ptr<CategorieC> parent = shared_ptr<CategorieC>());
    shared_ptr<const Categorie> getCategorie(const QString& code) const;
    shared_ptr<Categorie> getCategorie(const QString& code);
    bool existeCat(const QString& c);

    vector<shared_ptr<Categorie>>::iterator begin() { return categories.begin(); }
    vector<shared_ptr<Categorie>>::iterator end() { return categories.end(); }

    vector<shared_ptr<Categorie>>& getCategories();
    void setCategories(const vector<shared_ptr<Categorie>> &value);
};
inline QTextStream& operator>>(QTextStream& f, shared_ptr<const Categorie> cat){
    QString str;
    f>>str;
    cat = categorieManager::getInstance().getCategorie(str);
    return f;
}
inline QTextStream& operator<<(QTextStream& f,const Categorie& cat){
    return f<<cat.getCode();
}

#endif // CATEGORIE_H
