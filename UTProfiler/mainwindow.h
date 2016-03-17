#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QInputDialog>
#include "UVEditeur.h"
#include "etudiantediteur.h"
#include <QStandardItemModel>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    EtudiantEditeur* getCurrentEtudiantEditeur();
    void checkCurrentEtudiantEditeur();
    QStandardItemModel *standardItemModelUV;
    QStandardItemModel *standardItemModelCat;
    QStandardItemModel *standardItemModelForm;
public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

private slots:
    void itemClickedUV(QModelIndex index);
    void itemClickedCat(QModelIndex index);
    void itemClickedForm(QModelIndex index);
public slots:
    void openChargerUV();
    void openUV();
    void annulerEditerUV();
    void openChargerCat();
    void annulerEditerCat();
    void openChargerForm();
    void annulerEditerForm();
    void cbNouveauProfil();
    void cbProfilSave();
    void cbProfilSaveAs();
    void cbLoadProfil();
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
