#ifndef ACCOUNTSFORM_H
#define ACCOUNTSFORM_H

#include <QWidget>
#include <QTableView>

#include <QtSql>
#include <QSqlTableModel>
#include <QModelIndexList>
#include <QMessageBox>

namespace Ui {
class AccountsForm;
}

class AccountsForm : public QWidget
{
    Q_OBJECT

public:
    explicit AccountsForm(QWidget *parent = 0);
    ~AccountsForm();
    bool initModel(const QSqlDatabase& db);

private slots:
    void on_pbAdd_clicked();

    void on_pbRemove_clicked();

private:
    Ui::AccountsForm *ui;

    QSqlTableModel *model;
};

#endif // ACCOUNTSFORM_H
