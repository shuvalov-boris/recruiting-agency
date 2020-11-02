#include "accounting.h"

bool Accounting::createConnection()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(DATABASE_NAME);
    db.setUserName(USERNAME);
    db.setHostName("localHost");
    db.setPassword(PASSWORD);

    if (!db.open())
    {
        qDebug() << "Произошла ошибка при открытии базы данных:"
                 << db.lastError();
        QMessageBox msgBox;
        msgBox.setWindowTitle("Ошибка");
        msgBox.setText(QString("Произошла ошибка при открытии базы данных:") +
                       db.lastError().text());
        msgBox.exec();
        return false;
    }

    return true;
}

bool Accounting::createFormsTable()
{
    QMessageBox msgBox;
    msgBox.setText(QString("Создание таблицы анкет"));
    msgBox.exec();

    QSqlQuery query;
    QString str = "CREATE TABLE employee ( "
        "number INTEGER PRIMARY KEY NOT NULL, "
        "photo LONGBLOB, "
        "lastname VARCHAR(15), "
        "firstname VARCHAR(15), "
        "patronymic VARCHAR(15), "
        "sex VARCHAR(10), "
        "birthday DATE, "
        "address VARCHAR(200), "
        "activity VARCHAR(200), "
        "experience INTEGER, "
        "phone VARCHAR(20), "
        "email VARCHAR(20), "
        "status VARCHAR(30), "
        "editor INTEGER NOT NULL"
        ");";
    if (!query.exec(str))
    {
        qDebug() << "Unable to create forms table";
        QMessageBox msgBox;
        msgBox.setText(QString("Произошла ошибка при создании таблицы"
                         "'forms' базы данных"));
        msgBox.exec();
        return false;
    }
    return true;
}

bool Accounting::createAuthTable()
{
    qDebug() << "creating auth table";
    QSqlQuery query;
    QString str = "CREATE TABLE users ( "
            "number INTEGER PRIMARY KEY NOT NULL, "
            "login VARCHAR(20), "
            "password VARCHAR(20), "
            "role VARCHAR(20) "
        ");";
    if (!query.exec(str))
    {
        qDebug() << "Unable to create authorization table";
        QMessageBox msgBox;
        msgBox.setText(QString("Произошла ошибка при создании таблицы"
                         " 'users' базы данных"));
        msgBox.exec();
        return false;
    }
    QString strF = "INSERT INTO users (number, login, password, role)"
            "VALUES(%1, '%2', '%3', '%4');";
    str = strF.arg("1")
            .arg(USERNAME)
            .arg(PASSWORD)
            .arg(STR_ROLES[role]);
    if (!query.exec(str))
    {
        qDebug() << "Unable to make auth insert operation: " << query.lastError();
        return false;
    }
    return true;
}

bool Accounting::createCommentTable()
{
    qDebug() << "creating comment table";
    QSqlQuery query;
    if (!query.exec("CREATE TABLE comments ("
                    "formID INTEGER PRIMARY KEY NOT NULL, "
                    "comment VARCHAR(300)"
                    ");"))
    {
        qDebug() << "Unable to create comments table";
        QMessageBox msgBox;
        msgBox.setText(QString("Произошла ошибка при создании таблицы"
                         " 'comments' базы данных"));
        msgBox.exec();
        return false;
    }
    return true;
}

bool Accounting::isAuthDataByDefault(const QString& login,
                                     const QString& password)
{
    return (login == USERNAME && password == PASSWORD);
}

bool Accounting::identifyUser(const QString& login, const QString& password)
{
    if (db.isOpen())
    {
        QSqlQuery query;
        QString strF = "SELECT * FROM users WHERE login = '%1';";
        QString str = strF.arg(login);
        if (!query.exec(str))
        {
            qDebug() << "Unable identify user";
            return false;
        }
        QSqlRecord rec = query.record();
        if (!query.next()) return false;
        QString strPassword = query.value(rec.indexOf("password")).toString();
        if (password != strPassword) return false;
        QString strRole = query.value(rec.indexOf("role")).toString();
        userID = query.value(rec.indexOf("number")).toInt();
        if (strRole == STR_ROLES[ER_ADMINISTRATOR]) role = ER_ADMINISTRATOR;
        else if (strRole == STR_ROLES[ER_MODERATOR]) role = ER_MODERATOR;
        else if (strRole == STR_ROLES[ER_EXECUTOR]) role = ER_EXECUTOR;
        else return false;
        return true;
    }
    else if (isAuthDataByDefault(login, password))
    {
        role = ER_ADMINISTRATOR;
        return true;
    }
    else return false;
}

