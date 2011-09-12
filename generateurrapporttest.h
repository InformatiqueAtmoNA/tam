#ifndef GENERATEURRAPPORTTEST_H
#define GENERATEURRAPPORTTEST_H

#include <QObject>
#include "et_paramsTest.h"
#include "definitions_globales.h"

class GenerateurRapportTest
{
private:
    QWeakPointer<QVector<MesureInfo> > m_tableauMesures;
    QWeakPointer<et_ParamsTest> m_paramsTest;
    QWeakPointer<BdHandler> m_bdHandler;
    QVector<float> m_tabTemp;

    int test_zero(QVector<float> &tab);
    int test_negatif(QVector<float> &tab);
    int rmax(QVector<float> &r);
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
    int ajustement(QVector<float> &Xi,QVector<float> &Yi,int n);

public:
    explicit GenerateurRapportTest(const QWeakPointer<et_ParamsTest> paramsTest,
                                   const QWeakPointer<QVector<MesureInfo> > tableauMesures,
                                   const QWeakPointer<BdHandler> bdHandler);

    bool genererRapport();
    bool genererRapportRepetabilite_1();
    bool genererRapportRepetabilite_2();
    bool genererRapportLinearite();
    bool genererRapportTempsReponse();
    bool genererRapportRendementFour();
};
#endif // GENERATEURRAPPORTTEST_H
