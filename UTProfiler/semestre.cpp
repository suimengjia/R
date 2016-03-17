#include "semestre.h"

#include <QTextStream>

QString Semestre::code() const {
    QString res;
    QTextStream stream(&res);

    stream << this->saison();

    stream.setPadChar('0');
    stream.setFieldWidth(2);
    stream.setFieldAlignment(QTextStream::AlignRight);
    stream << this->annee() % 100;

    return res;
}


const Semestre Semestre::parseCode(const QString&  code)
{
    if (code.length() != 3) {
        throw new SemestreException("ParseError : Le code d'UV doit faire 3 caractères");
    }
    // On détermine la saison
    Saison s = Saison::Automne;
    if (code[0] == 'A' || code[0] == 'a') {
        s = Saison::Automne;
    } else if (code[0] == 'P' || code[0] == 'p') {
        s = Saison::Printemps;
    } else {
        throw new SemestreException("ParseError : Le premier caractère doit être A ou P");
    }
    // on détermine l'année
    bool ok;
    unsigned int annee = code.right(2).toUInt(&ok, 10);
    if (!ok) {
        throw new SemestreException("ParseError : Impossible de parser l'annéee");
    }

    return Semestre(s, 2000 + annee);
}
