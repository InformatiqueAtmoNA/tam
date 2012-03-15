/*////////////////////////////////////////////////////////////
// \file et_listeattentetests.cpp
// \brief Classe d'interface gérant la liste d'attente des tests en cours
// \author FOUQUART Christophe
// \version 1.0
// \date 10/08/2011
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

#include "et_listeattentetests.h"
#include "ui_et_listeattentetests.h"

et_listeAttenteTests::et_listeAttenteTests(QPointer<BdHandler> bdHandler, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::et_listeAttenteTests)
{
    ui->setupUi(this);

    m_bdHandler = bdHandler;
    m_nbPagesTest = 0;

    m_timer = new QTimer(this);
    m_timer->setInterval(60000);
    m_timer->setSingleShot(false);

    ui->tableWidget_TestsEnAttente->setColumnHidden(ET_TABLEW_TEST_ATTENTE_ID_TEST,true);

    connect(ui->button_AjouterTest,SIGNAL(clicked()),this,SLOT(buttonAjouterTestClicked()));
    connect(ui->button_Annuler,SIGNAL(clicked()),this,SLOT(buttonAnnulerClicked()));
    connect(ui->button_SupprimerTest,SIGNAL(clicked()),this,SLOT(buttonSupprimerTestClicked()));
    connect(ui->button_ExecuterTests,SIGNAL(clicked()),this,SLOT(buttonExecuterClicked()));
    connect(this,SIGNAL(lancementTest(ushort)),this,SLOT(lancerTest(ushort)));

    ui->stackedWidget->setCurrentIndex(1);
}

et_listeAttenteTests::~et_listeAttenteTests()
{
    delete ui;
    delete m_timer;
}

void et_listeAttenteTests::buttonAjouterTestClicked()
{
    Dlg_testXml dlgChoixFichierXml(this,m_bdHandler,true);
    int retour = dlgChoixFichierXml.exec();
    if(retour == QDialog::Rejected)
        return;
    ushort idTestXML = dlgChoixFichierXml.getIdSelection();
    QString fichierDescriptionTest = dlgChoixFichierXml.getFichierDescriptionSelection();

    m_interfaceparamsTest = new et_InterfaceExecutionTest(m_bdHandler,idTestXML,fichierDescriptionTest,true);

    connect(m_interfaceparamsTest,SIGNAL(miseEnAttente(QPointer<et_ParamsTest>)),
            this,SLOT(miseEnAttente(QPointer<et_ParamsTest>)));
    connect(m_interfaceparamsTest,SIGNAL(fermeture()),this,SLOT(testAnnule()));

    ui->stackedWidget->setCurrentIndex(0);
    ui->h_Layout_et_InterfaceExecutionTest->addWidget(m_interfaceparamsTest);
    m_interfaceparamsTest->show();
}

void et_listeAttenteTests::buttonSupprimerTestClicked()
{
    if(ui->tableWidget_TestsEnAttente->currentIndex().isValid()) {
        const ushort rowToDelete = ui->tableWidget_TestsEnAttente->currentIndex().row();
        m_listeParamsTestsEnAttente.remove(rowToDelete);
        ui->tableWidget_TestsEnAttente->removeRow(rowToDelete);
    }
}

void et_listeAttenteTests::buttonExecuterClicked()
{
    verifierTestsExecutables();
    connect(m_timer,SIGNAL(timeout()),this,SLOT(verifierTestsExecutables()));
    m_timer->start();
}

void et_listeAttenteTests::verifierTestsExecutables()
{
    QCoreApplication::processEvents();

    QMapIterator<ushort, QPointer<et_ParamsTest> > it_listeTestEnAttente(m_listeParamsTestsEnAttente);

    while(it_listeTestEnAttente.hasNext()) {
        it_listeTestEnAttente.next();
        QPointer<et_ParamsTest>  paramTest = it_listeTestEnAttente.value();

        if(paramTest->m_debutImmediat || paramTest->m_dateHeureDebutPrevu <= QDateTime::currentDateTime()) {
            bool conflit=false;

            if(m_listeParamsTestsEnCours.isEmpty())
                emit(lancementTest(it_listeTestEnAttente.key()));
            else {
                QMapIterator<ushort, QPointer<et_ParamsTest> > it_listeTestEnCours(m_listeParamsTestsEnCours);
                while(it_listeTestEnCours.hasNext()) {
                    it_listeTestEnCours.next();

                    if(paramTest->testerConflit(it_listeTestEnCours.value(),m_bdHandler))
                        conflit=true;
                }
                if(!conflit)
                    emit(lancementTest(it_listeTestEnAttente.key()));
            }
        }
    }

    if(!m_listeParamsTestsEnCours.isEmpty()) {
        ui->button_ExecuterTests->setEnabled(false);
    }
    else
        ui->button_ExecuterTests->setEnabled(true);
}

void et_listeAttenteTests::buttonAnnulerClicked()
{
    QMessageBox msgBox;
    msgBox.setText("Annuler?");
    msgBox.setInformativeText("Voulez-vous annuler et revenir à l'acceuil?");
    msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);

    if(msgBox.exec()==QMessageBox::Ok)
        emit(this->fermeture());
}

void et_listeAttenteTests::miseEnAttente(QPointer<et_ParamsTest> paramsTest)
{
    ui->h_Layout_et_InterfaceExecutionTest->removeWidget(m_interfaceparamsTest);

    QTableWidgetItem* item_idtest = new QTableWidgetItem(QString::number(paramsTest->m_test->getIdTest()));
    QTableWidgetItem* item_nomTest = new QTableWidgetItem(paramsTest->m_nomTest);
    QTableWidgetItem* item_DateHeureDebut = new QTableWidgetItem(paramsTest->m_dateHeureDebutPrevu.toString("dd/MM/yy hh:mm:ss"));
    QTableWidgetItem* item_Etat = new QTableWidgetItem("En attente");
    QTableWidgetItem* item_Interface = new QTableWidgetItem(paramsTest->m_interfaceCalibrateur);
    QTableWidgetItem* item_Canal = new QTableWidgetItem(paramsTest->m_canalCalibrateur);

    ushort idxNewRecord = this->ui->tableWidget_TestsEnAttente->rowCount();
    this->ui->tableWidget_TestsEnAttente->insertRow(idxNewRecord);
    this->ui->tableWidget_TestsEnAttente->setItem(idxNewRecord,ET_TABLEW_TEST_ATTENTE_ID_TEST,item_idtest);
    this->ui->tableWidget_TestsEnAttente->setItem(idxNewRecord,ET_TABLEW_TEST_ATTENTE_NOM_TEST,item_nomTest);
    this->ui->tableWidget_TestsEnAttente->setItem(idxNewRecord,ET_TABLEW_TEST_ATTENTE_DATE_HEURE_DEBUT,item_DateHeureDebut);
    this->ui->tableWidget_TestsEnAttente->setItem(idxNewRecord,ET_TABLEW_TEST_ATTENTE_CANAL_CALIBRATEUR,item_Etat);
    this->ui->tableWidget_TestsEnAttente->setItem(idxNewRecord,ET_TABLEW_TEST_ATTENTE_INTERFACE_CALIBRATEUR,item_Interface);
    this->ui->tableWidget_TestsEnAttente->setItem(idxNewRecord,ET_TABLEW_TEST_ATTENTE_ETAT,item_Canal);

    m_listeParamsTestsEnAttente.insert(idxNewRecord,paramsTest);

    delete m_interfaceparamsTest;
    ui->stackedWidget->setCurrentIndex(1);
}

void et_listeAttenteTests::lancerTest(ushort idxTest)
{
    QCoreApplication::processEvents();

    QPointer<et_InfosTestEnCours> infosTestEnCours = new et_InfosTestEnCours(m_bdHandler,this);

    m_listeInfosTestsEnCours.insert(idxTest,infosTestEnCours);

    ui->tabWidget->addTab(infosTestEnCours,m_listeParamsTestsEnAttente.value(idxTest)->m_nomTest);
    int nbtab = ui->tabWidget->count();
    ui->tabWidget->setCurrentIndex(nbtab-1);

    QPointer<et_ParamsTest> paramsTest = m_listeParamsTestsEnAttente.value(idxTest);
    infosTestEnCours->enregistrerParamsTest(paramsTest);

    QPointer<ExecutionTest> testAExecuter = new ExecutionTest(m_listeParamsTestsEnAttente.value(idxTest),m_bdHandler,idxTest);

    connect(testAExecuter,SIGNAL(traceTest(QString,ushort)),infosTestEnCours,SLOT(afficherTraceTest(QString,ushort)));

    m_listeParamsTestsEnCours.insert(idxTest,m_listeParamsTestsEnAttente.value(idxTest));
    m_listeParamsTestsEnAttente.remove(idxTest);

//    QPointer<QThreadEx> threadExecution = new QThreadEx();

//    m_listeThreadsExecution.insert(idxTest,threadExecution);
    m_listeTestsEnExecution.insert(idxTest,testAExecuter);

//    testAExecuter->moveToThread(threadExecution);

//    connect(threadExecution,SIGNAL(started()),testAExecuter,SLOT(run()));
//    connect(testAExecuter,SIGNAL(exitTest()),threadExecution,SLOT(quit()));
    connect(testAExecuter,SIGNAL(killMeAndMyThread(short)),this,SLOT(killExecutionTestEtThread(short)));
    testAExecuter->run();
//    threadExecution->start();
}

void et_listeAttenteTests::killExecutionTestEtThread(const short id)
{

    this->ui->tabWidget->setCurrentIndex(0); //astuce trouvée pour que les ports séries se ferment bien entre deux tests


    QPointer<ExecutionTest> testEnCours = m_listeTestsEnExecution.value(id);
    if(!testEnCours.isNull())
        delete testEnCours;
    m_listeTestsEnExecution.remove(id);

//    QPointer<QThreadEx> thread = m_listeThreadsExecution.value(id);
//    if(!thread.isNull()) {
//        thread->quit();
//        thread->wait();
//        delete thread;
//    }
//    m_listeThreadsExecution.remove(id);
    QPointer<et_ParamsTest> paramsTest = m_listeParamsTestsEnCours.value(id);
    if(!paramsTest.isNull())
        delete paramsTest;
    m_listeParamsTestsEnCours.remove(id);
}

void et_listeAttenteTests::testAnnule()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->h_Layout_et_InterfaceExecutionTest->removeWidget(m_interfaceparamsTest);
    if(!m_interfaceparamsTest.isNull()) {
        delete m_interfaceparamsTest;
    }
}
