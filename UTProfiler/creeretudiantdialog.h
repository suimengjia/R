#ifndef CREERETUDIANTDIALOG_H
#define CREERETUDIANTDIALOG_H

#include <QDialog>
#include <memory>
#include "etudiant.h"
#include <QLabel>
#include <QLineEdit>
#include <QFormLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

class CreerEtudiantDialog : public QDialog
{
    Q_OBJECT
    QLabel* nomLabel;
    QLabel* prenomLabel;
    QLabel* loginLabel;
    QLineEdit* nom;
    QLineEdit* prenom;
    QLineEdit* login;
    QFormLayout* formLayout;
    QPushButton* btnOk;
    QPushButton* btnAnnuler;
    QVBoxLayout* baseLayout;
    QHBoxLayout* buttonsLayout;
public:
    explicit CreerEtudiantDialog(QWidget *parent = 0);
    QString getNom() const { return nom->text(); }
    QString getPrenom() const { return prenom->text(); }
    QString getLogin() const { return login->text(); }

signals:

public slots:

};

#endif // CREERETUDIANTDIALOG_H
