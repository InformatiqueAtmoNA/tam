/*////////////////////////////////////////////////////////////
// \file homewidget.h
// \brief Widget d'interface graphique de la fenetre principale
// \author FOUQUART Christophe
// \version 1.0
// \date 26/06/2011
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
#ifndef HOMEWIDGET_H
#define HOMEWIDGET_H

#include <QWidget>
#include "creationtest.h"
#include "executiontest.h"

namespace Ui {
    class HomeWidget;
}

class HomeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HomeWidget(QPointer<BdHandler> bdHandler,QWidget *parent = 0);
    ~HomeWidget();

private:
    Ui::HomeWidget *ui;
    QPointer<BdHandler> m_bdHandler;
    QPointer<QSqlTableModel> m_modelTestXml;
    QModelIndex m_idxSelectionTest;

    void getListeTests();

private Q_SLOTS:
    void tableWidgetTestXmlIndexChanged(const QModelIndex & index);
    void buttonNouveauClicked();
    void buttonModifierClicked();
    void buttonExecuterClicked();
    void buttonAfficherClicked();
    void buttonSupprimerClicked();

Q_SIGNALS:
    void creerTest();
    void modifierTest(const QString fichierDescription);
    void executerTest(const ushort idTestXML, const QString fichierDescription);
    void programmerSerieTest();
};

#endif // HOMEWIDGET_H
