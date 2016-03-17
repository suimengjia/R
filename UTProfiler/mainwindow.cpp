#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "UVEditeur.h"
#include "creeretudiantdialog.h"
#include <QMenuBar>
#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QStandardItem>

#include <QModelIndex>
#include "categorieediteur.h"
#include "formationediteur.h"

//test
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // les deux instructions ci dessous permettent une
    // mise en page correcte de la liste des UVs
    ui->tab->setLayout(ui->verticalLayout_2);
    ui->verticalLayout_2->addWidget(ui->listUV);
    ui->tabForm->setLayout(ui->verticalLayout_3);
    ui->verticalLayout_3->addWidget(ui->listFormation);
    ui->tabCat->setLayout(ui->verticalLayout_4);
    ui->verticalLayout_4->addWidget(ui->listCategorie);
    openChargerUV(); // TEMPORAIRE !
    openChargerCat();
    openChargerForm();
    //setCentralWidget(ui->tabWidget);
}

void MainWindow::openChargerUV(){
    standardItemModelUV = new QStandardItemModel(this);
    QStringList strList;
    for(auto it=UVManager::getInstance().getUvs().begin(); it!=UVManager::getInstance().getUvs().end(); it++)
         strList.append((*it).second->getCode());
    int nCount = strList.size();
    for(int i = 0; i < nCount; i++)
    {
        QString string = static_cast<QString>(strList.at(i));
        QStandardItem *item = new QStandardItem(string);
        if(i % 2 == 1)
        {
            QLinearGradient linearGrad(QPointF(0, 0), QPointF(200, 200));
            linearGrad.setColorAt(0, Qt::white);
            linearGrad.setColorAt(1, Qt::yellow);
            QBrush brush(linearGrad);
            item->setBackground(brush);
        }
        standardItemModelUV->appendRow(item);
    }
    ui->listUV->setModel(standardItemModelUV);
    connect(ui->listUV,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(itemClickedUV(QModelIndex)));
    ui->listUV->setVisible(1);
/*    QString chemin =  "../BDbase.db";
            //QFileDialog::getOpenFileName();
    try {
        if (chemin!="")UVManager::getInstance().load(chemin);
        QMessageBox::information(this, "Chargement Catalogue", "Le catalogue d’UVs a été chargé.");
    }catch(exception& e){
        QMessageBox::warning(this, "Chargement Catalogue", "Erreur lors du chargement du fichier (non valide ?)");
    }*/
}
void MainWindow::itemClickedUV(QModelIndex index)
{
    qDebug() << index.data().toString();
    ui->listUV->setVisible(0);
    try {
        shared_ptr<UV> uv=UVManager::getInstance().getUV(index.data().toString());
        UVEditeur* fenetre=new UVEditeur(uv, ui->tab);
        QObject::connect(fenetre, SIGNAL(destroyed()), this, SLOT(annulerEditerUV()));
        ui->verticalLayout_2->addWidget(fenetre);
        fenetre->show();
    }catch(exception& e){
        QMessageBox::warning(this, "Edition UV", QString("Erreur : l’UV ")+index.data().toString()+" n’existe pas.");
    }
}

void MainWindow::itemClickedCat(QModelIndex index){
    qDebug() << index.data().toString();
    ui->listCategorie->setVisible(0);
    try {
        shared_ptr<Categorie> categorie=categorieManager::getInstance().getCategorie(index.data().toString());
        CategorieEditeur* fenetre=new CategorieEditeur(categorie,ui->tabCat);
        QObject::connect(fenetre, SIGNAL(destroyed()), this, SLOT(annulerEditerCat()));
        ui->verticalLayout_4->addWidget(fenetre);
        fenetre->show();
    }catch(exception& e){
        QMessageBox::warning(this, "Edition UV", QString("Erreur : le Categorie ")+index.data().toString()+" n’existe pas.");
    }
}

void MainWindow::itemClickedForm(QModelIndex index)
{
    qDebug() << index.data().toString();
    ui->listFormation->setVisible(0);
    try {
        shared_ptr<Formation> categorie=FormationManager::getInstance().getFormation(index.data().toString());
        FormationEditeur* fenetre=new FormationEditeur(categorie,ui->tabForm);
        QObject::connect(fenetre, SIGNAL(destroyed()), this, SLOT(annulerEditerForm()));
        ui->verticalLayout_3->addWidget(fenetre);
        fenetre->show();
    }catch(exception& e){
        QMessageBox::warning(this, "Edition UV", QString("Erreur : la Formation ")+index.data().toString()+" n’existe pas.");
    }
}

