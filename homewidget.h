#ifndef HOMEWIDGET_H
#define HOMEWIDGET_H

#include <QWidget>
#include "creationtest.h"

namespace Ui {
    class HomeWidget;
}

class HomeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HomeWidget(QPointer<BdHandler> bdHandler,QWidget *parent = 0);
    ~HomeWidget();

private:
    Ui::HomeWidget *ui;
    QPointer<BdHandler> m_bdHandler;
    QPointer<QSqlTableModel> m_modelTestXml;
    uint m_idxSelectionTest;

    void getListeTests();

private slots:
    void tableWidgetTestXmlIndexChanged(const QModelIndex & index);
    void buttonNouveauClicked();
    void buttonModifierClicked();
    void buttonExecuterClicked();
    void buttonAfficherClicked();

signals:
    void creerTest();
    void modifierTest(const QString fichierDescription);
    void executerTest(const QString fichierDescription);
};

#endif // HOMEWIDGET_H
