#include "generateurrapporttest.h"
#include <math.h>
#include <stdlib.h>

GenerateurRapportTest::GenerateurRapportTest(const QWeakPointer<et_ParamsTest> paramsTest,
                                             const QWeakPointer<QVector<MesureInfo> > tableauMesures,
                                             const QWeakPointer<BdHandler> bdHandler)
{
    m_tableauMesures = tableauMesures;
    m_paramsTest = paramsTest;
    m_bdHandler = bdHandler;
}

bool GenerateurRapportTest::genererRapport()
{
    switch(m_paramsTest.data()->m_test->getTypeTest()) {
    case REPETABILITE_1:
        return genererRapportRepetabilite_1();
        break;
    case REPETABILITE_2:
        return genererRapportRepetabilite_2();
        break;
    case LINEARITE:
        return genererRapportLinearite();
        break;
    case TEMPS_REPONSE:
        return genererRapportTempsReponse();
        break;
    case RENDEMENT_FOUR:
        return genererRapportRendementFour();
        break;
    default:
        return true;
    }
}

bool GenerateurRapportTest::genererRapportRepetabilite_1()
{

    return true;
}

bool GenerateurRapportTest::genererRapportRepetabilite_2()
{

    return true;
}

bool GenerateurRapportTest::genererRapportLinearite()
{
    ushort nbPolluants = m_tableauMesures.data()->at(0).mesure.data()->count();
    ushort nbMesures =  m_tableauMesures.data()->count();

    QVector<float> tabPentes,tabOrdonnees,tabConcentrationsParPhase;
    QVector<QVector<float> > tabResidusAbsolus;

    for(ushort idxPolluants=0;idxPolluants<nbPolluants;idxPolluants++) {
        QVector<float> tabMoyennesParPhase;
        ushort noPhase=1;
        ushort nbMesuresParPhase=1;
        float moyenneParPhase = m_tableauMesures.data()->at(0).mesure.data()->at(idxPolluants);
        for(ushort idxMesures=1;idxMesures<nbMesures;idxMesures++) {
            if(noPhase<m_tableauMesures.data()->at(idxMesures).noPhase) {
                QSqlRecord* record = m_bdHandler.data()->getConcentrationRow(m_paramsTest.data()->m_test->getPhase(noPhase).getIdConcentration());
                if(m_bdHandler.data()->getConcentrationRow(m_paramsTest.data()->m_test->getPolluants().at(0)=="O3"))
                    tabConcentrationsParPhase.append(record->value(CONCENTRATION_OZONE).toFloat());
                else
                    tabConcentrationsParPhase.append(record->value(CONCENTRATION_REELLE).toFloat());
                noPhase++;
                tabMoyennesParPhase.append(moyenneParPhase/nbMesuresParPhase);
                moyenneParPhase = m_tableauMesures.data()->at(idxMesures).mesure.data()->at(idxPolluants);
                nbMesuresParPhase=1;
            }
            else {
                nbMesuresParPhase++;
                moyenneParPhase+=m_tableauMesures.data()->at(idxMesures).mesure.data()->at(idxPolluants);
            }
        }
        tabPentes.append(pente(tabConcentrationsParPhase,tabMoyennesParPhase,tabConcentrationsParPhase.count()));
        tabOrdonnees.append(pente(tabConcentrationsParPhase,tabMoyennesParPhase,tabConcentrationsParPhase.count()));
        QVector<float> tabTempCalculResidu;
//        tabTempCalculResidu.append(tabConcentrationsParPhase.at(idxPolluants)
    }


    return true;
}

bool GenerateurRapportTest::genererRapportTempsReponse()
{

    return true;
}

bool GenerateurRapportTest::genererRapportRendementFour()
{

    return true;
}

int GenerateurRapportTest::test_zero(QVector<float> &tab)
{
    for(int i=0;i<tab.count();i++)
    {
            if(tab[i]==0)
            { return i;}
    }

    return -1;
}

int GenerateurRapportTest::test_negatif(QVector<float> &tab)
{
    for(int i=0;i<tab.count();i++)
    {
            if(tab[i]<0)
            { return i;}
    }

    return -1;
}

float GenerateurRapportTest::val_abs(float x)
{
    if(x<0) {return -x;}
    else {return x;}
}

int GenerateurRapportTest::rmax(QVector<float> &r)
{
    float temp=0;
    int ajust=0;

    for(int i=0;i<5;i++)
    {
            if(r[i]>temp)
            {
                 temp=r[i];
                 ajust = i+1;
            }
    }

    return ajust;
}

int GenerateurRapportTest::somme(QVector<int> &tab)
{
    int somme=0;

    for (int i=0;i<tab.count();i++)
    {
     somme=((tab[i])+(somme));
    }

    return(somme);
}

