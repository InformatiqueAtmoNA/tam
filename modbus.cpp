#include "modbus.h"

Modbus::Modbus(const QString & adressePeriph,const TypePeripherique & typePeriph, const DesignationProtocole & protocoleUtilise)
{
    this->adresse = adressePeriph;
    this->typePeripherique = typePeriph;
    this->versionProtocole = protocoleUtilise;
}
QString *Modbus::creerTrameCommande(QByteArray adr_registre, QByteArray nbrRegistres)
{
    QString* trame = new QString;
    QByteArray crc;
    trame->append(char(this->adresse.toInt())); // adresse de l'appareil
    trame->append(uint16_t(0x03)); // type de lecture
    trame->append(adr_registre[0]); // bits de poid fort
    trame->append(adr_registre[1]); // bits de poid faible
    trame->append(nbrRegistres[0]); // nombre de registres(codés sur 16bits) bit de poid fort
    trame->append(nbrRegistres[1]); // nombre de registres(codés sur 16bits) bit de poid faible
    crc =calcul_CRC(trame->toLatin1(),trame->length());
    trame->append(crc[1]); // crc(codé sur 16bits) bit de poid faible
    trame->append(crc[0]); // crc(codé sur 16bits) bit de poid fort
    return trame;
}
QPointer<MesureIndividuelle> Modbus::demandeMesure()
{
    QByteArray nbrRegistres;
    uint16_t nbr_registre_high = 0x00;
            uint16_t nbr_registre_low = 0x01;
            nbrRegistres.append(nbr_registre_high);
            nbrRegistres.append(nbr_registre_low);
    QString cmd ;

    cmd = *(this->creerTrameCommande(getRegistre(this->polluantAssocie), nbrRegistres));
    QString reponse = this->transaction(cmd);

    QPointer<MesureIndividuelle> tabMesures(new MesureIndividuelle());
    if(reponse.isEmpty())
        return tabMesures;

    tabMesures.data()->append(this->getDataFromTrame(reponse));

    return tabMesures;

}
float Modbus::getDataFromTrame(const QString &mesure)
{
    uchar value_High = mesure.at(3).toLatin1();
    uchar value_Low = mesure.at(4).toLatin1();
    float Value= ((value_High<<8)+value_Low);
    return Value/10;
}

QByteArray Modbus::calcul_CRC(QByteArray trame, int taille_Trame)
{
    uint16_t crc = 0xFFFF;
    for (int pos = 0; pos < taille_Trame; pos++) {
        crc ^= (uint16_t)trame[pos];          // XOR byte into least sig. byte of crc

        for (int i = 8; i != 0; i--) {    // Loop over each bit
          if ((crc & 0x0001) != 0) {      // If the LSB is set
            crc >>= 1;                    // Shift right and XOR 0xA001
            crc ^= 0xA001;
          }
          else                            // Else LSB is not set
            crc >>= 1;                    // Just shift right
        }
    }
    QByteArray crcHexa = QByteArray::fromHex(QString::number(crc,16).toLatin1());
    return crcHexa;
}

ushort Modbus::demandeAlarme()
{
    uint16_t adr_high = 0x00;
    uint16_t adr_low = 0x30;
    QByteArray adr_registres;
    adr_registres.append(adr_high);
    adr_registres.append(adr_low);

    uint16_t nrb_reg_high = 0x00;
    uint16_t nrb_reg_low = 0x01;
    QByteArray nbrRegistres;
    nbrRegistres.append(nrb_reg_high);
    nbrRegistres.append(nrb_reg_low);

    QString cmd = *(this->creerTrameCommande(adr_registres, nbrRegistres));
    QString reponse = this->transaction(cmd);
    return 0;
}


QByteArray Modbus::getRegistre(TypePolluant polluantAssocie)
{
    uint16_t registre_high = 0x00;
    uint16_t registre_low = 0x00;
    QByteArray adr_registres;

    switch(this->versionProtocole){
        case MODBUS_RTU_TXXXX :
            switch(polluantAssocie){
                case TEMP :
                    registre_high = 0x00;
                    registre_low = 0x30;
                    break;
                default :
                    break;
            }
            break;
        default :
            break;
    }
    adr_registres.append(registre_high);
    adr_registres.append(registre_low);
    return adr_registres;

}


void Modbus::passageZero()
{

}

void Modbus::passageEtalon()
{

}

void Modbus::passageMesure()
{

}

const QVector<Commandes> *Modbus::getListeCommandes()
{

}

SpanHandler *Modbus::getSpanHandler(Commandes commandeSpan)
{

}








