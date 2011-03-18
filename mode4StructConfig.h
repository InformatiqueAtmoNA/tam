/*////////////////////////////////////////////////////
// \file mode4structconfig.h
// \brief Structure de configuration des appareils Mode4
// \author FOUQUART Christophe
// \version 0.1
// \date 01/02/2011
////////////////////////////////////////////////////*/

#ifndef MODE4STRUCTCONFIG_H
#define MODE4STRUCTCONFIG_H

#include <QDateTime>

/*////////////////////////////////////////////////////
// \struct mode4_config
//
// Contient les informations de configuration
// d'un périphérique MODE4
////////////////////////////////////////////////////*/
typedef struct Mode4_AnalyseurConfig{
    QDateTime dateHeure;
    short offset;
    ushort alarme;
    ushort periodeZeroRef;
    ushort periodeZero;
    ushort periodeAutoCalib;
    ushort periodeCalib;
    ushort dureeCycleZero;
    ushort dureeCycleCalib;
    QStringList* infoConfig;
}Mode4_AnalyseurConfig;

/*////////////////////////////////////////////////////
// \struct mode4_config
//
// Contient les informations de configuration
// d'un périphérique MODE4
////////////////////////////////////////////////////*/
typedef struct Mode4_DiluteurConfig{
    ushort pressionAir;
    ushort pressionDil;
    float tempOzoneur;
    float tempInterne;
    float tempFour;
    ushort uvMesuree;
    ushort uvCalculee;
    ushort pressionAtm;
    char urdm1;
    QString urdm2;
    QString opt1;
    QString opt2;
    QString opt3;
    QString opt4;
    QStringList* infoConfig;
}Mode4_DiluteurConfig;


#endif // MODE4STRUCTCONFIG_H
