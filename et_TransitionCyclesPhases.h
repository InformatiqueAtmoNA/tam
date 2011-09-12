/*////////////////////////////////////////////////////////////
// \file et_TransitionCyclesPhases.h
// \brief Classe implémentant la transition testant le nombre de cycles de phases restants
//        lors de l'exécution des tests métrologiques
// \author FOUQUART Christophe
// \date 11/07/2011
//
// TAM - Tests Automatiques Métrologiques
// Copyright (C) 2011 FOUQUART Christophe
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
#ifndef ET_TRANSITIONCYCLESPHASES_H
#define ET_TRANSITIONCYCLESPHASES_H

//#include "ev_PhasesRestantes.h"
#define EVENT_CYCLES_PHASES_RESTANTS QEvent::User+7

/*////////////////////////////////////////////////////////////////////////////////////
// \class ev_CyclesPhasesRestants
// \brief Classe d'évènement utilisé pour la transition testant le
//        nombre de phases restantes lors de l'exécution des tests
////////////////////////////////////////////////////////////////////////////////////*/
class ev_nbCyclesPhasesRestants : public QEvent
{
    ushort m_nbCyclesPhasesRestants;

public :
    ev_nbCyclesPhasesRestants(const ushort &val)
        : QEvent(QEvent::Type(EVENT_CYCLES_PHASES_RESTANTS)),
          m_nbCyclesPhasesRestants(val) {}

    ushort getNbCyclesPhasesRestants() {return m_nbCyclesPhasesRestants;}
};

/*////////////////////////////////////////////////////////////////////////////////////
// \class et_TransitionCyclesPhasesRestants
// \brief Classe implémentant la transition testant le nombre de cycles de phases restants
//        lors de l'exécution des tests métrologiques
//
// Classe implémentant la transition testant le nombre de cycles de phases restants
// lors de l'exécution des tests métrologiques
////////////////////////////////////////////////////////////////////////////////////*/
class et_TransitionCyclesPhasesRestants : public QAbstractTransition
{
public:
     et_TransitionCyclesPhasesRestants() {}

 protected:
     virtual bool eventTest(QEvent *e)
     {
         if (e->type() != QEvent::Type(EVENT_CYCLES_PHASES_RESTANTS))
             return false;
         ev_nbCyclesPhasesRestants *pre = static_cast<ev_nbCyclesPhasesRestants*>(e);
         return (pre->getNbCyclesPhasesRestants() > 0);
     }

     virtual void onTransition(QEvent *) {}
};

/*//////////////////////////////////////////////////////////////////////////////////////////
// \class et_TransitionCyclesPhasesTermines
// \brief Classe implémentant la transition testant si toutes les cycles de phases ont été effectuées
//        lors de l'exécution des tests métrologiques
//
// Classe implémentant la transition testant si toutes les cycles de phases ont été effectuées
// lors del'exécution des tests métrologiques
//////////////////////////////////////////////////////////////////////////////////////////*/
class et_TransitionCyclesPhasesTermines : public QAbstractTransition
{
public:
     et_TransitionCyclesPhasesTermines() {}

 protected:
     virtual bool eventTest(QEvent *e)
     {
         if (e->type() != QEvent::Type(EVENT_CYCLES_PHASES_RESTANTS))
             return false;
         ev_nbCyclesPhasesRestants *pre = static_cast<ev_nbCyclesPhasesRestants*>(e);
         return (pre->getNbCyclesPhasesRestants() == 0);
     }

     virtual void onTransition(QEvent *) {}
};

#endif // ET_TRANSITIONCYCLESPHASES_H
