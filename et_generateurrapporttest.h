/*////////////////////////////////////////////////////////////
// \file et_generateurrapporttest.h
// \brief Classe d'interface gérant la génération des rapports des tests
// \author PIET Régis
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

#ifndef ET_GENERATEURRAPPORTTEST_H
#define ET_GENERATEURRAPPORTTEST_H

#include <QWidget>
#include "et_resultatpolluant.h"
#include "et_paramsTest.h"
#include "definitions_globales.h"
#include "bdhandler.h"


namespace Ui {
    class et_GenerateurRapportTest;
}

class et_GenerateurRapportTest: public QWidget
{
    Q_OBJECT

private:
    Ui::et_GenerateurRapportTest *ui;
    QPointer<BdHandler> m_bdHandler;
    QVector<QVector<float> > m_tabMesures;
    QVector <float> m_tabConcentration;
    QVector <float> m_tabMoyenne;
    QVector <float> m_tabEcartType;
    QVector<int> mtabPoints;
    QVector<float> m_tabTemp;
    ushort m_idTest;
    ushort m_idAnalyseur;
    ushort m_typeTest;
    ushort m_nbPolluantTest;
    QPointer<QSqlQueryModel> m_TestPhase;
    QPointer<QSqlQueryModel> m_ConcTestAnalyseur;
    QPointer<QSqlQueryModel> m_MesureTestAnalyseur;
    QPointer<QSqlQueryModel> m_PolluantTest;
    QPointer<QSqlQueryModel> m_CyclePhaseTest;
    float m_pente;
    float m_ordonnee;
    QVector <float> m_tabResidu;
    float m_ResiduZero;
    float m_ResiduMax;
    QVector <float> m_tabResiduRel;
    QVector <float> m_tabResiduInc;
    QString m_NomTest;
    QString m_Operateur;
    QDateTime m_DateDeb;
    QDateTime m_DateFin;
    QString m_Lieu;
    QString m_Temperature;
    QString m_Pression;
    QStringList m_listeEnteteLigne;
    QVector <float>  m_tabValeurPourCritere;
    QVector <int> m_tabCodeMolecule;
    QVector<QVector<float> > m_tabMoyenneCalculTPG;
    QVector <float> m_tabResultatTPG;
    QStringList m_listeEnteteLigneTPG;
    QVector<QVector<float> > m_tabTpsReponse;
    QVector <float> m_tabMoyenneTpsReponse;
    QVector <int> m_tabNbreAcquisition;
    int m_tpsAcquisition;
    QVector<double> m_tabCritere;


    void tableauMesure2(int idMolecule, int codeMolecule);
    void tableauMesure(int idMolecule, int codeMolecule);
    void tableauMesure(int no_Phase);
    void affichageTableauResidu ();
    void affichageTableauTpsReponse ();
    void affichageEquipement(ushort idEquipement,QString nomLigne);

    bool genererRapport();
    bool genererRapportRepetabilite();
    bool genererRapportLinearite();
    bool genererRapportTempsReponse();
    bool genererRapportRendementFour();

    int test_zero(QVector<float> &tab);
    int test_negatif(QVector<float> &tab);
    float rmax(QVector<float> &Xi,QVector<float> &Yi);
    int somme(QVector<int> &tab);
    float somme(QVector<float> &tab);
    float val_abs(float x);
    float moyenne(QVector<int> &tab,int n);
    float moyenne(QVector<float> &tab,int n);
    float moyenne2(float somme,int n);
    void produittab(QVector<float> &tab1,QVector<float> &tab2,int n);
    void ecart_a_moyenne(QVector<float> &tab,float Moyenne,int n);
    float covariance(QVector<float> &Xi,QVector<float> &Yi,int n);
    float variance(QVector<float> &val,int n);
    float ecarttype(QVector<float> &val,int n);
    void logtab(QVector<float> &tab,QVector<float> &tabTemp,int n);
    void lntab(QVector<float> &tab,QVector<float> &tabTemp,int n);
    void invtab(QVector<float> &tab,QVector<float> &tabTemp,int n);
    float pente(QVector<float> &Xi,QVector<float> &Yi,int n);
    float ordonnee(QVector<float> &Xi,QVector<float> &Yi,int n);
    float corr(QVector<float> &Xi,QVector<float> &Yi,int n);
    float calculTpsReponseMontee(QVector<float> &Xi,float moyenne,int tpsAcq);
    float calculTpsReponseDescente(QVector<float> &Xi,float moyenne,int tpsAcq);

public:
    explicit et_GenerateurRapportTest(QPointer<BdHandler> bdHandler,const ushort idTest,const ushort idAnalyseur,const ushort typeTest,QWidget *parent = 0);
    ~et_GenerateurRapportTest();

private Q_SLOTS:
    void buttonFermerClicked();

Q_SIGNALS:
    void fermeture(int index);
};
#endif // ET_GENERATEURRAPPORTTEST_H
