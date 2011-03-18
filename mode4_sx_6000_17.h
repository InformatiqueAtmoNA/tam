/*////////////////////////////////////////////////////
// \file mode4_sx_6000_17.h
// \brief Classe dérivée de Mode4 pour les diluteurs SX6000 V1.7
// \author FOUQUART Christophe
// \version 0.1
// \date 10/03/2011
////////////////////////////////////////////////////*/

#ifndef MODE4_SX_6000_17_H
#define MODE4_SX_6000_17_H

#include "mode4.h"

/*////////////////////////////////////////////////////////////////////////////////////
// \class Mode4_SX_6000_17
// \brief Classe dérivée de Mode4 pour les diluteurs SX6000 V1.7
//
// Cette classe redéfinie certaines méthodes
// héritées de la classe Mode4.
////////////////////////////////////////////////////////////////////////////////////*/

class Mode4_SX_6000_17 : public Mode4
{

public:
    Mode4_SX_6000_17(QString adressePeriph, TypePeripherique typePeriph, bool accepteFloat);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void demandeAlarme()
    // \brief Demande d'alarme
    //
    // \return ushort Valeur de l'alarme
    ///////////////////////////////////////////////////////////////////////////*/
    ushort demandeAlarme();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void commandeSpan(ushort canal,ushort point=0, ushort conc03=0)
    // \brief Commande au diluteur de se mettre à un certain point de gaz
    //
    // \param canal canal sur lequel le polluant souhaité est programmé
    // \param point Point de consigne
    // \param concO3 Concentration en ozone
    ///////////////////////////////////////////////////////////////////////////*/
    void commandeSpan(ushort canal,ushort point=0, ushort concO3=0);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void commandeSpan(ushort canal,ushort point)
    // \brief Commande au diluteur de se mettre au point de gaz zero
    ///////////////////////////////////////////////////////////////////////////*/
    void commandeSpanZero(ushort canal);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void commandeSpan(ushort canal,ushort point)
    // \brief Commande au diluteur de se mettre à un certain point de gaz
    ///////////////////////////////////////////////////////////////////////////*/
    void commandeSpanTpg(ushort canalNox,ushort point,ushort concO3);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void commandeSpan(ushort canal,ushort point
    // \brief Commande au diluteur de se mettre à un certain point de gaz O3
    ///////////////////////////////////////////////////////////////////////////*/
    void commandeSpanO3(ushort canal,ushort point03);

};

#endif // MODE4_SX_6000_17_H
