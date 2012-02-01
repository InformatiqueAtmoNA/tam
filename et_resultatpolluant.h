/*////////////////////////////////////////////////////////////
// \file et_resultatpolluant.h
// \brief Classe d'interface gérant l'affihage des rapports des tests
// \author PIET Régis
// \version 1.0
// \date 10/08/2011
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

#ifndef ET_RESULTATPOLLUANT_H
#define ET_RESULTATPOLLUANT_H

#include <QWidget>
#include <QDebug>

namespace Ui {
    class et_Resultatpolluant;
}

class et_Resultatpolluant : public QWidget
{
    Q_OBJECT

public:
    explicit et_Resultatpolluant(
            QVector<float>tabConcentration,QVector<QVector<float> > tabMesPolluant,
            QVector<float>  tabMoyenne,QVector<float>  tabEcartType,
            QWidget *parent = 0);
    explicit et_Resultatpolluant(
            QVector<float> tabConcentration,QVector<QVector<float> > tabMesPolluant,
            QVector<float>  tabMoyenneTypePolluant,QVector<float>  tabEcartType,QVector <float> tabValeurPourCritere,
            QVector<float> tabResidu,QVector<float> tabResiduRel,QVector<float> tabResiduInc,
            QWidget *parent = 0);

    ~et_Resultatpolluant();

private:
    Ui::et_Resultatpolluant *ui;

    QStringList m_listeEnteteLigne;

    void creationColonneTab (QVector<float> tabPolluant);
    void affichageTableauMesure (QVector<QVector<float> > tabMesure);
    void affichageTableauCalcul (QVector<float> tabCalcul,QString typeCalcul);
    void affichageTableauEcartType(QVector<float> tabEcartType);
    void affichageTableauResidu(QVector<float> tabResidu);

};

#endif // ET_RESULTATPOLLUANT_H
