/*////////////////////////////////////////////////////////////
// \file et_interfaceexecutiontest.h
// \brief Classe d'interface gérant l'exécution des tests
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

#ifndef ET_INTERFACEEXECUTIONTEST_H
#define ET_INTERFACEEXECUTIONTEST_H

#include <QWidget>

#include "bdhandler.h"
#include "test.h"
#include "dlg_equipement.h"
#include "protocole.h"
#include "executiontest.h"
#include "et_infostestencours.h"
#include <QFileDialog>

namespace Ui {
    class et_InterfaceExecutionTest;
}

class et_InterfaceExecutionTest : public QWidget
{
    Q_OBJECT

private:
    Ui::et_InterfaceExecutionTest *ui;
    QPointer<BdHandler> m_bdHandler;
    QPointer<et_InfosTestEnCours> m_infosTestEnCours;
    QThreadEx m_threadExecutionTest;
    ushort m_idOperateur;
    QPointer<QSqlQueryModel> m_modelTestXml;
    QPointer<QSqlTableModel> m_modelOperateur;
    QPointer<QSqlTableModel> m_modelLieu;
    QModelIndex m_idxSelectionAnalyseurs;
    QModelIndex m_idxCommunicationAnalyseurs;
    QString m_type_Socket;
    QString m_fichierDescription;
    ushort m_etape;
    ushort m_idTestXML;
    int m_idSonde;
    QPointer<Test> m_test;
    QMap<ushort,QString> m_listeInterfaceAnalyseurs;
    QMap<ushort,bool> m_listeEtatComAnalyseurs;
    QPointer<Protocole> m_appareilEnTest;
    bool m_etatComCalibrateur;
    bool m_etatComSonde;
    QPointer<ExecutionTest> m_testAExecuter;
    ushort m_niveauInfos;
    bool m_miseEnAttente;
    bool m_debutImmediat;
    bool m_sondePresente;
    QDateTime m_dateHeureDebutTest;
    QString m_nomCheminCSV;

    bool controleEtape1();
    bool controleEtape2();
    void afficherMsgBoxErreurCom();
    QPointer<et_ParamsTest> preparerInfosTest();
    QString m_typeConnexion;
    QString m_IP;
    quint16 m_numPort;
    QString m_typeSocket;

public:
    explicit et_InterfaceExecutionTest(QPointer<BdHandler> bdHandler,const ushort idTestXml,const QString fichierDescription,bool miseEnAttente=false,ushort idOperateur=0,QWidget *parent = 0);
    ~et_InterfaceExecutionTest();

private Q_SLOTS:
    void buttonAjouterClicked();
    void buttonAjouterSondeClicked();
    void buttonSupprimerSondeClicked();
    void buttonAnnulerClicked();
    void buttonPrecedentClicked();
    void buttonSuivantClicked();
    void buttonSupprimerClicked();
    void buttonTestAnalyseurClicked();
    void buttonTestCalibrateurClicked();
    void buttonTestSondeClicked();
    void tableWidgetCommunicationClicked(const QModelIndex index);
    void lineEditCanalCalibrateurTextChanged(const QString text);
    void lineEditInterfaceCalibrateurTextChanged(const QString text);
    void lineEditInterfaceAnalyseurTextChanged(const QString text);
    void lineEditInterfaceSondeTextChanged(const QString text);
    void lineEditParamsCalibrateurEnterPressed();
    void lineEditInterfaceAnalyseurEnterPressed();
    void lineEditInterfaceSondeEnterPressed();
    void erreurCommunicationAnalyseur();
    void erreurCommunicationCalibrateur();
    void erreurCommunicationSonde();
    void ouverturePortComAnalyseur(const bool ouverturePort);
    void ouverturePortComCalibrateur(const bool ouverturePort);
    void ouverturePortComSonde(const bool ouverturePort);
    void tabWidgetExecutionTestIndexChanged(const int index);
    void buttonExecuterClicked();
    void buttonMettreEnAttenteClicked();
    void bgChoixDebutTestValueChanged(int idButton);
    void dateTimeDebutTestValueChanged(QDateTime dateTimeDebutTest);
    inline void finTest() {delete m_testAExecuter;}
    inline void tableWidgetAnalyseursClicked(const QModelIndex index) {m_idxSelectionAnalyseurs = index;}
    void button_choixEnregistrementCSV();

Q_SIGNALS:
    void fermeture();
    void miseEnAttente(QPointer<et_ParamsTest> paramsTest);
};

#endif // ET_INTERFACEEXECUTIONTEST_H
