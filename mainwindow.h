/*////////////////////////////////////////////////////////////
// \file mainwindow.h
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QSettings>
#include <QCloseEvent>
#include "creationtest.h"
#include "homewidget.h"
#include "et_interfaceexecutiontest.h"
#include "et_listeattentetests.h"
#include "dlg_lieu.h"
#include "dlg_operateur.h"
#include "dlg_port_serie.h"
#include "et_generateurrapporttest.h"
#include "parametreshandler.h"


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    QPointer<BdHandler> m_bdHandler;
    QPointer<HomeWidget> m_homeWidget;
    QPointer<CreationTest> m_dlgCreationtest;
    QPointer<et_InterfaceExecutionTest> m_dlgExecutionTest;
    QPointer<et_GenerateurRapportTest> m_dlgGenerateurRapportTest;
    QPointer<et_listeAttenteTests> m_dlgListeAttenteTest;
    ExecutionTest* testAExecuter;

    void changeEvent(QEvent *e);
    void afficherHomeWidget();
    void afficherCreationTest(const QString fichierDescription="");

private:
    Ui::MainWindow *ui;

public Q_SLOTS:
    void quitter();
    void closeEvent(QCloseEvent *event);
    void nouveauTest();
    void modifierTest(const QString fichierDescription);
    void executerTest(const ushort idTestXML, const QString fichierDescription);
    void afficherRapport(const ushort idTest,const ushort idAnalyseur, const ushort typeTest);
    void fermetureTestWidget();
    void afficherDlgEquipement();
    void afficherDlgLieu();
    void afficherDlgOperateur();
    void afficherDlgPortSerie();
    void programmerSerieTests();
    void afficherParametres();
};

#endif // MAINWINDOW_H
