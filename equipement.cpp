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

#include "equipement.h"

Equipement::Equipement()
{

}

Equipement::Equipement(Equipement const & equipement) : QObject() {
    this->m_polluants = equipement.m_polluants;
    this->m_type = equipement.m_type;
}

Equipement& Equipement::operator=(const Equipement& equipement) {
    this->m_polluants = equipement.m_polluants;
    this->m_type = equipement.m_type;

    return *this;
}

QDomElement Equipement::exportToXml(QDomDocument & xmlTest) {
    QDomElement xml_equipement;
    if(this->m_type == ANALYSEUR)
        xml_equipement = xmlTest.createElement("analyseur");
    else
        xml_equipement = xmlTest.createElement("etalon");
    for(int i =0;i<this->m_polluants.length();i++) {
        QDomElement polluant = xmlTest.createElement("polluant");
        polluant.setAttribute("type",this->m_polluants.at(i));
        xml_equipement.appendChild(polluant);
    }

    return xml_equipement;
}

QPointer<Equipement> Equipement::importFromXml(QDomElement const & domEquipement) {
    QPointer<Equipement> newEquipement = new Equipement();
    if(domEquipement.tagName()=="analyseur")
        newEquipement.data()->setTypePeripherique(ANALYSEUR);
    else
        newEquipement.data()->setTypePeripherique(ETALON);
    QDomNodeList listePolluants =  domEquipement.childNodes();
    for(int i=0;i<listePolluants.size();i++)
        newEquipement.data()->ajouterPolluant(listePolluants.at(i).toElement().attribute("type"));
    return newEquipement;
}
