#ifndef INSCRIPTION_H
#define INSCRIPTION_H

#include <memory>
#include "semestre.h"
#include "ects.h"

using namespace std;

class Inscription
{
    shared_ptr<const ECTS> ects_;
    Semestre semestre_;
    Note resultat_;
public:
    Inscription(shared_ptr<const ECTS> ects, const Semestre& semestre, Note res = Note::EC);
    shared_ptr<const ECTS> ects() const { return ects_; }
    const Semestre& semestre() const { return semestre_; }
    Semestre& semestre() { return semestre_; }
    Note resultat() const { return resultat_; }
    void setEcts(shared_ptr<const ECTS> ects) { ects_ = ects; }
    void setSemestre(const Semestre& semestre) { semestre_ = semestre; }
    void setResultat(Note res) { resultat_ = res; }

};

#endif // INSCRIPTION_H
