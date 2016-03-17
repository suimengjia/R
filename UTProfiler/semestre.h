#ifndef SEMESTRE_H
#define SEMESTRE_H

#include <QString>
#include <utility>

#include "UTProfiler.h"

using namespace std;

class SemestreException : public UTProfilerException {
public:
    SemestreException(const QString& m) : UTProfilerException(m) {}
};

/*! \class Semestre
 * \brief représente un semestre UTC
 */
class Semestre : pair<Saison, unsigned int>
{
public:
    explicit Semestre(Saison saison, unsigned int annee) : pair(saison, annee) {}
    Semestre(const QString& code) : pair(Semestre::parseCode(code)) {}
    Saison saison() const {
        return first;
    }
    unsigned int annee() const {
        return second;
    }
    /*!
     * \brief génère le code du semestre (A14, P06...)
     * \return QString contenant le code à 3 chiffres du semestre
     */
    QString code() const;
    static const Semestre parseCode(const QString& code);

};

inline QTextStream& operator<<(QTextStream& f, const Semestre& s) { return f << s.code(); }

#endif // SEMESTRE_H
