/*////////////////////////////////////////////////////////////
// \file et_NbCyclesMesureRestants.h
// \brief Classe implémentant la transition testant le
//        nombre de cycle de mesure restant lors de l'exécution des tests
//
// \author FOUQUART Christophe
// \date 20/06/2011
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

#ifndef ET_NBCYCLESMESURERESTANTS_H
#define ET_NBCYCLESMESURERESTANTS_H

#include "QEvent"
#include "QAbstractTransition"

//#include "ev_NbCyclesMesure.h"
#define EVENT_NB_CYCLES_MESURES QEvent::User+5

/*////////////////////////////////////////////////////////////////////////////////////
// \class ev_PhasesRestantesEvent
// \brief Classe d'évènement utilisé pour la transition testant le
//        nombre de cycle de mesure restant lors de l'exécution des tests
////////////////////////////////////////////////////////////////////////////////////*/
class ev_NbCyclesMesure : public QEvent
{
    ushort m_nbCyclesMesureRestants;

public :
    ev_NbCyclesMesure(const uint &val)
        : QEvent(QEvent::Type(EVENT_NB_CYCLES_MESURES)),
          m_nbCyclesMesureRestants(val) {}

    ushort getNbCyclesMesuresRestant() {return m_nbCyclesMesureRestants;}
};

/*////////////////////////////////////////////////////////////////////////////////////
// \class et_TransitionPhasesRestantes
// \brief Classe implémentant la transition testant le nombre de phases restantes
//        lors de l'exécution des tests métrologiques
//
// Classe implémentant la transition testant le nombre de phases restantes
// lors del'exécution des tests métrologiques
////////////////////////////////////////////////////////////////////////////////////*/
class et_NbCyclesMesureRestants : public QAbstractTransition
{
public:
     et_NbCyclesMesureRestants(QState * sourceState = 0) {m_sourceState = sourceState;}

 protected:
     virtual bool eventTest(QEvent *e)
     {
         if (e->type() != QEvent::Type(EVENT_NB_CYCLES_MESURES))
             return false;
         ev_NbCyclesMesure *pre = static_cast<ev_NbCyclesMesure*>(e);
         return (pre->getNbCyclesMesuresRestant() > 0);
     }

     virtual void onTransition(QEvent *) {}

private:
     QState* m_sourceState;
};

/*////////////////////////////////////////////////////////////////////////////////////
// \class et_CyclesMesureFinis
// \brief Classe implémentant la transition testant les
//        cycles de mesure ont été tous exécutés
//
// Classe implémentant la transition testant les cycles de mesure ont été tous exécutés
////////////////////////////////////////////////////////////////////////////////////*/
class et_CyclesMesureFinis : public QAbstractTransition
{
public:
     et_CyclesMesureFinis(QState * sourceState = 0) {m_sourceState = sourceState;}

 protected:
     virtual bool eventTest(QEvent *e)
     {
         if (e->type() != QEvent::Type(EVENT_NB_CYCLES_MESURES))
             return false;
         ev_NbCyclesMesure *pre = static_cast<ev_NbCyclesMesure*>(e);
         return (pre->getNbCyclesMesuresRestant() == 0);
     }

     virtual void onTransition(QEvent *) {}

private:
     QState* m_sourceState;
};

#endif // ET_NBCYCLESMESURERESTANTS_H