//void Accounting::setEmployeeModel()
//{
//    model.setQuery("SELECT * FROM employee;");
//    if (model.lastError().isValid()) {
//        qDebug() << "setTableModel" << model.lastError();
//    }
//    view.setModel(&model);
//    view.show();
//}

//void Accounting::setAccountModel()
//{
//    authModel.setQuery("SELECT * FROM users;");
//    if (authModel.lastError().isValid()) {
//        qDebug() << "setAuthTableModel" << authModel.lastError();
//    }
//    authView.setModel(&authModel);
//    authView.show();
//}

void Accounting::setAuthEditTableModel()
{
    accountsForm = new AccountsForm();
    accountsForm->initModel(db);
    accountsForm->show();
}

//void Accounting::setCommentsModel()
//{
//    commentsModel.setQuery("SELECT * FROM comments;");
//    if (commentsModel.lastError().isValid()) {
//        qDebug() << "setAuthTableModel" << commentsModel.lastError();
//    }
//    commentsView.setModel(&commentsModel);
//    commentsView.show();
//}

bool Accounting::initLastID()
{
    QString str = "SELECT MAX(number) FROM employee";
    QSqlQuery query;
    if (!query.exec(str))
    {
        qDebug() << "initLastID: Unable to execute query";
        return false;
    }
    if (query.first())
        lastID = query.value(0).toInt();
    return true;
}

void Accounting::initMainWindow()
{
    switch (role)
    {
    case ER_ADMINISTRATOR:
        mainWindow.initAddForm();
        mainWindow.initViewForm();
        mainWindow.initViewAllForms();
        mainWindow.initEditAccount();
        mainWindow.initStatistics();
        break;
    case ER_MODERATOR:
        mainWindow.initViewForm();
        mainWindow.initViewAllForms();
        mainWindow.initAllocateForms();
        mainWindow.initConfirmForms();
        mainWindow.initStatistics();
        break;
    case ER_EXECUTOR:
        mainWindow.initConsiderForms();
        break;
    default:
        qDebug() << "role error";
        break;
    }
}

bool Accounting::getExecutorsList(QStringList& list)
{
    QSqlQuery query;
    if (!query.exec("SELECT login FROM users WHERE role = 'Executor'"))
    {
        qDebug() << "getExecList: Unable exec query: " << query.lastError();
        return false;
    }
    QSqlRecord rec = query.record();
    while (query.next())
        list.append(query.value(0).toString());
    return true;
}

FormData Accounting::getEmployeeRecord(const QSqlQuery& query, const QSqlRecord& rec)
{
    FormData form_data;
    form_data.photo = query.value(rec.indexOf("photo")).toByteArray();
    form_data.last_name = query.value(rec.indexOf("lastname")).toString();
    form_data.first_name = query.value(rec.indexOf("firstname")).toString();
    form_data.patronymic = query.value(rec.indexOf("patronymic")).toString();
    form_data.sex = query.value(rec.indexOf("sex")).toString();
    form_data.birthday = query.value(rec.indexOf("birthday")).toDate();
    form_data.address = query.value(rec.indexOf("address")).toString();
    form_data.activity = query.value(rec.indexOf("activity")).toString();
    form_data.experience = query.value(rec.indexOf("experience")).toInt();
    form_data.phone = query.value(rec.indexOf("phone")).toString();
    form_data.email = query.value(rec.indexOf("email")).toString();

    qDebug() << "get Employee: birthday is "
             << query.value(rec.indexOf("birthday")).toString();
    return form_data;
}

