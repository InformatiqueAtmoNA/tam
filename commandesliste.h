/*///////////////////////////////////////////////////////////////////////////////////
// \file commandesliste.h
// \brief Classe gérant les commandes utilisables selon les protocoles et le matériel
// \author FOUQUART Christophe
// \version 0.1
// \date 10/03/2011
///////////////////////////////////////////////////////////////////////////////////*/

#ifndef COMMANDESLISTE_H
#define COMMANDESLISTE_H

#include <QVector>
#include <QStringList>
#include "mode4_sx_6000_17.h"
#include "tei_146i.h"
#include "tei_146c.h"
#include "tei_49c.h"
#include "api.h"

class CommandesListe
{
    QStringList* listeCommandes; // Listes des commandes autorisées par le protocole et l'appareil
    QVector<ushort>* idAssocies; // Liste des identifiant associés aux commandes
public:
    CommandesListe();
    QStringList* getListeCommandes(Protocoles protocole);
    ushort getIdCommande(ushort index);
};

#endif // COMMANDESLISTE_H
