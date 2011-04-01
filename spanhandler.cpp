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

void SpanHandler::setSpanArguments(QString const & canal,ushort const & point, ushort const & concO3) {
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
