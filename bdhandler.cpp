/*///////////////////////////////////////////////////////////////////////////////////
// \file bdhandler.cpp
// \brief Classe gérant l'acces a la base de donnees
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
    model->setRelation(EQUIPEMENT_ID_MODELE, QSqlRelation("Modele_Equipement", "id_modele", "designation"));
    model->setRelation(EQUIPEMENT_ID_TX_TRANSMISSION, QSqlRelation("Taux_Transmission","id_tx_transmission","taux_transmission"));

    model->setHeaderData(EQUIPEMENT_ID_MODELE, Qt::Horizontal, "Modèle");
    model->setHeaderData(EQUIPEMENT_NO_SERIE, Qt::Horizontal, "N° Série");
    model->setHeaderData(EQUIPEMENT_ID_TX_TRANSMISSION, Qt::Horizontal, "Taux transmission");
    model->setHeaderData(EQUIPEMENT_EN_SERVICE, Qt::Horizontal, "En service ?");
    model->setHeaderData(EQUIPEMENT_MAX_GAMME, Qt::Horizontal, "Max. Gamme");
    model->setHeaderData(EQUIPEMENT_MIN_GAMME, Qt::Horizontal, "Min. Gamme");
    model->setHeaderData(EQUIPEMENT_OFFSET, Qt::Horizontal, "Offset");
    model->setHeaderData(EQUIPEMENT_ADRESSE, Qt::Horizontal, "Adresse");
    model->setHeaderData(EQUIPEMENT_CONTROLE_FLUX, Qt::Horizontal, "Contrôle de flux");
    model->setHeaderData(EQUIPEMENT_NB_BITS_STOP, Qt::Horizontal, "Nb. bits de stop");
    model->setHeaderData(EQUIPEMENT_NB_BITS_TRANSMISSION, Qt::Horizontal, "Nb. bits de transmission");
    model->setHeaderData(EQUIPEMENT_PARITE, Qt::Horizontal, "Parité");
    model->setHeaderData(EQUIPEMENT_OFFSET, Qt::Horizontal, "Offset");
    model->setSort(EQUIPEMENT_ID, Qt::AscendingOrder);
    if(!model->select())
        emit(afficherTrace(model->lastError().text()));

    return model;
}

QPointer<QSqlRelationalTableModel> BdHandler::getConcentrationAssocieeModel(const uint idConcentration)
{
    QPointer<QSqlRelationalTableModel> model = new QSqlRelationalTableModel(this);
    model->setTable("Concentration_Associee");
    model->setRelation(CONC_ASSOCIEE_ID_MOLECULE, QSqlRelation("Molecule", "id_molecule", "formule"));

    model->setHeaderData(CONC_ASSOCIEE_ID_MOLECULE, Qt::Horizontal, "Molécule");
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
    model->setHeaderData(SYS_ETALON_DILUTEUR, Qt::Horizontal, "Diluteur ou Générateur O3");
    model->setHeaderData(SYS_ETALON_BOUTEILLE, Qt::Horizontal, "Bouteille");
    model->setHeaderData(SYS_ETALON_GZERO, Qt::Horizontal, "Générateur air zéro");
    model->setSort(SYS_ETALON_ID, Qt::AscendingOrder);

    if(idSystemeEtalon>0)
        model->setFilter(QString("id_systeme_etalon=%1").arg(idSystemeEtalon));

    if(!model->select())
        emit(afficherTrace(model->lastError().text()));

    emit(afficherTrace("call : BdHandler::getSystemeEtalonModel() rowCount = "+ QString::number(model->rowCount())));

    return model;
}

QPointer<QSqlQueryModel> BdHandler::getEquipementFiltreParModele(const QString & filtre) {
    QString requete(QString("SELECT E.id_equipement,E.id_modele,M.type FROM Equipement E, Modele_Equipement M WHERE E.id_modele=M.id_modele AND (%1) ORDER BY E.id_equipement ASC").arg(filtre));

    QPointer<QSqlQueryModel> model = new QSqlQueryModel;
    model->setQuery(requete,m_baseMySql);

    emit(afficherTrace("getEquipementFiltreParModele rowCount " + QString::number(model->rowCount())));

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
    model->setSort(CONCENTRATION_ID, Qt::AscendingOrder);
    model->setHeaderData(CONCENTRATION_POINT, Qt::Horizontal, "Point consigne");
    model->setHeaderData(CONCENTRATION_REELLE, Qt::Horizontal, "Concentration réelle");
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

QPointer<QSqlRelationalTableModel> BdHandler::getPolluantAssocieModel(const uint idEquipement)
{
    QPointer<QSqlRelationalTableModel> model = new QSqlRelationalTableModel(0,m_baseMySql);

    model->setTable("Polluant_Associe");
    model->setFilter(QString("id_pa_equipement = %1").arg(idEquipement));
    model->setRelation(POLLUANT_ASSOCIE_ID_EQUIPEMENT,QSqlRelation("Equipement","id_equipement","numero_serie"));
    model->setRelation(POLLUANT_ASSOCIE_ID_MOLECULE,QSqlRelation("Molecule","id_molecule","formule"));
    model->setHeaderData(POLLUANT_ASSOCIE_ID_MOLECULE, Qt::Horizontal, "Polluant");
    model->setSort(POLLUANT_ASSOCIE_ID_MOLECULE, Qt::AscendingOrder);
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
    QString requete = "SELECT E.id_equipement,E.numero_serie,MO.designation,MA.designation,E.min_gamme,";
    requete.append("E.max_gamme,E.offset,E.adresse ");
    requete.append("FROM Equipement E,Modele_Equipement MO, Marque_Equipement MA ");
    requete.append("WHERE E.id_modele = MO.id_modele AND MO.id_marque = MA.id_marque ");
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

ushort BdHandler::getTxTransmission(const ushort idTxTransmission)
{
    emit(afficherTrace("call getTxTransmissionRow("+QString::number(idTxTransmission)+")"));
    QSqlRecord* record = getTableRow(QString("SELECT * FROM Taux_Transmission WHERE id_tx_transmission=%1").arg(idTxTransmission));
    return record->value(TX_TRANSMISSION_DESIGNATION).toInt();
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

bool BdHandler::insertIntoMesure(const MesureInfo mesureInfos)
{
    QString strRequete = "INSERT INTO `Mesure` (`id_test`,`id_equipement`,`no_cyclePhase`,`no_phase`,";
    strRequete.append("`no_cycleMesure`,`mesure_1`,`mesure_2`,`mesure_3`) VALUES (");
    strRequete.append(QString::number(mesureInfos.idTest)+",");
    strRequete.append(QString::number(mesureInfos.idEquipement)+",");
    strRequete.append(QString::number(mesureInfos.noCyclePhase)+",");
    strRequete.append(QString::number(mesureInfos.noPhase)+",");
    strRequete.append(QString::number(mesureInfos.noCycleMesure)+",");
    strRequete.append(QString::number(mesureInfos.mesure.data()->at(0))+",");
    if(mesureInfos.mesure.data()->count()>1)
        strRequete.append(QString::number(mesureInfos.mesure.data()->at(1))+",");
    if(mesureInfos.mesure.data()->count()>2)
        strRequete.append(QString::number(mesureInfos.mesure.data()->at(2))+",");

    m_baseMySql.transaction();
    QSqlQuery requete;
    bool succes = requete.exec(strRequete);
    m_baseMySql.commit();

    return succes;
}

void BdHandler::setSpanHandlerFromIdConcentration(ushort idConcentration, QString canal, SpanHandler* spanHandler)
{
    QSqlRecord* record = this->getConcentrationRow(idConcentration);

    spanHandler->setSpanArguments(canal,record->value(CONCENTRATION_POINT).toUInt(),record->value(CONCENTRATION_OZONE).toUInt());
}

bool BdHandler::miseAjourDateHeureFinTest(const ushort idTestMetro)
{
    QString strRequete = "UPDATE `Test_Metrologique` SET `date_fin` = "+QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QSqlQuery requete;
    bool succes = requete.exec(strRequete);
    if(!succes) {
        emit(afficherTrace("Problème lors de la mise à jour du champs date_fin de la table Test_Metrologique"));
        emit(afficherTrace("id_testMetro ="+QString::number(idTestMetro)));
    }
    return succes;
}
