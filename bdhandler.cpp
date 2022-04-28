/*///////////////////////////////////////////////////////////////////////////////////
// \file bdhandler.cpp
// \brief Classe gerant l'acces a la base de donnees
// \author FOUQUART Christophe
// \version 1.0
// \date 07/04/2011
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
///////////////////////////////////////////////////////////////////////////////////*/

#include "bdhandler.h"

BdHandler::BdHandler(const QString & driver,const QString & host,const QString & username,
              const QString & password, const QString & dbName)
{
    m_baseMySql = QSqlDatabase::addDatabase(driver);
    m_baseMySql.setHostName(host);
    m_baseMySql.setUserName(username);
    m_baseMySql.setPassword(password);
    m_baseMySql.setDatabaseName(dbName);
}

bool BdHandler::connexionBD()
{
    if(!m_baseMySql.isOpen())
        m_baseMySql.open();

    if(!m_baseMySql.isOpen())
    {
        emit(afficherTrace(QObject::tr("Database Error ")+m_baseMySql.lastError().text()));
        return false;
    }
    emit(afficherTrace("Connexion effectuee"));
    return true;
}

void BdHandler::deconnexionBD()
{
    if(m_baseMySql.isOpen())
        m_baseMySql.close();
}

bool BdHandler::isOpen()
{
    return this->m_baseMySql.isOpen();
}

QPointer<QSqlRelationalTableModel> BdHandler::getEquipementModel()
{
    QPointer<QSqlRelationalTableModel> model = new QSqlRelationalTableModel(this);
    model->setTable("Equipement");
    model->setRelation(EQUIPEMENT_ID_MODELE, QSqlRelation("Modele_Equipement", "id_modele", "me_designation"));
    model->setRelation(EQUIPEMENT_ID_TX_TRANSMISSION, QSqlRelation("Taux_Transmission","id_tx_transmission","taux_transmission"));
    model->setRelation(EQUIPEMENT_PORT,QSqlRelation("Port_Serie","no_port","designation"));

    model->setHeaderData(EQUIPEMENT_NO_SERIE, Qt::Horizontal, "N° Serie");
    model->setHeaderData(EQUIPEMENT_ID_MODELE, Qt::Horizontal, "Modèle");
    model->setHeaderData(EQUIPEMENT_ID_TX_TRANSMISSION, Qt::Horizontal, "Taux transmission");
    model->setHeaderData(EQUIPEMENT_EN_SERVICE, Qt::Horizontal, "En service ?");
    model->setHeaderData(EQUIPEMENT_MAX_GAMME, Qt::Horizontal, "Max. Gamme");
    model->setHeaderData(EQUIPEMENT_MIN_GAMME, Qt::Horizontal, "Min. Gamme");
    model->setHeaderData(EQUIPEMENT_OFFSET, Qt::Horizontal, "Offset");
    model->setHeaderData(EQUIPEMENT_ADRESSE, Qt::Horizontal, "Adresse");
    model->setHeaderData(EQUIPEMENT_CONTROLE_FLUX, Qt::Horizontal, "Contrôle de flux");
    model->setHeaderData(EQUIPEMENT_NB_BITS_STOP, Qt::Horizontal, "Nb. bits de stop");
    model->setHeaderData(EQUIPEMENT_NB_BITS_TRANSMISSION, Qt::Horizontal, "Nb. bits de transmission");
    model->setHeaderData(EQUIPEMENT_PARITE, Qt::Horizontal, "Parite");
    //model->setHeaderData(EQUIPEMENT_OFFSET, Qt::Horizontal, "Offset");
    model->setHeaderData(EQUIPEMENT_PORT, Qt::Horizontal, "Port Serie");
    model->setSort(EQUIPEMENT_ID, Qt::AscendingOrder);
    if(!model->select())
        emit(afficherTrace(model->lastError().text()));

    return model;
}

QPointer<QSqlRelationalTableModel> BdHandler::getConcentrationAssocieeModel(const uint idConcentration)
{
    QPointer<QSqlRelationalTableModel> model = new QSqlRelationalTableModel(this);
    model->setTable("Concentration_Associee");
    model->setRelation(CONC_ASSOCIEE_FORMULE, QSqlRelation("Molecule", "id_molecule", "formule"));

    model->setHeaderData(CONC_ASSOCIEE_FORMULE, Qt::Horizontal, "Molecule");
    model->setHeaderData(CONC_ASSOCIEE_CONCENTRATION, Qt::Horizontal, "Concentration");
    model->setSort(EQUIPEMENT_ID, Qt::AscendingOrder);
    if(idConcentration>0)
        model->setFilter(QString("id_concentration=%1").arg(idConcentration));
    if(!model->select())
        emit(afficherTrace(model->lastError().text()));

    return model;
}

