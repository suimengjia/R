#include "categorieediteur.h"
#include "ui_categorieediteur.h"
#include <QMessageBox>

CategorieEditeur::CategorieEditeur(shared_ptr<Categorie> cat,QWidget *parent) :
    cat(cat),
    QWidget(parent),
    ui(new Ui::CategorieEditeur)
{
    ui->setupUi(this);
    ui->lineEditCode->setText(cat->getCode());
    ui->textEditTitre->setText(cat->getTitre());
    ui->comboBoxListCat->addItem("NULL");
    for(auto it=categorieManager::getInstance().getCategories().begin();
        it!=categorieManager::getInstance().getCategories().end(); ++it){
        ui->comboBoxListCat->addItem((*it)->getCode());
    }
}

CategorieEditeur::~CategorieEditeur()
{
    delete ui;
}

void CategorieEditeur::on_pushButtonAnnuler_clicked()
{
    this->deleteLater();
}

void CategorieEditeur::on_pushButtonSave_clicked()
{
    categorieManager::getInstance().ajouterCategorie(ui->lineEditCode->text(),ui->textEditTitre->toPlainText());
              /* dynamic_pointer_cast<shared_ptr<CategorieC>> (categorieManager::getInstance().getCategorie(ui->comboBoxListCat->currentText())));*/
    try {
        categorieManager::getInstance().save("../BDbase.db",ui->lineEditCode->text());
        QMessageBox::information(this, "Sauvegarde", "Categorie sauvegardée...");
    }catch(UTProfilerException& e){
        QMessageBox::warning(this, "Save" + ui->lineEditCode->text(), e.getInfo());
    }
    this->deleteLater();
}

void CategorieEditeur::on_pushButtonSupprimer_clicked()
{
    try {
        categorieManager::getInstance().remove(cat->getCode());
        QMessageBox::information(this, "Suppression", "Catégorie supprimé...");
    }catch(UTProfilerException& e){
        QMessageBox::warning(this, "delete" + cat->getCode(), e.getInfo());
    }
    this->deleteLater();
}
