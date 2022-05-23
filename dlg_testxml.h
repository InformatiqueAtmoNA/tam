#ifndef DLG_TESTXML_H
#define DLG_TESTXML_H

#include <QDialog>
#include "bdhandler.h"

namespace Ui {
    class Dlg_testXml;
}

class Dlg_testXml : public QDialog
{
    Q_OBJECT

public:
    explicit Dlg_testXml(QWidget *parent = 0,const QPointer<BdHandler> bdHandler = new BdHandler(),
                         const bool returnSelection=false);
    ~Dlg_testXml();

    QList<ushort> getIdSelection();
    QList<QString> getFichierDescriptionSelection();


private:
    Ui::Dlg_testXml *ui;
    QPointer<QSqlTableModel> m_model;
    QModelIndex m_indexSelection;
    QPointer<BdHandler> m_bdHandler;
    bool m_returnSelection;
    QList<QModelIndex> indexTestsSelectionne;
    void afficherTable();

private Q_SLOTS:
    void changementSelection(QModelIndex idxSelection);
    void initialiserChamps();
    void buttonFermerClicked();
    void buttonSelectionnerClicked();
    void buttonConfirmerClicked();
    void buttonSupprimerClicked();
};

#endif // DLG_TESTXML_H
