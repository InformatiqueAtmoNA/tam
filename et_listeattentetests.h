/*////////////////////////////////////////////////////////////
// \file et_listeattentetests.h
// \brief Classe d'interface gérant la liste d'attente des tests en cours
// \author FOUQUART Christophe
// \version 1.0
// \date 10/08/2011
//
// TAM - Tests Automatiques Métrologiques
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

#ifndef ET_LISTEATTENTETESTS_H
#define ET_LISTEATTENTETESTS_H

#include <QWidget>
#include "et_infostestencours.h"
#include "dlg_testxml.h"
#include "et_interfaceexecutiontest.h"

namespace Ui {
    class et_listeAttenteTests;
}

class et_listeAttenteTests : public QWidget
{
    Q_OBJECT

public:
    explicit et_listeAttenteTests(QPointer<BdHandler> bdHandler, QWidget *parent = 0);
    ~et_listeAttenteTests();

private:
    Ui::et_listeAttenteTests *ui;

    QPointer<BdHandler> m_bdHandler;
    QMap<ushort, QPointer<et_ParamsTest> > m_listeParamsTestsEnAttente;
    QMap<ushort, QPointer<et_ParamsTest> > m_listeParamsTestsEnCours;
    QMap<ushort, QPointer<et_InfosTestEnCours> > m_listeInfosTestsEnCours;
    QMap<ushort, QPointer<QThreadEx> > m_listeThreadsExecution;
    QMap<ushort, QPointer<ExecutionTest> > m_listeTestsEnExecution;
    QPointer<et_InterfaceExecutionTest> m_interfaceparamsTest;
    ushort m_nbPagesTest;
    QPointer<QTimer> m_timer;

private Q_SLOTS:
    void buttonAjouterTestClicked();
    void buttonSupprimerTestClicked();
    void buttonExecuterClicked();
    void buttonAnnulerClicked();
    void verifierTestsExecutables();
    void testAnnule();
    void lancerTest(ushort idxTest);
    void killExecutionTestEtThread(const short id);
    void miseEnAttente(QPointer<et_ParamsTest> paramsTest);

Q_SIGNALS:
    void fermeture();
    void lancementTest(ushort idxTest);
};

#endif // ET_LISTEATTENTETESTS_H
