#include "homewidget.h"
#include "ui_homewidget.h"

HomeWidget::HomeWidget(QPointer<BdHandler> bdHandler,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomeWidget)
{
    ui->setupUi(this);

    this->m_bdHandler = bdHandler;
    this->m_idxSelectionTest = 0;
    this->getListeTests();

    connect(this->ui->button_Afficher,SIGNAL(clicked()),this,SLOT(buttonAfficherClicked()));
    connect(this->ui->button_Executer,SIGNAL(clicked()),this,SLOT(buttonExecuterClicked()));
    connect(this->ui->button_Modifier,SIGNAL(clicked()),this,SLOT(buttonModifierClicked()));
    connect(this->ui->button_Nouveau,SIGNAL(clicked()),this,SLOT(buttonNouveauClicked()));
    connect(this->ui->tableWidget_TestXml,SIGNAL(clicked(QModelIndex)),this,SLOT(tableWidgetTestXmlIndexChanged(QModelIndex)));
}

HomeWidget::~HomeWidget()
{
    delete m_modelTestXml;
    delete ui;
}

void HomeWidget::getListeTests()
{
    this->m_modelTestXml = this->m_bdHandler->getTestXmlModel();

    for(int i=0;i<this->m_modelTestXml->rowCount();i++) {
        QSqlRecord recordTestXml = this->m_modelTestXml->record(i);
        uint idSystemeEtalon = recordTestXml.value(TEST_XML_ID_SYSTEME_ETALON).toUInt();
        QSqlRecord recordSystemeEtalon = this->m_bdHandler->getSystemeEtalonModel(idSystemeEtalon)->record(0);

        QTableWidgetItem* item_idTest = new QTableWidgetItem(recordTestXml.value(TEST_XML_ID).toString());
        QTableWidgetItem* item_fichier = new QTableWidgetItem(recordTestXml.value(TEST_XML_NOM_FICHIER).toString());
        QTableWidgetItem* item_type = new QTableWidgetItem(recordTestXml.value(TEST_XML_TYPE_TEST).toString());
        QTableWidgetItem* item_numSerieDiluteur = new QTableWidgetItem(recordSystemeEtalon.value(SYS_ETALON_DILUTEUR).toString());
        QTableWidgetItem* item_numSerieBouteille = new QTableWidgetItem(recordSystemeEtalon.value(SYS_ETALON_BOUTEILLE).toString());
        QTableWidgetItem* item_numSerieGoz = new QTableWidgetItem(recordSystemeEtalon.value(SYS_ETALON_GZERO).toString());

        uint idxNewRecord = this->ui->tableWidget_TestXml->rowCount();
        this->ui->tableWidget_TestXml->insertRow(idxNewRecord);
        this->ui->tableWidget_TestXml->setItem(idxNewRecord,HOMEW_TABLEW_TEST_ID_TEST,item_idTest);
        this->ui->tableWidget_TestXml->setItem(idxNewRecord,HOMEW_TABLEW_TEST_FICHIER,item_fichier);
        this->ui->tableWidget_TestXml->setItem(idxNewRecord,HOMEW_TABLEW_TEST_TYPE,item_type);
        this->ui->tableWidget_TestXml->setItem(idxNewRecord,HOMEW_TABLEW_TEST_DILUTEUR,item_numSerieDiluteur);
        this->ui->tableWidget_TestXml->setItem(idxNewRecord,HOMEW_TABLEW_TEST_BOUTEILLE,item_numSerieBouteille);
        this->ui->tableWidget_TestXml->setItem(idxNewRecord,HOMEW_TABLEW_TEST_GZERO,item_numSerieGoz);
        this->ui->tableWidget_TestXml->setColumnHidden(HOMEW_TABLEW_TEST_ID_TEST,true);
    }
}

void HomeWidget::tableWidgetTestXmlIndexChanged(const QModelIndex & index)
{
    if(index.isValid())
        this->m_idxSelectionTest = index.row();
    else
        this->m_idxSelectionTest = 0;
}

void HomeWidget::buttonNouveauClicked()
{
    emit(this->creerTest());
}

void HomeWidget::buttonModifierClicked()
{
    QString fichierDescription = this->ui->tableWidget_TestXml->item(m_idxSelectionTest,HOMEW_TABLEW_TEST_FICHIER)->text();
    emit(this->modifierTest(fichierDescription));
}

void HomeWidget::buttonExecuterClicked()
{
    QString fichierDescription = this->ui->tableWidget_TestXml->item(m_idxSelectionTest,HOMEW_TABLEW_TEST_FICHIER)->text();
    emit(this->executerTest(fichierDescription));
}

void HomeWidget::buttonAfficherClicked()
{

}
