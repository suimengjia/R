#ifndef EQUIVALENCE_H
#define EQUIVALENCE_H

#include <QString>
#include "ects.h"

class Equivalence : public ECTS
{
    QString universite_;
public:
    /*!
     * \brief Equivalence
     * \param universite : Nom de l'université dans laquelle l'étudiant a validé les crédits
     */
    Equivalence(const QString& universite) : universite_(universite) {}
    const QString& universite() const { return universite_; }
    const QString& getCode() const override { return universite_; }
};

#endif // EQUIVALENCE_H
