#ifndef DLG_SYSTEME_ETALON_H
#define DLG_SYSTEME_ETALON_H

#include <QDialog>
#include "dlg_equipement.h"
#include "dlg_concentration.h"

namespace Ui {
    class Dlg_Systeme_Etalon;
}

class Dlg_Systeme_Etalon : public QDialog
{
    Q_OBJECT

public:
    explicit Dlg_Systeme_Etalon(QWidget *parent = 0,const QPointer<BdHandler> bdHandler = new BdHandler(),
                                const bool returnSelection=false,const int indexSelection =-1);
    ~Dlg_Systeme_Etalon();

    int getIdSelection();
private:
    Ui::Dlg_Systeme_Etalon *ui;

    QPointer<QSqlRelationalTableModel> m_model;
    QPointer<QSqlTableModel> m_model_sansRelation;
    QModelIndex m_indexSelection;
    QPointer<BdHandler> m_bdHandler;
    QString m_noSerieEquipemenent;
    int m_idDiluteur;
    int m_idBouteille;
    int m_idGenAirZero;
    int m_ligneModifiee;
    bool m_modifEnCours;
    bool m_returnSelection;

    void peuplerTable();
    void afficherFormulaire();

    int afficherDlg_Equipement(const QString & filtreType,const int index=-1);

private slots:
    void changementSelection(const QModelIndex & idxSelection);
    void buttonSupprimerClicked();
    void buttonAjouterClicked();
    void initialiserChamps();
    void buttonValiderClicked();
    void buttonFermerClicked();
    void buttonModifierClicked();
    void buttonSelectionnerClicked();
    void buttonChoixDiluteurClicked();
    void buttonChoixBouteilleClicked();
    void buttonChoixGenAirZeroClicked();
    void buttonEditConcentrationClicked();
};

#endif // DLG_SYSTEME_ETALON_H
