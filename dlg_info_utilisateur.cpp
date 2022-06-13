#include "dlg_info_utilisateur.h"
#include "ui_dlg_info_utilisateur.h"

dlg_info_utilisateur::dlg_info_utilisateur(QAuthenticator aUser,const QPointer<BdHandler> m_bdHandler, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlg_info_utilisateur)
{
    ui->setupUi(this);
    this->bdhandler = m_bdHandler;
    this->user = aUser;
    this->afficherInfo();
    connect(this->ui->Buton_Fermer, SIGNAL(clicked()),this,SLOT(butonFermerClicked()));
    connect(this->ui->Buton_ModifierMDP, SIGNAL(clicked()),this,SLOT(ButonModifierMDPClicked()));
    connect(this->ui->Buton_valider, SIGNAL(clicked()),this,SLOT(ButonValiderClicked()));
}

dlg_info_utilisateur::~dlg_info_utilisateur()
{
    delete ui;
}

const QAuthenticator &dlg_info_utilisateur::getUser() const
{
    return user;
}

void dlg_info_utilisateur::afficherInfo()
{
    this->ui->label_5->hide();
    this->ui->lineEdit_NouveauMDP->hide();
    this->ui->Buton_valider->hide();
    QSqlRecord *record = bdhandler->getOperateurRow(this->user.user());

    this->ui->label_Nom->setText(record->value(OPERATEUR_NOM).toString());
    this->ui->label_Prenom->setText(record->value(OPERATEUR_PRENOM).toString());
    if(record->value(OPERATEUR_ADMIN).toInt()==1){
        this->ui->label_Droits->setText("Administrateur");
    }
    else{
        this->ui->label_Droits->setText("Utilisateur");
    }

    this->ui->label_identifiant->setText(user.user());
    this->ui->label_MDP->setText(user.password());
}


// Q_SLOTS

void dlg_info_utilisateur::butonFermerClicked()
{
    this->accept();
}

void dlg_info_utilisateur::ButonModifierMDPClicked()
{
    this->ui->label_5->show();
    this->ui->lineEdit_NouveauMDP->show();
    this->ui->Buton_valider->show();
}

void dlg_info_utilisateur::ButonValiderClicked()
{
    QString requete =QString("UPDATE Operateur SET Mot_de_passe=%1 ").arg(this->ui->lineEdit_NouveauMDP->text());
    requete.append(QString("WHERE user_name=%1").arg('"' + this->user.user() + '"'));
    QSqlQuery query(requete);
    if(!query.exec()){
        QMessageBox msgBox;
        msgBox.setText(QLatin1String("Erreur lors de l'enregistrement du mot de passe"));
        msgBox.exec();
        return;
    }
    this->user.setPassword(this->ui->lineEdit_NouveauMDP->text());
    this->afficherInfo();
}


