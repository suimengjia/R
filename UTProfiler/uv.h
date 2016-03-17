#ifndef UV_H
#define UV_H

#include <QString>
#include "ects.h"
#include "categorie.h"
#include <QtSql>
#include <memory>
#include <map>
#include <utility>

using namespace std;

/*!
 * \brief Représente une UV avec les propriétés suivantes:
 * un code ("LO21")
 * un titre ("Programmation et conception orientées objet")
 * des semestres d'ouverture (automne et/ou printemps)
 */
class UV : public ECTS {
private:
    QString code;
    QString titre;
    bool automne;
    bool printemps;
    UV& operator=(const UV& u);
    friend class UVManager;
public:
    /*!
     * \brief UV sans crédits associés
     * \param c : Code de l'UV
     * \param t : Intitulé de l'UV
     * \param automne : Est-ce que l'UV est ouverte à l'automne ?
     * \param printemps : Est-ce que l'UV est ouverture au printemps ?
     */
    UV(const QString& c, const QString& t, bool automne = false, bool printemps = false);

    /*!
     * \brief UV avec une catégorie de crédits associée
     * \param c : Code de l'UV
     * \param t : Intitulé de l'UV
     * \param nbc : Nombre de crédits
     * \param cat : Catégorie pour les crédits
     * \param automne : Est-ce que l'UV est ouverte à l'automne ?
     * \param printemps : Est-ce que l'UV est ouverture au printemps ?
     */
    UV(const QString& c, const QString& t, unsigned int nbc, shared_ptr<Categorie> cat, bool automne = false, bool printemps = false);
    UV(const UV& u) = delete;
    bool operator ==(const UV& other) const { return getCode() == other.getCode(); }
    const QString& getCode() const override { return code; }
    const QString& getTitre() const { return titre; }
    bool ouvertureAutomne() const { return automne; }
    bool ouverturePrintemps() const { return printemps; }
    void setCode(const QString& c) { code = c; }
    void setTitre(const QString& t) { titre = t; }
    void setOuvertureAutomne(bool b) { automne=b; }
    void setOuverturePrintemps(bool b) { printemps=b; }
};


class UVManager {
private:
    map<QString, shared_ptr<UV>> uvs;
    void addItem(shared_ptr<UV> uv);
    bool modification;
    shared_ptr<UV> trouverUV(const QString& c) const;
    UVManager(const UVManager& um);
    UVManager& operator=(const UVManager& um);
    UVManager();
    QString file;
    friend struct Handler;
    struct Handler{
        UVManager* instance;
        Handler():instance(0){}
        ~Handler(){ if (instance) delete instance; instance=0; }
    };
    static Handler handler;

public:

    /*!
     * \brief Charge les UVs depuis la base SQlite
     * Il faut charger les formations grâce à categorieManager::getInstance().load(); avant d'utiliser cette fonction
     * \param f : nom de la base de données
     */
    void load(const QString& f);
    /*!
     * \brief Enregistre une UV dans la base SQlite
     * \param f : nom de la base de données
     * \param code : code de l'UV à enregistrer
     */
    void save(const QString& f, QString code);
    void remove(const QString& code);
    static UVManager& getInstance();
    static void libererInstance();
    /*!
     * \brief ajouterUV
     * \param c : Code de l'UV
     * \param t : titre de l'UV
     * \param nbc : nombre de crédits (à modifier !)
     * \param cat : Catégorie de l'UV
     * \param a : ouverte à l'automne ?
     * \param p : ouverte au printemps ?
     * \return shared_ptr<UV> : UV créée
     */
    shared_ptr<UV> ajouterUV(const QString& c, const QString& t, unsigned int nbc, shared_ptr<Categorie> cat, bool a, bool p);
    shared_ptr<const UV> getUV(const QString& code) const;
    shared_ptr<UV> getUV(const QString& code);
    bool existeUV(const QString& code);
    map<QString, shared_ptr<UV>>& getUvs();
    map<QString, shared_ptr<UV>>::const_iterator beginUvs() const { return uvs.cbegin(); }
    map<QString, shared_ptr<UV>>::const_iterator endUvs() const { return uvs.cend(); }
    void setUvs(const map<QString, shared_ptr<UV>> &value);
};

#endif // UV_H
