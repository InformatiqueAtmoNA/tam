/*////////////////////////////////////////////////////
// \file Tei_146c_v2.h
// \brief Classe dérivée de Tei pour les diluteur 146c d'AIRPARIF
// \author FOUQUART Christophe
// \version 0.1
// \date 08/06/2011
////////////////////////////////////////////////////*/

#ifndef TEI_146C_V2_H
#define TEI_146C_V2_H

#include "tei.h"

class Tei_146c_v2 : public Tei
{
public:

    /*///////////////////////////////////////////////////////////////////////////
    // \fn Tei_146c_v2(const QString adressePeriph, const TypePeripherique typePeriph, const bool accepteFloat)
    // \brief Constructeur
    //
    // \param adressePeriph Adresse du périphérique
    // \param typePeriph Analyseur ou Diluteur
    // \param typePolluant Type de polluant associé à l'appareil
    // \param optionTpg Paramètre définissant si l'appareil dispose d'un ozoniseur ou d'un photometre
    ///////////////////////////////////////////////////////////////////////////*/
    Tei_146c_v2(const QString & adressePeriph, const TypePeripherique & typePeriph, const OptionTpg & optionTpg=AUCUNE);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void commandeSpan(ushort canal,ushort point)
    // \brief Commande au diluteur de se mettre à un certain point de gaz
    ///////////////////////////////////////////////////////////////////////////*/
    void commandeSpan(const SpanHandler & spanData);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void commandeSpan(ushort canal,ushort point)
    // \brief Commande au diluteur de se mettre au point de gaz zero
    ///////////////////////////////////////////////////////////////////////////*/
    void commandeSpanZero(const QString & canal="");

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void commandeSpan(ushort canal,ushort point)
    // \brief Commande au diluteur de se mettre à un certain point de gaz
    //
    // \param pointO3 Point de consigne ozone
    ///////////////////////////////////////////////////////////////////////////*/
    void commandeSpanTpg(const SpanHandler & spanTpgData);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void commandeSpan03(ushort point03)
    // \brief Commande au diluteur de se mettre à un certain point de gaz O3
    ///////////////////////////////////////////////////////////////////////////*/
    void commandeSpanO3(const SpanHandler & spanO3Data);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual QVector<Commandes> const* getListeCommandes()
    // \brief Renvoie la liste des commandes autorisées par le protocole
    //
    // \return QVector<Commandes> const* Liste des commandes autorisées par le protocole
    ///////////////////////////////////////////////////////////////////////////*/
    virtual QVector<Commandes> const* getListeCommandes();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual SpanHandler* getSpanHandler(Commandes commandeSpan)
    // \brief Renvoie une instance de SpanHandler contenant les infos
    //        sur les arguments de la commande
    //
    // \param commandeSpan Commande dont on veut connaitre les arguments à utiliser
    // \return SpanHandler* Instance de SpanHandler contenant les infos sur les arguments de la commande
    ///////////////////////////////////////////////////////////////////////////*/
    virtual SpanHandler* getSpanHandler(Commandes commandeSpan);
};
#endif // TEI_146C_V2_H
