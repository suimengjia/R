#ifndef ETUDIANTSTORAGE_H
#define ETUDIANTSTORAGE_H

#include <QString>
#include <memory>
#include "etudiant.h"

using namespace std;

class EtudiantStorageException : public UTProfilerException {
public:
    EtudiantStorageException(const QString& m) : UTProfilerException(m) {}
};

/*! \class EtudiantStorage
 * \brief Classe abstraite delaquelle doivent dériver toutes classes permettant de sauvegarder
 * un profil étudiant
 *
 * Les classes filles doivent définir *où* le profil sera enregistré et depuis où il sera chargé
 * (fichier, base de données, service web...)
 */
class EtudiantStorage
{
public:
    EtudiantStorage();

    /*!
     * \brief Sauvegarde un profil étudiant et les solutions liées
     * \return true en cas de réussite, false sinon
     */
    virtual bool save(const Etudiant& etudiant) = 0;

    /*!
     * \brief Charge un profil étudiant
     * \return shared_ptr vers une instance d'Etudiant complètement initialisée
     */
    virtual shared_ptr<Etudiant> load() = 0;

};

#endif // ETUDIANTSTORAGE_H
