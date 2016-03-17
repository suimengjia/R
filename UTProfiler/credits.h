#ifndef CREDITS_H
#define CREDITS_H
#include "categorie.h"
#include <utility>
#include <memory>

using namespace std;

class Credits
{
    shared_ptr<const Categorie> categorie_;
    unsigned int credits_;
public:
    /*!
     * \brief Credits représente le nombre de crédits que donne une ECTS dans une catégorie particulière
     * \param nbCredits Nombre de crédits dans cette catégorie
     * \param categorie Catégorie des crédits
     */
    Credits(shared_ptr<const Categorie> categorie, unsigned int nbCredits) : categorie_(categorie), credits_(nbCredits) {}
    shared_ptr<const Categorie> categorie() const { return categorie_; }
    unsigned int nbCredits() const { return credits_; }
    void setCategorie(shared_ptr<const Categorie> cat) { categorie_ = cat; }
    void setNbCredits(unsigned int nbc) { credits_ = nbc; }

};

#endif // CREDITS_H
