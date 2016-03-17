#ifndef ECTS_H
#define ECTS_H
#include <QString>
#include <vector>
#include <memory>
#include "categorie.h"
#include "credits.h"

using namespace std;

class ECTSException : public UTProfilerException {
public:
    ECTSException(const QString& m) : UTProfilerException(m) {}
};

class ECTS {
private:
    vector<shared_ptr<Credits>> credits;
public:
    ECTS(): credits() {}
    ECTS(unsigned int nbCredits, shared_ptr<const Categorie> categorie);
    ECTS(const ECTS& other) = delete;

    /*!
     * \brief Ajoute une catégorie de crédits données par cet ECTS
     * \param categorie : catégorie des crédits
     * \param nbCredits : nombre de crédits donnés
     */
    shared_ptr<Credits> addCredits(shared_ptr<const Categorie> categorie, unsigned int nbCredits);

    /*!
     * \brief const_iterator sur le premier Credits de l'ECTS
     */
    vector<shared_ptr<Credits>>::const_iterator beginCredits() const { return credits.cbegin(); }
    /*!
     * \brief const_iterator sur les Credits de l'ECTS pour la condition d'arrêt
     */
    vector<shared_ptr<Credits>>::const_iterator endCredits() const { return credits.cend(); }

    /*!
     * \brief Si l'ECTS ne possède qu'une catégorie de crédit retourne cette catégorie
     * sinon throw un ECTSException
     */
    shared_ptr<const Categorie> getCategorie() const;
    /*!
     * \brief Si l'ECTS ne possède qu'une catégorie de crédit retourne le nombre de crédits pour cette catégorie
     * sinon throw un ECTSException
     */
    unsigned int getNbCredits() const;
    /*!
     * \brief Si l'ECTS ne possède qu'une catégorie de crédit défini la catégorie des crédits
     * sinon throw un ECTSException
     */
    void setCategorie(shared_ptr<const Categorie> cat);
    /*!
     * \brief Si l'ECTS ne possède qu'une catégorie de crédit défini le nombre de crédits pour cette catégorie
     * sinon throw un ECTSException
     * \param nbc : Nombre de crédits
     */
    void setNbCredits(unsigned int nbc);

    virtual const QString& getCode() const = 0;

    virtual ~ECTS() {}
};

#endif // ECTS_H
