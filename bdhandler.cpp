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

bool BdHandler::connexionBD() {
    if(!m_baseMySql.isOpen())
        m_baseMySql.open();

    if(!m_baseMySql.isOpen())
    {
        qDebug()<<QObject::tr("Database Error")<<m_baseMySql.lastError().text();
        return false;
    }
    qDebug()<<"Connexion effectuee";
    return true;
}

void BdHandler::deconnexionBD() {
    if(m_baseMySql.isOpen())
        m_baseMySql.close();
}

bool BdHandler::isOpen() {
    return this->m_baseMySql.isOpen();
}

QPointer<QSqlRelationalTableModel> BdHandler::getEquipementModel() {
    QPointer<QSqlRelationalTableModel> model = new QSqlRelationalTableModel(this);
    model->setTable("Equipement");
    model->setRelation(EQUIPEMENT_ID_MODELE, QSqlRelation("Modele_Equipement", "id_modele", "designation"));
    model->setRelation(EQUIPEMENT_ID_TX_TRANSMISSION, QSqlRelation("Taux_Transmission","id_tx_transmission","taux_transmission"));

    model->setHeaderData(EQUIPEMENT_ID_MODELE, Qt::Horizontal, "Modèle");
    model->setHeaderData(EQUIPEMENT_NO_SERIE, Qt::Horizontal, "N° Série");
    model->setHeaderData(EQUIPEMENT_ID_TX_TRANSMISSION, Qt::Horizontal, "Taux transmission");
    model->setHeaderData(EQUIPEMENT_TYPE, Qt::Horizontal, "Type");
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
    model->select();

    return model;
}

QPointer<QSqlTableModel> BdHandler::getMoleculesModel() {
    QPointer<QSqlTableModel> model = new QSqlTableModel(0,m_baseMySql);
    model->setTable("Molecule");
    model->setSort(MOLECULE_ID, Qt::AscendingOrder);
    model->select();

    return model;
}

QPointer<QSqlTableModel> BdHandler::getProtocolesModel() {
    QPointer<QSqlTableModel> model = new QSqlTableModel(0,m_baseMySql);
    model->setTable("Protocole");
    model->setSort(PROTOCOLE_ID, Qt::AscendingOrder);
    model->select();

    return model;
}

QPointer<QSqlTableModel> BdHandler::getMarquesModel() {
    QPointer<QSqlTableModel> model = new QSqlTableModel(0,m_baseMySql);
    model->setTable("Marque_Equipement");
    model->setSort(MARQUE_ID, Qt::AscendingOrder);
    model->select();

    return model;
}

QPointer<QSqlTableModel> BdHandler::getTxTransmissionModel() {
    QPointer<QSqlTableModel> model = new QSqlTableModel(0,m_baseMySql);
    model->setTable("Taux_Transmission");
    model->setSort(TX_TRANSMISSION_DESIGNATION, Qt::AscendingOrder);
    model->select();

    return model;
}

QPointer<QSqlRelationalTableModel> BdHandler::getModelesModel() {
    QPointer<QSqlRelationalTableModel> model = new QSqlRelationalTableModel(0,m_baseMySql);

    model->setTable("Modele_Equipement");
    model->setRelation(MODELE_ID_MARQUE,QSqlRelation("Marque_Equipement","id_marque","designation"));
    model->setRelation(MODELE_ID_PROTOCOLE,QSqlRelation("Protocole","id_Protocole","designation"));
    model->setHeaderData(MODELE_ID_MARQUE, Qt::Horizontal, "Marque");
    model->setHeaderData(MODELE_ID_PROTOCOLE, Qt::Horizontal, "Protocole");
    model->setHeaderData(MODELE_DESIGNATION, Qt::Horizontal, "Designation");
    model->setSort(MODELE_ID, Qt::AscendingOrder);
    model->select();

    return model;
}

QPointer<QSqlRelationalTableModel> BdHandler::getSystemeEtalonModel() {
    QPointer<QSqlRelationalTableModel> model = new QSqlRelationalTableModel(0,m_baseMySql);

    model->setTable("System_Etalonnage");
    model->setRelation(SYS_ETALON_DILUTEUR,QSqlRelation("Equipement","id_equipement","numero_serie"));
    model->setRelation(SYS_ETALON_BOUTEILLE,QSqlRelation("Equipement","id_equipement","numero_serie"));
    model->setRelation(SYS_ETALON_GZERO,QSqlRelation("Equipement","id_equipement","numero_serie"));
    model->setHeaderData(SYS_ETALON_DILUTEUR, Qt::Horizontal, "Diluteur ou Générateur O3");
    model->setHeaderData(SYS_ETALON_BOUTEILLE, Qt::Horizontal, "Bouteille");
    model->setHeaderData(SYS_ETALON_GZERO, Qt::Horizontal, "Générateur air zéro");
    model->setSort(SYS_ETALON_ID, Qt::AscendingOrder);
    model->select();

    return model;
}

QPointer<QSqlTableModel> BdHandler::getSystemeEtalonModelSansRelation() {
    QPointer<QSqlTableModel> model = new QSqlTableModel(0,m_baseMySql);
    model->setTable("System_Etalonnage");
    model->setSort(SYS_ETALON_ID, Qt::AscendingOrder);
    model->select();

    return model;
}

QPointer<QSqlTableModel> BdHandler::getConcentrationModel(const int idSystemeEtalon) {
    QPointer<QSqlTableModel> model = new QSqlTableModel(0,m_baseMySql);
    model->setTable("Concentration");
    model->setFilter(QString("id_systeme_etalon = %1").arg(idSystemeEtalon));
    model->setSort(CONCENTRATION_ID, Qt::AscendingOrder);
    model->setHeaderData(CONCENTRATION_POINT, Qt::Horizontal, "Point consigne");
    model->setHeaderData(CONCENTRATION_REELLE, Qt::Horizontal, "Concentration réelle");
    model->setHeaderData(CONCENTRATION_OZONE, Qt::Horizontal, "Concentration O3");
    model->select();

    return model;
}

QPointer<QSqlRelationalTableModel> BdHandler::getPolluantAssocieModel(const int idEquipement) {
    QPointer<QSqlRelationalTableModel> model = new QSqlRelationalTableModel(0,m_baseMySql);

    model->setTable("Polluant_Associe");
    model->setFilter(QString("id_pa_equipement = %1").arg(idEquipement));
    model->setRelation(POLLUANT_ASSOCIE_ID_EQUIPEMENT,QSqlRelation("Equipement","id_equipement","numero_serie"));
    model->setRelation(POLLUANT_ASSOCIE_ID_MOLECULE,QSqlRelation("Molecule","id_molecule","formule"));
    model->setHeaderData(POLLUANT_ASSOCIE_ID_MOLECULE, Qt::Horizontal, "Polluant");
    model->setSort(POLLUANT_ASSOCIE_ID_MOLECULE, Qt::AscendingOrder);
    qDebug()<<model->select();
    qDebug()<<model->lastError().text();
    qDebug()<<"equipement : "<<idEquipement;
    qDebug()<<"nb polluants associes : "<<model->rowCount();

    return model;
}
