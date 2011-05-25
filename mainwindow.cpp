/*////////////////////////////////////////////////////////////
// \file mainwindow.cpp
// \brief Classe d'interface graphique de la fenetre principale
// \author FOUQUART Christophe
// \version 1.0
// \date 25/03/2011
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


#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString driver = getParam("BD_Driver").toString();
    QString host = getParam("Host").toString();
    QString userName = getParam("UserName").toString();
    QString password = getParam("Password").toString();
    QString dbName = getParam("DB_Name").toString();

    m_bdHandler = new BdHandler(driver,host,userName,password,dbName);
    m_bdHandler->connexionBD();

    m_gridLayout = new QGridLayout();

    this->setLayout(m_gridLayout);

    this->afficherHomeWidget();
}

MainWindow::~MainWindow()
{
    if(!this->m_bdHandler.isNull())
        delete this->m_bdHandler;
    if(!this->m_dlg_test.isNull())
        delete this->m_dlg_test;
    if(!this->m_homeWidget.isNull())
        delete this->m_homeWidget;
    if(!this->m_gridLayout.isNull())
        delete this->m_gridLayout;

    delete ui;
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

void MainWindow::afficherHomeWidget()
{
    if(!this->m_dlg_test.isNull()) {
        this->m_gridLayout->removeWidget(m_dlg_test);
        delete this->m_dlg_test;
    }
    m_homeWidget = new HomeWidget(m_bdHandler,this);
    this->m_gridLayout->addWidget(m_homeWidget,0,0);
    m_homeWidget->show();

    connect(this->m_homeWidget,SIGNAL(creerTest()),this,SLOT(nouveauTest()));
    connect(this->m_homeWidget,SIGNAL(modifierTest(QString)),this,SLOT(modifierTest(QString)));
    connect(this->m_homeWidget,SIGNAL(executerTest(QString)),this,SLOT(executerTest(QString)));
    connect(this->m_homeWidget,SIGNAL(creerTest()),this,SLOT(nouveauTest()));
}

void MainWindow::afficherCreationTest(const QString fichierDescription)
{
    if(!this->m_homeWidget.isNull()) {
        this->m_gridLayout->removeWidget(m_homeWidget);
        delete m_homeWidget;
    }
    if(fichierDescription.isEmpty())
        this->m_dlg_test = new CreationTest(m_bdHandler,this);
    else
        this->m_dlg_test = new CreationTest(m_bdHandler,this,fichierDescription);
    this->m_gridLayout->addWidget(m_dlg_test,0,0);

    m_dlg_test->show();

    connect(this->m_dlg_test,SIGNAL(fermeture()),this,SLOT(fermetureCreationTestWidget()));
}


void MainWindow::nouveauTest()
{
    this->afficherCreationTest();
}

void MainWindow::modifierTest(const QString fichierDescription)
{
    this->afficherCreationTest(fichierDescription);
}

void MainWindow::executerTest(const QString fichierDescription)
{
    qDebug()<<"execution du test "<<fichierDescription<<" pas encore développé";
}

void MainWindow::fermetureCreationTestWidget()
{
    this->afficherHomeWidget();
}
