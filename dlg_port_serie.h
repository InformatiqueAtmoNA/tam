/*////////////////////////////////////////////////////////////
// \file dlg_port_serie.h
// \brief Classe d'interface graphique de la fen�tre port serie
// \author PIET R�gis
// \version 1.0
// \date 11/09/2011
//
// TAM - Tests Automatiques M�trologiques
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

#ifndef DLG_PORT_SERIE_H
#define DLG_PORT_SERIE_H

#include <QDialog>
#include "bdhandler.h"
#include "definitions_globales.h"

namespace Ui {
    class Dlg_Port_Serie;
}

class Dlg_Port_Serie : public QDialog {
    Q_OBJECT
public:
    Dlg_Port_Serie(QWidget *parent = 0,const QPointer<BdHandler> bdHandler = new BdHandler(),
                        const bool returnSelection=false, const int indexSelection =-1);
    ~Dlg_Port_Serie();

    int getIdSelection();
protected:
    void changeEvent(QEvent *e);

private:
    Ui::Dlg_Port_Serie *ui;
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
};

#endif // DLG_PORT_SERIE_H
