#ifndef ETUDIANTEDITEUR_H
#define ETUDIANTEDITEUR_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <memory>
#include "etudiant.h"
#include "etudiantstoragexml.h"
#include "solutionediteur.h"

class EtudiantEditeur : public QWidget
{
    Q_OBJECT
    shared_ptr<Etudiant> etu;
    shared_ptr<EtudiantStorage> store;
    QLabel* titre;
    QVBoxLayout* mainLayout;
    QHBoxLayout* header;
    QPushButton* btnSave;
    SolutionEditeur* solutionEdit;

    void updateTitre();
public:
    explicit EtudiantEditeur(shared_ptr<Etudiant> etudiant, QWidget *parent = 0, shared_ptr<EtudiantStorage> storage = shared_ptr<EtudiantStorage>());
    shared_ptr<Etudiant> etudiant() const { return etu; }

signals:

public slots:
    void save();
    void saveAs();

};

#endif // ETUDIANTEDITEUR_H
