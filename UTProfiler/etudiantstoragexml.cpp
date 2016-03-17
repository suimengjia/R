#include "etudiantstoragexml.h"
#include "UTProfiler.h"

#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QTextStream>
#include <QDebug>

/*! \brief Enregistre le profil étudiant dans un fichier au format XML
 */
bool EtudiantStorageXML::save(const Etudiant &etudiant)
{
    if (!file_->open(QIODevice::WriteOnly | QIODevice::Text)) {
        throw new EtudiantStorageException("Erreur ouverture fichier profil étudiant pour sauvegarde");
    }

    QXmlStreamWriter stream(file_.get());
    stream.setAutoFormatting(true);
    stream.writeStartDocument();
    stream.writeStartElement("etudiant");
    stream.writeTextElement("nom", etudiant.nom());
    stream.writeTextElement("prenom", etudiant.prenom());
    stream.writeTextElement("login", etudiant.login());
    auto solutionEnd = etudiant.endSolutions();
    stream.writeStartElement("solutions");
    for (auto solution = etudiant.beginSolutions(); solution != solutionEnd; solution++) {
        stream.writeStartElement("solution");
        // on enregistre la date de création
        stream.writeAttribute("created_at", solution->createdAt().toString(Qt::ISODate));

        // on sauvegarde les demandes particulières de l'étudiant
        // concernant les uvs qu'il veut faire
        stream.writeStartElement("demandes");
        this->writeDemandesUVs(stream, "rejet", solution->beginRejets(), solution->endRejets());
        this->writeDemandesUVs(stream, "preference", solution->beginPreferences(), solution->endPreferences());
        this->writeDemandesUVs(stream, "exigence", solution->beginExigences(), solution->endExigences());
        stream.writeEndElement(); // demandes

        stream.writeStartElement("inscriptions");
        auto inscriptionsEnd = solution->endInscriptions();
        for (auto inscription = solution->beginInscriptions(); inscription != inscriptionsEnd; inscription++) {
            stream.writeStartElement("inscription");
            stream.writeAttribute("semestre", (*inscription)->semestre().code());
            stream.writeAttribute("resultat", noteToString((*inscription)->resultat()));
            this->writeECTS(stream, (*inscription)->ects());
            stream.writeEndElement();// inscription
        }
        stream.writeEndElement(); // inscriptions

        stream.writeStartElement("formations");
        auto formationsEnd = solution->endFormations();
        for (auto formation = solution->beginFormations(); formation != formationsEnd; formation++) {
            stream.writeStartElement("formation");
            stream.writeAttribute("code", (*formation)->getCode());
            stream.writeEndElement(); // formation
        }
        stream.writeEndElement(); // formations

        stream.writeEndElement(); // solution
    }

    stream.writeEndElement(); // solutions
    stream.writeEndElement(); // etudiant
    stream.writeEndDocument();

    file_->close();
    return true;
}

void EtudiantStorageXML::writeECTS(QXmlStreamWriter &stream, shared_ptr<const ECTS> ects) {
    if (auto uv = dynamic_pointer_cast<const UV>(ects)) {
        qDebug() << "uv";
        stream.writeAttribute("type", "uv");
        stream.writeAttribute("code", uv->getCode());
    } else if (auto equiv = dynamic_pointer_cast<const Equivalence>(ects)) {
        stream.writeAttribute("type", "equivalence");
        stream.writeAttribute("universite", equiv->universite());
        // on va maintenant sauvegarder tous les crédits donnés par cette équivalance
        auto creditsEnd = equiv->endCredits();
        for (auto credits = equiv->beginCredits(); credits != creditsEnd; credits++) {
            stream.writeStartElement("credits");
            stream.writeAttribute("categorie", (*credits)->categorie()->getCode());
            stream.writeAttribute("quantite", QString::number((*credits)->nbCredits()));
            stream.writeEndElement();
        }
    }
}

