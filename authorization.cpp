#include "authorization.h"
#include "ui_authorization.h"

Authorization::Authorization(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint),
    ui(new Ui::Authorization)
{
    ui->setupUi(this);
    ui->lIncorrectData->setVisible(false);
}

Authorization::~Authorization()
{
    delete ui;
}

void Authorization::on_pbSignIn_clicked()
{
    authorize(ui->leLogin->text(), ui->lePassword->text());
}

void Authorization::incorrectAuthorizationData()
{
    ui->lIncorrectData->setVisible(true);
}
