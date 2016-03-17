#include "inscription.h"



Inscription::Inscription(shared_ptr<const ECTS> ects, const Semestre& semestre, Note res) :
    ects_(ects), semestre_(semestre), resultat_(res)
{

}