QPointer<QSqlTableModel> BdHandler::getMoleculesModel()
{
    QPointer<QSqlTableModel> model = new QSqlTableModel(0,m_baseMySql);
    model->setTable("Molecule");
    model->setSort(MOLECULE_ID, Qt::AscendingOrder);
    if(!model->select())
        emit(afficherTrace(model->lastError().text()));

    return model;
}

QPointer<QSqlQueryModel> BdHandler::getMoleculesModel(const uint idmoleculeFiltre)
{
    QPointer<QSqlQueryModel> model = new QSqlQueryModel();

    QString query = QString("SELECT * FROM Molecule WHERE id_Molecule <> %1").arg(QString::number(idmoleculeFiltre));
    model->setQuery(query,m_baseMySql);
    model->setHeaderData(MOLECULE_CODE,Qt::Horizontal,"Code");
    model->setHeaderData(MOLECULE_FORMULE,Qt::Horizontal,"Formule");
    model->setHeaderData(MOLECULE_NOM,Qt::Horizontal,"Nom");

    return model;
}

QPointer<QSqlTableModel> BdHandler::getProtocolesModel()
{
    QPointer<QSqlTableModel> model = new QSqlTableModel(0,m_baseMySql);
    model->setTable("Protocole");
    model->setSort(PROTOCOLE_ID, Qt::AscendingOrder);
    if(!model->select())
        emit(afficherTrace(model->lastError().text()));

    return model;
}

QPointer<QSqlTableModel> BdHandler::getMarquesModel()
{
    QPointer<QSqlTableModel> model = new QSqlTableModel(0,m_baseMySql);
    model->setTable("Marque_Equipement");
    model->setSort(MARQUE_ID, Qt::AscendingOrder);
    if(!model->select())
        emit(afficherTrace(model->lastError().text()));

    return model;
}

QPointer<QSqlTableModel> BdHandler::getTxTransmissionModel()
{
    QPointer<QSqlTableModel> model = new QSqlTableModel(0,m_baseMySql);
    model->setTable("Taux_Transmission");
    model->setSort(TX_TRANSMISSION_DESIGNATION, Qt::AscendingOrder);
    if(!model->select())
        emit(afficherTrace(model->lastError().text()));

    return model;
}

QPointer<QSqlTableModel> BdHandler::getNoPortSerie()
{
    QPointer<QSqlTableModel> model = new QSqlTableModel(0,m_baseMySql);
    model->setTable("Port_Serie");
    model->setSort(PORT_SERIE_DESIGNATION, Qt::AscendingOrder);
    if(!model->select())
        emit(afficherTrace(model->lastError().text()));

    return model;
}

QPointer<QSqlTableModel> BdHandler::getTestXmlModel(const uint idTestXml)
{
    QPointer<QSqlTableModel> model = new QSqlTableModel(0,m_baseMySql);
    model->setTable("Test_XML");
    if(idTestXml>0)
        model->setFilter(QString("id_Test_Xml=%1").arg(idTestXml));

    model->setSort(TEST_XML_ID, Qt::AscendingOrder);
    if(!model->select())
        emit(afficherTrace(model->lastError().text()));

    return model;
}

QPointer<QSqlRelationalTableModel> BdHandler::getModelesModel()
{
    QPointer<QSqlRelationalTableModel> model = new QSqlRelationalTableModel(0,m_baseMySql);

    model->setTable("Modele_Equipement");
    model->setRelation(MODELE_ID_MARQUE,QSqlRelation("Marque_Equipement","id_marque","designation"));
    model->setRelation(MODELE_ID_PROTOCOLE,QSqlRelation("Protocole","id_Protocole","designation"));
    model->setHeaderData(MODELE_ID_MARQUE, Qt::Horizontal, "Marque");
    model->setHeaderData(MODELE_ID_PROTOCOLE, Qt::Horizontal, "Protocole");
    model->setHeaderData(MODELE_DESIGNATION, Qt::Horizontal, "Designation");
    model->setSort(MODELE_ID, Qt::AscendingOrder);
    if(!model->select())
        emit(afficherTrace(model->lastError().text()));

    return model;
}

QPointer<QSqlRelationalTableModel> BdHandler::getSystemeEtalonModel(const uint idSystemeEtalon)
{
    QPointer<QSqlRelationalTableModel> model = new QSqlRelationalTableModel(0,m_baseMySql);

    model->setTable("Systeme_Etalonnage");
    model->setRelation(SYS_ETALON_DILUTEUR,QSqlRelation("Equipement","id_equipement","numero_serie"));
    model->setRelation(SYS_ETALON_BOUTEILLE,QSqlRelation("Equipement","id_equipement","numero_serie"));
    model->setRelation(SYS_ETALON_GZERO,QSqlRelation("Equipement","id_equipement","numero_serie"));
    model->setHeaderData(SYS_ETALON_DILUTEUR, Qt::Horizontal, "Diluteur ou Generateur O3");
    model->setHeaderData(SYS_ETALON_BOUTEILLE, Qt::Horizontal, "Bouteille");
    model->setHeaderData(SYS_ETALON_GZERO, Qt::Horizontal, "Generateur air zero");
    model->setHeaderData(SYS_CANAL_DEFAUT, Qt::Horizontal, "Canal par defaut");
    model->setSort(SYS_ETALON_ID, Qt::AscendingOrder);

    if(idSystemeEtalon>0)
        model->setFilter(QString("id_systeme_etalon=%1").arg(idSystemeEtalon));

    if(!model->select())
        emit(afficherTrace(model->lastError().text()));

    emit(afficherTrace("call : BdHandler::getSystemeEtalonModel() rowCount = "+ QString::number(model->rowCount())));

    return model;
}

