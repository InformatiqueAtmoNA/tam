/*    Copyright 2011 FOUQUART Christophe
//
//    This file is part of Test_Automatiques_Metrologiques.
//
//    Test_Automatiques_Metrologiques is free software:
//    you can redistribute it and/or modify it under the terms of the
//    GNU General Public License as published by the Free Software Foundation,
//    either version 3 of the License, or (at your option) any later version.
//
//    Test_Automatiques_Metrologiques is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with Test_Automatiques_Metrologiques.
//    If not, see <http://www.gnu.org/licenses/>.
*/

#include "definitions_globales.h"

DesignationProtocole stringToProtocole(QString protocole) {
    if(protocole=="MODE4_ANA_CMD04")
        return MODE4_ANA_CMD04;
    if(protocole=="MODE4_ANA_CMD16")
        return MODE4_ANA_CMD16;
    if(protocole=="MODE4_SX6000_17")
        return MODE4_SX6000_17;
    if(protocole=="MODE4_LNI")
        return MODE4_LNI;
    if(protocole=="TEI_ANA")
        return TEI_ANA;
    if(protocole=="TEI_146i")
        return TEI_146i;
    if(protocole=="TEI_146i_OZONISEUR")
        return TEI_146i_OZONISEUR;
    if(protocole=="TEI_146i_PHOTOMETRE")
        return TEI_146i_PHOTOMETRE;
    if(protocole=="TEI_146c")
        return TEI_146c;
    if(protocole=="TEI_146c_V2")
        return TEI_146c;
    if(protocole=="TEI_49ps")
        return TEI_49ps;
    if(protocole=="API_ANA")
        return API_ANA;
    if(protocole=="API_DIL")
        return API_DIL;
    return PROTOCOLE_INCONNU;
}

TypePolluant stringToTypePolluant(QString type) {
    if(type=="CO")
        return CO;
    if(type=="O3")
        return O3;
    if(type=="SO2")
        return SO2;
    if(type=="NO")
        return NO;
    if(type=="NO2")
        return NO2;
    if(type=="NOX")
        return NOX;
    return DIL;
}

QString typePolluantToString(TypePolluant type) {
    if(type==CO)
        return "CO";
    if(type==O3)
        return "O3";
    if(type==SO2)
        return "SO2";
    if(type==NO)
        return "NO";
    if(type==NO2)
        return "NO2";
    if(type==NOX)
        return "NOX";
    if(type==DIL)
        return "DIL";
    return "";
}

QString typeTestToString(TypeTest type) {
    if(type==REPETABILITE_1)
        return "REPETABILITE_1";
    if(type==REPETABILITE_2)
        return "REPETABILITE_2";
    if(type==LINEARITE)
        return "LINEARITE";
    if(type==TEMPS_REPONSE)
        return "TEMPS_REPONSE";
    if(type==RENDEMENT_FOUR)
        return "RENDEMENT_FOUR";
    return "PERSO";
}

TypeTest stringToTypeTest(QString type) {
    if(type=="REPETABILITE_1")
        return REPETABILITE_1;
    if(type=="REPETABILITE_2")
        return REPETABILITE_2;
    if(type=="LINEARITE")
        return LINEARITE;
    if(type=="TEMPS_REPONSE")
        return TEMPS_REPONSE;
    if(type=="RENDEMENT_FOUR")
        return RENDEMENT_FOUR;
    return PERSO;
}

Commandes stringToCommande(QString cmd) {
    if(cmd=="OFFSET")
        return OFFSET;
    if(cmd=="MESURES")
        return MESURES;
    if(cmd=="ALARME")
        return ALARME;
    if(cmd=="DATE_HEURE")
        return DATE_HEURE;
    if(cmd=="MODE_ZERO")
        return MODE_ZERO;
    if(cmd=="MODE_ETALON")
        return MODE_ETALON;
    if(cmd=="MODE_MESURE")
        return MODE_MESURE;
    if(cmd=="SPAN")
        return SPAN;
    if(cmd=="SPAN_ZERO")
        return SPAN_ZERO;
    if(cmd=="SPAN_TPG")
        return SPAN_TPG;
    if(cmd=="SPAN_O3")
        return SPAN_O3;
    if(cmd=="RESET")
        return RESET;
    if(cmd=="STAND_BY")
        return STAND_BY;
    if(cmd=="EVENT")
        return EVENT;
    return NO_CMD;
}

QString commandesToString(Commandes cmd) {
    if(cmd==OFFSET)
        return "OFFSET";
    if(cmd==MESURES)
        return "MESURES";
    if(cmd==ALARME)
        return "ALARME";
    if(cmd==DATE_HEURE)
        return "DATE_HEURE";
    if(cmd==MODE_ZERO)
        return "MODE_ZERO";
    if(cmd==MODE_ETALON)
        return "MODE_ETALON";
    if(cmd==MODE_MESURE)
        return "MODE_MESURE";
    if(cmd==SPAN)
        return "SPAN";
    if(cmd==SPAN_ZERO)
        return "SPAN_ZERO";
    if(cmd==SPAN_TPG)
        return "SPAN_TPG";
    if(cmd==SPAN_O3)
        return "SPAN_O3";
    if(cmd==RESET)
        return "RESET";
    if(cmd==STAND_BY)
        return "STAND_BY";
    if(cmd==EVENT)
        return "EVENT";
    return "NO_CMD";
}

QVariant getParam(QString const & key) {
    QSettings parametres("params.ini",QSettings::IniFormat);
    return parametres.value(key);
}

void setParam(QString const & key, QVariant const & param) {
    QSettings parametres("params.ini",QSettings::IniFormat);
    parametres.setValue(key,param);
    parametres.sync();
}
