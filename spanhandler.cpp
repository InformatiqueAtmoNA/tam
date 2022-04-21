/*////////////////////////////////////////////////////
// \file spanhandler.cpp
// \brief Classe gerant les paramètres de span
// \author FOUQUART Christophe
// \version 1.0
// \date 31/03/2011
//
// TAM - Tests Automatiques Metrologiques
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
////////////////////////////////////////////////////*/

#include "spanhandler.h"

SpanHandler::SpanHandler()
{
    this->spanArguments = new QVector<QVariant>(3);
}

void SpanHandler::setTabArgumentsSpan(QVector<bool> argumentUtiles) {
    this->tabArgumentsUtiles = &argumentUtiles;
}

QVector<bool>* SpanHandler::getTabArgumentsSpan() {
    return this->tabArgumentsUtiles;
}

void SpanHandler::setSpanArguments(const QString canal,const ushort point, const ushort concO3)
{
    (*this->spanArguments)[0] = canal;
    (*this->spanArguments)[1] = point;
    (*this->spanArguments)[2] = concO3;
}

QString SpanHandler::getCanal() const {
    QString canal = this->spanArguments->at(0).toString();
    return canal;
}

ushort SpanHandler::getPoint() const {
    ushort point = this->spanArguments->at(1).toUInt();
    return point;
}

ushort SpanHandler::getConcO3() const {
    ushort concO3 = this->spanArguments->at(2).toUInt();
    return concO3;
}
