/*////////////////////////////////////////////////////////////
// \file et_TransitionPhasesRestantes.h
// \brief Classe implémentant la transition testant le nombre de phases restantes
//        lors del'exécution des tests métrologiques
// \author FOUQUART Christophe
// \date 20/06/2011
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

#ifndef ET_TRANSITIONPHASESRESTANTES_H
#define ET_TRANSITIONPHASESRESTANTES_H

//#include "ev_PhasesRestantes.h"
#define EVENT_PHASE_RESTANTES QEvent::User+1

/*////////////////////////////////////////////////////////////////////////////////////
// \class ev_PhasesRestantesEvent
// \brief Classe d'évènement utilisé pour la transition testant le
//        nombre de phases restantes lors de l'exécution des tests
////////////////////////////////////////////////////////////////////////////////////*/
class ev_PhasesRestantes : public QEvent
{
    uint m_nbPhasesRestantes;

public :
    ev_PhasesRestantes(const uint &val)
        : QEvent(QEvent::Type(EVENT_PHASE_RESTANTES)),
          m_nbPhasesRestantes(val) {}

    uint getNbPhasesRestantes() {return m_nbPhasesRestantes;}
};

/*////////////////////////////////////////////////////////////////////////////////////
// \class et_TransitionPhasesRestantes
// \brief Classe implémentant la transition testant le nombre de phases restantes
//        lors de l'exécution des tests métrologiques
//
// Classe implémentant la transition testant le nombre de phases restantes
// lors del'exécution des tests métrologiques
////////////////////////////////////////////////////////////////////////////////////*/
class et_TransitionPhasesRestantes : public QAbstractTransition
{
public:
     et_TransitionPhasesRestantes() {}

 protected:
     virtual bool eventTest(QEvent *e)
     {
         if (e->type() != QEvent::Type(EVENT_PHASE_RESTANTES))
             return false;
         ev_PhasesRestantes *pre = static_cast<ev_PhasesRestantes*>(e);
         return (pre->getNbPhasesRestantes() > 0);
     }

     virtual void onTransition(QEvent *) {}
};

/*//////////////////////////////////////////////////////////////////////////////////////////
// \class et_TransitionFinTest
// \brief Classe implémentant la transition testant si toutes les phases d'un cycle ont été effectuées
//        lors de l'exécution des tests métrologiques
//
// Classe implémentant la transition testant si toutes les phases d'un cycle ont été effectuées
//        lors del'exécution des tests métrologiques
//////////////////////////////////////////////////////////////////////////////////////////*/
class et_TransitionFinCycle : public QAbstractTransition
{
public:
     et_TransitionFinCycle() {}

 protected:
     virtual bool eventTest(QEvent *e)
     {
         if (e->type() != QEvent::Type(EVENT_PHASE_RESTANTES))
             return false;
         ev_PhasesRestantes *pre = static_cast<ev_PhasesRestantes*>(e);
         return (pre->getNbPhasesRestantes() == 0);
     }

     virtual void onTransition(QEvent *) {}
};


#endif // ET_TRANSITIONPHASESRESTANTES_H
