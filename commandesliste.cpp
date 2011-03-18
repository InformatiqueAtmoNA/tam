#include "commandesliste.h"

CommandesListe::CommandesListe()
{
    listeCommandes = new QStringList();
    idAssocies = new QVector<ushort>;
}

QStringList* CommandesListe::getListeCommandes(Protocoles protocole) {
    listeCommandes->clear();
    idAssocies->clear();

    switch(protocole) {

    case MODE4_ANA:
        listeCommandes->append("Reset");
        idAssocies->append(15);
        listeCommandes->append("Offset");
        idAssocies->append(0);
        listeCommandes->append("Stand by");
        idAssocies->append(16);
    case TEI_ANA:
        listeCommandes->append("Demande infos");
        idAssocies->append(3);
    case API_ANA:
        listeCommandes->append("Demande mesure");
        idAssocies->append(1);
        listeCommandes->append("Demande alarme");
        idAssocies->append(2);
        listeCommandes->append("Passage mode zero");
        idAssocies->append(4);
        listeCommandes->append("Passage mode etalon");
        idAssocies->append(5);
        listeCommandes->append("Passage mode mesure");
        idAssocies->append(6);
        break;

    case MODE4_SX3022:
        listeCommandes->append("Commande event");
        idAssocies->append(17);
    case MODE4_SX6000_17:
        listeCommandes->append("Demande alarme");
        idAssocies->append(2);
        listeCommandes->append("Commande span");
        idAssocies->append(7);
        listeCommandes->append("Commande span zero");
        idAssocies->append(8);
        listeCommandes->append("Commande span tpg");
        idAssocies->append(11);
        listeCommandes->append("Commande span O3");
        idAssocies->append(13);
        listeCommandes->append("Commande reset");
        idAssocies->append(15);
        listeCommandes->append("Stand by");
        idAssocies->append(16);
        break;

    case TEI_146i:
        listeCommandes->append("Demande alarme");
        idAssocies->append(2);
        listeCommandes->append("Commande span");
        idAssocies->append(7);
        listeCommandes->append("Commande span zero");
        idAssocies->append(8);
        listeCommandes->append("Commande span tpg");
        idAssocies->append(11);
        listeCommandes->append("Stand by");
        idAssocies->append(16);
        break;

    case TEI_146c:
        listeCommandes->append("Demande alarme");
        idAssocies->append(2);
        listeCommandes->append("Commande span");
        idAssocies->append(18);
        listeCommandes->append("Commande span zero");
        idAssocies->append(9);
        listeCommandes->append("Commande span tpg");
        idAssocies->append(12);
        listeCommandes->append("Commande span O3");
        idAssocies->append(14);
        listeCommandes->append("Stand by");
        idAssocies->append(16);
        break;

    case TEI_49c:
        listeCommandes->append("Demande alarme");
        idAssocies->append(2);
        listeCommandes->append("Commande span zero");
        idAssocies->append(9);
        listeCommandes->append("Commande span O3");
        idAssocies->append(14);
        break;
    case API_DIL:
        listeCommandes->append("Demande alarme");
        idAssocies->append(2);
        listeCommandes->append("Commande span");
        idAssocies->append(7);
        listeCommandes->append("Commande span zero");
        idAssocies->append(8);
        listeCommandes->append("Commande span tpg");
        idAssocies->append(19);
        listeCommandes->append("Commande span O3");
        idAssocies->append(14);
        listeCommandes->append("Stand by");
        idAssocies->append(16);
        break;
    }
    return listeCommandes;
}

ushort CommandesListe::getIdCommande(ushort index) {
    return idAssocies->at(index);
}