void EtudiantStorageXML::writeDemandesUVs(QXmlStreamWriter &stream, const QString &type, set<shared_ptr<UV>>::const_iterator start, set<shared_ptr<UV>>::const_iterator end)
{
    for (auto demande = start; demande != end; demande++) {
        stream.writeStartElement("uv");
        stream.writeAttribute("type", type);
        stream.writeAttribute("code", (*demande)->getCode());
        stream.writeEndElement();
    }
}

/*!
 * \brief Charge un profil d'étudiant à partir d'un fichier au format XML
 * \return std::shared_ptr<Etudiant>
 */
shared_ptr<Etudiant> EtudiantStorageXML::load()
{
    // On essaye d'ouvrir le QIODevice où est stocké le XML
    if (!file_->open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw new EtudiantStorageException("Erreur ouverture fichier profil étudiant pour chargement");
    }

    shared_ptr<Etudiant> etudiant = make_shared<Etudiant> ("", "", "");

    // On créer un XmlStreamReader
    // prend un QIODevice* en entrée
    // shared_ptr<T>.get() renvoie le pointeur T* sous-jacent
    QXmlStreamReader xml(file_.get());
    while(!xml.atEnd() && !xml.hasError()) {
        QXmlStreamReader::TokenType token = xml.readNext();
        // If token is just StartDocument, we'll go to next.
        if(token == QXmlStreamReader::StartDocument) continue;
        // If token is StartElement, we'll see if we can read it.
        if(token == QXmlStreamReader::StartElement) {
            // etudiant est l'élément racine
            if (xml.name() == "etudiant") continue;
            else if (xml.name() == "nom") {
                xml.readNext();
                etudiant->setNom(xml.text().toString());
            }
            else if (xml.name() == "prenom") {
                xml.readNext();
                etudiant->setPrenom(xml.text().toString());
            }
            else if (xml.name() == "login") {
                xml.readNext();
                etudiant->setLogin(xml.text().toString());
            } else if (xml.name() == "solutions") {
                token = xml.readNext();
                do {
                    token = xml.readNext();
                    qDebug() << xml.name();
                    if (token == QXmlStreamReader::StartElement && xml.name() == "solution") {
                        loadSolution(xml, etudiant);
                    }
                } while (!(token == QXmlStreamReader::EndElement && xml.name() == "solutions") && !xml.hasError());
                qDebug() << "all your solutions are belong to us";
            }
        }
    }

    file_->close();
    return etudiant;
}

void EtudiantStorageXML::loadSolution(QXmlStreamReader& xml, shared_ptr<Etudiant> etudiant) {
    qDebug() << "bonjour";
    // On recherche d'abord la date de création dans les attributs
    // afin de créer la solution
    QXmlStreamAttributes attrs = xml.attributes();
    QDateTime created = QDateTime::currentDateTime();
    if (attrs.hasAttribute("created_at")) {
        created = QDateTime::fromString(attrs.value("created_at").toString(), Qt::ISODate);
    }
    Solution& sol = etudiant->createSolution(created);

    // on recherche maintenant les inscriptions et les demandes
    QXmlStreamReader::TokenType token = xml.readNext();
    do {
        token = xml.readNext();
        if (token == QXmlStreamReader::StartElement) {
            qDebug() << "start : " << xml.name();
            if (xml.name() == "demandes") {
                loadDemandes(xml, sol);
            } else if (xml.name() == "inscriptions") {
                loadInscriptions(xml, sol);
            } else if (xml.name() == "formations") {
                loadFormations(xml, sol);
            }
        }
    } while (!(token == QXmlStreamReader::EndElement && xml.name() == "solution") && !xml.hasError());
    qDebug() << "finished solution";
}