QPointer<QSqlQueryModel> BdHandler::getTestRapportModel()
{
    QString requete(QString("SELECT T.id_Test,E.id_equipement,E.numero_serie,T.test_metro_type_test,T.date_debut FROM Equipement E , Liste_Analyseurs_Test L , Test_Metrologique T WHERE L.id_test = T.id_test AND E.id_equipement = L.id_equipement order by T.date_debut DESC LIMIT 20"));

    QPointer<QSqlQueryModel> model = new QSqlQueryModel;
    model->setQuery(requete,m_baseMySql);
    model->setHeaderData(HOMEW_TABVIEW_TEST_ID_TEST, Qt::Horizontal, tr("Numero Test"));
    model->setHeaderData(HOMEW_TABVIEW_TEST_ID_EQUIP, Qt::Horizontal, tr("id_equipement"));
    model->setHeaderData(HOMEW_TABVIEW_TEST_NO_EQUIP, Qt::Horizontal, tr("Numero Equipement"));
    model->setHeaderData(HOMEW_TABVIEW_TEST_TYPE_TEST, Qt::Horizontal, tr("Type de Test"));
    model->setHeaderData(HOMEW_TABVIEW_TEST_DATE, Qt::Horizontal, tr("Date de Debut"));
    return model;
}

QPointer<QSqlQueryModel> BdHandler::getEquipementFiltreParModele(const QString & filtre) {
    QString requete(QString("SELECT E.id_equipement,E.id_modele,M.type FROM Equipement E, Modele_Equipement M WHERE E.id_modele=M.id_modele AND (%1) ORDER BY E.id_equipement ASC").arg(filtre));

    QPointer<QSqlQueryModel> model = new QSqlQueryModel;
    model->setQuery(requete,m_baseMySql);

    emit(afficherTrace("getEquipementFiltreParModele rowCount " + QString::number(model->rowCount())));

    return model;
}

QPointer<QSqlQueryModel> BdHandler::getModeleAnalyseur() {
    QString requete(QString("SELECT ME.id_Modele,ME.me_designation FROM Modele_Equipement ME WHERE ME.type = 'ANALYSEUR' ORDER BY ME.me_designation ASC"));

    QPointer<QSqlQueryModel> model = new QSqlQueryModel;
    model->setQuery(requete,m_baseMySql);

    emit(afficherTrace("getModeleAnalyseur rowCount " + QString::number(model->rowCount())));

    return model;
}


QPointer<QSqlQueryModel> BdHandler::getListeEquipementParModele(const uint id_Modele) {
    QString requete(QString("SELECT E.id_equipement,E.numero_serie FROM Equipement E WHERE E.id_modele=%1 ORDER BY E.id_equipement ASC").arg(id_Modele));

    QPointer<QSqlQueryModel> model = new QSqlQueryModel;
    model->setQuery(requete,m_baseMySql);

    emit(afficherTrace("getListeEquipementParModele rowCount " + QString::number(model->rowCount())));

    return model;
}

QPointer<QSqlQueryModel> BdHandler::getListeTypeTestParEquipement(const uint id_Equipement) {
    QString requete = "SELECT TM.test_metro_type_test FROM Test_Metrologique TM, Liste_Analyseurs_Test LAM ";
    requete.append(QString("WHERE TM.id_Test = LAM.id_test and LAM.id_equipement =%1 ").arg(id_Equipement));
    requete.append("GROUP BY TM.test_metro_type_test");

    QPointer<QSqlQueryModel> model = new QSqlQueryModel;
    model->setQuery(requete,m_baseMySql);

    emit(afficherTrace("getListeTypeTestParEquipement rowCount " + QString::number(model->rowCount())));

    return model;
}

QPointer<QSqlQueryModel> BdHandler::getListeDateTestParEquipParTypeTest(const uint id_Equipement,const QString & id_TypeTest) {
    QString requete = "SELECT TM.id_test,TM.date_debut FROM Test_Metrologique TM, Liste_Analyseurs_Test LAM ";
    requete.append(QString("WHERE TM.id_test = LAM.id_test and LAM.id_Equipement = %1 ").arg(id_Equipement));
    requete.append(QString("AND TM.test_metro_type_test = '%1' ").arg(id_TypeTest));
    requete.append("ORDER BY TM.date_debut desc");
    QPointer<QSqlQueryModel> model = new QSqlQueryModel;
    model->setQuery(requete,m_baseMySql);

    emit(afficherTrace("getListeTypeTestParEquipement rowCount " + QString::number(model->rowCount())));

    return model;
}

