#ifndef MODE4_SX_6000_1808_H
#define MODE4_SX_6000_1808_H


#include "mode4.h"

/*////////////////////////////////////////////////////////////////////////////////////
// \class Mode4_SX_6000_1808
// \brief Classe dérivée de Mode4 pour les diluteurs SX6000 V1.808 (AIRBREIZH)
//
// Cette classe redéfinie certaines méthodes
// héritées de la classe Mode4.
////////////////////////////////////////////////////////////////////////////////////*/

class Mode4_SX_6000_1808 : public Mode4
{

public:
    Mode4_SX_6000_1808(QString const & adressePeriph, TypePeripherique const & typePeriph, bool const & accepteFloat);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void commandeSpan(QVector<QVariant> const & spanData)=0
    // \brief Commande au diluteur de se mettre à un certain point de gaz
    //
    // \param &spanData Argument nécessaires à la commande
    ///////////////////////////////////////////////////////////////////////////*/
    void commandeSpan(SpanHandler const & spanData);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void commandeSpanZero(QVariant const & canal=0)
    // \brief Commande au diluteur de se mettre au point de gaz zero
    //
    // \param &canal Canal optionnel
    ///////////////////////////////////////////////////////////////////////////*/
    void commandeSpanZero(QString const & canal="");

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void commandeSpanTpg(QVector<QVariant> const & spanTpgData)
    // \brief Commande au diluteur de se mettre à un certain point de gaz
    //
    // \param &spanTpgData Argument nécessaires à la commande
    ///////////////////////////////////////////////////////////////////////////*/
    void commandeSpanTpg(SpanHandler const & spanTpgData);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void commandeSpanO3(QVector<QVariant> const & spanO3Data)
    // \brief Commande au diluteur de se mettre à un certain point de gaz O3
    //
    // \param &spanO3Data Argument nécessaires à la commande
    ///////////////////////////////////////////////////////////////////////////*/
    void commandeSpanO3(SpanHandler const & spanO3Data);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual QVector<Commandes> const* getListeCommandes()
    // \brief Renvoie la liste des commandes autorisées par le protocole
    //
    // \return QVector<Commandes> const* Liste des commandes autorisées par le protocole
    ///////////////////////////////////////////////////////////////////////////*/
    virtual QVector<Commandes> const* getListeCommandes();

};

#endif // Mode4_SX_6000_1808_H
