/*////////////////////////////////////////////////////
// \file ct_choixequipement.cpp
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

#include "ct_choixequipement.h"
#include "ui_ct_choixequipement.h"

CT_ChoixEquipement::CT_ChoixEquipement(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CT_ChoixEquipement)
{
    ui->setupUi(this);
}

CT_ChoixEquipement::~CT_ChoixEquipement()
{
    delete ui;
}

void CT_ChoixEquipement::pb_AjouterEquipementClicked() {

}

void CT_ChoixEquipement::pb_supprEquipementClicked() {

}

void CT_ChoixEquipement::pb_editEquipementClicked() {

}

void CT_ChoixEquipement::treeEquipementItemClicked() {

}