void MainWindow::openUV(){
    QString code=QInputDialog::getText(this,"Entrez le code de l’UV à éditer","UV");
    if (code!="")
        try {
            shared_ptr<UV> uv=UVManager::getInstance().getUV(code);
            UVEditeur* fenetre=new UVEditeur(uv,ui->tab);
            QObject::connect(fenetre, SIGNAL(destroyed()), this, SLOT(annulerEditerUV()));
            fenetre->show();
        }catch(exception& e){
            QMessageBox::warning(this, "Edition UV", QString("Erreur : l’UV ")+code+" n’existe pas.");
        }
}

void MainWindow::annulerEditerUV() {
    delete standardItemModelUV;
    standardItemModelUV = new QStandardItemModel(this);
    QStringList strList;
    for(auto it=UVManager::getInstance().getUvs().begin(); it!=UVManager::getInstance().getUvs().end(); it++)
         strList.append((*it).second->getCode());
    int nCount = strList.size();
    for(int i = 0; i < nCount; i++)
    {
        QString string = static_cast<QString>(strList.at(i));
        QStandardItem *item = new QStandardItem(string);
        if(i % 2 == 1)
        {
            QLinearGradient linearGrad(QPointF(0, 0), QPointF(200, 200));
            linearGrad.setColorAt(0, Qt::white);
            linearGrad.setColorAt(1, Qt::yellow);
            QBrush brush(linearGrad);
            item->setBackground(brush);
        }
        standardItemModelUV->appendRow(item);
    }
    ui->listUV->setModel(standardItemModelUV);
    ui->listUV->setVisible(1);
}

void MainWindow::openChargerCat()
{
    standardItemModelCat = new QStandardItemModel(this);
    QStringList strList;
    for(auto it=categorieManager::getInstance().getCategories().begin();
        it!=categorieManager::getInstance().getCategories().end(); it++)
         strList.append((*it)->getCode());
    int nCount = strList.size();
    for(int i = 0; i < nCount; i++)
    {
        QString string = static_cast<QString>(strList.at(i));
        QStandardItem *item = new QStandardItem(string);
        if(i % 2 == 1)
        {
            QLinearGradient linearGrad(QPointF(0, 0), QPointF(200, 200));
            linearGrad.setColorAt(0, Qt::white);
            linearGrad.setColorAt(1, Qt::yellow);
            QBrush brush(linearGrad);
            item->setBackground(brush);
        }
        standardItemModelCat->appendRow(item);
    }
    ui->listCategorie->setModel(standardItemModelCat);
    connect(ui->listCategorie,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(itemClickedCat(QModelIndex)));
    ui->listCategorie->setVisible(1);
}

void MainWindow::annulerEditerCat()
{
    delete standardItemModelCat;
    standardItemModelCat = new QStandardItemModel(this);
    QStringList strList;
    for(auto it=categorieManager::getInstance().getCategories().begin();
        it!=categorieManager::getInstance().getCategories().end(); it++)
         strList.append((*it)->getCode());
    int nCount = strList.size();
    for(int i = 0; i < nCount; i++)
    {
        QString string = static_cast<QString>(strList.at(i));
        QStandardItem *item = new QStandardItem(string);
        if(i % 2 == 1)
        {
            QLinearGradient linearGrad(QPointF(0, 0), QPointF(200, 200));
            linearGrad.setColorAt(0, Qt::white);
            linearGrad.setColorAt(1, Qt::yellow);
            QBrush brush(linearGrad);
            item->setBackground(brush);
        }
        standardItemModelCat->appendRow(item);
    }
    ui->listCategorie->setModel(standardItemModelCat);
    ui->listCategorie->setVisible(1);
}

