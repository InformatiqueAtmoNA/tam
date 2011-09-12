#ifndef DLG_EQUIPEMENT_H
#define DLG_EQUIPEMENT_H

#include <QDialog>
#include "dlg_modele.h"
#include "dlg_tx_transmission.h"
#include "dlg_molecule.h"

namespace Ui {
    class Dlg_Equipement;
}

class Dlg_Equipement : public QDialog
{
    Q_OBJECT

public:
    explicit Dlg_Equipement(QWidget *parent = 0,const QPointer<BdHandler> bdHandler = new BdHandler(),
                            const bool returnSelection=false, const int indexSelection =-1,
                            const QString & filtreType = "");
    ~Dlg_Equipement();

    uint getIdSelection();
    QString getNoSerieSelection();
private:
    Ui::Dlg_Equipement *ui;
    QPointer<QSqlRelationalTableModel> m_model;
    QPointer<QSqlRelationalTableModel> m_model_polluant_associe;
    QPointer<QSqlTableModel> m_model_Modele;
    QPointer<QSqlTableModel> m_model_tx_transmission;
    QPointer<QSqlTableModel> m_model_port_serie;
    QPointer<QSqlTableModel> m_model_molecule;

    QModelIndex m_indexSelection;
    QModelIndex m_indexSelectionMolecule;
    QModelIndex m_indexSelectionPolluant;
    QPointer<BdHandler> m_bdHandler;
    QString m_filtreType;
    bool m_returnSelection;
    bool m_modifEnCours;
    bool m_nouvelEnregistrement;
    int m_idModele;
    ushort m_idTxTransmission;
    ushort m_noport;
    int m_idModifie;

    void peuplerTable();
    void afficherFormulaire();

private Q_SLOTS:
    void changementSelection(const QModelIndex & idxSelection);
    void buttonSupprimerClicked();
    void buttonAjouterClicked();
    void initialiserChamps();
    void buttonValiderClicked();
    void buttonFermerClicked();
    void buttonSelectionnerClicked();
    void cb_ModeleChanged(const int index);
    void buttonEditModeleClicked();
    void cb_tauxTransmissionChanged(const int index);
    void cb_noPortSerieChanged(const int index);
    void buttonEditTxTransmissionClicked();
    void buttonModifierClicked();
    void buttonAddPolluantClicked();
    void buttonRemovePolluantClicked();
    void tableViewMoleculeIndexChanged(const QModelIndex & idxSelection);
    void tableViewPolluantIndexChanged(const QModelIndex & idxSelection);
};


#endif // DLG_EQUIPEMENT_H
