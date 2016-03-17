#include "creeretudiantdialog.h"

CreerEtudiantDialog::CreerEtudiantDialog(QWidget *parent) :
    QDialog(parent)
{
    setModal(true);
    setWindowTitle("Nouveau profil étudiant");

    // création des labels
    nomLabel = new QLabel("Nom", this);
    prenomLabel = new QLabel("Prénom", this);
    loginLabel = new QLabel("Login", this);
    // création des champs texte
    nom = new QLineEdit("", this);
    prenom = new QLineEdit("", this);
    login = new QLineEdit("", this);
    // création des boutons
    btnOk = new QPushButton("Créer");
    btnOk->setDefault(true);
    QObject::connect(btnOk, SIGNAL(clicked()), this, SLOT(accept()));
    btnAnnuler = new QPushButton("Annuler");
    QObject::connect(btnAnnuler, SIGNAL(clicked()), this, SLOT(reject()));

    // on place le tout dans la fenêtre
    baseLayout = new QVBoxLayout(this);

    formLayout = new QFormLayout;
    formLayout->addRow(nomLabel, nom);
    formLayout->addRow(prenomLabel, prenom);
    formLayout->addRow(loginLabel, login);
    baseLayout->addLayout(formLayout);

    buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(btnAnnuler);
    buttonsLayout->addWidget(btnOk);
    baseLayout->addLayout(buttonsLayout);

    setLayout(baseLayout);
}
