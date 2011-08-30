#include "generateurrapporttest.h"

GenerateurRapportTest::GenerateurRapportTest(const QWeakPointer<et_ParamsTest> paramsTest,
                                             const QSharedPointer<QVector<MesureInfo> > tableauMesures,
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
