#include "ects.h"

ECTS::ECTS(unsigned int nbCredits, shared_ptr<const Categorie> categorie) :
    credits()
{
    addCredits(categorie, nbCredits);
}

shared_ptr<Credits> ECTS::addCredits(shared_ptr<const Categorie> categorie, unsigned int nbCredits)
{
    auto credit = make_shared<Credits>(categorie, nbCredits);
    credits.push_back(credit);
    return credit;
}

shared_ptr<const Categorie> ECTS::getCategorie() const
{
    if (credits.size() != 1) {
        throw new ECTSException("Appel de getCategorie alors qu'il n'y a pas qu'une catégorie");
    }
    return credits.front()->categorie();
}

unsigned int ECTS::getNbCredits() const
{
    if (credits.size() != 1) {
        throw new ECTSException("Appel de getNbCredits alors qu'il n'y a pas qu'une catégorie");
    }
    return credits.front()->nbCredits();
}

void ECTS::setCategorie(shared_ptr<const Categorie> cat)
{
    if (credits.size() != 1) {
        throw new ECTSException("Appel de setCategorie alors qu'il n'y a pas qu'une catégorie");
    }
    credits.front()->setCategorie(cat);
}

void ECTS::setNbCredits(unsigned int nbc)
{
    if (credits.size() != 1) {
        throw new ECTSException("Appel de setNbCredits alors qu'il n'y a pas qu'une catégorie");
    }
    credits.front()->setNbCredits(nbc);
}
