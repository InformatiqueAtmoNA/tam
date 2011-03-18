#include "tei_146i.h"

Tei_146i::Tei_146i()
{
}

Tei_146i::Tei_146i(QString adressePeriph, TypePeripherique typePeriph, TypePolluant typePolluant,OptionTpg optionTpg)
        :Tei(adressePeriph, typePeriph,typePolluant,optionTpg) {}

// Commande au diluteur de se mettre à un certain point de gaz
void Tei_146i::commandeSpan(QString canal,ushort point, ushort concO3) {
    QString cmd;
    QString reponse;
    // Si pas de span ozone
    if(concO3 == 0) {
        cmd = *(this->creerTrameCommande("set ozon off"));
        reponse = this->transaction(cmd);
        if(reponse.isEmpty())
            return;
    }

    if(point >0) {
        QString trame = "set gas ";
        trame.append(canal);
        trame.append(" span ");
        trame.append(QString::number(point));

        cmd = *(this->creerTrameCommande(trame));
        reponse = this->transaction(cmd);
        if(reponse.isEmpty())
            return;
    }

    if(concO3 > 0) {
        switch(this->optionTpg) {
        case AUCUNE:
            return;
        case OZONISEUR:
            trame = "set ozon ";
            break;
        case PHOTOMETRE:
            trame = "set o3 conc ";
            break;
        }
        trame.append(QString::number(concO3));
        cmd = *(this->creerTrameCommande(trame));
        this->transaction(cmd);
    }
}

