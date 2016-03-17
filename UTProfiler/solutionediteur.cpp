#include "solutionediteur.h"

#include <QStandardItem>
#include <QInputDialog>
#include <QMessageBox>

SolutionEditeur::SolutionEditeur(Solution& solution, QWidget *parent) :
    QTabWidget(parent), sol(solution)
{
    // formations
    formationsTab = new QWidget;
    formationsLayout = new QVBoxLayout;
    formationsTab->setLayout(formationsLayout);

    formationsBtns = new QHBoxLayout;
    formationsLayout->addLayout(formationsBtns);

    formationAddBtn = new QPushButton("Ajouter");
    QObject::connect(formationAddBtn, SIGNAL(clicked()), this, SLOT(addFormation()));
    formationsBtns->addWidget(formationAddBtn);
    formationDelBtn = new QPushButton("Enlever");
    QObject::connect(formationDelBtn, SIGNAL(clicked()), this, SLOT(removeFormation()));
    formationDelBtn->setDisabled(true);
    formationsBtns->addWidget(formationDelBtn);

    formationsListView = new QListWidget;
    QObject::connect(formationsListView, SIGNAL(itemSelectionChanged()), this, SLOT(updateBtnDelFormation()));
    formationsLayout->addWidget(formationsListView);

    addTab(formationsTab, "Formation");
    updateFormations();

    inscriptionsTab = new QWidget;
    inscriptionsLayout = new QVBoxLayout;
    inscriptionsTab->setLayout(inscriptionsLayout);

    inscriptionsBtns = new QHBoxLayout;
    inscriptionsLayout->addLayout(inscriptionsBtns);

    inscriptionAddBtn = new QPushButton("Ajouter");
    //QObject::connect(inscriptionAddBtn, SIGNAL(clicked()), this, SLOT(addInscription()));
    inscriptionsBtns->addWidget(inscriptionAddBtn);
    inscriptionDelBtn = new QPushButton("Enlever");
    //QObject::connect(inscriptionDelBtn, SIGNAL(clicked()), this, SLOT(removeInscription()));
    inscriptionDelBtn->setDisabled(true);
    inscriptionsBtns->addWidget(inscriptionDelBtn);

    inscriptionsView = new QTableWidget;
    //QObject::connect(inscriptionsListView, SIGNAL(itemSelectionChanged()), this, SLOT(updateBtnDelinscription()));
    inscriptionsLayout->addWidget(inscriptionsView);

    addTab(inscriptionsTab, "UVs");
    updateInscriptions();

    addTab(new QLabel("Solution : saisir les demandes"), "Solution");

}


void SolutionEditeur::updateFormations() {
    formationsListView->clear();

    auto end = sol.endFormations();
    for(auto it= sol.beginFormations(); it != end; it++) {
        formationsListView->addItem((*it)->getCode());
    }
}

void SolutionEditeur::addFormation() {
    QStringList items;

    auto end = FormationManager::getInstance().end();
    for(auto it= FormationManager::getInstance().begin(); it != end; it++) {
        items.append((*it)->getCode());
    }

    bool ok;
    QString code = QInputDialog::getItem(this, "Ajouter une formation à ce profil",
                                         "Merci de choisir parmi la liste", items,
                                         0, false, &ok);
    if(ok) {
        if (sol.addFormation(FormationManager::getInstance().getFormation(code))) {
            updateFormations();
        } else {
            QMessageBox::information(this, "Ajouter une formation", "Impossible d'ajouter la formation");
        }
    }
}

void SolutionEditeur::removeFormation() {
    auto item = formationsListView->currentItem();
    if (item != 0) {
        shared_ptr<Formation> formation = FormationManager::getInstance().getFormation(item->text());
        sol.removeFormation(formation);
        updateFormations();
    }
}

void SolutionEditeur::updateBtnDelFormation() {
    formationDelBtn->setEnabled(formationsListView->currentItem() != 0);
}

void SolutionEditeur::updateInscriptions() {
    inscriptionsView->clear();
    inscriptionsView->setColumnCount(5);
    QStringList labels;
    labels.append("Semestre");
    labels.append("Code");
    labels.append("Catégorie");
    labels.append("Crédits");
    labels.append("Résultat");
    inscriptionsView->setHorizontalHeaderLabels(labels);
    inscriptionsView->setRowCount(sol.nbInscriptions());
    auto end = sol.endInscriptions();
    unsigned int i = 0;
    for (auto ins = sol.beginInscriptions(); ins != end; ins++) {
        QTableWidgetItem *sem = new QTableWidgetItem((*ins)->semestre().code());
        inscriptionsView->setItem(i, 0, sem);

        QTableWidgetItem *code = new QTableWidgetItem((*ins)->ects()->getCode());
        inscriptionsView->setItem(i, 1, code);

        QTableWidgetItem *cat = new QTableWidgetItem((*ins)->ects()->getCategorie()->getCode());
        inscriptionsView->setItem(i, 2, cat);

        QTableWidgetItem *credits = new QTableWidgetItem(QString::number((*ins)->ects()->getNbCredits()));
        inscriptionsView->setItem(i, 3, credits);

        QTableWidgetItem *res = new QTableWidgetItem(noteToString((*ins)->resultat()));
        inscriptionsView->setItem(i, 4, res);
        i++;
    }
}
