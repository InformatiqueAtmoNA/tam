/*///////////////////////////////////////////////////////////////////////////////////
// \file bdhandler.h
// \brief Classe g�rant l'acces a la base de donnees
// \author FOUQUART Christophe
// \version 1.0
// \date 07/04/2011
//
// TAM - Tests Automatiques M�trologiques
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

#ifndef BDHANDLER_H
#define BDHANDLER_H

#include "definitions_globales.h"

class BdHandler : public QObject
{
protected :
    QSqlDatabase m_baseMySql;

public:
    BdHandler() {}
    BdHandler(const QString & driver,const QString & host,const QString & username,
              const QString & password, const QString & dbName);
    bool connexionBD();
    void deconnexionBD();
    QPointer<QSqlRelationalTableModel> getEquipementModel();
    QPointer<QSqlRelationalTableModel> getModelesModel();
    QPointer<QSqlRelationalTableModel> getSystemeEtalonModel();
    QPointer<QSqlRelationalTableModel> getPolluantAssocieModel(const int idEquipement);
    QPointer<QSqlTableModel> getSystemeEtalonModelSansRelation();
    QPointer<QSqlTableModel> getMoleculesModel();
    QPointer<QSqlTableModel> getProtocolesModel();
    QPointer<QSqlTableModel> getMarquesModel();
    QPointer<QSqlTableModel> getTxTransmissionModel();
    QPointer<QSqlTableModel> getConcentrationModel(const int idSystemeEtalon);

    bool isOpen();
};

#endif // BDHANDLER_H
