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

    connect(this->ui->tableView->selectionModel(),
            SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this,SLOT(changementSelection(const QModelIndex &)));
    connect(this->ui->button_Fermer,SIGNAL(clicked()),
            this,SLOT(buttonFermerClicked()));
    connect(this->ui->button_Selectionner,SIGNAL(clicked()),
            this,SLOT(buttonSelectionnerClicked()));
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

void Dlg_testXml::changementSelection(const QModelIndex & idxSelection)
{
    this->m_indexSelection = idxSelection;
}

void Dlg_testXml::buttonFermerClicked()
{
    this->reject();
}

void Dlg_testXml::buttonSelectionnerClicked()
{
    this->accept();
}

short Dlg_testXml::getIdSelection()
{
    if(m_indexSelection.isValid())
        return this->m_model->record(m_indexSelection.row()).value(TEST_XML_ID).toInt();
    else
        return -1;
}

QString Dlg_testXml::getFichierDescriptionSelection()
{
    if(m_indexSelection.isValid())
        return this->m_model->record(m_indexSelection.row()).value(TEST_XML_NOM_FICHIER).toString();
    else
        return "";
}