QPointer<QSqlTableModel> BdHandler::getSystemeEtalonModelSansRelation()
{
    QPointer<QSqlTableModel> model = new QSqlTableModel(0,m_baseMySql);
    model->setTable("Systeme_Etalonnage");
    model->setSort(SYS_ETALON_ID, Qt::AscendingOrder);
    if(!model->select())
        emit(afficherTrace(model->lastError().text()));

    return model;
}

QPointer<QSqlTableModel> BdHandler::getConcentrationModel(const uint idSystemeEtalon,const uint idPolluant)
{
    QPointer<QSqlTableModel> model = new QSqlTableModel(0,m_baseMySql);
    model->setTable("Concentration");
    QString filtre = QString("id_systeme_etalon = %1 AND id_molecule = %2").arg(QString::number(idSystemeEtalon),QString::number(idPolluant));
    model->setFilter(filtre);
    model->setSort(CONCENTRATION_POINT, Qt::AscendingOrder);
    model->setHeaderData(CONCENTRATION_POINT, Qt::Horizontal, "Point consigne");
    model->setHeaderData(CONCENTRATION_REELLE, Qt::Horizontal, "Concentration reelle");
    model->setHeaderData(CONCENTRATION_OZONE, Qt::Horizontal, "Concentration O3");
    if(!model->select())
        emit(afficherTrace(model->lastError().text()));

    emit(afficherTrace("Concentration model rowcount()="+ QString::number(model->rowCount())));
    emit(afficherTrace("filtre : "+filtre));

    return model;
}

QPointer<QSqlTableModel> BdHandler::getConcentrationAssocieeModel()
{
    QPointer<QSqlTableModel> model = new QSqlTableModel(0,m_baseMySql);
    model->setTable("Concentration_Associee");

    model->setSort(CONC_ASSOCIEE_ID, Qt::AscendingOrder);

    if(!model->select())
        emit(afficherTrace(model->lastError().text()));

    emit(afficherTrace("Concentration_Associee model rowcount()="+ QString::number(model->rowCount())));

    return model;
}

QPointer<QSqlTableModel> BdHandler::getConcentrationAssocieeModelRow(const uint idConcentration)
{
    QPointer<QSqlTableModel> model = new QSqlTableModel(0,m_baseMySql);
    model->setTable("Concentration_Associee");
    QString filtre = QString("id_concentration = %1").arg(QString::number(idConcentration));
    model->setFilter(filtre);
    model->setSort(CONC_ASSOCIEE_ID, Qt::AscendingOrder);

    if(!model->select())
        emit(afficherTrace(model->lastError().text()));

    emit(afficherTrace("Concentration_Associee model rowcount()="+ QString::number(model->rowCount())));

    return model;
}

QPointer<QSqlTableModel> BdHandler::getOperateurModel()
{
    QPointer<QSqlTableModel> model = new QSqlTableModel(0,m_baseMySql);
    model->setTable("Operateur");

    model->setSort(OPERATEUR_ID, Qt::AscendingOrder);

    if(!model->select())
        emit(afficherTrace(model->lastError().text()));

    emit(afficherTrace("Operateur model rowcount()=" + QString::number(model->rowCount())));

    return model;
}

QPointer<QSqlTableModel> BdHandler::getLieuModel()
{
    QPointer<QSqlTableModel> model = new QSqlTableModel(0,m_baseMySql);
    model->setTable("Lieu");

    model->setSort(LIEU_ID, Qt::AscendingOrder);

    if(!model->select())
        emit(afficherTrace(model->lastError().text()));

    emit(afficherTrace("Lieu model rowcount()=" + QString::number(model->rowCount())));

    return model;
}

QPointer<QSqlTableModel> BdHandler::getTestMetroModel()
{
    QPointer<QSqlTableModel> model = new QSqlTableModel(0,m_baseMySql);
    model->setTable("Test_Metrologique");

    model->setSort(TEST_METRO_ID, Qt::AscendingOrder);

    if(!model->select())
        emit(afficherTrace(model->lastError().text()));

    emit(afficherTrace("Test_Metro model rowcount()=" + QString::number(model->rowCount())));

    return model;
}


 QPointer<QSqlTableModel> BdHandler:: getListeAnalyseurTestModel()
 {
     QPointer<QSqlTableModel> model = new QSqlTableModel(0,m_baseMySql);
     model->setTable("Liste_Analyseurs_Test");

     model->setSort(LISTE_ANA_TEST_ID_TEST, Qt::AscendingOrder);

     if(!model->select())
         emit(afficherTrace(model->lastError().text()));

     emit(afficherTrace("Liste_Analyseurs_Test model rowcount()=" + QString::number(model->rowCount())));

     return model;
 }


 QPointer<QSqlTableModel> BdHandler:: getConcTestMetro()
 {
     QPointer<QSqlTableModel> model = new QSqlTableModel(0,m_baseMySql);
     model->setTable("Concentration_Test_Metro");

     model->setSort(LISTE_ANA_TEST_ID_TEST, Qt::AscendingOrder);

     if(!model->select())
         emit(afficherTrace(model->lastError().text()));

     emit(afficherTrace("Concentration_Test_Metro rowcount()=" + QString::number(model->rowCount())));

     return model;
 }

