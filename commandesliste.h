/*///////////////////////////////////////////////////////////////////////////////////
// \file commandesliste.h
// \brief Classe gérant les commandes utilisables selon les protocoles et le matériel
// \author FOUQUART Christophe
// \version 1.0
// \date 10/03/2011
//
// TAM - Tests Automatiques Métrologiques
// Copyright (C) 2011-2012 TAM Team
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
///////////////////////////////////////////////////////////////////////////////////*/

#ifndef COMMANDESLISTE_H
#define COMMANDESLISTE_H

#include "mode4_sx_6000_17.h"
#include "tei_146i.h"
#include "tei_146c.h"
#include "tei_49ps.h"
#include "api.h"


class CommandesListe
{
    QMap<ushort,QString> m_IntitulesCommandes; // Indexes des commandes associés à leurs intitulés
    QMap<ushort,ushort>  m_IndexCommandesListe; //  Indexes des commandes associés à leurs index de liste
public:
    CommandesListe();
    QStringList getIntitulesCommandes(QVector<Commandes> const & commandesAutorisees);
    ushort getIdCommande(ushort indexListe);
};

#endif // COMMANDESLISTE_H
