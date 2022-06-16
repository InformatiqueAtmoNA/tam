#ifndef MODBUS_H
#define MODBUS_H

#include <iostream>

#include "protocole.h"

class Modbus : public Protocole
{
protected :
    QString* creerTrameCommande(QByteArray addr_registres, QByteArray nbrRegistres);
    float getDataFromTrame(const QString & mesure);
    QByteArray calcul_CRC(QByteArray trame, int taille);
    int calculNbrRegistres();

public:
    Modbus(const QString & adressePeriph,const TypePeripherique & typePeriph, const DesignationProtocole & protocoleUtilise);

    virtual QPointer<MesureIndividuelle> demandeMesure();

    virtual ushort demandeAlarme();

    virtual void passageZero();

    virtual void passageEtalon();

    virtual void passageMesure();

    virtual QVector<Commandes> const* getListeCommandes();

    virtual SpanHandler* getSpanHandler(Commandes commandeSpan);

    QByteArray getRegistre(QList<TypePolluant> polluantAssocie);

};

#endif // MODBUS_H
