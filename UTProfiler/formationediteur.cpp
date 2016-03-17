#include "formationediteur.h"
#include "ui_formationediteur.h"
#include <QMessageBox>

FormationEditeur::FormationEditeur(shared_ptr<Formation> form, QWidget *parent) :
    form(form),
    QWidget(parent),
    ui(new Ui::FormationEditeur)
{
    ui->setupUi(this);
    ui->lineEditCode->setText(form->getCode());
    ui->textEditTitre->setText(form->getTitre());
    ui->comboBoxListCat->addItem("NULL");
    for(auto it=FormationManager::getInstance().getFormations().begin();
        it!=FormationManager::getInstance().getFormations().end(); ++it){
        ui->comboBoxListCat->addItem((*it)->getCode());
    }
}

FormationEditeur::~FormationEditeur()
{
    delete ui;
}

void FormationEditeur::on_pushButtonAnnuler_clicked()
{
    this->deleteLater();
}

void FormationEditeur::on_pushButtonSave_clicked()
{
    FormationManager::getInstance().ajouterFormation(ui->lineEditCode->text(),ui->textEditTitre->toPlainText());
              /* dynamic_pointer_cast<shared_ptr<CategorieC>> (categorieManager::getInstance().getCategorie(ui->comboBoxListCat->currentText())));*/
    try {
        FormationManager::getInstance().save("../BDbase.db",ui->lineEditCode->text());
        QMessageBox::information(this, "Sauvegarde", "Categorie sauvegardée...");
    }catch(UTProfilerException& e){
        QMessageBox::warning(this, "Save" + ui->lineEditCode->text(), e.getInfo());
    }
    this->deleteLater();
}

void FormationEditeur::on_pushButtonSupprimer_clicked()
{
    try {
        FormationManager::getInstance().remove(form->getCode());
        QMessageBox::information(this, "Suppression", "Formation supprimée...");
    }catch(UTProfilerException& e){
        QMessageBox::warning(this, "delete" + form->getCode(), e.getInfo());
    }
    this->deleteLater();
}
