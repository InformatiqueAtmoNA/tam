#include "dlg_systeme_etalon.h"
#include "ui_dlg_systeme_etalon.h"

Dlg_Systeme_Etalon::Dlg_Systeme_Etalon(QWidget *parent,const QPointer<BdHandler> bdHandler,
                                       const bool returnSelection,const int indexSelection) :
    QDialog(parent),
    ui(new Ui::Dlg_Systeme_Etalon)
{
    ui->setupUi(this);
    this->setWindowTitle("Système étalon");

    this->m_bdHandler = bdHandler;
    this->m_returnSelection = returnSelection;

    this->peuplerTable();
    this->initialiserChamps();

    connect(this->ui->tableView->selectionModel(),
            SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this,SLOT(changementSelection(const QModelIndex &)));
    connect(this->ui->button_Supprimer,SIGNAL(clicked()),
            this,SLOT(buttonSupprimerClicked()));
    connect(this->ui->button_Ajouter,SIGNAL(clicked()),
            this,SLOT(buttonAjouterClicked()));
    connect(this->ui->button_Selectionner,SIGNAL(clicked()),
            this,SLOT(buttonSelectionnerClicked()));
    connect(this->ui->button_Fermer,SIGNAL(clicked()),
            this,SLOT(buttonFermerClicked()));
    connect(this->ui->button_Annuler,SIGNAL(clicked()),
            this,SLOT(initialiserChamps()));
    connect(this->ui->button_Valider,SIGNAL(clicked()),
            this,SLOT(buttonValiderClicked()));
    connect(this->ui->button_ChoixDiluteur,SIGNAL(clicked()),
            this,SLOT(buttonChoixDiluteurClicked()));
    connect(this->ui->button_ChoixBouteille,SIGNAL(clicked()),
            this,SLOT(buttonChoixBouteilleClicked()));
    connect(this->ui->button_ChoixGenAirZero,SIGNAL(clicked()),
            this,SLOT(buttonChoixGenAirZeroClicked()));
    connect(this->ui->button_Modifier,SIGNAL(clicked()),
            this,SLOT(buttonModifierClicked()));
    connect(this->ui->button_EditConcentration,SIGNAL(clicked()),
            this,SLOT(buttonEditConcentrationClicked()));

    if(indexSelection > -1) {
        this->ui->tableView->selectRow(indexSelection);
    }
}

Dlg_Systeme_Etalon::~Dlg_Systeme_Etalon()
{
    delete ui;

    if(!this->m_returnSelection)
        m_bdHandler->deconnexionBD();
}

void Dlg_Systeme_Etalon::peuplerTable() {
    if(!this->m_bdHandler->isOpen())
       m_bdHandler->connexionBD();

    m_model = m_bdHandler->getSystemeEtalonModel();
    m_model_sansRelation = m_bdHandler->getSystemeEtalonModelSansRelation();

    m_model->setParent(this);
    this->ui->tableView->setModel(m_model);
    this->ui->tableView->setColumnHidden(SYS_ETALON_ID, true);
    this->ui->tableView->resizeColumnsToContents();
    this->ui->tableView->setItemDelegate(new QSqlRelationalDelegate(this));
}

void Dlg_Systeme_Etalon::afficherFormulaire() {
    this->ui->gb_edit_champs->setVisible(true);
    this->ui->button_Ajouter->setEnabled(false);
    this->ui->button_Supprimer->setEnabled(false);
    this->ui->button_Fermer->setEnabled(false);
    this->ui->button_Modifier->setEnabled(false);

    this->ui->button_Valider->setDefault(true);
    this->ui->button_Selectionner->setEnabled(false);
    this->ui->button_EditConcentration->setEnabled(false);
}

void Dlg_Systeme_Etalon::initialiserChamps() {
    this->m_noSerieEquipemenent.clear();
    this->m_idDiluteur = 0;
    this->m_idBouteille = 0;
    this->m_idGenAirZero = 0;

    this->ui->gb_edit_champs->setVisible(false);

    this->ui->lineEdit_ChoixDiluteur->clear();
    this->ui->lineEdit_ChoixBouteille->clear();
    this->ui->lineEdit_ChoixGenAirZero->clear();
    this->ui->button_Ajouter->setEnabled(true);
    this->ui->button_Supprimer->setEnabled(true);
    this->ui->button_Fermer->setEnabled(true);
    this->ui->button_Modifier->setEnabled(true);
    this->ui->button_Selectionner->setVisible(true);
    this->ui->button_Selectionner->setEnabled(true);
    this->ui->button_EditConcentration->setEnabled(true);

    if(!this->m_returnSelection || this->m_model->rowCount()==0) {
        this->ui->button_Selectionner->setVisible(false);
    }
}

void Dlg_Systeme_Etalon::changementSelection(const QModelIndex & idxSelection) {
    this->m_indexSelection = idxSelection;
    QSqlRecord record = m_model_sansRelation->record(idxSelection.row());
    this->m_idDiluteur = record.value(SYS_ETALON_DILUTEUR).toInt();
    this->m_idBouteille = record.value(SYS_ETALON_BOUTEILLE).toInt();
    this->m_idGenAirZero = record.value(SYS_ETALON_GZERO).toInt();
}

