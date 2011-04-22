/*////////////////////////////////////////////////////
// \file ct_choixequipement.h
// \brief Classe d'interface graphique gérant le widget de choix d'equipement
// \author FOUQUART Christophe
// \version 1.0
// \date 01/04/2011
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
////////////////////////////////////////////////////*/

#ifndef CT_CHOIXEQUIPEMENT_H
#define CT_CHOIXEQUIPEMENT_H

#include <QWidget>
#include "test.h"

namespace Ui {
    class CT_ChoixEquipement;
}

class CT_ChoixEquipement : public QWidget
{
    Q_OBJECT

    bool m_editionEnCours; // Indique si un equipement est en cours d'édition
    QList<Equipement> m_listeEquipement; // Liste des equipements

public:
    explicit CT_ChoixEquipement(QWidget *parent = 0);
    ~CT_ChoixEquipement();

private:
    Ui::CT_ChoixEquipement *ui;

private slots:
    void pb_AjouterEquipementClicked();
    void pb_supprEquipementClicked();
    void pb_editEquipementClicked();
    void treeEquipementItemClicked();

signals:
    void ajouterEquipement(Equipement newEquipement);

};

#endif // CT_CHOIXEQUIPEMENT_H