QPointer<QSqlRelationalTableModel> BdHandler::getPolluantAssocieModel(const uint idEquipement)
{
    QPointer<QSqlRelationalTableModel> model = new QSqlRelationalTableModel(0,m_baseMySql);

    model->setTable("Polluant_Associe");
    model->setFilter(QString("id_pa_equipement = %1").arg(idEquipement));
    model->setRelation(POLLUANT_ASSOCIE_ID_EQUIPEMENT,QSqlRelation("Equipement","id_equipement","numero_serie"));
    model->setRelation(POLLUANT_ASSOCIE_ID_MOLECULE,QSqlRelation("Molecule","id_molecule","formule"));
    model->setHeaderData(POLLUANT_ASSOCIE_ID_MOLECULE, Qt::Horizontal, "Polluant");
    model->setSort(POLLUANT_ASSOCIE_ID, Qt::AscendingOrder);
    if(!model->select())
        emit(afficherTrace(model->lastError().text()));

    emit(afficherTrace("equipement : "+QString::number(idEquipement)));
    emit(afficherTrace("nb polluants associes : "+ QString::number(model->rowCount())));

    return model;
}

QPointer<QSqlQueryModel> BdHandler::getPolluantsParSystemeEtalon(const uint idSystemeEtalon,const bool filtrerRdf)
{
    QSqlQuery requete(QString("SELECT * FROM Systeme_Etalonnage WHERE id_systeme_etalon=%1").arg(QString::number(idSystemeEtalon)));

    int idDiluteur,idBouteille;

    if(requete.next()) {
        idDiluteur = requete.value(SYS_ETALON_DILUTEUR).toInt();
        idBouteille = requete.value(SYS_ETALON_BOUTEILLE).toInt();
    }
    else
        return NULL;

    QPointer<QSqlQueryModel> model = new QSqlQueryModel;
    QString query;
    if(idBouteille!=1)
        query = QString("SELECT id_molecule,formule FROM Molecule M,Polluant_Associe PA WHERE id_pa_equipement = %1 AND id_pa_molecule=id_molecule").arg(QString::number(idBouteille));
    else
        query = QString("SELECT id_molecule,formule FROM Molecule M,Polluant_Associe PA WHERE id_pa_equipement = %1 AND id_pa_molecule=id_molecule").arg(QString::number(idDiluteur));

    if(filtrerRdf)
        query.append(" AND formule IN ('NO','NO2','NOX')");

    model->setQuery(query,m_baseMySql);
    model->setHeaderData(0, Qt::Horizontal, tr("ID Molecule"));
    model->setHeaderData(1, Qt::Horizontal, tr("Formule"));

    emit(afficherTrace("getPolluantsParSystemeEtalon() rowCount = " + QString::number(model->rowCount())));

    return model;
}

QSqlRecord* BdHandler::getTableRow(const QString sqlTableRequete)
{
    QSqlQuery requete(sqlTableRequete);

    emit(afficherTrace("call BdHandler::getTableRow()"));
    emit(afficherTrace(sqlTableRequete));
    emit(afficherTrace("nb. enregistrements : " + QString::number(requete.size())));

    QSqlRecord* record = NULL;
    if(requete.next()) {
        record = new QSqlRecord(requete.record());
        for(int i=0;i<record->count();i++)
            qDebug()<<record->fieldName(i)<<" = "<<record->value(i);
    }
    return record;
}

QSqlRecord* BdHandler::getConcentrationRow(const uint idConcentration)
{
    return getTableRow(QString("SELECT * FROM Concentration WHERE id_Concentration=%1").arg(idConcentration));
}

QSqlRecord* BdHandler::getConcentrationRow(const uint idSystemeEtalon, const uint idMolecule, const uint pointGaz)
{
    QString requete = QString("SELECT * FROM Concentration WHERE id_systeme_etalon=%1 AND id_molecule=%2 AND point_consigne=%3").arg(
                      QString::number(idSystemeEtalon),QString::number(idMolecule),QString::number(pointGaz));

    return getTableRow(requete);
}


