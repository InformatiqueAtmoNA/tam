#ifndef DLG_INFO_UTILISATEUR_H
#define DLG_INFO_UTILISATEUR_H

#include "bdhandler.h"

#include <QAuthenticator>
#include <QDialog>

namespace Ui {
class dlg_info_utilisateur;
}

class dlg_info_utilisateur : public QDialog
{
    Q_OBJECT

public:
    explicit dlg_info_utilisateur(QAuthenticator aUser,const QPointer<BdHandler> bdHandler = new BdHandler(), QWidget *parent = nullptr);
    ~dlg_info_utilisateur();

    const QAuthenticator &getUser() const;
    void afficherInfo();
private:
    Ui::dlg_info_utilisateur *ui;
    QAuthenticator user;
    QPointer<BdHandler> bdhandler;

private Q_SLOTS:
    void butonFermerClicked();
    void ButonModifierMDPClicked();
    void ButonValiderClicked();
};

#endif // DLG_INFO_UTILISATEUR_H
