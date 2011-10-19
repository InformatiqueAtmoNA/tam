/*////////////////////////////////////////////////////////////
// \file et_infostestencours.h
// \brief Classe regroupant les infos d'exécution d'un test
// \author FOUQUART Christophe
// \version 1.0
// \date 11/08/2011
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
#ifndef ET_INFOSTESTENCOURS_H
#define ET_INFOSTESTENCOURS_H

#include <QWidget>
#include <QDebug>
#include "et_paramsTest.h"
#include "bdhandler.h"

namespace Ui {
    class et_InfosTestEnCours;
}

class et_InfosTestEnCours : public QWidget
{
    Q_OBJECT

public:
    explicit et_InfosTestEnCours(QPointer<BdHandler> bdHandler,QWidget *parent = 0);
    ~et_InfosTestEnCours();

    void enregistrerParamsTest(QPointer<et_ParamsTest> paramsTest);

private:
    Ui::et_InfosTestEnCours *ui;

    QPointer<BdHandler> m_bdHandler;
    et_ParamsTest m_param_Test;
    ushort m_niveauInfos;
    void afficherParamsTest(QPointer<et_ParamsTest> paramsTest);
    void enregistrerAnalyseurTest(QPointer<et_ParamsTest> paramsTest);
    void enregistrerConcTestMetro(QPointer<et_ParamsTest> paramsTest);

public Q_SLOTS:
    void afficherTraceTest(const QString trace, const ushort niveau);

private Q_SLOTS:
    void comboBoxNiveauInfosIndexChanged(const int index);
};

#endif // ET_INFOSTESTENCOURS_H
