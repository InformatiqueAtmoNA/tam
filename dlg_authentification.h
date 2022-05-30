#ifndef DLG_AUTHENTIFICATION_H
#define DLG_AUTHENTIFICATION_H

#include <QDialog>
#include "bdhandler.h"
#include <QAuthenticator>

namespace Ui {
class dlg_Authentification;
}

class dlg_Authentification : public QDialog
{
    Q_OBJECT

public:
    explicit dlg_Authentification(QWidget *parent = 0,const QPointer<BdHandler> bdHandler = new BdHandler());
    ~dlg_Authentification();
    bool connexionBD();
    bool connexionLDAP();
    const QAuthenticator &getUser() const;

private:
    Ui::dlg_Authentification *ui;

    bool connecte;
    bool connexionLocale;
    QAuthenticator user;
    QPointer<BdHandler> bdhandler;
    QList<QAuthenticator> listeUsers;

public Q_SLOTS:
    void closeEvent(QCloseEvent *event);
private Q_SLOTS:
    void butonValiderClicked();
};

#endif // DLG_AUTHENTIFICATION_H
