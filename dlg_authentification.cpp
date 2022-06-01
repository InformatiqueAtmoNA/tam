#include "dlg_authentification.h"
#include "QCloseEvent"
#include "ui_dlg_authentification.h"

dlg_Authentification::dlg_Authentification(QWidget *parent,const QPointer<BdHandler> bdHandler) :
    QDialog(parent),
    ui(new Ui::dlg_Authentification)
{
    ui->setupUi(this);
    this->ui->erreur->hide();
    this->ui->checkBox->hide();
    this->ui->mot_de_passe->setEchoMode(QLineEdit::Password);
    this->bdhandler = bdHandler;
    this->connecte = false;
    this->connexionLocale = true;
    connect(this->ui->buton_Valider, SIGNAL(clicked()), this, SLOT(butonValiderClicked())); 
}

dlg_Authentification::~dlg_Authentification()
{
    delete ui;
}

bool dlg_Authentification::connexionBD()
{
    bool validation = false;

    this->user.setUser(this->ui->indentifiant->text().toLatin1());
    this->user.setPassword(this->ui->mot_de_passe->text().toLatin1());
    QSqlRecord *record = bdhandler->getOperateurRow(this->user.user());

    if(record!=NULL && user.password()== record->value(OPERATEUR_MDP).toString()){
        validation=true;
    }
    return validation;
}

bool dlg_Authentification::connexionLDAP()
{
    bool validation = false;
    this->connexionLocale = false;
    return validation;
}

const QAuthenticator &dlg_Authentification::getUser() const
{
    return user;
}

void dlg_Authentification::butonValiderClicked(){

    if(this->ui->checkBox->checkState()==Qt::Checked){
        connecte = this->connexionLDAP();
    }
    else{
        connecte = this->connexionBD();
    }

    if(connecte){
        this->ui->erreur->setText(QLatin1String("<font color=\"#64FF64\">Connecté</font>"));
        this->ui->erreur->show();
        this->accept();
    }
    else{
        this->ui->erreur->setText(QLatin1String("<font color=\"#FF0000\">Erreur: mot de passe ou identifiant erroné</font>"));
        this->ui->erreur->show();
    }
}

void dlg_Authentification::closeEvent(QCloseEvent *event)
 {

    QMessageBox msgBox;
    msgBox.setText(QLatin1String("Fermer ?"));
    msgBox.setInformativeText(QLatin1String("Voulez-vous quitter l'application ?"));
    msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);

    if(msgBox.exec()==QMessageBox::Ok) {
        event->accept();
        this->reject();
    }
    else{
        event->ignore();
    }
 }
