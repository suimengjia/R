#include "semestresmanager.h"

SemestresManager* SemestresManager::instance = nullptr;

SemestresManager::SemestresManager() : semestres()
{
}

SemestresManager& SemestresManager::getInstance() {
    if (SemestresManager::instance == nullptr) {
        SemestresManager::instance = new SemestresManager();
    }
    return *(SemestresManager::instance);
}

const Semestre& SemestresManager::get(Saison saison, unsigned int annee) {
    std::map<unsigned int, Semestre>& sems = semestres[saison];
    std::map<unsigned int, Semestre>::iterator sem = sems.find(annee);
    if (sem == sems.end()) {
        // On crée le semestre
        sem = sems.insert(sem, std::make_pair(annee, Semestre(saison, annee)));
    }
    return (*sem).second;
}
