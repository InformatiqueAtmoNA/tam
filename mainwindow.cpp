/*////////////////////////////////////////////////////////////
// \file mainwindow.cpp
// \brief Classe d'interface graphique de la fenetre principale
// \author FOUQUART Christophe
// \version 1.0
// \date 25/03/2011
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
////////////////////////////////////////////////////////////*/


#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QLabel>
#include <QPushButton>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(this->ui->actionDlgEquipement,SIGNAL(triggered()),this,SLOT(afficherDlgEquipement()));
    connect(this->ui->actionDlgSystemeEtalon,SIGNAL(triggered()),this,SLOT(afficherDlgSystemeEtalon()));
    connect(this->ui->actionDlgLieu,SIGNAL(triggered()),this,SLOT(afficherDlgLieu()));
    connect(this->ui->actionDlgOperateur,SIGNAL(triggered()),this,SLOT(afficherDlgOperateur()));
    connect(this->ui->actionDlgPortSerie,SIGNAL(triggered()),this,SLOT(afficherDlgPortSerie()));
    connect(this->ui->action_Parametres,SIGNAL(triggered()),this,SLOT(afficherParametres()));
    connect(this->ui->action_SerieTest,SIGNAL(triggered()),this,SLOT(programmerSerieTests()));
    connect(this->ui->action_aPropos,SIGNAL(triggered()),this,SLOT(aPropos()));
    connect(this->ui->action_Fichier_Quitter,SIGNAL(triggered()),this,SLOT(quitter()));
    connect(this->ui->actionDeconnexion,SIGNAL(triggered()),this,SLOT(connexion()));
    connect(this->ui->actionInformations,SIGNAL(triggered()),this,SLOT(afficherInformationsUser()));

    QString driver = getParam("BD_Driver").toString();
    QString host = getParam("Host").toString();
    QString userName = getParam("UserName").toString();
    QString password= getParam("Password").toString().toLatin1();

    QString dbName = getParam("DB_Name").toString();

    m_bdHandler = new BdHandler(driver,host,userName,password,dbName);


    if(!m_bdHandler->connexionBD()){
        password="";
        for(char index : getParam("Password").toString().toLatin1()){
            index-=17;
            password.append(index);
        }
        m_bdHandler = new BdHandler(driver,host,userName,password,dbName);
        if(!m_bdHandler->connexionBD()){
            return;
        }
    }
    else{
        // modification du mot de passe pour le rendre non lisible dans le fichier params.ini
        QString newPassword;
        for(char index :password.toLatin1()){
            index+=17;
            newPassword.append(index);
        }
        setParam("Password",QVariant::fromValue(newPassword));
    }

    this->connexion();
    this->afficherHomeWidget();

}

MainWindow::~MainWindow()
{
    if(!this->m_bdHandler.isNull())
        delete this->m_bdHandler;
    if(!this->m_dlgCreationtest.isNull())
        delete this->m_dlgCreationtest;
    if(!this->m_homeWidget.isNull())
        delete this->m_homeWidget;
    if(!this->m_dlgListeAttenteTest.isNull()) {
        delete this->m_dlgListeAttenteTest;
    }

    delete ui;
}


void MainWindow::quitter()
{
    emit(this->close());
}

void MainWindow::closeEvent(QCloseEvent *event)
 {

    QMessageBox msgBox;
    msgBox.setText(QLatin1String("Fermer ?"));
    msgBox.setInformativeText(QLatin1String("Voulez-vous quitter l'application ?"));
    msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);

    if(msgBox.exec()==QMessageBox::Ok)
         event->accept();
    else
        event->ignore();
 }

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::afficherHomeWidget(int index)
{
    m_homeWidget = new HomeWidget(m_bdHandler,this,&index);
    this->setCentralWidget(m_homeWidget);

    if(!this->m_dlgCreationtest.isNull()) {
        delete this->m_dlgCreationtest;
    }

    if(!this->m_dlgExecutionTest.isNull()) {
        delete this->m_dlgExecutionTest;
    }

    if(!this->m_dlgListeAttenteTest.isNull()) {
        delete this->m_dlgListeAttenteTest;
    }

    connect(this->m_homeWidget,SIGNAL(creerTest()),this,SLOT(nouveauTest()));
    connect(this->m_homeWidget,SIGNAL(modifierTest(QString)),this,SLOT(modifierTest(QString)));
    connect(this->m_homeWidget,SIGNAL(executerTest(ushort,QString)),this,SLOT(executerTest(ushort,QString)));
    connect(this->m_homeWidget,SIGNAL(afficherRapport(ushort,ushort,ushort)),this,SLOT(afficherRapport(ushort,ushort,ushort)));
    connect(this->m_homeWidget,SIGNAL(programmerSerieTest()),this,SLOT(programmerSerieTests()));

    m_homeWidget->show();
}

void MainWindow::afficherCreationTest(const QString fichierDescription)
{
    if(fichierDescription.isEmpty())
        this->m_dlgCreationtest = new CreationTest(m_bdHandler,this);
    else {
        if(!QFile::exists(fichierDescription)) {
            QMessageBox msgBox;
            msgBox.setText(QLatin1String("Un problème est survenu lors du chargement du fichier de configuration du test"));
            msgBox.setInformativeText("Le fichier "+fichierDescription+" n'a pas pu être charge");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.exec();

            return;
        }
        this->m_dlgCreationtest = new CreationTest(m_bdHandler,this,fichierDescription);
    }
    if(!this->m_homeWidget.isNull()) {
        delete m_homeWidget;
    }

    this->setCentralWidget(m_dlgCreationtest);

    connect(this->m_dlgCreationtest,SIGNAL(fermeture(int)),this,SLOT(fermetureTestWidget(int)));

    m_dlgCreationtest->show();
}

