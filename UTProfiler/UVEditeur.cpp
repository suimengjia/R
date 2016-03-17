#include "UVEditeur.h"
#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <list>
#include <iterator>
#include <categorie.h>

UVEditeur::UVEditeur(shared_ptr<UV> uvToEdit, QWidget *parent) : QWidget(parent),uv(uvToEdit){
    this->setWindowTitle(QString("Edition de l’UV ")+uv->getCode());
    // creation des labels
    codeLabel = new QLabel("code",this);
    titreLabel = new QLabel("titre",this);
    creditsLabel = new QLabel("credits",this);
    categorieLabel = new QLabel("categorie",this);
    ouvertureLabel = new QLabel("ouverture",this);
    // création des composants éditables
    code = new QLineEdit(uv->getCode(),this);
    titre = new QTextEdit(uv->getTitre(),this);
    credits=new QSpinBox(this);
    credits->setRange(1,8);
    credits->setValue(uv->getNbCredits());
    categorie=new QComboBox(this);
//    int num=0;
//    bool trouve=false;
    for(auto it=categorieManager::getInstance().getCategories().begin();
        it!=categorieManager::getInstance().getCategories().end(); ++it){
//        if (QString((*it)->getCode()) != uv.getCategorie().getCode() && !\)
//            num++;
//        else trouve=true;
        categorie->addItem((*it)->getCode());
    }
    categorie->setCurrentText(uv->getCategorie()->getCode()); // categorie->setCurrentIndex(num);
    automne=new QCheckBox("automne",this);
    automne->setChecked(uv->ouvertureAutomne());
    printemps=new QCheckBox("printemps",this);
    printemps->setChecked(uv->ouverturePrintemps());
    sauver= new QPushButton("Sauver", this);
    annuler= new QPushButton("Annuler", this);
    supprimer= new QPushButton("Supprimer", this);
    QObject::connect(sauver, SIGNAL(clicked()), this, SLOT(sauverUV()));
    QObject::connect(annuler,SIGNAL(clicked()), this, SLOT(annulerEditeur()));
    QObject::connect(supprimer,SIGNAL(clicked()), this, SLOT(supprimerUV()));
    // diposition des couches
    coucheH1 = new QHBoxLayout;
    //QMessageBox::warning(this, "Edition UV", QString("sssbbbbbbbbs."));
    coucheH1->addWidget(codeLabel);
    coucheH1->addWidget(code);
    coucheH1->addWidget(categorieLabel);
    coucheH1->addWidget(categorie);
    coucheH1->addWidget(creditsLabel);
    coucheH1->addWidget(credits);

    coucheH2 = new QHBoxLayout;
    coucheH2->addWidget(titreLabel);
    coucheH2->addWidget(titre);
    coucheH3 = new QHBoxLayout;
    coucheH3->addWidget(ouvertureLabel);
    coucheH3->addWidget(automne);
    coucheH3->addWidget(printemps);
    coucheH4 = new QHBoxLayout;
    coucheH4->addWidget(annuler);
    coucheH4->addWidget(sauver);
    coucheH4->addWidget(supprimer);
    couche = new QVBoxLayout;
    couche->addLayout(coucheH1);
    couche->addLayout(coucheH2);
    couche->addLayout(coucheH3);
    couche->addLayout(coucheH4);

    setLayout(couche);
}
void UVEditeur::sauverUV(){

    if (!UVManager::getInstance().existeUV(code->text())){

        UVManager::getInstance().ajouterUV(code->text(),titre->toPlainText(),credits->value(),
                  categorieManager::getInstance().getCategorie(categorie->currentText()),
                  printemps->isChecked(),automne->isChecked());

        try {
            UVManager::getInstance().save("../BDbase.db",code->text());
            QMessageBox::information(this, "Sauvegarde", "UV sauvegardée...");
        }catch(UTProfilerException& e){
            QMessageBox::warning(this, "Save" + uv->getCode(), e.getInfo());
        }
    }else{

        uv->setCode(code->text());
        uv->setTitre(titre->toPlainText());
        uv->setNbCredits(credits->value());
        uv->setCategorie(categorieManager::getInstance().getCategorie(categorie->currentText()));
        uv->setOuverturePrintemps(printemps->isChecked());
        uv->setOuvertureAutomne(automne->isChecked());
        try {
            UVManager::getInstance().save("../BDbase.db",uv->getCode());
            QMessageBox::information(this, "Sauvegarde", "UV sauvegardée...");
        }catch(UTProfilerException& e){
            QMessageBox::warning(this, "Save" + uv->getCode(), e.getInfo());
        }
    }
    this->deleteLater();
    //this->parent()->parent()->parent()->parent()->parent()->parent()->openChargerUV();
}

void UVEditeur::annulerEditeur() {
    this->deleteLater();
}

void UVEditeur::supprimerUV()
{
    try {
        UVManager::getInstance().remove(uv->getCode());
        QMessageBox::information(this, "Suppression", "UV supprimée...");
    }catch(UTProfilerException& e){
        QMessageBox::warning(this, "delete" + uv->getCode(), e.getInfo());
    }
    this->deleteLater();
}
