/*//////////////////////////////////////////////////////////////////////
// \file tei_49c.h
// \brief Classe dérivée de Tei pour les générateurs d'ozone SONIMIX 49c
// \author FOUQUART Christophe
// \version 0.1
// \date 10/03/2011
//////////////////////////////////////////////////////////////////////*/

#ifndef TEI_49C_H
#define TEI_49C_H

#include "tei.h"

/*////////////////////////////////////////////////////////////////////////////////////
// \class Tei_49c
// \brief Classe dérivée de Tei pour les générateurs d'ozone SONIMIX 49c
//
// Cette classe redéfinie certaines méthodes
// héritées de la classe Tei.
////////////////////////////////////////////////////////////////////////////////////*/
class Tei_49c : public Tei
{
public:
    /*///////////////////////////////////////////////////////////////////////////
    // \fn Tei_49c(QString adressePeriph, TypePeripherique typePeriph, bool accepteFloat)
    // \brief Constructeur
    //
    // \param adressePeriph Adresse du périphérique
    // \param typePeriph Analyseur ou Diluteur
    // \param typePolluant Type de polluant associé à l'appareil
    // \param optionTpg Paramètre définissant si l'appareil dispose d'un ozoniseur ou d'un photometre
    ///////////////////////////////////////////////////////////////////////////*/
    Tei_49c(QString adressePeriph, TypePeripherique typePeriph, TypePolluant typePolluant,OptionTpg optionTpg=AUCUNE);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void commandeSpan(ushort canal,ushort point)
    // \brief Commande au diluteur de se mettre au point de gaz zero
    ///////////////////////////////////////////////////////////////////////////*/
    void commandeSpanZero();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void commandeSpanO3(ushort point03=0)
    // \brief Commande au diluteur de se mettre à un certain point de gaz O3
    ///////////////////////////////////////////////////////////////////////////*/
    void commandeSpanO3(ushort point03=0);
};

#endif // TEI_49C_H
