#ifndef SEMESTRESMANAGER_H
#define SEMESTRESMANAGER_H

#include "semestre.h"
#include <vector>
#include <map>
#include <utility>

using namespace std;

class SemestresManager
{
    map<Saison, map<unsigned int, Semestre>> semestres;
    SemestresManager();
    static SemestresManager* instance;
public:
    static SemestresManager& getInstance();
    const Semestre& get(Saison saison, unsigned int annee);
};

#endif // SEMESTRESMANAGER_H