QSqlRecord* BdHandler::getMoleculeRow(const uint idMolecule)
{
    return getTableRow(QString("SELECT * FROM Molecule WHERE id_molecule=%1").arg(idMolecule));
}

QSqlRecord* BdHandler::getEquipementRow(const ushort idEquipement)
{
    return getTableRow(QString("SELECT * FROM Equipement WHERE id_equipement=%1").arg(idEquipement));
}

QSqlRecord* BdHandler::getSystemeEtalonRow(const ushort idSystemeEtalon)
{
    return getTableRow(QString("SELECT * FROM Systeme_Etalonnage WHERE id_systeme_etalon=%1").arg(idSystemeEtalon));
}

QSqlRecord* BdHandler::getEquipementModeledRow(const ushort idEquipement)
{
    QString requete = "SELECT E.id_equipement,E.numero_serie,ME.me_designation,MA.designation,E.min_gamme,";
    requete.append("E.max_gamme,E.offset,E.adresse,PS.designation ");
    requete.append("FROM Equipement E,Modele_Equipement ME, Marque_Equipement MA, Port_Serie PS ");
    requete.append("WHERE E.id_modele = ME.id_modele AND ME.id_marque = MA.id_marque AND E.no_port = PS.no_port ");
    requete.append(QString("AND id_equipement=%1").arg(idEquipement));

    emit(afficherTrace("call getEquipementRow("+QString::number(idEquipement)+")"));

    return getTableRow(requete);
}

QString BdHandler::getEquipementNumSerie(const ushort idEquipement) {
    QSqlRecord* record = getTableRow(QString("SELECT * FROM Equipement WHERE id_equipement=%1").arg(idEquipement));
    QString numSerie;
    if(record != NULL)
        numSerie = record->value(EQUIPEMENT_NO_SERIE).toString();
    return numSerie;
}

QString BdHandler::getEquipementTypeSocket(const ushort idEquipement){
    QSqlRecord* record = getTableRow(QString("SELECT * FROM Equipement WHERE id_equipement=%1").arg(idEquipement));
    QString typeSocket;
    if(record != NULL)
        typeSocket = record->value(EQUIPEMENT_TYPE_SOCKET).toString();
    return typeSocket;
}

QString BdHandler::getEquipementTypeConnexion(const ushort idEquipement){
    QSqlRecord* record = getTableRow(QString("SELECT * FROM Equipement WHERE id_equipement=%1").arg(idEquipement));
    QString typeSocket;
    if(record != NULL)
        typeSocket = record->value(EQUIPEMENT_TYPE_CONNEXION).toString();
    return typeSocket;
}

ushort BdHandler::getTxTransmission(const ushort idTxTransmission)
{
    emit(afficherTrace("call getTxTransmissionRow("+QString::number(idTxTransmission)+")"));
    QSqlRecord* record = getTableRow(QString("SELECT * FROM Taux_Transmission WHERE id_tx_transmission=%1").arg(idTxTransmission));
    return record->value(TX_TRANSMISSION_DESIGNATION).toInt();
}

QSqlRecord* BdHandler::getDesignationPortSerie(const ushort idEquipement)
{
    QString str_requete = "SELECT PS.designation FROM Equipement E,Port_Serie PS ";
    str_requete.append("WHERE E.no_port = PS.no_port ");
    str_requete.append(QString("AND id_equipement=%1").arg(idEquipement));

    return getTableRow(str_requete);
}

DesignationProtocole BdHandler::getDesignationProtocole(const ushort idEquipement)
{
    QString str_requete = "SELECT P.designation FROM Equipement E, Modele_Equipement M, Protocole P ";
    str_requete.append("WHERE E.id_modele=M.id_modele AND M.id_protocole=P.id_protocole ");
    str_requete.append(QString("AND id_equipement=%1").arg(idEquipement));
    QSqlQuery requete(str_requete);

    DesignationProtocole protocole = PROTOCOLE_INCONNU;

    if(requete.next()) {
        QSqlRecord record = requete.record();
        protocole = stringToProtocole(record.value("designation").toString());
    }
    return protocole;
}

ushort BdHandler::getIdCalibrateur(const uint idSystemeEtalon)
{
    emit(afficherTrace("call getIdCalibrateur("+QString::number(idSystemeEtalon)+")"));

    QSqlQuery requete(QString("SELECT id_etalon FROM Systeme_Etalonnage WHERE id_systeme_etalon=%1").arg(idSystemeEtalon));

    ushort idCalibrateur =0;
    if(requete.next()) {
        QSqlRecord record = requete.record();
        idCalibrateur = record.value("id_etalon").toUInt();
    }

    return idCalibrateur;
}

//Recuperation des informations du test

