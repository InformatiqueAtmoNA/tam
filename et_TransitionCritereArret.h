/*////////////////////////////////////////////////////////////
// \file et_TransitionCritereArretDefini.h
// \brief Classe impl�mentant la transition testant si un crit�re d'arr�t est d�fini
//
// \author FOUQUART Christophe
// \date 29/06/2011
//
// TAM - Tests Automatiques M�trologiques
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

#ifndef ET_TRANSITIONCRITEREARRETDEFINI_H
#define ET_TRANSITIONCRITEREARRETDEFINI_H

#include "QEvent"
#include "QAbstractTransition"
//#include "ev_TestCritereArret.h"

#define EVENT_TEST_CRITERE_ARRET QEvent::User+6

/*//////////////////////////////////////////////////////////////////////////////////////////
// \class TempsMaxPhaseEvent
// \brief Classe d'�v�nement utilis� pour la transition testant si un crit�re d'arr�t est d�fini
//////////////////////////////////////////////////////////////////////////////////////////*/
class ev_TestCritereArret : public QEvent
{
    ushort m_noPhase;

public:
    ev_TestCritereArret(ushort noPhase)
        : QEvent(QEvent::Type(EVENT_TEST_CRITERE_ARRET)),
          m_noPhase(noPhase) {}

    ushort getNoPhase() {return m_noPhase;}
};

/*//////////////////////////////////////////////////////////////////////////////////////////
// \class et_TransitionCritereArretDefini
// \brief Classe impl�mentant la transition testant si un crit�re d'arr�t est d�fini
//
// Classe impl�mentant la transition testant si un crit�re d'arr�t est d�fini
//////////////////////////////////////////////////////////////////////////////////////////*/
class et_TransitionCritereArretDefini : public QAbstractTransition
{
public:
     et_TransitionCritereArretDefini(QVector<ushort> listeCritereArretDefinis)
         {m_listeCritereArretDefinis = listeCritereArretDefinis;}

 protected:
     virtual bool eventTest(QEvent *e)
     {
         if (e->type() != QEvent::Type(EVENT_TEST_CRITERE_ARRET))
             return false;
         ev_TestCritereArret *pre = static_cast<ev_TestCritereArret*>(e);
         bool critereArretDefini = m_listeCritereArretDefinis.contains(pre->getNoPhase());
         if(critereArretDefini)
             qDebug()<<"Crit�re d'arr�t d�fini";
         return critereArretDefini;
     }

     virtual void onTransition(QEvent *) {}

private:
     QVector<ushort> m_listeCritereArretDefinis;
};

/*//////////////////////////////////////////////////////////////////////////////////////////
// \class et_TransitionCritereArretNonDefini
// \brief Classe impl�mentant la transition testant si un crit�re d'arr�t n'est pas d�fini
//
// Classe impl�mentant la transition testant si un crit�re d'arr�t n'est pas d�fini
//////////////////////////////////////////////////////////////////////////////////////////*/
class et_TransitionCritereArretNonDefini : public QAbstractTransition
{
public:
     et_TransitionCritereArretNonDefini(QVector<ushort> listeCritereArretDefinis)
         {m_listeCritereArretDefinis = listeCritereArretDefinis;}

 protected:
     virtual bool eventTest(QEvent *e)
     {
         if (e->type() != QEvent::Type(EVENT_TEST_CRITERE_ARRET))
             return false;
         ev_TestCritereArret *pre = static_cast<ev_TestCritereArret*>(e);
         bool critereArretNonDefini = !(m_listeCritereArretDefinis.contains(pre->getNoPhase()));
         if(critereArretNonDefini)
             qDebug()<<"Crit�re d'arr�t non d�fini";
         return critereArretNonDefini;
     }

     virtual void onTransition(QEvent *) {}

private:
     QVector<ushort> m_listeCritereArretDefinis;
};

#endif // ET_TRANSITIONCRITEREARRETDEFINI_H
