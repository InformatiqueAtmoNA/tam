/*////////////////////////////////////////////////////////////
// \file et_resultatpolluant.cpp
// \brief Classe d'interface gérant l'affihage des rapports des tests
// \author PIET Régis
// \version 1.0
// \date 10/08/2011
//
// TAM - Tests Automatiques Métrologiques
// Copyright (C) 2011-2012 TAM Team
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
////////////////////////////////////////////////////////////*/

#include "et_resultatpolluant.h"
#include "ui_et_resultatpolluant.h"

et_Resultatpolluant::et_Resultatpolluant(QList<QString> listNomMolecule,QVector<QVector<float> > tabMesPolluant,
                                         QVector<float>  tabMoyenne,QWidget *parent):
    QWidget(parent),
    ui(new Ui::et_Resultatpolluant)
{
    ui->setupUi(this);
    creationColonneTabListe(listNomMolecule);
    affichageTableauMesure(tabMesPolluant,"Mesure "," (ppb)");
    affichageTableauCalcul(tabMoyenne,"MOYENNE (ppb)");
    this->ui->tableWidget->setVerticalHeaderLabels(m_listeEnteteLigne);
    this->ui->groupBox->hide();
}

et_Resultatpolluant::et_Resultatpolluant(QList<QString> listNomColTpsRep,QVector<QVector<float> > tabMesPolluant,
                                         QVector<float>  tabMoyenne,QVector <float> tabValeurPourCritere,
                                         QVector<double> tabCritere,QWidget *parent):
    QWidget(parent),
    ui(new Ui::et_Resultatpolluant)
{
    ui->setupUi(this);
    creationColonneTabListe(listNomColTpsRep);
    affichageTableauMesure(tabMesPolluant,"Tps Reponse "," (s)");
    affichageTableauCalcul(tabMoyenne,"MOYENNE Tps Reponse (s)");
    this->ui->tableWidget->setVerticalHeaderLabels(m_listeEnteteLigne);
    this->ui->label->hide();
    this->ui->lineEditDroiteReg->hide();
    this->ui->label_Valeur1->setText("Max Tps de Réponse =");
    this->ui->lineEdit_Valeur1->setText(QString::number(tabValeurPourCritere.value(0),'f',2));
    this->ui->label_Critere1->setText("Critère (s) <");
    this->ui->lineEdit_Critere1->setText(QString::number(tabCritere.value(0),'f',2));
    this->ui->label_Valeur2->setText("Difference Tps Réponse (s) =");
    this->ui->lineEdit_Valeur2->setText(QString::number(tabValeurPourCritere.value(1),'f',2));
    this->ui->label_Critere2->setText("Critère Max (s) <");
    this->ui->lineEdit_Critere2->setText(QString::number(tabCritere.value(1),'f',2));
}

et_Resultatpolluant::et_Resultatpolluant(QVector<float>tabConcentration,QVector<QVector<float> > tabMesPolluant,
                                         QVector<float>  tabMoyenne,QVector<float>  tabEcartType,
                                         QVector<double> tabCritere,QWidget *parent):
    QWidget(parent),
    ui(new Ui::et_Resultatpolluant)
{
    ui->setupUi(this);
    creationColonneTab(tabConcentration);
    affichageTableauMesure(tabMesPolluant,"Mesure "," (ppb)");
    affichageTableauCalcul(tabMoyenne,"MOYENNE (ppb)");
    affichageTableauCalcul(tabEcartType,"ECART TYPE (ppb");
    this->ui->tableWidget->setVerticalHeaderLabels(m_listeEnteteLigne);
    this->ui->tableWidget->resizeColumnsToContents();
    this->ui->tableWidget->resizeRowsToContents();
    this->ui->label->hide();
    this->ui->lineEditDroiteReg->hide();
    this->ui->label_Valeur1->setText("Ecart-Type Zero =");
    this->ui->lineEdit_Valeur1->setText(QString::number(tabEcartType.value(0),'f',2));
    this->ui->label_Critere1->setText("Critère (ppb) <");
    this->ui->lineEdit_Critere1->setText(QString::number(tabCritere.value(0),'f',2));
    this->ui->label_Valeur2->setText("Ecart-Type Cons =");
    this->ui->lineEdit_Valeur2->setText(QString::number(tabEcartType.value(1),'f',2));
    this->ui->label_Critere2->setText("Critère (ppb) <");
    this->ui->lineEdit_Critere2->setText(QString::number(tabCritere.value(1),'f',2));

}

