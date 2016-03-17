#ifndef CATEGORIEEDITEUR_H
#define CATEGORIEEDITEUR_H

#include <QWidget>
#include <memory>
#include "categorie.h"
namespace Ui {
class CategorieEditeur;
}

class CategorieEditeur : public QWidget
{
    shared_ptr<Categorie> cat;
    Q_OBJECT

public:
    explicit CategorieEditeur(shared_ptr<Categorie> cat, QWidget *parent = 0);
    ~CategorieEditeur();

private slots:
    void on_pushButtonAnnuler_clicked();

    void on_pushButtonSave_clicked();

    void on_pushButtonSupprimer_clicked();

private:
    Ui::CategorieEditeur *ui;
};

#endif // CATEGORIEEDITEUR_H