void Accounting::setNextPendingFormsQuery()
{
    if (!pendingFormsQuery.next())
    {
        delete pendingForm;
        QMessageBox msgBox;
        msgBox.setText("Все анкеты рассмотрены");
        msgBox.exec();
        return;
    }

    FormData form_data = getEmployeeRecord(pendingFormsQuery, pendingRec);

    emit viewFormForAllocating(form_data);
}

void Accounting::setNextConfirmFormsQuery()
{
    if (!confirmQuery.next()) return;

    FormData form_data = getEmployeeRecord(confirmQuery, confirmRec);

    emit viewFormForConfirming(form_data);

    QSqlQuery query;
    uint idEmployee = confirmQuery.value(
                confirmRec.indexOf("number")).toInt();
    QString strF = "SELECT comment FROM comments WHERE formID = %1;";
    QString str = strF.arg(QString::number(idEmployee));
    if (!query.exec(str))
    {
        qDebug() << "setNCFQ: unable to execute query: " << query.lastError();
        return;
    }
    if (query.first())
        teComment->setPlainText(query.value(0).toString());
}

void Accounting::showMessageThatsAll()
{
    QMessageBox msgBox;
    msgBox.setText("Все анкеты рассмотрены");
    msgBox.setWindowTitle("Учет анкет");
    msgBox.exec();
}

Accounting::Accounting()
{
    connect(&authForm,   SIGNAL(authorize(const QString&, const QString&)),
                         SLOT(authorize(const QString&, const QString&)));
    connect( this,       SIGNAL(incorrectAuthorizationData()),
            &authForm,   SLOT(incorrectAuthorizationData()));
    connect(&mainWindow, SIGNAL(addNewForm()),   this, SLOT(createNewForm()));
    connect(&mainWindow, SIGNAL(viewForms()),    this, SLOT(viewFormsData()));
    connect(&mainWindow, SIGNAL(viewAllForms()), SLOT(createAllForms()));
    connect(&mainWindow, SIGNAL(viewAccounts()), this, SLOT(viewAccounts()));
    connect(&mainWindow, SIGNAL(allocateForms()),
            this, SLOT(createAllocatingForms()));
    connect(&mainWindow, SIGNAL(considerForms()),
            this, SLOT(createConsiderForms()));
    connect(&mainWindow, SIGNAL(confirmForms()),
            this, SLOT(createConfirmingForms()));
    connect(&mainWindow, SIGNAL(viewStatistics()),
            this, SLOT(createStatisticsForm()));
}

Accounting::~Accounting()
{
//    delete form;
}

void Accounting::run()
{
    createConnection();

//    QSqlQuery query;
//    if (!query.exec("UPDATE employee SET status = 'Ожидает распределения'"
//                    ", editor = -1;"))
//        qDebug() << "Unable to update data";

    authForm.show();
}

void Accounting::authorize(const QString& login, const QString& password)
{
    if (identifyUser(login, password))
    {
        authForm.close();

        QStringList tables = db.tables();
        if (tables.length() < 3/*.empty()*/)
        {
            createFormsTable();
            createAuthTable();
            createCommentTable();
        }

//        setEmployeeModel();
//        setAccountModel();
//        setCommentsModel();

        initLastID();

        initMainWindow();
        mainWindow.show();
    }
    else
        incorrectAuthorizationData();
}

void Accounting::createNewForm()
{
    newForm = new Form();
    QPushButton *pbAdd = new QPushButton("Добавить");
    connect(pbAdd, SIGNAL(clicked()), newForm, SLOT(on_pbAdd_clicked()));
    connect(newForm, SIGNAL(addFormData(const FormData&)),
            this,       SLOT(addFormData(const FormData&)));
    QHBoxLayout *layout = newForm->setAdditionalLayout();
    layout->addWidget(pbAdd);
    newForm->setWindowTitle("Добавление анкеты");
    newForm->show();
}