void MainWindow::openChargerForm()
{
    standardItemModelForm = new QStandardItemModel(this);
    QStringList strList;
    for(auto it=FormationManager::getInstance().getFormations().begin();
        it!=FormationManager::getInstance().getFormations().end(); it++)
         strList.append((*it)->getCode());
    int nCount = strList.size();
    for(int i = 0; i < nCount; i++)
    {
        QString string = static_cast<QString>(strList.at(i));
        QStandardItem *item = new QStandardItem(string);
        if(i % 2 == 1)
        {
            QLinearGradient linearGrad(QPointF(0, 0), QPointF(200, 200));
            linearGrad.setColorAt(0, Qt::white);
            linearGrad.setColorAt(1, Qt::yellow);
            QBrush brush(linearGrad);
            item->setBackground(brush);
        }
        standardItemModelForm->appendRow(item);
    }
    ui->listFormation->setModel(standardItemModelForm);
    connect(ui->listFormation,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(itemClickedForm(QModelIndex)));
    ui->listFormation->setVisible(1);
}

void MainWindow::annulerEditerForm()
{
    delete standardItemModelForm;
    standardItemModelForm = new QStandardItemModel(this);
    QStringList strList;
    for(auto it=FormationManager::getInstance().getFormations().begin();
        it!=FormationManager::getInstance().getFormations().end(); it++)
         strList.append((*it)->getCode());
    int nCount = strList.size();
    for(int i = 0; i < nCount; i++)
    {
        QString string = static_cast<QString>(strList.at(i));
        QStandardItem *item = new QStandardItem(string);
        if(i % 2 == 1)
        {
            QLinearGradient linearGrad(QPointF(0, 0), QPointF(200, 200));
            linearGrad.setColorAt(0, Qt::white);
            linearGrad.setColorAt(1, Qt::yellow);
            QBrush brush(linearGrad);
            item->setBackground(brush);
        }
        standardItemModelForm->appendRow(item);
    }
    ui->listFormation->setModel(standardItemModelForm);
    ui->listFormation->setVisible(1);
}

void MainWindow::cbNouveauProfil() {
    checkCurrentEtudiantEditeur();

    CreerEtudiantDialog dialog;
    int res = dialog.exec();
    if (res == 1) {
        shared_ptr<Etudiant> etu = EtudiantManager::getInstance().creerEtudiant(dialog.getLogin(), dialog.getNom(), dialog.getPrenom());
        etu->createSolution();
        EtudiantEditeur* fenetre = new EtudiantEditeur(etu, ui->tabEtu);
        fenetre->show();
    }
}

void MainWindow::cbProfilSave() {
    EtudiantEditeur* editeur = getCurrentEtudiantEditeur();
    if (editeur != 0) {
        editeur->save();
    }
}

void MainWindow::cbProfilSaveAs() {
    EtudiantEditeur* editeur = getCurrentEtudiantEditeur();
    if (editeur != 0) {
        editeur->saveAs();
    }
}

void MainWindow::checkCurrentEtudiantEditeur() {
    EtudiantEditeur* editeur = getCurrentEtudiantEditeur();
    if (editeur != 0) {
        QMessageBox::StandardButton resp = QMessageBox::question(this,
                                                            "Enregistrer le profil",
                                                            "Voulez vous enregistrer le profil courant avant d'en ouvrir un nouveau ?",
                                                            QMessageBox::Yes | QMessageBox::No);
        if (resp == QMessageBox::Yes) {
            editeur->save();
        }
        editeur->deleteLater();
    }
}

void MainWindow::cbLoadProfil() {
    checkCurrentEtudiantEditeur();

    // on récupère le chemin du fichier où l'utilisateur veut enregistrer le profil
    QString filename = QFileDialog::getOpenFileName(this, tr("Choisir le profil à charger"));

    // création du QFile correspondant
    shared_ptr<QFile> file = make_shared<QFile>(filename);
    // création du storage XML
    shared_ptr<EtudiantStorageXML> store = make_shared<EtudiantStorageXML>(file);
    try {
        shared_ptr<Etudiant> etu = store->load();
        EtudiantEditeur* fenetre = new EtudiantEditeur(etu, ui->tabEtu, store);
        fenetre->show();
    } catch (EtudiantStorageException* e) {
        qDebug() << e->getInfo();
    }
}

EtudiantEditeur* MainWindow::getCurrentEtudiantEditeur() {
    QObjectList enfants = ui->tabEtu->children();
    if (enfants.size() == 0) {
        return 0;
    }
    QObject* editeur = enfants.first();
    if (editeur != 0) {
        return dynamic_cast<EtudiantEditeur*>(editeur);
    } else {
        return 0;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


