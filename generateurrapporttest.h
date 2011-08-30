#ifndef GENERATEURRAPPORTTEST_H
#define GENERATEURRAPPORTTEST_H

#include <QObject>
#include "et_paramsTest.h"
#include "definitions_globales.h"

class GenerateurRapportTest
{
private:
    QSharedPointer<QVector<MesureInfo> > m_tableauMesures;
    QWeakPointer<et_ParamsTest> m_paramsTest;
    QWeakPointer<BdHandler> m_bdHandler;

public:
    explicit GenerateurRapportTest(const QWeakPointer<et_ParamsTest> paramsTest,
                                   const QSharedPointer<QVector<MesureInfo> > tableauMesures,
                                   const QWeakPointer<BdHandler> bdHandler);

    bool genererRapport();
    bool genererRapportRepetabilite_1();
    bool genererRapportRepetabilite_2();
    bool genererRapportLinearite();
    bool genererRapportTempsReponse();
    bool genererRapportRendementFour();
};

#endif // GENERATEURRAPPORTTEST_H
