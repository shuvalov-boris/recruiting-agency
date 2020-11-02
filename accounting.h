#ifndef ACCOUNTING_H
#define ACCOUNTING_H

#include "authorization.h"
#include "mainwindow.h"
#include "form.h"
#include "accountsform.h"

#include <QObject>
#include <QtSql>
#include <QMessageBox>
#include <QTableView>
#include <QFileDialog>
#include <QFile>
#include <QByteArray>
#include <QPushButton>
#include <QLayout>
#include <QBoxLayout>
#include <QWidget>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QSignalMapper>
#include <QTextEdit>
#include <QLabel>
#include <QTableWidget>
#include <QSpacerItem>

enum E_ROLE
{
    ER_ADMINISTRATOR,
    ER_MODERATOR,
    ER_EXECUTOR,
    ER_UNKNOW
};

const QString DATABASE_NAME = "forms";
const QString USERNAME = "admin";
const QString PASSWORD = "password";
const QString STR_ROLES[] = {"Administrator", "Moderator", "Executor"};

class Accounting : public QObject
{
    Q_OBJECT


private:
    E_ROLE role;
    uint userID;

    Authorization authForm;
    MainWindow mainWindow;
    Form *newForm;
    Form *viewForms;
    Form *pendingForm;
    AccountsForm *accountsForm;

    QTableView view;
    QSqlQueryModel model;
    QTableView authView;
    QSqlQueryModel authModel;
    QTableView commentsView;
    QSqlQueryModel commentsModel;

    QSqlDatabase db;
    QSqlQuery viewFormQuery;
    QSqlRecord viewRec;
    QSqlQuery pendingFormsQuery;
    QSqlRecord pendingRec;
    QSqlQuery confirmQuery;
    QSqlRecord confirmRec;

    QTextEdit   *teComment; //TODO

    int lastID;

    bool createConnection();
    bool createFormsTable();
    bool createAuthTable();
    bool createCommentTable();

    bool isAuthDataByDefault(const QString& login, const QString& password);
    bool identifyUser(const QString& login, const QString& password);

//    void setEmployeeModel();
//    void setAccountModel();
    void setAuthEditTableModel();
//    void setCommentsModel();

    bool initLastID();

    void initMainWindow();

    bool getExecutorsList(QStringList& list);

    FormData getEmployeeRecord(const QSqlQuery &query, const QSqlRecord& rec);

    void setNextPendingFormsQuery();
    void setNextConfirmFormsQuery();

    void showMessageThatsAll();

public:
    Accounting();
    ~Accounting();

    void run();

signals:
    void incorrectAuthorizationData();
    void viewForm(const FormData&);
    void viewFormForAllocating(const FormData&);
    void viewFormForConfirming(const FormData&);

public slots:
    void authorize(const QString& login, const QString& password);
    void createNewForm();
    void addFormData(const FormData& form_data);
    void viewFormsData();
    void createAllForms();
    void viewAccounts()
    { setAuthEditTableModel(); }
    void setNextViewFormsQuery();
    void createAllocatingForms();
    void sendFormToExecutor(const QString& executor);
    void createConsiderForms();
    void confirmForm();
    void reworkForm();
    void createConfirmingForms();
    void completeForm(const FormData &form_data);
    void createStatisticsForm();
};

#endif // ACCOUNTING_H
