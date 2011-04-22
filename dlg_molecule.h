/*///////////////////////////////////////////////////////////////////////////////////
// \file dlg_molecule.h
// \brief Classe d'interface gérant la table Molecule
// \author FOUQUART Christophe
// \version 1.0
// \date 07/04/2011
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
///////////////////////////////////////////////////////////////////////////////////*/

#ifndef DLG_MOLECULE_H
#define DLG_MOLECULE_H

#include <QDialog>
#include "bdhandler.h"

namespace Ui {
    class Dlg_Molecule;
}

class Dlg_Molecule : public QDialog
{
    Q_OBJECT

public:
    explicit Dlg_Molecule(QWidget *parent = 0,const QPointer<BdHandler> bdHandler = new BdHandler());
    ~Dlg_Molecule();

private:
    Ui::Dlg_Molecule *ui;
    QPointer<QSqlTableModel> model;
    QModelIndex indexSelection;
    QPointer<BdHandler> bdHandler;

    void afficherTable();

private slots:
    void changementSelection(const QModelIndex & idxSelection);
    void buttonSupprimerClicked();
    void buttonAjouterClicked();
    void initialiserChamps();
    void buttonValiderClicked();
};

#endif // DLG_MOLECULE_H