void Accounting::addFormData(const FormData& form_data)
{
    QSqlQuery query;
    QString strF =
        "INSERT INTO employee (number, photo, lastname, firstname, patronymic, "
        "sex, birthday, address, activity, experience, phone, email, "
        "status, editor) "
        "VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";
    query.prepare(strF);

    if (!form_data.photo.isNull())
    {
        qDebug() << "addFD: loaded photo";
        qDebug() << "addFD: photo length is " << form_data.photo.length();
        query.addBindValue(form_data.photo);
    }
    else
    {
        qDebug() << "addFD: empty photo";
        query.addBindValue(QByteArray());
    }

    query.addBindValue(QString::number(++lastID));
    query.addBindValue(form_data.last_name);
    query.addBindValue(form_data.first_name);
    query.addBindValue(form_data.patronymic);
    query.addBindValue(form_data.sex);
    query.addBindValue(form_data.birthday.toString());
    query.addBindValue(form_data.address);
    query.addBindValue(form_data.activity);
    query.addBindValue(QString::number(form_data.experience));
    query.addBindValue(form_data.phone);
    query.addBindValue(form_data.email);
    query.addBindValue("Ожидает распределения");
    query.addBindValue("-1");

    if (!query.exec())
        qDebug() << "Unable to make insert operation: " << query.lastError();

    while (query.next())
        qDebug() << "one record";

//    setEmployeeModel();
}

void Accounting::viewFormsData()
{
    if (!(viewFormQuery = db.exec("SELECT * FROM employee;")).isActive())
    {
        qDebug() << "viewForm: Unable to execute query — exiting"
                 << viewFormQuery.lastError();
        return;
    }
    viewRec = viewFormQuery.record();

    viewForms = new Form();
    connect(this, SIGNAL(viewForm(FormData)),
            viewForms, SLOT(viewFormData(FormData)));
    QPushButton * pbNext = new QPushButton("Следующий");
    connect(pbNext, SIGNAL(clicked()), this, SLOT(setNextViewFormsQuery()));
    connect(this, SIGNAL(viewForm(FormData)),
            viewForms, SLOT(viewFormData(FormData)));
    QHBoxLayout *layout = viewForms->setAdditionalLayout();
    layout->addWidget(pbNext);
    viewForms->show();

    setNextViewFormsQuery();
}

void Accounting::createAllForms()
{
    model.setQuery("SELECT * FROM employee;");
    if (model.lastError().isValid()) {
        qDebug() << "setTableModel" << model.lastError();
    }
    view.setModel(&model);
    view.show();
}

void Accounting::setNextViewFormsQuery()
{
    if (!viewFormQuery.next()) return;

    FormData form_data = getEmployeeRecord(viewFormQuery, viewRec);

    emit viewForm(form_data);
}

void Accounting::createAllocatingForms()
{
    if (!(pendingFormsQuery = db.exec("SELECT * FROM employee "
                    "WHERE status = 'Ожидает распределения';")).isActive())
    {
        qDebug() << "allocForm: Unable to execute query — exiting: "
                 << pendingFormsQuery.lastError();
        return;
    }
    if (!pendingFormsQuery.first())
    {
        qDebug() << "allocateForm: There are no records";
        showMessageThatsAll();
        return;
    }
    pendingFormsQuery.previous();
    pendingRec = pendingFormsQuery.record();

    pendingForm = new Form();
    QStringList executors;
    if (!getExecutorsList(executors)) return;

    QSignalMapper *signalMapper = new QSignalMapper(this);
    QHBoxLayout *layout = pendingForm->setAdditionalLayout();
    QPushButton *pbExecutor;
    foreach (QString str, executors)
    {
        pbExecutor = new QPushButton(str);
        signalMapper->setMapping(pbExecutor, str);
        layout->addWidget(pbExecutor);
        connect(pbExecutor, SIGNAL(clicked()),
                signalMapper, SLOT(map()));
    }
    connect(signalMapper, SIGNAL(mapped(const QString &)),
            this, SLOT(sendFormToExecutor(const QString &)));
    connect(this, SIGNAL(viewFormForAllocating(FormData)),
            pendingForm, SLOT(viewFormData(FormData)));

    pendingForm->setWindowTitle("Распределение анкет по исполнителям");
    pendingForm->show();

    setNextPendingFormsQuery();
}