QSqlRecord* BdHandler::getInformationsTest(const ushort idTest)
{
    QString strequete = QString("SELECT TM.id_test, TM.test_metro_type_test, O.Nom, O.Prenom, L.designation, TM.pression,");
    strequete.append("TM.temperature, TM.date_debut, TM.date_fin, TM.id_systeme_etalon, TM.tps_acquisition ,TM.critere1 , TM.critere2 FROM Test_Metrologique TM, Operateur O, Lieu L");
    strequete.append(QString (" WHERE TM.id_operateur = O.id_operateur AND TM.id_lieu = L.id_lieu AND TM.id_test =%1").arg(idTest));

    return getTableRow(strequete);
}

//Recuperation des concentrations de chaque phase du test par polluant

QPointer<QSqlQueryModel> BdHandler::getTestPhaseConcentration (const ushort idTest,const ushort idmolecule)
{
    QString strRequete = QString("SELECT noPhase , concentration ,nbre_acquisition FROM Concentration_Test_Metro WHERE id_test =%1 AND id_molecule = %2")
            .arg(QString::number(idTest),QString::number(idmolecule));
    QPointer<QSqlQueryModel> model = new QSqlQueryModel;
    model->setQuery(strRequete,m_baseMySql);
    return model;

}

//Recuperation des polluants definis dans la totalite des phases du tests

QPointer<QSqlQueryModel> BdHandler::getPolluantTestConcentration (const ushort idTest)
{
    QString strRequete = QString("SELECT CTM.id_molecule, M.code, M.formule FROM Concentration_Test_Metro CTM, Molecule M WHERE CTM.id_molecule = M.id_molecule AND CTM.id_test = %1 GROUP BY M.code").arg(idTest);
    QPointer<QSqlQueryModel> model = new QSqlQueryModel;
    model->setQuery(strRequete,m_baseMySql);
    return model;

}

//Recuperation des cycles de phases pour le test en argument

QPointer<QSqlQueryModel> BdHandler::getCyclePhaseTest (const ushort idTest)
{
    QString strRequete = QString("SELECT M.no_cyclePhase FROM Mesure M WHERE M.id_test = %1 GROUP BY M.no_cyclePhase").arg(idTest);
    QPointer<QSqlQueryModel> model = new QSqlQueryModel;
    model->setQuery(strRequete,m_baseMySql);
    return model;

}


//Recuperation des mesures d'un test d'un analyseur

QPointer<QSqlQueryModel> BdHandler::getMesureTestAnalyseur (const ushort idTest , const ushort idEquipement , const ushort codeMolecule, const ushort noPhase,const ushort noCycle)
{
    QString strRequete = "SELECT `no_cyclePhase` , `no_phase` , `no_cycleMesure` ";
    //for (ushort idMesEquip=1;idMesEquip<nbMesureEquipement+1;idMesEquip++) {
    if (codeMolecule==3){
        strRequete.append(QString(", mesure_3"));
    }
    else if (codeMolecule==12){
        strRequete.append(QString(", mesure_2"));
    }
    else {
        strRequete.append(QString(", mesure_1"));
    }
    //}
    strRequete.append(QString (" FROM Mesure WHERE id_test=%1 ").arg(idTest));
    strRequete.append(QString ("AND id_equipement=%2 ").arg(idEquipement));
    strRequete.append(QString ("AND no_phase=%3 ").arg(noPhase));
    if (!noCycle==0) strRequete.append(QString ("AND no_cyclePhase=%4 ").arg(noCycle));
    QPointer<QSqlQueryModel> model = new QSqlQueryModel;
    model->setQuery(strRequete,m_baseMySql);
    return model;
}

//Recuperation des phases d'un test à partir table mesure

QPointer<QSqlQueryModel> BdHandler::getTestPhase (const ushort idTest)
{
    QString strRequete = QString("SELECT no_Phase FROM Mesure WHERE id_test =%1 GROUP BY no_Phase")
            .arg(QString::number(idTest));
    QPointer<QSqlQueryModel> model = new QSqlQueryModel;
    model->setQuery(strRequete,m_baseMySql);
    return model;

}

bool BdHandler::insertIntoMesure(const MesureInfo mesureInfos)
{
    QString strRequeteDebut = "INSERT INTO `Mesure` (`id_test`,`id_equipement`,`no_cyclePhase`,`no_phase`,";
    strRequeteDebut.append("`no_cycleMesure`,`mesure_1`");

    QString strRequeteValeurs = QString::number(mesureInfos.idTest)+",";
    strRequeteValeurs.append(QString::number(mesureInfos.idEquipement)+",");
    strRequeteValeurs.append(QString::number(mesureInfos.noCyclePhase)+",");
    strRequeteValeurs.append(QString::number(mesureInfos.noPhase)+",");
    strRequeteValeurs.append(QString::number(mesureInfos.noCycleMesure)+",");
    strRequeteValeurs.append(QString::number(mesureInfos.mesure.data()->at(0)));
    if(mesureInfos.mesure.data()->count()>1) {
        strRequeteDebut.append(",`mesure_2`");
        strRequeteValeurs.append(","+QString::number(mesureInfos.mesure.data()->at(1)));
        if(mesureInfos.mesure.data()->count()>2) {
            strRequeteDebut.append(",`mesure_3`");
            strRequeteValeurs.append(","+QString::number(mesureInfos.mesure.data()->at(2))+");");
        }
        else
            strRequeteValeurs.append(");");
    }
    else strRequeteValeurs.append(");");

    strRequeteDebut.append(") VALUES (");

    QString strRequete = strRequeteDebut+strRequeteValeurs;
    bool succes = m_baseMySql.transaction();
    QSqlQuery requete;
    succes = requete.exec(strRequete);

    if(!succes)
        emit(afficherTrace("Problème lors de l'enregistrement de la mesure moyenne"));
    succes = m_baseMySql.commit();
    return succes;
}

