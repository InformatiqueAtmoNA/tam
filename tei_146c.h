/*////////////////////////////////////////////////////
// \file tei_146c.h
// \brief Classe d�riv�e de Tei pour les diluteur 146c
// \author FOUQUART Christophe
// \version 0.1
// \date 10/03/2011
////////////////////////////////////////////////////*/

#ifndef TEI_146C_H
#define TEI_146C_H

#include "tei.h"

/*////////////////////////////////////////////////////////////////////////////////////
// \class Tei_146c
// \brief Classe d�riv�e de Tei pour les diluteur 146i
//
// Cette classe red�finie certaines m�thodes
// h�rit�es de la classe Tei.
////////////////////////////////////////////////////////////////////////////////////*/
class Tei_146c : public Tei
{

public:
    /*///////////////////////////////////////////////////////////////////////////
    // \fn Tei_146c(QString adressePeriph, TypePeripherique typePeriph, bool accepteFloat)
    // \brief Constructeur
    //
    // \param adressePeriph Adresse du p�riph�rique
    // \param typePeriph Analyseur ou Diluteur
    // \param typePolluant Type de polluant associ� � l'appareil
    // \param optionTpg Param�tre d�finissant si l'appareil dispose d'un ozoniseur ou d'un photometre
    ///////////////////////////////////////////////////////////////////////////*/
    Tei_146c(QString adressePeriph, TypePeripherique typePeriph, TypePolluant typePolluant,OptionTpg optionTpg=AUCUNE);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void commandeSpan(ushort canal,ushort point)
    // \brief Commande au diluteur de se mettre � un certain point de gaz
    //
    // \param canal Canal sur lequel le polluant souhait� est programm�
    // \param point Point de consigne
    ///////////////////////////////////////////////////////////////////////////*/
    void commandeSpan_146c(QString canal,ushort point);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void commandeSpan(ushort canal,ushort point)
    // \brief Commande au diluteur de se mettre au point de gaz zero
    ///////////////////////////////////////////////////////////////////////////*/
    void commandeSpanZero();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void commandeSpan(ushort canal,ushort point)
    // \brief Commande au diluteur de se mettre � un certain point de gaz
    //
    // \param pointO3 Point de consigne ozone
    ///////////////////////////////////////////////////////////////////////////*/
    void commandeSpanTpg(ushort pointO3);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void commandeSpan03(ushort point03)
    // \brief Commande au diluteur de se mettre � un certain point de gaz O3
    ///////////////////////////////////////////////////////////////////////////*/
    void commandeSpanO3(ushort point03=0);
};

#endif // TEI_146C_H