void EtudiantStorageXML::loadDemandes(QXmlStreamReader &xml, Solution &sol)
{
    QXmlStreamReader::TokenType token = xml.readNext();
    if (token == QXmlStreamReader::EndElement && xml.name() == "demandes") {
        return;
    }

    do {
        token = xml.readNext();
        if (token == QXmlStreamReader::StartElement && xml.name() == "uv") {
            QXmlStreamAttributes attrs = xml.attributes();
            if (attrs.hasAttribute("type") && attrs.hasAttribute("code")) {
                shared_ptr<UV> uv = findUV(attrs.value("code"));
                QStringRef type = attrs.value("type");
                if (type == "rejet") {
                    sol.addRejet(uv);
                } else if (type == "preference") {
                    sol.addPreference(uv);
                } else if (type == "exigence") {
                    sol.addExigence(uv);
                }
            }
        }
        qDebug() << xml.name();
    } while (!(token == QXmlStreamReader::EndElement && xml.name() == "demandes") && !xml.hasError());
}

void EtudiantStorageXML::loadInscriptions(QXmlStreamReader &xml, Solution &sol)
{
    QXmlStreamReader::TokenType token = xml.readNext();
    if (token == QXmlStreamReader::EndElement && xml.name() == "inscriptions") {
        return;
    }

    do {
        token = xml.readNext();
        if (token == QXmlStreamReader::StartElement && xml.name() == "inscription") {
            QXmlStreamAttributes attrs = xml.attributes();
            if (attrs.hasAttribute("semestre") && attrs.hasAttribute("type") && attrs.hasAttribute("resultat")) {

                shared_ptr<ECTS> ects;
                // UV
                if (attrs.value("type") == "uv" && attrs.hasAttribute("code")) {
                    ects = findUV(attrs.value("code"));
                }
                // Equivalence
                else if (attrs.value("type") == "equivalence" && attrs.hasAttribute("universite")) {
                    qDebug() << attrs.value("universite").toString();
                    shared_ptr<Equivalence> equiv = make_shared<Equivalence>(attrs.value("universite").toString());

                    // On ajoute les crédits données par l'equivalence
                    token = xml.readNext();
                    if (token != QXmlStreamReader::EndElement) {
                        do {
                            token = xml.readNext();
                            if (token == QXmlStreamReader::StartElement && xml.name() == "credits") {
                                QXmlStreamAttributes attrsCredits = xml.attributes();
                                if (attrsCredits.hasAttribute("categorie") && attrsCredits.hasAttribute("quantite")) {
                                    equiv->addCredits(categorieManager::getInstance().getCategorie(attrsCredits.value("categorie").toString()),
                                                      attrsCredits.value("quantite").toUInt());
                                }
                            }
                        } while(!(token == QXmlStreamReader::EndElement && xml.name() == "inscription") && !xml.hasError());
                    } else {
                        qDebug() << "pas de crédits";
                    }
                    ects = equiv;
                }

                if (ects) {
                    try {
                        Note note = stringToNote(attrs.value("resultat").toString());
                        shared_ptr<Inscription> ins = make_shared<Inscription>(ects,
                                                                       Semestre(attrs.value("semestre").toString()),
                                                                       note);
                        sol.addInscription(ins);
                    } catch (UTProfilerException* e) {
                        throw new EtudiantStorageException("Note invalide pour l'inscription " + ects->getCode());
                    }
                }
            }
        }
        qDebug() << xml.name();
    } while (!(token == QXmlStreamReader::EndElement && xml.name() == "inscriptions") && !xml.hasError());
}

void EtudiantStorageXML::loadFormations(QXmlStreamReader &xml, Solution &sol)
{
    QXmlStreamReader::TokenType token = xml.readNext();
    if (token == QXmlStreamReader::EndElement && xml.name() == "formations") {
        return;
    }

    do {
        token = xml.readNext();
        if (token == QXmlStreamReader::StartElement && xml.name() == "formation") {
            QXmlStreamAttributes attrs = xml.attributes();
            if (attrs.hasAttribute("code")) {
                shared_ptr<Formation> formation = FormationManager::getInstance().getFormation(attrs.value("code").toString());
                sol.addFormation(formation);
            }
        }
        qDebug() << xml.name();
    } while (!(token == QXmlStreamReader::EndElement && xml.name() == "demandes") && !xml.hasError());
}

shared_ptr<UV> EtudiantStorageXML::findUV(QStringRef code)
{
    return UVManager::getInstance().getUV(code.toString());
}

