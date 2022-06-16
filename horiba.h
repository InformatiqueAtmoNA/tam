#ifndef HORIBA_H
#define HORIBA_H

#include <protocole.h>

class horiba : public Protocole
{
protected:
    /*///////////////////////////////////////////////////////////////////////////
    // \fn QString* creerTrameCommande(QString & noCommande,QString & data="")
    // \brief Cr�e une trame en fonction du num�ro de commande MODE4
    //
    // \param noCommande Cum�ro de commande MODE4
    // \param data Donn�es facultatives
    ///////////////////////////////////////////////////////////////////////////*/
    QString* creerTrameCommande(const QString & codeCommande,const QString & paramCommande);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn QString* calculerBCC(QString & trame)
    // \brief Calcule du BCC (Block Check Character)
    //
    // Le BCC est obtenu en effectuant un OU exclusif (XOR)
    // sur tous les octets d� partir de STX exclu jusqu'au BCC exclu
    //
    // \param trame Trame MODE4
    // \return QString* BCC
    ///////////////////////////////////////////////////////////////////////////*/
    QString* calculerFCS(const QString & trame);



public:
    /*///////////////////////////////////////////////////////////////////////////
    // \fn horiba()
    // \brief Constructeur
    ///////////////////////////////////////////////////////////////////////////*/
    horiba(const QString & adressePeriph,const TypePeripherique & typePeriph);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void demandeMesure()
    // \brief Demande de mesure imm�diate
    //
    // \return QVector<float> Tableau des mesures
    ///////////////////////////////////////////////////////////////////////////*/
    virtual QPointer<MesureIndividuelle> demandeMesure();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void init()
    // \brief Initialisation des parametres du p�riph�rique
    ///////////////////////////////////////////////////////////////////////////*/
    virtual bool init();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void parDefault()
    // \brief R�gle l'appareil sur son mode de fonctionnement par d�faut
    ///////////////////////////////////////////////////////////////////////////*/
    virtual bool parDefault();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void demandeAlarme()
    // \brief Demande d'alarme
    //
    // \return ushort Valeur de l'alarme
    ///////////////////////////////////////////////////////////////////////////*/
    virtual ushort demandeAlarme();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void passageZero()
    // \brief Demande de passage en mode zero
    ///////////////////////////////////////////////////////////////////////////*/
    virtual void passageZero();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void passageEtalon()
    // \brief Demande de passage en mode �talon
    ///////////////////////////////////////////////////////////////////////////*/
    virtual void passageEtalon();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void passageMesure()
    // \brief Demande de passage en mode mesure
    ///////////////////////////////////////////////////////////////////////////*/
    virtual void passageMesure();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual QVector<Commandes> const* getListeCommandes()
    // \brief Renvoie la liste des commandes autoris�es par le protocole
    //
    // \return QVector<Commandes> const* Liste des commandes autoris�es par le protocole
    ///////////////////////////////////////////////////////////////////////////*/
    virtual QVector<Commandes> const* getListeCommandes();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual SpanHandler* getSpanHandler(Commandes commandeSpan)
    // \brief Renvoie une instance de SpanHandler contenant les infos
    //        sur les arguments de la commande
    //
    // \param commandeSpan Commande dont on veut connaitre les arguments � utiliser
    // \return SpanHandler* Instance de SpanHandler contenant les infos sur les arguments de la commande
    ///////////////////////////////////////////////////////////////////////////*/
    virtual SpanHandler* getSpanHandler(Commandes commandeSpan);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void demandeDateHeure()
    // \brief Demande la date et l'heure
    //
    // \return QDateTime* Date et heure du p�riph�rique
    ///////////////////////////////////////////////////////////////////////////*/
    virtual QDateTime *demandeDateHeure();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn QMap<QString,QString> getInformationFromResponse(QString reponse);
    // \brief D�coupe la tame et renvoie les mesures r�cup�r�es
    //
    // \return QMap<QString,QString> Liste des informations r�cup�r�es
    ///////////////////////////////////////////////////////////////////////////*/
    QMap<QString,QString> getInformationFromResponse(QString reponse);

private :
    QString adresse; // adresse de l'�quipement
    TypePeripherique typePeriph;
};

#endif // HORIBA_H