void MainWindow::nouveauTest()
{
    this->afficherCreationTest();
}

void MainWindow::modifierTest(const QString fichierDescription)
{
    this->afficherCreationTest(fichierDescription);
}

void MainWindow::executerTest(const ushort idTestXML, const QString fichierDescription)
{

    if(!QFile::exists(fichierDescription)) {
        QMessageBox msgBox;
        msgBox.setText(QLatin1String("Un problème est survenu lors du chargement du fichier de configuration du test"));
        msgBox.setInformativeText("Le fichier "+fichierDescription+" n'a pas pu être charge");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();

        return;
    }

    QList<ushort> idTestXml;
    QList<QString> fichierDescriptions;
    idTestXml.append(idTestXML);
    fichierDescriptions.append(fichierDescription);

    this->m_dlgExecutionTest = new et_InterfaceExecutionTest(m_bdHandler,m_user,idTestXml,fichierDescriptions,false,1,this);

    if(!this->m_homeWidget.isNull()) {
        delete m_homeWidget;
    }

    this->setCentralWidget(m_dlgExecutionTest);
    m_dlgExecutionTest->show();

    connect(this->m_dlgExecutionTest,SIGNAL(fermeture(int)),this,SLOT(fermetureTestWidget(int)));
}

void MainWindow::afficherRapport(const ushort idTest,const ushort idAnalyseur, const ushort typeTest)
{

    this->m_dlgGenerateurRapportTest = new et_GenerateurRapportTest(m_bdHandler,m_user,idTest,idAnalyseur,typeTest,this);

    if(!this->m_homeWidget.isNull()) {
        delete m_homeWidget;
    }

    this->setCentralWidget(m_dlgGenerateurRapportTest);
    m_dlgGenerateurRapportTest->show();

    connect(this->m_dlgGenerateurRapportTest,SIGNAL(fermeture(int)),this,SLOT(fermetureTestWidget(int)));

}

void MainWindow::fermetureTestWidget(int index)
{
    this->afficherHomeWidget(index);
}

void MainWindow::afficherDlgEquipement()
{
    Dlg_Equipement dlgEquipement(this,this->m_bdHandler);
    dlgEquipement.exec();
}

void MainWindow::afficherDlgSystemeEtalon()
{
    Dlg_Systeme_Etalon dlgSystemeEtalon (this,this->m_bdHandler);
    dlgSystemeEtalon.exec();
}

void MainWindow::afficherDlgLieu()
{
    Dlg_Lieu dlgLieu(this,this->m_bdHandler);
    dlgLieu.exec();
}

void MainWindow::afficherDlgOperateur()
{
    Dlg_Operateur dlgOperateur(this,this->m_bdHandler);
    dlgOperateur.exec();
}

void MainWindow::afficherDlgPortSerie()
{
    Dlg_Port_Serie dlgPortSerie(this,this->m_bdHandler);
    dlgPortSerie.exec();
}

void MainWindow::afficherParametres()
{
    ParametresHandler parametresHandler(this);
    parametresHandler.exec();
}

void MainWindow::aPropos()
{
    QMessageBox::about(this,"A propos de TAM","TAM 3.2.0 \n"
                       "Version du 16/06/2022 \n\n"
                       "Base sur Qt " QT_VERSION_STR "\n\n"
                       "Copyright (C) 2011-2014 TAM Team \n"
                       "TAM est distribue sous les termes de la \n"
                                                     "Licence Publique Generale (GPL) V2 \n\n");
}

void MainWindow::connexion()
{
    dlg_Authentification dlg_Authentification(this, this->m_bdHandler);
    int retour = dlg_Authentification.exec();
    if(retour == QDialog::Rejected){
       exit(0);
    }
    this->m_user=dlg_Authentification.getUser();

    QSqlRecord *record = this->m_bdHandler->getOperateurRow(this->m_user.user());
    if(!record->value(OPERATEUR_ADMIN).isNull()){
        if(record->value(OPERATEUR_ADMIN).toInt()!=1){
            this->ui->action_Parametres->setDisabled(true);
            this->ui->actionDlgOperateur->setDisabled(true);
        }
        else{
            this->ui->action_Parametres->setDisabled(false);
            this->ui->actionDlgOperateur->setDisabled(false);
        }
    }
    else{
        QMessageBox msgBox;
        msgBox.setText(QLatin1String("Fermer ?"));
        msgBox.setInformativeText(QLatin1String("Il semblerait que vous n'ayez aucun droit de configuré en base, l'application va se fermer pour éviter tout bug"));
        exit(0);
    }

}

void MainWindow::afficherInformationsUser()
{
    dlg_info_utilisateur info_utilisateur(this->m_user, m_bdHandler, this);
    info_utilisateur.exec();
    m_user=info_utilisateur.getUser();
}


void MainWindow::programmerSerieTests()
{
    this->m_dlgListeAttenteTest = new et_listeAttenteTests(m_bdHandler,m_user,this);

    if(!this->m_homeWidget.isNull()) {
        delete m_homeWidget;
    }

    this->setCentralWidget(m_dlgListeAttenteTest);
    m_dlgListeAttenteTest->show();

    connect(this->m_dlgListeAttenteTest,SIGNAL(fermeture(int)),this,SLOT(fermetureTestWidget(int)));
}
