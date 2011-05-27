/*////////////////////////////////////////////////////
// \file creationtest.h
// \brief Classe d'interface graphique gérant la création des tests
// \author FOUQUART Christophe
// \version 1.0
// \date 01/04/2011
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
////////////////////////////////////////////////////*/

#ifndef CREATIONTEST_H
#define CREATIONTEST_H

#include <QWidget>
#include "ct_phasewidget.h"
#include "test.h"
#include "dlg_systeme_etalon.h"

namespace Ui {
    class CreationTest;
}

class CreationTest : public QWidget
{
    Q_OBJECT

public:
    explicit CreationTest(const QPointer<BdHandler> bdHandler,QWidget *parent = 0,
                          const QString nomFichier="",const QPointer<Test> test=0);
    ~CreationTest();

private:
    Ui::CreationTest *ui;
    QPointer<Test> m_test;
    QString m_nomFichier;
    QPointer<BdHandler> m_bdHandler;
    TypeTest m_typeTest;
    int m_indexTypeTest;
    uint m_nbPhases;
    QPointer<ct_PhaseWidget> m_phaseWidget;
    ushort m_etape;
    bool m_autoriserCreationPhase;

    void initialiserChamps();
    void afficherPhaseWidget(const ushort noPhase, bool readOnly=false);
    void incrementerNbPhases();
    void decrementerNbPhases();

private slots:
    inline void timeEdit_TempsMaxTestValueChanged(const QTime & time) {this->m_test->setTempsMaxTest(time);}
    inline void spinBox_TempsAcquisitionValueChanged(const int value) {this->m_test->setTempsAcquisition(value);}
    inline void spinBox_nbCyclesMesuresValueChanged(const int value) {this->m_test->setNbCyclesMesureParPhase(value);}
    inline void spinBox_nbCyclesPhasesValueChanged(const int value) {this->m_test->setNbCyclesDePhase(value);}
    inline void timeEdit_TempsStabilisation(const QTime & time) {this->m_test->setTempsStabilisation(time);}
    inline void timeEdit_TempsMoyennageMesuresValueChanged(const QTime & time) {this->m_test->setTempsMoyennageMesure(time);}
    inline void timeEdit_TempsAttenteEntreMesureValueChanged(const QTime & time) {this->m_test->setTempsAttenteEntreMesure(time);}

    void button_AjouterPhaseClicked();
    void button_InsererPhaseClicked();
    void button_SupprimerPhaseClicked();
    void button_ModifierPhaseClicked();
    void button_ChoixSystemeEtalonClicked();
    void button_AnnulerClicked();
    void button_SuivantClicked();
    void button_PrecedentClicked();
    void button_SauvegarderClicked();
    void tabWidgetIndexChanged(const int index);
    void cb_ChoixTypeTestIndexChanged(const int index);
    void listWidgetCurrentTextChanged(const QString & text);
    void listWidgetItemDoubleClicked(QListWidgetItem* item);
    void listWidgetCurrentRowChanged(const int row);
    void lineEditNomTestTextChanged(const QString nomTest);

public slots:
    void editionPhaseAnnulee();
    void editionPhaseValidee(const Phase & phase);

signals:
    void fermeture();
};

#endif // CREATIONTEST_H
