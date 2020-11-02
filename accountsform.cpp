#include "accountsform.h"
#include "ui_accountsform.h"

AccountsForm::AccountsForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AccountsForm)
{
    ui->setupUi(this);
}

AccountsForm::~AccountsForm()
{
    delete ui;
}

bool AccountsForm::initModel(const QSqlDatabase& db)
{
    model = new QSqlTableModel(0, db);
    model->setTable("users");
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    if (!model->select())
    {
        qDebug() << "setAuthEditTableModel" << model->lastError();
        return false;
    }

    ui->view->setModel(model);
    ui->view->show();
    return true;
}

void AccountsForm::on_pbAdd_clicked()
{
    model->insertRow(model->rowCount());
    model->setData(model->index(model->rowCount() - 1, 4), "0");
    model->setData(model->index(model->rowCount() - 1, 5), "0");
}

void AccountsForm::on_pbRemove_clicked()
{
    QModelIndexList list = ui->view->selectionModel()->selectedRows();
    if (list.empty())
    {
        QMessageBox msgBox;
        msgBox.setText("Чтобы удалить учетную запись, "
                       "необходимо выделить всю строку!");
        msgBox.setWindowTitle("Справка");
        msgBox.exec();
        return;
    }
    foreach(QModelIndex index, list)
        model->removeRow(index.row());
}
