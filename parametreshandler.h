#ifndef PARAMETRESHANDLER_H
#define PARAMETRESHANDLER_H

#include <QDialog>
#include <QFileDialog>
#include "definitions_globales.h"

namespace Ui {
    class ParametresHandler;
}

class ParametresHandler : public QDialog
{
    Q_OBJECT

public:
    explicit ParametresHandler(QWidget *parent = 0);
    ~ParametresHandler();

private:
    Ui::ParametresHandler *ui;
    QStringList choixRepertoire(QString repertoire);

private Q_SLOTS:
    void buttonAnnulerClicked();
    void buttonOkClicked();
    void buttonParcourirXMLClicked();
    void buttonParcourirCSVClicked();
    void buttonParcourirRapportClicked();
    void controlValueChanged();

};

#endif // PARAMETRESHANDLER_H
