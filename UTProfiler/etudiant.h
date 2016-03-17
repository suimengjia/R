#ifndef ETUDIANT_H
#define ETUDIANT_H

#include <QObject>
#include <QString>
#include <vector>
#include <memory>
#include "solution.h"
#include "formation.h"

using namespace std;

class Etudiant
{
    QString login_;
    QString nom_;
    QString prenom_;
    vector<Solution> solutions;
public:
    Etudiant(const QString& login, const QString& nom, const QString& prenom);
    Etudiant(const Etudiant& etu) = delete;

    const QString& login() const { return login_; }
    const QString& nom() const { return nom_; }
    const QString& prenom() const { return prenom_; }

    void setLogin(const QString& login) { login_ = login; }
    void setNom(const QString& nom) { nom_ = nom; }
    void setPrenom(const QString& prenom) { prenom_ = prenom; }

    Solution& createSolution(const QDateTime& created = QDateTime::currentDateTime());

    vector<Solution>::iterator beginSolutions() { return solutions.begin(); }
    vector<Solution>::iterator endSolutions() { return solutions.end(); }
    vector<Solution>::const_iterator beginSolutions() const { return solutions.cbegin(); }
    vector<Solution>::const_iterator endSolutions() const { return solutions.cend(); }
};

class EtudiantManager {
    vector<shared_ptr<Etudiant>> etudiants;
    EtudiantManager();
    ~EtudiantManager();
    friend struct Handler;
    struct Handler{
        EtudiantManager* instance;
        Handler():instance(0){}
        ~Handler(){ if (instance) delete instance; instance=0; }
    };
    static Handler handler;

public:

    static EtudiantManager& getInstance();
    static void libererInstance();
    void ajouter(shared_ptr<Etudiant> etu);
    shared_ptr<Etudiant> creerEtudiant(const QString& nom, const QString& prenom, const QString& login);

    vector<shared_ptr<Etudiant>>::iterator beginEtudiants() { return etudiants.begin(); }
    vector<shared_ptr<Etudiant>>::iterator endEtudiants() { return etudiants.end(); }
};

#endif // ETUDIANT_H
