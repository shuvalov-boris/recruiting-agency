#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initAddForm()
{
    pbAddForm = new QPushButton("Загрузить анкету");
    ui->layout->addWidget(pbAddForm);
    connect(pbAddForm, SIGNAL(clicked()),
            this, SLOT(on_pbAddForm_clicked()));
}

void MainWindow::initViewForm()
{
    pbViewForm = new QPushButton("Просмотреть анкеты");
    ui->layout->addWidget(pbViewForm);
    connect(pbViewForm, SIGNAL(clicked()),
            this, SLOT(on_pbViewForm_clicked()));
}

void MainWindow::initViewAllForms()
{
    pbAllForms = new QPushButton("Все анкеты");
    ui->layout->addWidget(pbAllForms);
    connect(pbAllForms, SIGNAL(clicked()), SIGNAL(viewAllForms()));
}

void MainWindow::initEditAccount()
{
    pbEditAccount = new QPushButton("Учетные записи");
    ui->layout->addWidget(pbEditAccount);
    connect(pbEditAccount, SIGNAL(clicked()),
            this, SLOT(on_pbAccount_clicked()));
}

void MainWindow::initAllocateForms()
{
    pbAllocateForms = new QPushButton(
                "Распределить новые анкеты по исполнителям");
    ui->layout->addWidget(pbAllocateForms);
    connect(pbAllocateForms, SIGNAL(clicked()),
            this, SLOT(on_pbAllocateForms_clicked()));
}

void MainWindow::initConsiderForms()
{
    pbConsiderForms = new QPushButton("Проверить анкеты на корректность");
    ui->layout->addWidget(pbConsiderForms);
    connect(pbConsiderForms, SIGNAL(clicked()),
            SLOT(on_pbConsiderForms_clicked()));
}

void MainWindow::initConfirmForms()
{
    pbConfirmForms = new QPushButton("Утвердить отведенные анкеты");
    ui->layout->addWidget(pbConfirmForms);
    connect(pbConfirmForms, SIGNAL(clicked()),
            SLOT(on_pbConfirmForms_clicked()));
}

void MainWindow::initStatistics()
{
    pbStatistics = new QPushButton("Статистика");
    ui->layout->addWidget(pbStatistics);
    connect(pbStatistics, SIGNAL(clicked()), SIGNAL(viewStatistics()));
}

void MainWindow::on_pbAddForm_clicked()
{
    emit addNewForm();
}

void MainWindow::on_pbViewForm_clicked()
{
    emit viewForms();
}

void MainWindow::on_pbAccount_clicked()
{
    emit viewAccounts();
}

void MainWindow::on_pbAllocateForms_clicked()
{
    emit allocateForms();
}

void MainWindow::on_pbConsiderForms_clicked()
{
    emit considerForms();
}

void MainWindow::on_pbConfirmForms_clicked()
{
    emit confirmForms();
}
