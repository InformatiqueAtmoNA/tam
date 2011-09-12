/*////////////////////////////////////////////////////////////
// \file dlg_lieu.h
// \brief Classe d'interface graphique gérant la table Lieu
// \author FOUQUART Christophe
// \version 1.0
// \date 31/08/2011
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
#ifndef DLG_LIEU_H
#define DLG_LIEU_H

#include <QDialog>
#include "bdhandler.h"

namespace Ui {
    class Dlg_Lieu;
}

class Dlg_Lieu : public QDialog {
    Q_OBJECT
public:
    Dlg_Lieu(QWidget *parent = 0,const QPointer<BdHandler> bdHandler = new BdHandler(),
                        const bool returnSelection=false, const int indexSelection =-1);
    ~Dlg_Lieu();
    int getIdSelection();
private:
    Ui::Dlg_Lieu *ui;
    QPointer<QSqlTableModel> m_model;
    QModelIndex m_indexSelection;
    QPointer<BdHandler> m_bdHandler;
    bool m_returnSelection;

    void afficherTable();

private Q_SLOTS:
    void changementSelection(const QModelIndex & idxSelection);
    void buttonSupprimerClicked();
    void buttonAjouterClicked();
    void initialiserChamps();
    void buttonValiderClicked();
    void buttonFermerClicked();
    void buttonSelectionnerClicked();

protected:
    void changeEvent(QEvent *e);

};

#endif // DLG_LIEU_H
