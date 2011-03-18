/*////////////////////////////////////////////////////
// \file tei_146i.h
// \brief Classe dérivée de Tei pour les diluteur 146i
// \author FOUQUART Christophe
// \version 0.1
// \date 09/03/2011
////////////////////////////////////////////////////*/

#ifndef TEI_146I_H
#define TEI_146I_H

#include "tei.h"

/*////////////////////////////////////////////////////////////////////////////////////
// \class Tei_146i
// \brief Classe dérivée de Tei pour les diluteur 146i
//
// Cette classe redéfinie certaines méthodes
// héritées de la classe Tei.
////////////////////////////////////////////////////////////////////////////////////*/

class Tei_146i : public Tei
{
public:
    Tei_146i();
    /*///////////////////////////////////////////////////////////////////////////
    // \fn Tei_146i(QString adressePeriph, TypePeripherique typePeriph, bool accepteFloat)
    // \brief Constructeur
    //
    // \param adressePeriph Adresse du périphérique
    // \param typePeriph Analyseur ou Diluteur
    // \param typePolluant Type de polluant associé à l'appareil
    // \param optionTpg Paramètre définissant si l'appareil dispose d'un ozoniseur ou d'un photometre
    ///////////////////////////////////////////////////////////////////////////*/
    Tei_146i(QString adressePeriph, TypePeripherique typePeriph, TypePolluant typePolluant,OptionTpg optionTpg=AUCUNE);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void commandeSpan(ushort canal,ushort point)
    // \brief Commande au diluteur de se mettre à un certain point de gaz
    //
    // \param canal canal sur lequel le polluant souhaité est programmé
    // \param point Point de consigne
    // \param concO3 Concentration en ozone
    ///////////////////////////////////////////////////////////////////////////*/
    void commandeSpan(QString canal,ushort point=0, ushort concO3=0);
};

#endif // TEI_146I_H