float GenerateurRapportTest::somme(QVector<float> &tab)
{
    float somme=0;

    for (int i=0;i<tab.count();i++)
    {
     somme=((tab[i])+(somme));
    }

    return(somme);
}

float GenerateurRapportTest::moyenne(QVector<int> &tab,int n)
{
    float moyenne = float(somme(tab))/n;

    return (moyenne);
}

float GenerateurRapportTest::moyenne(QVector<float> &tab,int n)
{
    float moyenne = somme(tab)/n;

    return (moyenne);
}

float GenerateurRapportTest::moyenne2(float somme,int n)
{
    float moyenne = somme/n;

    return (moyenne);
}

void GenerateurRapportTest::produittab(QVector<float> &tab1,QVector<float> &tab2,int n)
{
    m_tabTemp.resize(n);

    for (int i=0;i<n;i++)
    {
        m_tabTemp[i]=(tab1[i]*tab2[i]);
    }
}

void GenerateurRapportTest::lntab(QVector<float> &tab,QVector<float> &tabTemp,int n)
{
    tabTemp.resize(n);

    for (int i=0;i<n;i++)
    {
            tabTemp[i]=(log(tab[i]));
    }
}

void GenerateurRapportTest::logtab(QVector<float> &tab,QVector<float> &tabTemp,int n)
{
    tabTemp.resize(n);

    for (int i=0;i<n;i++)
    {
            tabTemp[i]=(log10(tab[i]));
    }
}

void GenerateurRapportTest::invtab(QVector<float> &tab,QVector<float> &tabTemp,int n)
{
    tabTemp.resize(n);

    for (int i=0;i<n;i++)
    {
            tabTemp[i]=(1/tab[i]);
    }
}

void GenerateurRapportTest::ecart_a_moyenne(QVector<float> &tab,float Moyenne,int n)
{
    m_tabTemp.resize(n);

    for (int i=0;i<n;i++)
    {
            m_tabTemp[i]=(tab[i] - Moyenne);
    }
}

float GenerateurRapportTest::covariance(QVector<float> &Xi,QVector<float> &Yi,int n)
{
    float cov;

    produittab(Xi,Yi,n);
    cov = moyenne(m_tabTemp,n) - ( moyenne(Xi,n) * moyenne(Yi,n) );

    return (cov);
}

float GenerateurRapportTest::variance(QVector<float> &val,int n)
{
    float sce;

    produittab(val,val,n);
    sce = moyenne(m_tabTemp,n) - ( moyenne(val,n) * moyenne(val,n));

    return (sce);
}

float GenerateurRapportTest::ecarttype(QVector<float> &val,int n)
{
    float ect= sqrt(variance(val,n));

    return (ect);
}

float GenerateurRapportTest::pente(QVector<float> &Xi,QVector<float> &Yi,int n)
{
    float a = covariance(Xi,Yi,n)/variance(Xi,n);

    return (a);
}

float GenerateurRapportTest::ordonnee(QVector<float> &Xi,QVector<float> &Yi,int n)
{
    float b = moyenne(Yi,n) - ( pente(Xi,Yi,n) * moyenne(Xi,n) );

    return (b);
}

float GenerateurRapportTest::corr(QVector<float> &Xi,QVector<float> &Yi,int n)
{
    float r = covariance(Xi,Yi,n)/(ecarttype(Xi,n)*ecarttype(Yi,n));
    //float r=pente(Xi,Yi,n)*pente(Xi,Yi,n)*(variance(Xi,n)/variance(Yi,n));
    return (r);
}

int GenerateurRapportTest::ajustement(QVector<float> &Xi,QVector<float> &Yi,int n)
{
    QVector<float> r(5);
    QVector<float> lnXi(100);
    QVector<float> lnYi(100);
    QVector<float> logXi(100);
    QVector<float> logYi(100);
    QVector<float> invXi(100);

    //corrélation pour linéaire

    r[0]=val_abs(corr(Xi,Yi,n));

    //corrélation pour exponetielle

    lntab(Yi,lnYi,n);
    r[1]=val_abs(corr(Xi,lnYi,n));

    //corrélation pour puissance

    logtab(Xi,logXi,n);
    logtab(Yi,logYi,n);
    r[2]=val_abs(corr(logXi,logYi,n));

    //corrélation pour inverse

    invtab(Xi,invXi,n);
    r[3]=val_abs(corr(invXi,Yi,n));

    //corrélation pour logarithmique

    lntab(Xi,lnXi,n);
    r[4]=val_abs(corr(lnXi,Yi,n));

    //Test du meilleur ajustement

    return rmax(r);
}
