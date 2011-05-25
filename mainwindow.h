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
#include "creationtest.h"
#include "homewidget.h"

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
    QPointer<CreationTest> m_dlg_test;
    QPointer<QGridLayout> m_gridLayout;

    void changeEvent(QEvent *e);
    void afficherHomeWidget();
    void afficherCreationTest(const QString fichierDescription="");

private:
    Ui::MainWindow *ui;

public slots:
    void nouveauTest();
    void modifierTest(const QString fichierDescription);
    void executerTest(const QString fichierDescription);
    void fermetureCreationTestWidget();
};

#endif // MAINWINDOW_H
