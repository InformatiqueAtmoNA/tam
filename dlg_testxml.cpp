/*////////////////////////////////////////////////////////////
// \file dlg_testxml.h
// \brief Classe d'interface graphique gerant la table testXML
// \author FOUQUART Christophe
// \version 1.0
// \date 25/03/2011
//
// TAM - Tests Automatiques Metrologiques
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

#include "dlg_testxml.h"
#include "ui_dlg_testxml.h"
#include <QCheckBox>

Dlg_testXml::Dlg_testXml(QWidget *parent,const QPointer<BdHandler> bdHandler,
                         const bool returnSelection) :
    QDialog(parent),
    ui(new Ui::Dlg_testXml)
{
    ui->setupUi(this);
    this->setWindowTitle("Description des tests");
    this->m_bdHandler = bdHandler;
    this->m_returnSelection = returnSelection;

    this->afficherTable();
    this->initialiserChamps();
    this->ui->testsSelectionnes->insertColumn(0);
    this->ui->testsSelectionnes->insertColumn(1);
    this->ui->testsSelectionnes->setColumnWidth(0,50);
    this->ui->testsSelectionnes->setColumnWidth(1,500);
    connect(this->ui->tableView->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(changementSelection(QModelIndex)));
    connect(this->ui->button_Fermer,SIGNAL(clicked()),this,SLOT(buttonFermerClicked()));
    connect(this->ui->button_Selectionner,SIGNAL(clicked()),this,SLOT(buttonSelectionnerClicked()));
    connect(this->ui->button_Confirmer,SIGNAL(clicked()),this,SLOT(buttonConfirmerClicked()));
    connect(this->ui->button_Supprimer,SIGNAL(clicked()),this,SLOT(buttonSupprimerClicked()));
    connect(this->ui->checkBoxFavoris,SIGNAL(stateChanged(int)),this,SLOT(filter(int)));
}

Dlg_testXml::~Dlg_testXml()
{
    delete ui;
}

void Dlg_testXml::afficherTable()
{
    if(!this->m_returnSelection)
       m_bdHandler->connexionBD();
    m_model = m_bdHandler->getTestXmlModel();
    this->ui->tableView->setModel(m_model);
    this->ui->tableView->setColumnHidden(TEST_XML_ID, true);
    this->ui->tableView->setColumnHidden(TEST_XML_EST_FAVORI, true);

    this->ui->tableView->resizeColumnsToContents(); 
}

void Dlg_testXml::initialiserChamps()
{
    this->ui->button_Fermer->setEnabled(true);
    this->ui->button_Selectionner->setEnabled(true);

    if(!this->m_returnSelection || this->m_model->rowCount()==0) {
        this->ui->button_Selectionner->setVisible(false);
    }
}

void Dlg_testXml::changementSelection(QModelIndex idxSelection)
{
    this->m_indexSelection = idxSelection;
}

void Dlg_testXml::buttonFermerClicked()
{
    this->reject();
}

void Dlg_testXml::buttonConfirmerClicked(){
    this->accept();
}

void Dlg_testXml::buttonSelectionnerClicked()
{
    for(int i=0; i<indexTestsSelectionne.count(); i++){
        if(this->m_model->record(m_indexSelection.row()).value(TEST_XML_ID_SYSTEME_ETALON).toInt()!=this->m_model->record(indexTestsSelectionne[i].row()).value(TEST_XML_ID_SYSTEME_ETALON).toInt()){
            QMessageBox msgBox;
            msgBox.setText(QLatin1String("Les tests choisis consécutivement doivent avoir le meme etalon"));
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.exec();
            return;
        }
    }

    this->indexTestsSelectionne.append(m_indexSelection);
    this->ui->testsSelectionnes->insertRow(this->indexTestsSelectionne.count()-1);
    this->ui->testsSelectionnes->setItem(this->indexTestsSelectionne.count()-1,0,new QTableWidgetItem(this->m_model->record(indexTestsSelectionne.last().row()).value(TEST_XML_ID).toString()));
    this->ui->testsSelectionnes->setItem(this->indexTestsSelectionne.count()-1,1,new QTableWidgetItem(this->m_model->record(indexTestsSelectionne.last().row()).value(TEST_XML_NOM_FICHIER).toString()));
}

void Dlg_testXml::buttonSupprimerClicked(){
    if(this->ui->testsSelectionnes->currentIndex().isValid()){
        indexTestsSelectionne.removeAt(this->ui->testsSelectionnes->currentIndex().row());
        this->ui->testsSelectionnes->removeRow(this->ui->testsSelectionnes->currentIndex().row());
    }
}

void Dlg_testXml::filter(int etatCheckBox)
{
    if(etatCheckBox == 2)
    {
        for(int i=0 ; i<= this->ui->tableView->model()->rowCount(); i++)
        {
            QSqlRecord recordTestXml = this->m_model->record(i);
            QString estFavori = recordTestXml.value(TEST_XML_EST_FAVORI).toString();

            if(estFavori=="NON")
            {
                this->ui->tableView->setRowHidden(i,true);
            }
        }
    }
    else
    {
        for(int i=0 ; i<= this->ui->tableView->model()->rowCount(); i++)
        {
            this->ui->tableView->showRow(i);
        }
    }
}


QList<ushort> Dlg_testXml::getIdSelection()
{
    QList<ushort> idTestsSelectionne;
    QList<ushort> Probleme;
    Probleme.append(0);

    if(indexTestsSelectionne.isEmpty()){
        return  Probleme;
    }

    for(int i=0; i<indexTestsSelectionne.count() ; i++){
        if(indexTestsSelectionne[i].isValid()){
            idTestsSelectionne.append(this->m_model->record(indexTestsSelectionne[i].row()).value(TEST_XML_ID).toInt());          
        }
        else{
            return  Probleme;
        }
    } 
    return idTestsSelectionne;
}

QList<QString> Dlg_testXml::getFichierDescriptionSelection()
{

    QList<QString> nomTestsSelectionne;
    QList<QString> Probleme;
    Probleme.append(0);

    if(indexTestsSelectionne.isEmpty()){
        return  Probleme;
    }

    for(int i=0; i<indexTestsSelectionne.count() ; i++){
        if(indexTestsSelectionne[i].isValid()){
            nomTestsSelectionne.append(this->m_model->record(indexTestsSelectionne[i].row()).value(TEST_XML_NOM_FICHIER).toString());
        }
        else{
            return Probleme;
        }
    }
    return nomTestsSelectionne;

}
