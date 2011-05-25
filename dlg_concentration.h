/*////////////////////////////////////////////////////////////
// \file dlg_concentration.h
// \brief Classe d'interface graphique  gérant la table Cocentration
// \author FOUQUART Christophe
// \version 1.0
// \date 20/04/2011
//
// TAM - Tests Automatiques Métrologiques
// Copyright (C) 2011 FOUQUART Christophe
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

#ifndef DLG_CONCENTRATION_H
#define DLG_CONCENTRATION_H

#include <QDialog>
#include "bdhandler.h"
#include "dlg_equipement.h"

namespace Ui {
    class Dlg_Concentration;
}

class Dlg_Concentration : public QDialog
{
    Q_OBJECT

public:
    explicit Dlg_Concentration(QWidget *parent,const QPointer<BdHandler> bdHandler,
                               const int idSystemeEtalon, const bool filtrerRdf = false, const bool returnSelection=false,
                               const int indexSelection =-1);
    ~Dlg_Concentration();

    inline int getIdSelection() {return this->m_model->record(m_indexSelection.row()).value(CONCENTRATION_ID).toInt();}
private:
    Ui::Dlg_Concentration *ui;

    QPointer<QSqlTableModel> m_model;
    QModelIndex m_indexSelection;
    QModelIndex m_indexConcAssocieSelection;
    QPointer<BdHandler> m_bdHandler;
    QPointer<QSqlQueryModel> m_modelPolluants;
    QPointer<QSqlTableModel> m_modelMolecule;
    QPointer<QSqlRelationalTableModel> m_modelConcentrationAssociee;

    QString m_noSerieEquipemenent;
    QWidget* m_pageConcentrationAssociee;

    int m_idSystemeEtalon;
    ushort m_idPolluant;
    ushort m_idMoleculeConcentrationAssocie;
    bool m_modifEnCours;
    bool m_modifConcAssocieeEnCours;
    bool m_returnSelection;
    bool m_filtrerRdf;

    void peuplerCbPolluant();
    void peuplerTable();
    void afficherFormulaire();
    void afficherFormulaireConcAssociee();
    void setGbEditChampsReadOnly(const bool readOnly=true);
    void setGbEditChampsConcAssocieeReadOnly(const bool readOnly=true);
    void editCurrentRow(const QModelIndex & idxSelection);
    void editConcAssocieeCurrentRow(const QModelIndex & idxSelection);

private slots:
    inline void tableViewItemDoubleClicked(const QModelIndex & idxSelection) {this->editCurrentRow(idxSelection);}
    inline void buttonFermerClicked() {this->m_model->submitAll();this->reject();}
    inline void buttonSelectionnerClicked() {this->accept();}

    void tableViewSelectedItemChanged(const QModelIndex & idxSelection);
    void buttonAjouterClicked();
    void buttonSupprimerClicked();
    void initialiserChamps();
    void initialiserChampsConcAssociee();
    void buttonModifierClicked();
    void buttonValiderClicked();
    void cbSelectPolluantIndexChanged(const int index);
    void cbConcAssocieeSelectPolluantIndexChanged(const int index);
    void tabWidgetIndexChanged(const int index);

    void tableViewConcAssocieeItemChanged(const QModelIndex & idxSelection);
    void buttonSupprimerConcAssocieeClicked();
    void buttonModifierConcAssocieeClicked();
    void buttonAjouterConcAssocieeClicked();
    void buttonValiderConcAssocieeClicked();
};


#endif // DLG_CONCENTRATION_H