void Accounting::sendFormToExecutor(const QString& executor)
{
    qDebug() << "sendFormToExecutor: " << executor;
    uint idEmployee = pendingFormsQuery.value(
                pendingRec.indexOf("number")).toInt();
    QSqlQuery query;
    QString strF = "SELECT number FROM users WHERE login = '%1';";
    QString str = strF.arg(executor);
    if (!query.exec(str))
    {
        qDebug() << "sendFormToExecutor: Unable to get user ID: "
                 << query.lastError();
        return;
    }
    uint idExecutor = -1;
    if (query.first())
       idExecutor = query.value(0).toInt();
    else
    {
        qDebug() << "sendFormToExecutor: There are no such executor";
        return;
    }
    qDebug() << "sendFormToExecutor: ID executor " << idExecutor;
    strF = "UPDATE employee SET status = 'На рассмотрении исполнителя'"
            ", editor = %1 WHERE number = %2";
    str = strF.arg(QString::number(idExecutor))
            .arg(QString::number(idEmployee));
    if (!query.exec(str))
        qDebug() << "sendFormToExecutor: Unable to update form status: "
                 << query.lastError();

    setNextPendingFormsQuery();
//    setEmployeeModel();
}

void Accounting::createConsiderForms()
{
    qDebug() << "ConsiderForms!";
    QString strF = "SELECT * FROM employee "
                   "WHERE status = 'На рассмотрении исполнителя'"
                   " AND editor = %1;";
    QString str = strF.arg(QString::number(userID));
    if (!(pendingFormsQuery = db.exec(str)).isActive())
    {
        qDebug() << "considerForms: Unable to execute query — exiting: "
                 << pendingFormsQuery.lastError();
        return;
    }
    if (!pendingFormsQuery.first())
    {
        qDebug() << "considerForms: There are no records"; //TODO
        showMessageThatsAll();
        return;
    }
    pendingFormsQuery.previous();
    pendingRec = pendingFormsQuery.record();

    pendingForm = new Form();

    QHBoxLayout *layout = pendingForm->setAdditionalLayout();
    QVBoxLayout *vlayout = new QVBoxLayout;
    QPushButton *pbConfirm = new QPushButton("Утвердить");
    QPushButton *pbRework = new QPushButton("На доработку");
    teComment = new QTextEdit;
    vlayout->addWidget(teComment);
    vlayout->addWidget(pbRework);
    layout->addLayout(vlayout);
    layout->addWidget(pbConfirm);

    connect(pbConfirm, SIGNAL(clicked()), this, SLOT(confirmForm()));
    connect(pbRework, SIGNAL(clicked()), this, SLOT(reworkForm()));
    connect(this, SIGNAL(viewFormForAllocating(FormData)),
            pendingForm, SLOT(viewFormData(FormData)));

    pendingForm->setWindowTitle("Проверка анкет на корректность");
    pendingForm->show();

    setNextPendingFormsQuery();
}

void Accounting::confirmForm()
{
    uint idEmployee = pendingFormsQuery.value(
                pendingRec.indexOf("number")).toInt();
    QString strF = "UPDATE employee SET status = 'Утверждена' "
            "WHERE number = %1;";
    QString str = strF.arg(QString::number(idEmployee));
//            .arg(QString::number(userID));
    QSqlQuery query;
    if (!query.exec(str))
    {
        qDebug() << "confirmForm: Unable to update form status: "
                 << query.lastError();
        return;
    }
    strF = "UPDATE users SET confirmed = confirmed + 1 WHERE number = %1;";
    str = strF.arg(userID);
    if (!query.exec(str))
    {
        qDebug() << "confirmForm: unable update data: "
                 << query.lastError();
    }
    setNextPendingFormsQuery();
//    setEmployeeModel();
//    setAccountModel();
}

