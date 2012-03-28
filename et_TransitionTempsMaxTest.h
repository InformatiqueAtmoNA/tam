/*////////////////////////////////////////////////////////////
// \file et_TransitionTempsMaxTest.h
// \brief Classe implémentant la transition testant si un temps maximum de test est défini
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

#ifndef ET_TRANSITIONTEMPSMAXTEST_H
#define ET_TRANSITIONTEMPSMAXTEST_H

//#include "ev_TempsMaxTest.h"
#define EVENT_TEMPS_MAX_TEST QEvent::User+2

/*////////////////////////////////////////////////////////////////////////////////////////////
// \class TempsMaxTestEvent
// \brief Classe d'évènement utilisé pour la transition testant si un temps maximum de test est défini
////////////////////////////////////////////////////////////////////////////////////////////*/
class ev_TempsMaxTest : public QEvent
{
public:
    ev_TempsMaxTest()
        : QEvent(QEvent::Type(EVENT_TEMPS_MAX_TEST)) {}
};

/*////////////////////////////////////////////////////////////////////////////////////////////
// \class et_TransitionTempsMaxTest
// \brief Classe implémentant la transition testant si un temps maximum de test est défini
//
// Classe implémentant la transition testant si un temps maximum de test est défini
////////////////////////////////////////////////////////////////////////////////////////////*/
class et_TransitionTempsMaxTest : public QAbstractTransition
{
public:
     et_TransitionTempsMaxTest(bool tempsMaxTestDefini) {m_tempsMaxTestDefini = tempsMaxTestDefini;}

protected:
     virtual bool eventTest(QEvent *e)
     {
         if (e->type() != QEvent::Type(EVENT_TEMPS_MAX_TEST))
             return false;
         return (m_tempsMaxTestDefini);
     }

     virtual void onTransition(QEvent *) {}

private:
     bool m_tempsMaxTestDefini;
};

/*////////////////////////////////////////////////////////////////////////////////////////////
// \class et_TransitionTempsMaxTestNonDefini
// \brief Classe implémentant la transition testant si un temps maximum de test n'est pas défini
//
// Classe implémentant la transition testant si un temps maximum de test n'est pas défini
////////////////////////////////////////////////////////////////////////////////////////////*/
class et_TransitionTempsMaxTestNonDefini : public QAbstractTransition
{
public:
     et_TransitionTempsMaxTestNonDefini(bool tempsMaxTestDefini) {m_tempsMaxTestDefini = tempsMaxTestDefini;}

protected:
     virtual bool eventTest(QEvent *e)
     {
         if (e->type() != QEvent::Type(EVENT_TEMPS_MAX_TEST))
             return false;
         return (!m_tempsMaxTestDefini);
     }

     virtual void onTransition(QEvent *) {}

private:
     bool m_tempsMaxTestDefini;
};

#endif // ET_TRANSITIONTEMPSMAXTEST_H
