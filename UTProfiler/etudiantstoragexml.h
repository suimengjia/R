#ifndef ETUDIANTSTORAGEXML_H
#define ETUDIANTSTORAGEXML_H

#include <QIODevice>
#include <QXmlStreamWriter>
#include <memory>
#include "etudiantstorage.h"
#include "uv.h"
#include "equivalence.h"

using namespace std;

/*!
 *\class EtudiantStorageXML
 * \brief Sauvegarde et charge des profils étudiants depuis un fichier XML
 * (pas nécessairement un fichier d'ailleurs, n'importe quel QIODevice donc ça
 * peut être un socket, la réponse d'une requête HTTP...)
 */
class EtudiantStorageXML : public EtudiantStorage
{
    shared_ptr<QIODevice> file_;
    void writeECTS(QXmlStreamWriter &stream, shared_ptr<const ECTS> ects);
    void writeDemandesUVs(QXmlStreamWriter &stream, const QString& type, set<shared_ptr<UV>>::const_iterator start, set<shared_ptr<UV>>::const_iterator end);
    void loadSolution(QXmlStreamReader& xml, shared_ptr<Etudiant> etudiant);
    void loadDemandes(QXmlStreamReader& xml, Solution& sol);
    void loadInscriptions(QXmlStreamReader& xml, Solution& sol);
    void loadFormations(QXmlStreamReader& xml, Solution& sol);
    inline shared_ptr<UV> findUV(QStringRef code);
public:
    EtudiantStorageXML(shared_ptr<QIODevice> file) : EtudiantStorage(), file_(file) {}
    shared_ptr<QIODevice> file() { return file_; }
    void setFile(shared_ptr<QIODevice> file) { file_ = file; }

    bool save(const Etudiant& etudiant) override;
    shared_ptr<Etudiant> load() override;
};

#endif // ETUDIANTSTORAGEXML_H
