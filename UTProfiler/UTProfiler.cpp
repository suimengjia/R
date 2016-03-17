#include "UTProfiler.h"

QString noteToString(const Note& n) {
    switch (n) {
    case Note::A:
        return "A";
    case Note::B:
        return "B";
    case Note::C:
        return "C";
    case Note::D:
        return "D";
    case Note::E:
        return "E";
    case Note::F:
        return "F";
    case Note::FX:
        return "FX";
    case Note::RES:
        return "RES";
    case Note::ABS:
        return "ABS";
    case Note::EC:
        return "EC";
    default:
        return "";
    }
}

Note stringToNote(const QString& n) {
    if (n == "A") {
        return Note::A;
    } else if (n == "B") {
        return Note::B;
    } else if (n == "C") {
        return Note::C;
    } else if (n == "D") {
        return Note::D;
    } else if (n == "E") {
        return Note::E;
    } else if (n == "F") {
        return Note::F;
    } else if (n == "FX") {
        return Note::FX;
    } else if (n == "RES") {
        return Note::RES;
    } else if (n == "ABS") {
        return Note::ABS;
    } else if (n == "EC") {
        return Note::EC;
    } else {
        throw new UTProfilerException("Note invalide");
    }
}
