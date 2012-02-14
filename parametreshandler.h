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

private Q_SLOTS:
    void buttonAnnulerClicked();
    void buttonOkClicked();
    void buttonParcourirClicked();
    void controlValueChanged();

Q_SIGNALS:
    void fermeture();
};

#endif // PARAMETRESHANDLER_H
