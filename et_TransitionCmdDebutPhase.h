/*////////////////////////////////////////////////////////////
// \file et_TestCommandeDebutPhase.h
// \brief Classe implémentant la transition testant si une commande début de phase
//        a été spécifiée
//
// \author FOUQUART Christophe
// \date 29/06/2011
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
////////////////////////////////////////////////////////////*/

#ifndef ET_TRANSITIONCOMMANDEDEBUTPHASE_H
#define ET_TRANSITIONCOMMANDEDEBUTPHASE_H

#include <QEvent>
#include <QAbstractTransition>
//#include "ev_TestCommandeDebutPhase.h"
#define EVENT_TEST_CMD_DEBUT_PHASE QEvent::User+4

/*//////////////////////////////////////////////////////////////////////////////////////////
// \class TempsMaxPhaseEvent
// \brief Classe d'évènement utilisé pour la transition testant si un temps maximum de phase est défini
//////////////////////////////////////////////////////////////////////////////////////////*/
class ev_TestCommandeDebutPhase : public QEvent
{
    ushort m_noPhase;

public:
    ev_TestCommandeDebutPhase(ushort noPhase)
        : QEvent(QEvent::Type(EVENT_TEST_CMD_DEBUT_PHASE)),
          m_noPhase(noPhase) {}

    ushort getNoPhase() {return m_noPhase;}
};

/*//////////////////////////////////////////////////////////////////////////////////////////
// \class et_TransitionCmdDebutPhaseDefinie
// \brief Classe implémentant la transition testant si une commande début de phase
//        a été spécifiée
//
// Classe implémentant la transition testant si une commande début de phase
//        a été spécifiée
//////////////////////////////////////////////////////////////////////////////////////////*/
class et_TransitionCmdDebutPhaseDefinie : public QAbstractTransition
{
public:
     et_TransitionCmdDebutPhaseDefinie(QVector<ushort> listeCommandeDebutPhaseDefinies)
         {m_listeCommandeDebutPhaseDefinies = listeCommandeDebutPhaseDefinies;}

 protected:
     virtual bool eventTest(QEvent *e)
     {
         if (e->type() != QEvent::Type(EVENT_TEST_CMD_DEBUT_PHASE))
             return false;
         ev_TestCommandeDebutPhase *pre = static_cast<ev_TestCommandeDebutPhase*>(e);

         return (m_listeCommandeDebutPhaseDefinies.indexOf(pre->getNoPhase()>0));
     }

     virtual void onTransition(QEvent *) {}

private:
     QVector<ushort> m_listeCommandeDebutPhaseDefinies;
};

/*//////////////////////////////////////////////////////////////////////////////////////////
// \class et_TransitionCmdDebutPhaseNonDefinie
// \brief Classe implémentant la transition testant si une commande début de phase
//        n'a été spécifiée
//
// Classe implémentant la transition testant si une commande début de phase
//        n'a été spécifiée
//////////////////////////////////////////////////////////////////////////////////////////*/
class et_TransitionCmdDebutPhaseNonDefinie : public QAbstractTransition
{
public:
     et_TransitionCmdDebutPhaseNonDefinie(QVector<ushort> listeCommandeDebutPhaseDefinies)
         {m_listeCommandeDebutPhaseDefinies = listeCommandeDebutPhaseDefinies;}

 protected:
     virtual bool eventTest(QEvent *e)
     {
         if (e->type() != QEvent::Type(EVENT_TEST_CMD_DEBUT_PHASE))
             return false;
         ev_TestCommandeDebutPhase *pre = static_cast<ev_TestCommandeDebutPhase*>(e);

         return (m_listeCommandeDebutPhaseDefinies.indexOf(pre->getNoPhase()) < 0 );
     }

     virtual void onTransition(QEvent *) {}

private:
     QVector<ushort> m_listeCommandeDebutPhaseDefinies;
};

#endif // ET_TRANSITIONCOMMANDEDEBUTPHASE_H
