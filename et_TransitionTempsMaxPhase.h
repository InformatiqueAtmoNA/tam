/*////////////////////////////////////////////////////////////
// \file et_TransitionTempsMaxPhase.h
// \brief Classe implémentant la transition testant si un temps maximum de phase est défini
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

#ifndef ET_TRANSITIONTEMPSMAXPHASE_H
#define ET_TRANSITIONTEMPSMAXPHASE_H

//#include "ev_TempsMaxPhase.h"

#define EVENT_TEMPS_MAX_PHASE QEvent::User+3

/*//////////////////////////////////////////////////////////////////////////////////////////
// \class TempsMaxPhaseEvent
// \brief Classe d'évènement utilisé pour la transition testant si un temps maximum de phase est défini
//////////////////////////////////////////////////////////////////////////////////////////*/
class ev_TempsMaxPhase : public QEvent
{
    ushort m_noPhase;

public:
    ev_TempsMaxPhase(ushort noPhase)
        : QEvent(QEvent::Type(EVENT_TEMPS_MAX_PHASE)),
          m_noPhase(noPhase) {}

    ushort getNoPhase() {return m_noPhase;}
};

/*//////////////////////////////////////////////////////////////////////////////////////////
// \class et_TransitionTempsMaxPhase
// \brief Classe implémentant la transition testant si un temps maximum de phase est défini
//
// Classe implémentant la transition testant si un temps maximum de phase est défini
//////////////////////////////////////////////////////////////////////////////////////////*/
class et_TransitionTempsMaxPhase : public QAbstractTransition
{
public:
     et_TransitionTempsMaxPhase(QMap<ushort,QTime> listeTempsMaxPhase) {m_listeTempsMaxPhase = listeTempsMaxPhase;}

protected:
     virtual bool eventTest(QEvent *e)
     {
         if (e->type() != QEvent::Type(EVENT_TEMPS_MAX_PHASE))
             return false;
         ev_TempsMaxPhase *pre = static_cast<ev_TempsMaxPhase*>(e);
         QTime tempsMaxPhase = m_listeTempsMaxPhase.value(pre->getNoPhase());
         if(tempsMaxPhase.isNull())
             return false;
         if(!tempsMaxPhase.isValid())
             return false;
         if(tempsMaxPhase.hour()==0 && tempsMaxPhase.second()==0 && tempsMaxPhase.minute()==0)
             return false;
         return true;
     }

     virtual void onTransition(QEvent *) {}

private:
     QMap<ushort,QTime> m_listeTempsMaxPhase;
};

/*//////////////////////////////////////////////////////////////////////////////////////////
// \class et_TransitionTempsMaxPhaseNonDefini
// \brief Classe implémentant la transition testant si un temps maximum de phase n'est pas défini
//
// Classe implémentant la transition testant si un temps maximum de phase n'est pas défini
//////////////////////////////////////////////////////////////////////////////////////////*/
class et_TransitionTempsMaxPhaseNonDefini : public QAbstractTransition
{
public:
     et_TransitionTempsMaxPhaseNonDefini(QMap<ushort,QTime> listeTempsMaxPhase) {m_listeTempsMaxPhase = listeTempsMaxPhase;}

protected:
     virtual bool eventTest(QEvent *e)
     {
         if (e->type() != QEvent::Type(EVENT_TEMPS_MAX_PHASE))
             return false;
         ev_TempsMaxPhase *pre = static_cast<ev_TempsMaxPhase*>(e);
         QTime tempsMaxPhase = m_listeTempsMaxPhase.value(pre->getNoPhase());
         if(tempsMaxPhase.isNull())
             return true;
         if(!tempsMaxPhase.isValid())
             return true;
         if(tempsMaxPhase.hour()==0 && tempsMaxPhase.second()==0 && tempsMaxPhase.minute()==0)
             return true;
         return false;
     }

     virtual void onTransition(QEvent *) {}

private:
     QMap<ushort,QTime> m_listeTempsMaxPhase;
};

#endif // ET_TRANSITIONTEMPSMAXPHASE_H
