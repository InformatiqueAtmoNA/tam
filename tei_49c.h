/*//////////////////////////////////////////////////////////////////////
// \file tei_49c.h
// \brief Classe d�riv�e de Tei pour les g�n�rateurs d'ozone SONIMIX 49c
// \author FOUQUART Christophe
// \version 0.1
// \date 10/03/2011
//////////////////////////////////////////////////////////////////////*/

#ifndef TEI_49C_H
#define TEI_49C_H

#include "tei.h"

/*////////////////////////////////////////////////////////////////////////////////////
// \class Tei_49c
// \brief Classe d�riv�e de Tei pour les g�n�rateurs d'ozone SONIMIX 49c
//
// Cette classe red�finie certaines m�thodes
// h�rit�es de la classe Tei.
////////////////////////////////////////////////////////////////////////////////////*/
class Tei_49c : public Tei
{
public:
    /*///////////////////////////////////////////////////////////////////////////
    // \fn Tei_49c(QString adressePeriph, TypePeripherique typePeriph, bool accepteFloat)
    // \brief Constructeur
    //
    // \param adressePeriph Adresse du p�riph�rique
    // \param typePeriph Analyseur ou Diluteur
    // \param typePolluant Type de polluant associ� � l'appareil
    // \param optionTpg Param�tre d�finissant si l'appareil dispose d'un ozoniseur ou d'un photometre
    ///////////////////////////////////////////////////////////////////////////*/
    Tei_49c(QString adressePeriph, TypePeripherique typePeriph, TypePolluant typePolluant,OptionTpg optionTpg=AUCUNE);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void commandeSpan(ushort canal,ushort point)
    // \brief Commande au diluteur de se mettre au point de gaz zero
    ///////////////////////////////////////////////////////////////////////////*/
    void commandeSpanZero();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void commandeSpanO3(ushort point03=0)
    // \brief Commande au diluteur de se mettre � un certain point de gaz O3
    ///////////////////////////////////////////////////////////////////////////*/
    void commandeSpanO3(ushort point03=0);
};

#endif // TEI_49C_H
