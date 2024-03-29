/*///////////////////////////////////////////////////////////////////////////////////
// \file bdhandler.h
// \brief Classe g�rant l'acces a la base de donnees
// \author FOUQUART Christophe
// \version 1.0
// \date 07/04/2011
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
///////////////////////////////////////////////////////////////////////////////////*/

#ifndef BDHANDLER_H
#define BDHANDLER_H

#include "definitions_globales.h"
#include "spanhandler.h"

#include <QAuthenticator>

class BdHandler : public QObject
{
    Q_OBJECT

protected :
    QSqlDatabase m_baseMySql;

    QSqlRecord* getTableRow(QString sqlTableRequete);
public:
    BdHandler() {}
    BdHandler(const QString & driver,const QString & host,const QString & username,
              const QString & password, const QString & dbName);
    bool connexionBD();
    void deconnexionBD();
    bool isOpen();

    QPointer<QSqlRelationalTableModel> getEquipementModel();
    QPointer<QSqlRelationalTableModel> getModelesModel();
    QPointer<QSqlRelationalTableModel> getSystemeEtalonModel(const uint idSystemeEtalon=0);
    QPointer<QSqlRelationalTableModel> getPolluantAssocieModel(const uint idEquipement);
    QPointer<QSqlRelationalTableModel> getConcentrationAssocieeModel(const uint idConcentration=0);
    QPointer<QSqlQueryModel> getTestRapportModel(QList<QString> liste_filtres);
    QPointer<QSqlQueryModel> getEquipementFiltreParModele(const QString & filtre);
    QPointer<QSqlQueryModel> getPolluantsParSystemeEtalon(const uint idSystemeEtalon,const bool filtrerRdf=false);
    QPointer<QSqlTableModel> getSystemeEtalonModelSansRelation();
    QPointer<QSqlTableModel> getMoleculesModel();
    QPointer<QSqlQueryModel> getMoleculesModel(const uint idmoleculeFiltre);
    QPointer<QSqlTableModel> getProtocolesModel();
    QPointer<QSqlTableModel> getMarquesModel();
    QPointer<QSqlTableModel> getTxTransmissionModel();
    QPointer<QSqlTableModel> getNoPortSerie();
    QPointer<QSqlTableModel> getTestXmlModel(const uint idTestXml=0);
    QPointer<QSqlTableModel> getConcentrationModel(const uint idSystemeEtalon,const uint idPolluant);
    QPointer<QSqlTableModel> getConcentrationAssocieeModel();
    QPointer<QSqlTableModel> getConcentrationAssocieeModelRow(const uint idConcentration);
    QPointer<QSqlTableModel> getOperateurModel();
    QPointer<QSqlTableModel> getLieuModel();
    QPointer<QSqlTableModel> getTestMetroModel();
    QPointer<QSqlTableModel> getListeAnalyseurTestModel();
    QPointer<QSqlTableModel> getConcTestMetro();
    QSqlRecord* getConcentrationRow(const uint idConcentration);
    QSqlRecord* getConcentrationRow(const uint idSystemeEtalon, const uint idMolecule, const uint pointGaz);
    QSqlRecord* getMoleculeRow(const uint idMolecule);
    QSqlRecord* getEquipementRow(const ushort idEquipement);
    QSqlRecord* getSystemeEtalonRow(const ushort idSystemeEtalon);
    QSqlRecord* getEquipementModeledRow(const ushort idEquipement);
    QSqlRecord* getOperateurRow(const QString username);
    QString getEquipementNumSerie(const ushort idEquipement);
    QString getTestFavoriteState(const ushort idTest);
    ushort getTxTransmission(const ushort idTxTransmission);
    QSqlRecord* getDesignationPortSerie(const ushort idEquipement);
    QSqlRecord* getInformationsTest(const ushort idTest);
    QPointer<QSqlQueryModel> getTestPhaseConcentration (const ushort idTest,const ushort idmolecule);
    QPointer<QSqlQueryModel> getTestPhase (const ushort idTest);
    QPointer<QSqlQueryModel> getPolluantTestConcentration (const ushort idTest);
    QPointer<QSqlQueryModel> getCyclePhaseTest (const ushort idTest);
    QPointer<QSqlQueryModel> getMesureTestAnalyseur (const ushort idTest , const ushort idEquipement , const ushort nbMesureEquipement,const ushort noPhase,const ushort noCycle);
    DesignationProtocole getDesignationProtocole(const ushort idEquipement);
    QPointer<QSqlQueryModel> getModeleAnalyseur();
    QPointer<QSqlQueryModel> getListeEquipementParModele(const uint id_Modele);
    QPointer<QSqlQueryModel> getListeTypeTestParEquipement(const uint id_Equipement);
    QPointer<QSqlQueryModel> getListeDateTestParEquipParTypeTest(const uint id_Equipement,const QString & id_TypeTest);
    ushort getIdCalibrateur(const uint idSystemeEtalon);
    QString getEquipementTypeConnexion(const ushort idEquipement);
    QString getEquipementTypeSocket(const ushort idEquipement);
    QPointer<QStandardItemModel> getItemModelListeRapport();

    bool miseAjourDateHeureFinTest(const ushort idTestMetro);
    bool miseAjourTemperaturesFinTest(float tempMin,float tempMax, float tempMoyenne, int idTest);
    bool insertIntoMesure(const MesureInfo mesuresInfos);
    void setSpanHandlerFromIdConcentration(ushort idConcentration, QString canal, SpanHandler* spanHandler);

    void ValiderTest(const uint idTest,QAuthenticator aUser, ushort idAnalyseur);
    void InvaliderTest(const uint idTest, QAuthenticator aUser, ushort idAnalyseur);
    QList<QString>* getValidation(const ushort idTest,const ushort idAnalyseur);

Q_SIGNALS:
    void afficherTrace(QString trace);
};

#endif // BDHANDLER_H
