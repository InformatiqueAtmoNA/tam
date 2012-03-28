/*////////////////////////////////////////////////////
// \file mode4structconfig.h
// \brief Structure de configuration des appareils Mode4
// \author FOUQUART Christophe
// \version 1.0
// \date 31/03/2011
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
