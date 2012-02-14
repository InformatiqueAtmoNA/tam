#include "parametreshandler.h"
#include "ui_parametreshandler.h"

ParametresHandler::ParametresHandler(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParametresHandler)
{
    ui->setupUi(this);
    ui->comboBox_TypeBase->setCurrentIndex(ui->comboBox_TypeBase->findText(getParam("BD_Driver").toString()));
    ui->lineEdit_DbHost->setText(getParam("Host").toString());
    ui->lineEdit_DbName->setText(getParam("DB_Name").toString());
    ui->lineEdit_DbUser->setText(getParam("UserName").toString());
    ui->lineEdit_DbPassword->setText(getParam("Password").toString());
    ui->lineEdit_RepXML->setText(getParam("Path_XML").toString());
    ui->lineEdit_RepCSV->setText(getParam("Path_CSV").toString());
    ui->lineEdit_RepRapports->setText(getParam("Path_Rapports").toString());

    //ui->pushButton_Ok->setEnabled(false);

    connect(ui->comboBox_TypeBase,SIGNAL(currentIndexChanged(int)),this,SLOT(controlValueChanged()));
    connect(ui->pushButton_Ok,SIGNAL(clicked()),this,SLOT(buttonOkClicked()));
    connect(ui->pushButton_RepXML_Parcourir,SIGNAL(clicked()),this,SLOT(buttonParcourirClicked()));
}

ParametresHandler::~ParametresHandler()
{
    delete ui;
}

void ParametresHandler::buttonAnnulerClicked()
{
    emit(this->fermeture());
}

void ParametresHandler::buttonOkClicked()
{
    setParam("BD_Driver",QVariant::fromValue(ui->comboBox_TypeBase->currentText()));
    setParam("Host",QVariant::fromValue(ui->lineEdit_DbHost->text()));
    setParam("DB_Name",QVariant::fromValue(ui->lineEdit_DbName->text()));
    setParam("UserName",QVariant::fromValue(ui->lineEdit_DbUser->text()));
    setParam("Password",QVariant::fromValue(ui->lineEdit_DbPassword->text()));
    setParam("Path_XML",QVariant::fromValue(ui->lineEdit_RepXML->text()));
    setParam("Path_CSV",QVariant::fromValue(ui->lineEdit_RepCSV->text()));
    setParam("Path_Rapports",QVariant::fromValue(ui->lineEdit_RepRapports->text()));
}

void ParametresHandler::controlValueChanged()
{
    ui->pushButton_Ok->setEnabled(true);
}

void ParametresHandler::buttonParcourirClicked()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::ShowDirsOnly,true);
    dialog.exec();
    QStringList chemin =  dialog.selectedFiles();
    ui->lineEdit_RepXML->setText(chemin.value(0));
}