void Accounting::reworkForm()
{
    qDebug() << "reworkForm!";
    uint idEmployee = pendingFormsQuery.value(
                pendingRec.indexOf("number")).toInt();

    QString strF = "UPDATE employee SET status = 'На доработке' "
            "WHERE number = %1;";
    QString str = strF.arg(idEmployee);
    QSqlQuery query;
    if (!query.exec(str))
    {
        qDebug() << "reworkForm: Unable to update form status: "
                 << query.lastError();
        return;
    }

    strF = "INSERT INTO comments (formID, comment) VALUES (%1, '%2');";
    str = strF.arg(QString::number(idEmployee))
            .arg(teComment->toPlainText());
    if (!query.exec(str))
    {
        qDebug() << "reworkForm: Unable to insert comment record: "
                 << query.lastError();
        return;
    }
    teComment->clear();

    strF = "UPDATE users SET reworked = reworked + 1 WHERE number = %1;";
    str = strF.arg(QString::number(userID));
    if (!query.exec(str))
    {
        qDebug() << "reworkForm: unable update data by inc: "
                 << query.lastError();
    }

    setNextPendingFormsQuery();
//    setEmployeeModel();
//    setAccountModel();
//    setCommentsModel();
}

void Accounting::createConfirmingForms()
{
    qDebug() << "ConsiderForms!";
    QString str = "SELECT * FROM employee "
                   "WHERE status = 'На доработке';";
    if (!(confirmQuery = db.exec(str)).isActive())
    {
        qDebug() << "considerForms: Unable to execute query — exiting: "
                 << confirmQuery.lastError();
        return;
    }
    if (!confirmQuery.first())
    {
        qDebug() << "considerForms: There are no records";
        showMessageThatsAll();
        return;
    }
    confirmQuery.previous();
    confirmRec = confirmQuery.record();

    newForm = new Form();

    QHBoxLayout *layout = newForm->setAdditionalLayout();
    QVBoxLayout *vlayout = new QVBoxLayout;
    QLabel *label = new QLabel("Причина отвода:");
    QPushButton *pbConfirm = new QPushButton("Утвердить");
    teComment = new QTextEdit;
    vlayout->addWidget(label);
    vlayout->addWidget(teComment);
    layout->addLayout(vlayout);
    layout->addWidget(pbConfirm);

    connect(pbConfirm, SIGNAL(clicked()), newForm, SLOT(on_pbAdd_clicked()));
    connect(newForm, SIGNAL(addFormData(const FormData&)),
            this,       SLOT(completeForm(const FormData&)));
    connect(this, SIGNAL(viewFormForConfirming(FormData)),
            newForm, SLOT(viewFormData(FormData)));

    newForm->setWindowTitle("Проверка отведенных анкет");
    newForm->show();

    setNextConfirmFormsQuery();
}

void Accounting::completeForm(const FormData& form_data)
{
    uint idEmployee = confirmQuery.value(
                confirmRec.indexOf("number")).toInt();
    QSqlQuery query;
    if (!query.prepare("UPDATE employee SET "
            "lastname = ?, firstname = ?, patronymic = ?, sex = ?, "
            "birthday = ?, address = ?, activity = ?, experience = ?, "
            "phone = ?, email = ?, status = ? WHERE number = ?;"))
    {
        qDebug() << "completeForm: unable to prepare query: "
                 << query.lastError();
        return;
    }
    query.addBindValue(form_data.last_name);
    query.addBindValue(form_data.first_name);
    query.addBindValue(form_data.patronymic);
    query.addBindValue(form_data.sex);
    query.addBindValue(form_data.birthday.toString());
    query.addBindValue(form_data.address);
    query.addBindValue(form_data.activity);
    query.addBindValue(QString::number(form_data.experience));
    query.addBindValue(form_data.phone);
    query.addBindValue(form_data.email);
    query.addBindValue("Утверждена");
    query.addBindValue(QString::number(idEmployee));
    if (!query.exec())
    {
        qDebug() << "completeForm: unable to execute query: "
                 << query.lastError();
        return;
    }

    if (!query.prepare("DELETE FROM comments WHERE formID = ?;"))
    {
        qDebug() << "completeForm: unable to prepare query: "
                 << query.lastError();
        return;
    }
    query.addBindValue(QString::number(idEmployee));
    if (!query.exec())
    {
        qDebug() << "completeForm: unable to execute query: "
                 << query.lastError();
        return;
    }

    setNextConfirmFormsQuery();
//    setEmployeeModel();
//    setCommentsModel();
}

