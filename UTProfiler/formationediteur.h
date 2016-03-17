#ifndef FORMATIONEDITEUR_H
#define FORMATIONEDITEUR_H

#include <QWidget>
#include "formation.h"
namespace Ui {
class FormationEditeur;
}

class FormationEditeur : public QWidget
{
    Q_OBJECT
    shared_ptr<Formation> form;

public:
    explicit FormationEditeur( shared_ptr<Formation> form, QWidget *parent = 0);
    ~FormationEditeur();

private slots:
    void on_pushButtonAnnuler_clicked();

    void on_pushButtonSave_clicked();

    void on_pushButtonSupprimer_clicked();

private:
    Ui::FormationEditeur *ui;
};

#endif // FORMATIONEDITEUR_H
