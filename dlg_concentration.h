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

namespace Ui {
    class Dlg_Concentration;
}

class Dlg_Concentration : public QDialog
{
    Q_OBJECT

public:
    explicit Dlg_Concentration(QWidget *parent,const QPointer<BdHandler> bdHandler,
                               const int idSystemeEtalon, const bool returnSelection=false,
                               const int indexSelection =-1);
    ~Dlg_Concentration();

    int getIdSelection();
private:
    Ui::Dlg_Concentration *ui;

    QPointer<QSqlTableModel> m_model;
    QModelIndex m_indexSelection;
    QPointer<BdHandler> m_bdHandler;
    QString m_noSerieEquipemenent;
    int m_idSystemeEtalon;
    int m_ligneModifiee;
    bool m_modifEnCours;
    bool m_returnSelection;

    void peuplerTable();
    void afficherFormulaire();

private slots:
    void changementSelection(const QModelIndex & idxSelection);
    void buttonSupprimerClicked();
    void buttonAjouterClicked();
    void initialiserChamps();
    void buttonValiderClicked();
    void buttonFermerClicked();
    void buttonSelectionnerClicked();
};


#endif // DLG_CONCENTRATION_H
