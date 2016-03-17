#include "etudiant.h"
#include <utility>

Etudiant::Etudiant(const QString &login, const QString &nom, const QString &prenom) :
    login_(login), nom_(nom), prenom_(prenom), solutions(0)
{

}

Solution& Etudiant::createSolution(const QDateTime& created)
{
    solutions.emplace_back(Solution(created));
    return solutions.back();
}

EtudiantManager::EtudiantManager() {}

EtudiantManager::~EtudiantManager() {

}

EtudiantManager::Handler EtudiantManager::handler=Handler();

EtudiantManager& EtudiantManager::getInstance(){
    if (!handler.instance) handler.instance = new EtudiantManager; /* instance créée une seule fois lors de la première utilisation*/
    return *handler.instance;
}

void EtudiantManager::libererInstance(){
    if (handler.instance) { delete handler.instance; handler.instance=0; }
}

void EtudiantManager::ajouter(shared_ptr<Etudiant> etu) {
    etudiants.push_back(etu);
}

shared_ptr<Etudiant> EtudiantManager::creerEtudiant(const QString &login, const QString &nom, const QString &prenom) {
    shared_ptr<Etudiant> etu = make_shared<Etudiant>(login, nom, prenom);
    etudiants.push_back(etu);
    return etu;
}