void Accounting::createStatisticsForm()
{
    QStringList employeeList;
    if (!getExecutorsList(employeeList))
        return;//TODO

    QWidget *widget = new QWidget;
    QVBoxLayout *blayout = new QVBoxLayout;
    QLabel *execLabel = new QLabel("Статистика по исполнителям:", widget);
    QTableWidget *table = new QTableWidget;
    table->setRowCount(employeeList.size());
    table->setColumnCount(2);
    QLabel *allLabel = new QLabel("Общая статистика:", widget);
    QTableWidget *atable = new QTableWidget;
    atable->setRowCount(1);
    atable->setColumnCount(3);
    blayout->addWidget(execLabel);
    blayout->addWidget(table);
    blayout->addWidget(allLabel);
    blayout->addWidget(atable);
    widget->setLayout(blayout);

    QSqlQuery query;
    QTableWidgetItem *item;
    QString strF = "SELECT confirmed, reworked FROM users WHERE login = '%1'";
    for (int i = 0; i < employeeList.size(); ++i)
    {
        QString str = strF.arg(employeeList[i]);
        if (!query.exec(str))
        {
            qDebug() << "createSF: unable exec query: " << query.lastError();
            return;
        }
        if (!query.first())
            qDebug() << "createSF: no records";
        item = new QTableWidgetItem(employeeList[i]);
        table->setVerticalHeaderItem(i, item);
        item = new QTableWidgetItem(query.value(0).toString());
        table->setItem(i, 0, item);
        item = new QTableWidgetItem(query.value(1).toString());
        table->setItem(i, 1, item);
    }
    widget->setWindowTitle("Статистика");
    item = new QTableWidgetItem("Всего");
    atable->setHorizontalHeaderItem(0, item);
    item = new QTableWidgetItem("Утверждено");
    table->setHorizontalHeaderItem(0, item);
    atable->setHorizontalHeaderItem(1, item);
    item = new QTableWidgetItem("На доработку");
    table->setHorizontalHeaderItem(1, item);
    atable->setHorizontalHeaderItem(2, item);

    if (!query.exec("SELECT COUNT(*) FROM employee;"))
    {
        qDebug() << "createSF: unable exec query (SELECT COUNT): "
                 << query.lastError();
        return;
    }
    if (!query.first())
        qDebug() << "В таблице employee нет записей.";
    int formsAll = query.value(0).toInt();
    qDebug() << "Всего " << formsAll << " анкет";

    if (!query.exec("SELECT COUNT(*) FROM employee WHERE status = 'Утверждена';"))
    {
        qDebug() << "createSF: unable exec query (SELECT Утв): "
                 << query.lastError();
        return;
    }
    if (!query.first())
        qDebug() << "В таблице employee нет записей.";
    int formsConfirmed = query.value(0).toInt();
    qDebug() << "Утверждено " << formsConfirmed << " анкет";

    if (!query.exec("SELECT COUNT(*) FROM employee WHERE status = 'На доработке';"))
    {
        qDebug() << "createSF: unable exec query (SELECT на доработке): "
                 << query.lastError();
        return;
    }
    if (!query.first())
        qDebug() << "В таблице employee нет записей.";
    int formsReworked = query.value(0).toInt();
    qDebug() << "Утверждено " << formsReworked << " анкет";

    item = new QTableWidgetItem(QString::number(formsAll));
    atable->setItem(0, 0, item);
    item = new QTableWidgetItem(QString::number(formsConfirmed));
    atable->setItem(0, 1, item);
    item = new QTableWidgetItem(QString::number(formsReworked));
    atable->setItem(0, 2, item);

    widget->show();
}
