/*////////////////////////////////////////////////////
// \file creationtest.h
// \brief Classe d'interface graphique g�rant la cr�ation des tests
// \author FOUQUART Christophe
// \version 1.0
// \date 01/04/2011
//
// TAM - Tests Automatiques M�trologiques
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
////////////////////////////////////////////////////*/

#ifndef CREATIONTEST_H
#define CREATIONTEST_H

#include <QWidget>

namespace Ui {
    class CreationTest;
}

class CreationTest : public QWidget
{
    Q_OBJECT

public:
    explicit CreationTest(QWidget *parent = 0);
    ~CreationTest();

private:
    Ui::CreationTest *ui;
};

#endif // CREATIONTEST_H
