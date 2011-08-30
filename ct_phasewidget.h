/*////////////////////////////////////////////////////
// \file ct_phasewidget.h
// \brief Classe de widget gérant la création des phases de test
// \author FOUQUART Christophe
// \version 1.0
// \date 22/04/2011
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

#ifndef CT_PHASEWIDGET_H
#define CT_PHASEWIDGET_H

#include <QWidget>
#include "phase.h"
#include "dlg_concentration.h"

namespace Ui {
    class ct_PhaseWidget;
}

class ct_PhaseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ct_PhaseWidget(const int idSystemeEtalon,const TypeTest typeTest, const QPointer<BdHandler> bdHandler,
                            QWidget *parent = 0,const int noPhase=0,const Phase & phase = *(new Phase()));
    ~ct_PhaseWidget();

    inline const Phase& getPhase() {return m_phase;}
    bool isReadOnly();
    void setReadOnly(bool readOnly);
private:
    Ui::ct_PhaseWidget *ui;
    int m_idSystemeEtalon;
    Phase m_phase;
    Phase m_phaseOriginale;
    QModelIndex m_indexListePolluant;
    QPointer<BdHandler> m_bdHandler;
    bool m_isReadOnly;
    TypeTest m_typeTest;
    ushort m_idConcentration;

    void afficherPhase();
    void afficherPolluants(QSqlRecord* concentrationRecord,QSqlRecord* moleculeRecord);
private Q_SLOTS:
    inline void spinBoxNbCyclesMesuresValueChanged(const int value) {this->m_phase.setNbCyclesMesures(value);}
    inline void spinBox_NbCycleMesureArretValueChanged(const int value) {this->m_phase.setCritereArret_NbCyclesMesures(value);}
    inline void spinBoxPourcentageArretValueChanged(const int value) {this->m_phase.setCritereArret_PourcentageStabilisation(value);}
    inline void timeEditTempsMaxPhaseValueChanged(const QTime time) {this->m_phase.setTpsMaxPhase(time);}
    inline void timeEditTempsStabilisationValueChanged(const QTime time) {this->m_phase.setTpsStabilisation(time);}
    inline void timeEditTempsMoyennageMesuresValueChanged(const QTime time) {this->m_phase.setTpsMoyennageMesure(time);}
    inline void timeEditAttenteEntreMesuresValueChanged(const QTime time) {this->m_phase.setTpsAttenteEntreMesure(time);}
    inline void buttonAnnulerClicked() {m_phase = m_phaseOriginale;emit(this->annulation());}
    inline void buttonValiderClicked() {emit(this->phaseValidee(this->m_phase));}

    void tableViewPolluantsIndexChanged(const QModelIndex index);
    void cb_CmdDebutPhaseIndexChanged(const int index);
    void buttonAjouterPolluantClicked();
    void buttonSupprimerPolluantClicked();
    void ckb_CritereArretStateChanged(const int state);

Q_SIGNALS:
    void annulation();
    void phaseValidee(const Phase & phase);
};

#endif // CT_PHASEWIDGET_H
