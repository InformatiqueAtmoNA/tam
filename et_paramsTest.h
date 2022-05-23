/*////////////////////////////////////////////////////////////
// \file et_paramstest.h
// \brief Classe regroupant les infos d'exécution d'un test
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
#ifndef ET_PARAMSTEST_H
#define ET_PARAMSTEST_H

#include <QMultiMap>
#include "test.h"
#include "bdhandler.h"

class et_ParamsTest : public QObject {
    Q_OBJECT

public :
    ushort m_id_TestMetro;
    ushort m_id_TestXML;
    ushort m_idSonde;
    QPointer<Test> m_test;
    QMap<ushort,QString> m_listeNumSerieAnalyseurs;
    QMap<ushort,QString> m_listeInterfaceAnalyseurs;
    QString m_interfaceSonde;
    QString m_interfaceCalibrateur;
    QString m_canalCalibrateur;
    ushort m_idOperateur;
    ushort m_idLieu;
    QString m_nomTest;
    QString m_nomOperateur;
    QString m_designationLieu;
    QString m_nomTmpFichierCSV;
    QPointer<QFile> m_fichierCSV;
    float m_pression;
    float m_temperature;
    bool m_debutImmediat;
    bool sondePresente;
    QDateTime m_dateHeureDebutPrevu;
    int m_tpsAcquisition;

    et_ParamsTest() {}
    et_ParamsTest(const et_ParamsTest& paramsTest) : QObject()
    {
        this->m_id_TestMetro = paramsTest.m_id_TestMetro;
        this->m_id_TestXML = paramsTest.m_id_TestXML;
        this->m_test = paramsTest.m_test;
        this->m_listeNumSerieAnalyseurs = paramsTest.m_listeNumSerieAnalyseurs;
        this->m_listeInterfaceAnalyseurs = paramsTest.m_listeInterfaceAnalyseurs;
        this->m_interfaceCalibrateur = paramsTest.m_interfaceCalibrateur;
        this->m_canalCalibrateur = paramsTest.m_canalCalibrateur;
        this->m_idOperateur = paramsTest.m_idOperateur;
        this->m_idLieu = paramsTest.m_idLieu;
        this->m_nomTmpFichierCSV = paramsTest.m_nomTmpFichierCSV;
        this->m_fichierCSV = paramsTest.m_fichierCSV;
        this->m_nomTest = paramsTest.m_nomTest;
        this->m_nomOperateur = paramsTest.m_nomOperateur;
        this->m_designationLieu = paramsTest.m_designationLieu;
        this->m_pression = paramsTest.m_pression;
        this->m_temperature = paramsTest.m_temperature;
        this->m_debutImmediat = paramsTest.m_debutImmediat;
        this->m_dateHeureDebutPrevu = paramsTest.m_dateHeureDebutPrevu;
    }
    ~et_ParamsTest() {}

    et_ParamsTest& operator=(const et_ParamsTest& paramsTest)
    {
        this->m_id_TestMetro = paramsTest.m_id_TestMetro;
        this->m_id_TestXML = paramsTest.m_id_TestXML;
        this->m_test = paramsTest.m_test;
        this->m_listeNumSerieAnalyseurs = paramsTest.m_listeNumSerieAnalyseurs;
        this->m_listeInterfaceAnalyseurs = paramsTest.m_listeInterfaceAnalyseurs;
        this->m_interfaceCalibrateur = paramsTest.m_interfaceCalibrateur;
        this->m_canalCalibrateur = paramsTest.m_canalCalibrateur;
        this->m_idOperateur = paramsTest.m_idOperateur;
        this->m_idLieu = paramsTest.m_idLieu;
        this->m_nomTmpFichierCSV = paramsTest.m_nomTmpFichierCSV;
        this->m_fichierCSV = paramsTest.m_fichierCSV;
        this->m_nomTest = paramsTest.m_nomTest;
        this->m_nomOperateur = paramsTest.m_nomOperateur;
        this->m_designationLieu = paramsTest.m_designationLieu;
        this->m_pression = paramsTest.m_pression;
        this->m_temperature = paramsTest.m_temperature;
        this->m_debutImmediat = paramsTest.m_debutImmediat;
        this->m_dateHeureDebutPrevu = paramsTest.m_dateHeureDebutPrevu;
        return *this;
    }

    bool testerConflit(const QPointer<et_ParamsTest> paramTest,const QPointer<BdHandler> bdHandler)
    {
        if(bdHandler->getIdCalibrateur(m_test->getIdSystemeEtalon())
           == bdHandler->getIdCalibrateur(paramTest->m_test->getIdSystemeEtalon()))
            return true;

        if(this->m_interfaceCalibrateur == paramTest->m_interfaceCalibrateur)
            return true;

        QMapIterator<ushort,QString> it_listeInterfacesAnaArg(paramTest->m_listeInterfaceAnalyseurs);
        while(it_listeInterfacesAnaArg.hasNext()) {
            it_listeInterfacesAnaArg.next();

            QString interface =  it_listeInterfacesAnaArg.value();

            QMapIterator<ushort,QString> it_listeInterfacesAna(this->m_listeInterfaceAnalyseurs);
            while(it_listeInterfacesAna.hasNext()) {
                it_listeInterfacesAna.next();

                if(interface == it_listeInterfacesAna.value())
                    return true;
            }
        }
        return false;
    }
};

#endif // ET_PARAMSTEST_H
