#include "etudiantediteur.h"
#include <QTextStream>
#include <QFileDialog>
#include <QStandardItemModel>

EtudiantEditeur::EtudiantEditeur(shared_ptr<Etudiant> etudiant, QWidget *parent, shared_ptr<EtudiantStorage> storage) :
    QWidget(parent), etu(etudiant), store(storage)
{
    titre = new QLabel;
    updateTitre();
    mainLayout = new QVBoxLayout;

    header = new QHBoxLayout;
    header->addWidget(titre);
    mainLayout->addLayout(header);

    // bouton de sauvegarde
    btnSave = new QPushButton("Enregistrer");
    QObject::connect(btnSave, SIGNAL(clicked()), this, SLOT(save()));
    header->addWidget(btnSave);

    if (etu->beginSolutions() != etu->endSolutions()) {
        solutionEdit = new SolutionEditeur(*(etu->beginSolutions()));
        mainLayout->addWidget(solutionEdit);
    }

    setLayout(mainLayout);
}

void EtudiantEditeur::updateTitre() {
    QString res;
    QTextStream stream(&res);
    stream << etu->nom() << " " << etu->prenom() << " (" << etu->login() << ")";
    titre->setText(res);
}

void EtudiantEditeur::save() {
    if (!store) {
        saveAs();
    } else {
        store->save(*etu);
    }
}

void EtudiantEditeur::saveAs() {
    // on récupère le chemin du fichier où l'utilisateur veut enregistrer le profil
    QString filename = QFileDialog::getSaveFileName(this, tr("Enregistrer le profil"));

    // création du QFile correspondant
    shared_ptr<QFile> file = make_shared<QFile>(filename);
    // création du storage XML
    store = make_shared<EtudiantStorageXML>(file);
    store->save(*etu);
}

