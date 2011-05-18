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

    QString driver = this->getParam("BD_Driver").toString();
    QString host = this->getParam("Host").toString();
    QString userName = this->getParam("UserName").toString();
    QString password = this->getParam("Password").toString();
    QString dbName = this->getParam("DB_Name").toString();

    m_bdHandler = new BdHandler(driver,host,userName,password,dbName);

    CreationTest* dlg_test = new CreationTest(m_bdHandler,this);
    this->ui->gridLayout->addWidget(dlg_test,0,0);
}

MainWindow::~MainWindow()
{
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

QVariant MainWindow::getParam(QString const & key) {
    QSettings parametres("params.ini",QSettings::IniFormat);
    return parametres.value(key);
}

void MainWindow::setParam(QString const & key, QVariant const & param) {
    QSettings parametres("params.ini",QSettings::IniFormat);
    parametres.setValue(key,param);
    parametres.sync();
}

void MainWindow::pushButtonClicked() {
    Dlg_Systeme_Etalon dlg_test(this,m_bdHandler);
    dlg_test.exec();

}
