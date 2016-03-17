#ifndef SOLUTION_H
#define SOLUTION_H

#include <list>
#include <set>
#include <memory>
#include <QObject>
#include <QDateTime>
#include "ects.h"
#include "inscription.h"
#include "uv.h"
#include "formation.h"

using namespace std;

class Solution
{
    friend class Etudiant;
    Solution(const Solution& other);

    const QDateTime createdAt_;
    set<shared_ptr<UV>> exigences; // UV devrait être const
    set<shared_ptr<UV>> preferences;// ici aussi
    set<shared_ptr<UV>> rejets;// ici aussi
    list<shared_ptr<Inscription>> inscriptions;
    set<shared_ptr<Formation>> formations; // ici aussi

public:
    /* \brief Constructeur de Solution
     * Ne peut être appelé que depuis la classe Etudiant TODO
     */
    Solution(const QDateTime& createdAt = QDateTime::currentDateTime());
    Solution(Solution&& other);
    const QDateTime& createdAt() const { return createdAt_; }
    /*!
     * \brief Ajoute une UV que l'étudiant refuse de faire
     * \param uv : UV refusée
     * \return ajout réussi ?
     */
    bool addRejet(shared_ptr<UV> uv);
    /*!
     * \brief Ajoute une UV que l'étudiant souhaiterai faire
     * \param uv : UV souhaitée
     * \return ajout réussi ?
     */
    bool addPreference(shared_ptr<UV> uv);
    /*!
     * \brief Ajoute une UV exigée par l'étudiant
     * \param uv : UV demandée
     * \return ajout réussi ?
     */
    bool addExigence(shared_ptr<UV> uv);
    /*!
     * \brief Ajoute une inscription à un ECTS
     * \param inscription :
     * \return ajout réussi ?
     */
    bool addInscription(const shared_ptr<Inscription> inscription);
    set<shared_ptr<UV>>::const_iterator beginPreferences() const { return preferences.cbegin(); }
    set<shared_ptr<UV>>::const_iterator beginExigences() const { return exigences.cbegin(); }
    set<shared_ptr<UV>>::const_iterator beginRejets() const { return rejets.cbegin(); }
    set<shared_ptr<UV>>::const_iterator endPreferences() const { return preferences.cend(); }
    set<shared_ptr<UV>>::const_iterator endExigences() const { return exigences.cend(); }
    set<shared_ptr<UV>>::const_iterator endRejets() const { return rejets.cend(); }

    list<shared_ptr<Inscription>>::const_iterator beginInscriptions() const { return inscriptions.cbegin(); }
    list<shared_ptr<Inscription>>::const_iterator endInscriptions() const { return inscriptions.cend(); }
    unsigned int nbInscriptions() const { return inscriptions.size(); }

    /*!
     * \brief Ajoute une formation suivie par l'etudiant
     * \param formation Formation à ajouter à cette solution
     * \return ajout réussi ?
     */
    bool addFormation(shared_ptr<Formation> formation);
    /*!
     * \brief Enlève une formation des formations suivies par l'utilisateur
     * \param formation Formation à supprimer
     */
    void removeFormation(shared_ptr<Formation> formation);
    set<shared_ptr<Formation>>::const_iterator beginFormations() const { return formations.cbegin(); }
    set<shared_ptr<Formation>>::const_iterator endFormations() const { return formations.cend(); }
};

#endif // SOLUTION_H
