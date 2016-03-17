#ifndef SOLUTIONEDITEUR_H
#define SOLUTIONEDITEUR_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QListWidget>
#include <QTableWidget>
#include "solution.h"

class SolutionEditeur : public QTabWidget
{
    Q_OBJECT
    Solution& sol;
    QWidget* formationsTab;
    QVBoxLayout* formationsLayout;
    QHBoxLayout* formationsBtns;
    QPushButton* formationAddBtn;
    QPushButton* formationDelBtn;
    QListWidget* formationsListView;

    QWidget* inscriptionsTab;
    QVBoxLayout* inscriptionsLayout;
    QHBoxLayout* inscriptionsBtns;
    QPushButton* inscriptionAddBtn;
    QPushButton* inscriptionDelBtn;
    QTableWidget* inscriptionsView;

    void updateFormations();
    void updateInscriptions();
public:
    explicit SolutionEditeur(Solution& solution, QWidget *parent = 0);
    Solution& solution() { return sol; }

signals:

public slots:
    void addFormation();
    void removeFormation();
    void updateBtnDelFormation();

};

#endif // SOLUTIONEDITEUR_H