void BdHandler::setSpanHandlerFromIdConcentration(ushort idConcentration, QString canal, SpanHandler* spanHandler)
{
    QSqlRecord* record = this->getConcentrationRow(idConcentration);

    spanHandler->setSpanArguments(canal,record->value(CONCENTRATION_POINT).toUInt(),record->value(CONCENTRATION_OZONE).toUInt());
}

bool BdHandler::miseAjourDateHeureFinTest(const ushort idTestMetro)
{
    QString strRequete = "UPDATE `Test_Metrologique` SET `date_fin` = '"+QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    strRequete.append(QString("' WHERE `id_test` = %1 ").arg(idTestMetro));
    QSqlQuery requete;
    bool succes = requete.exec(strRequete);
    if(!succes) {
        emit(afficherTrace("Problème lors de la mise à jour du champs date_fin de la table Test_Metrologique"),2);
        emit(afficherTrace("id_testMetro ="+QString::number(idTestMetro)),2);
    }
    return succes;
}


QPointer<QStandardItemModel> BdHandler::getItemModelListeRapport(){

    QPointer<QSqlQueryModel> modeleAnalyseur = this->getModeleAnalyseur();
    QStandardItemModel *model = new QStandardItemModel;

    for (int i=0;i<modeleAnalyseur->rowCount();i++) {
        QSqlRecord recModeleAnalyseur = modeleAnalyseur->record(i);
        QString designModelAna = recModeleAnalyseur.value(REQ_MODELE_EQUIP_DESIGNATION).toString();
        QStandardItem * itemTypeAnalyseur = new QStandardItem(designModelAna);
        model->appendRow(itemTypeAnalyseur);
        itemTypeAnalyseur->setEditable(false);
        QPointer<QSqlQueryModel>  listeAnalyseur = this->getListeEquipementParModele(recModeleAnalyseur.value("id_modele").toInt());
        for (int j=0;j<listeAnalyseur->rowCount();j++) {
            QSqlRecord recAnalyseur = listeAnalyseur->record(j);
            QString numSerieAnalyseur = recAnalyseur.value("numero_serie").toString();
            QStandardItem * itemAnalyseur = new QStandardItem(numSerieAnalyseur);
            itemTypeAnalyseur->appendRow(itemAnalyseur);
            itemAnalyseur->setEditable(false);
            QPointer<QSqlQueryModel>  listeTypeTestAnalyseur = this->getListeTypeTestParEquipement(recAnalyseur.value("id_equipement").toInt());
            for (int k=0; k<listeTypeTestAnalyseur->rowCount();k++){
                QSqlRecord recTypeTest = listeTypeTestAnalyseur->record(k);
                QString NomTypeTest = recTypeTest.value("test_metro_type_test").toString();
                QStandardItem * itemTypeTest = new QStandardItem(NomTypeTest);
                itemAnalyseur->appendRow(itemTypeTest);
                itemTypeTest->setEditable(false);
                QPointer<QSqlQueryModel> listeDateTest = this->getListeDateTestParEquipParTypeTest(recAnalyseur.value("id_equipement").toInt(),recTypeTest.value("test_metro_type_test").toString());
                for (int l=0; l<listeDateTest->rowCount();l++){
                    QSqlRecord recDateTest = listeDateTest->record(l);
                    QString DateDebtest = recDateTest.value("date_debut").toString();
                    QStandardItem * itemDateDeb = new QStandardItem(DateDebtest);
                    itemTypeTest->appendRow(itemDateDeb);
                    itemDateDeb->setEditable(false);
                    QString IdTest = recDateTest.value("id_test").toString();
                    QStandardItem * itemIdTest = new QStandardItem (IdTest);
                    itemDateDeb->appendRow(itemIdTest);
                    itemIdTest->setEditable(false);
                    QString IdEquip = recAnalyseur.value("id_equipement").toString();
                    QStandardItem * itemIdEquip = new QStandardItem (IdEquip);
                    itemDateDeb->appendRow(itemIdEquip);
                    itemIdEquip->setEditable(false);

                }
            }
        }
     }
    return model;
}