void Dlg_Systeme_Etalon::buttonSupprimerClicked() {
    if(!m_indexSelection.isValid())
        return;
    int reponse = QMessageBox::question(this, "Supprimer un élément", "êtes-vous sûr de vouloir effacer cet enregistrement?",
                  QMessageBox::Yes | QMessageBox::No);

    if (reponse == QMessageBox::No)
        return;

    if(!m_model.data()->removeRow(m_indexSelection.row()))
        QMessageBox::critical(this,"Impossible de supprimer","Erreur de la suppression de l'enregistrement demandé",QMessageBox::Ok);
    else {
        m_model->submitAll();
        m_model_sansRelation->select();
        this->initialiserChamps();
    }
}

void Dlg_Systeme_Etalon::buttonAjouterClicked() {
    this->afficherFormulaire();
}

void Dlg_Systeme_Etalon::buttonValiderClicked() {

    int row;
    if(m_modifEnCours)
        row=m_ligneModifiee;
    else {
        row = m_model->rowCount();
        if(row>0)
            row-=1;
        m_model->insertRow(row);
    }

    m_model->setData(m_model->index(row,SYS_ETALON_DILUTEUR),QVariant::fromValue(this->m_idDiluteur));
    if(this->m_idBouteille>0)
        m_model->setData(m_model->index(row,SYS_ETALON_BOUTEILLE),QVariant::fromValue(this->m_idBouteille));
    else
        m_model->setData(m_model->index(row,SYS_ETALON_BOUTEILLE),QVariant::fromValue(1));
    if(this->m_idGenAirZero>0)
        m_model->setData(m_model->index(row,SYS_ETALON_GZERO),QVariant::fromValue(this->m_idGenAirZero));
    else
        m_model->setData(m_model->index(row,SYS_ETALON_GZERO),QVariant::fromValue(1));

    m_model->submitAll();
    m_model_sansRelation->select();
    m_modifEnCours = false;

    this->initialiserChamps();
}

int Dlg_Systeme_Etalon::afficherDlg_Equipement(const QString & filtreType,const int index) {
    Dlg_Equipement dlgEquipement(this,this->m_bdHandler,true,index,filtreType);

    int result = dlgEquipement.exec();

    delete m_model;
    this->peuplerTable();

    if(result) {
        this->m_noSerieEquipemenent = dlgEquipement.getNoSerieSelection();
        return dlgEquipement.getIdSelection();
    }
    else
        return index;
}

void Dlg_Systeme_Etalon::buttonChoixDiluteurClicked() {
    this->m_noSerieEquipemenent = this->ui->lineEdit_ChoixDiluteur->text();
    int oldIndex = this->m_idDiluteur;
    this->m_idDiluteur = this->afficherDlg_Equipement("DILUTEUR",oldIndex);
    this->ui->lineEdit_ChoixDiluteur->setText(this->m_noSerieEquipemenent);
}

void Dlg_Systeme_Etalon::buttonChoixBouteilleClicked() {
    this->m_noSerieEquipemenent = this->ui->lineEdit_ChoixBouteille->text();
    int oldIndex = this->m_idBouteille;
    this->m_idBouteille = this->afficherDlg_Equipement("BOUTEILLE",oldIndex);
    this->ui->lineEdit_ChoixBouteille->setText(this->m_noSerieEquipemenent);
}

void Dlg_Systeme_Etalon::buttonChoixGenAirZeroClicked() {
    this->m_noSerieEquipemenent = this->ui->lineEdit_ChoixGenAirZero->text();
    int oldIndex = this->m_idGenAirZero;
    this->m_idGenAirZero= this->afficherDlg_Equipement("GZERO",oldIndex);
    this->ui->lineEdit_ChoixGenAirZero->setText(this->m_noSerieEquipemenent);
}

void Dlg_Systeme_Etalon::buttonFermerClicked() {
    this->reject();
}

void Dlg_Systeme_Etalon::buttonSelectionnerClicked() {
    this->accept();
}

int Dlg_Systeme_Etalon::getIdSelection() {
    return this->m_model->record(m_indexSelection.row()).value(MODELE_ID).toInt();
}

void Dlg_Systeme_Etalon::buttonModifierClicked() {
    if(!m_indexSelection.isValid())
        return;
    this->m_modifEnCours = true;
    this->m_ligneModifiee = m_indexSelection.row();

    QSqlRecord selection = this->m_model->record(m_ligneModifiee);

    this->ui->lineEdit_ChoixDiluteur->setText(selection.value(SYS_ETALON_DILUTEUR).toString());
    this->ui->lineEdit_ChoixBouteille->setText(selection.value(SYS_ETALON_BOUTEILLE).toString());
    this->ui->lineEdit_ChoixGenAirZero->setText(selection.value(SYS_ETALON_GZERO).toString());

    this->afficherFormulaire();
}

void Dlg_Systeme_Etalon::buttonEditConcentrationClicked() {
    if(!m_indexSelection.isValid())
        return;
    QSqlRecord selection = this->m_model->record(m_indexSelection.row());

    qDebug()<<selection.value(SYS_ETALON_ID).toInt();

    Dlg_Concentration dlg_concentration(this,this->m_bdHandler,selection.value(SYS_ETALON_ID).toInt());
    dlg_concentration.exec();
}