et_Resultatpolluant::et_Resultatpolluant(QVector<float>tabConcentration,QVector<QVector<float> > tabMesPolluant,
                                         QVector<float>  tabMoyenne,QVector<float>  tabEcartType,QVector <float> tabValeurPourCritere,
                                         QVector<float> tabResidu,QVector<float> tabResiduRel,QVector<float> tabResiduInc,
                                         QVector<double> tabCritere,QWidget *parent):

    QWidget(parent),
    ui(new Ui::et_Resultatpolluant)
{
    ui->setupUi(this);
    creationColonneTab(tabConcentration);
    affichageTableauMesure(tabMesPolluant,"Mesure "," (ppb)");
    affichageTableauCalcul(tabMoyenne,"MOYENNE (ppb)");
    affichageTableauCalcul(tabEcartType,"ECART TYPE (ppb)");
    affichageTableauCalcul(tabResidu,"RESIDU (ppb)");
    affichageTableauCalcul(tabResiduRel,"RESIDU RELATIF (%)");
    affichageTableauCalcul(tabResiduInc,"RESIDU INCERTITUDE (%)");
    this->ui->tableWidget->setVerticalHeaderLabels(m_listeEnteteLigne);
    this->ui->tableWidget->resizeColumnsToContents();
    this->ui->tableWidget->resizeRowsToContents();
    this->ui->label_Valeur1->setText("Résidu Zéro (ppb) =");
    this->ui->lineEdit_Valeur1->setText(QString::number(tabValeurPourCritere.value(0),'f',2));
    this->ui->label_Critere1->setText("Critère Zéro (ppb) <");
    this->ui->lineEdit_Critere1->setText(QString::number(tabCritere.value(0),'f',2));
    this->ui->label_Valeur2->setText("Résidu Max (%) =");
    this->ui->lineEdit_Valeur2->setText(QString::number(tabValeurPourCritere.value(1),'f',2));
    this->ui->label_Critere2->setText("Critère Max (%) <");
    this->ui->lineEdit_Critere2->setText(QString::number(tabCritere.value(1),'f',2));
    this->ui->lineEditDroiteReg->setText("y = " + QString::number(tabValeurPourCritere.value(2),'f',3) +
                                         " x + " + QString::number(tabValeurPourCritere.value(3),'f',2));

}


et_Resultatpolluant::~et_Resultatpolluant()
{
    delete ui;
}

void et_Resultatpolluant::creationColonneTab (QVector<float> tabConcPolluant)
{
    QStringList listeEnteteColonne;
    for (int i =0; i < tabConcPolluant.size();i++){
        this->ui->tableWidget->insertColumn(i);
        listeEnteteColonne.append("Point :\n" + QString::number(tabConcPolluant.value(i))+" ppb");
    }
    this->ui->tableWidget->setHorizontalHeaderLabels(listeEnteteColonne);
}

void et_Resultatpolluant::creationColonneTabListe (QList<QString> listNomMolecule)
{
    QStringList listeEnteteColonne;
    for (int i =0; i < listNomMolecule.size();i++){
        this->ui->tableWidget->insertColumn(i);
        listeEnteteColonne.append(listNomMolecule.value(i));
    }
    this->ui->tableWidget->setHorizontalHeaderLabels(listeEnteteColonne);
}


void et_Resultatpolluant::affichageTableauMesure (QVector<QVector<float> > tabMesure,QString intituleLigne,QString uniteLigne)
{
    for (int i = 0;i <tabMesure.size(); i++){
        for (int j = 0;j <tabMesure[i].size(); j++){
            if (i==0){
                this->ui->tableWidget->insertRow(j);
                m_listeEnteteLigne.append(QString("%1").arg(intituleLigne) + QString::number(j+1) + QString("%2").arg(uniteLigne));
            }
            QTableWidgetItem* item = new QTableWidgetItem(QString::number(tabMesure[i].value(j),'f',2));         
            this->ui->tableWidget->setItem(j,i,item);
        }
    }
}

void et_Resultatpolluant::affichageTableauCalcul (QVector<float> tabCalcul,QString typeCalcul)
{
    int numeroDerniereLigne = this->ui->tableWidget->rowCount();
    this->ui->tableWidget->insertRow(numeroDerniereLigne);
    for (int i = 0;i <tabCalcul.size(); i++){
        if (!(tabCalcul.value(i)==-99) ){
            QTableWidgetItem* item = new QTableWidgetItem(QString::number(tabCalcul.value(i),'f',2));
            this->ui->tableWidget->setItem(numeroDerniereLigne,i,item);
        }
    }
    m_listeEnteteLigne.append(QString("%1").arg(typeCalcul));
}




