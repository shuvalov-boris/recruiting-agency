#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QPushButton>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void initAddForm();
    void initViewForm();
    void initViewAllForms();
    void initEditAccount();
    void initAllocateForms();
    void initConsiderForms();
    void initConfirmForms();
    void initStatistics();

private slots:
    void on_pbAddForm_clicked();
    void on_pbViewForm_clicked();
    void on_pbAccount_clicked();
    void on_pbAllocateForms_clicked();
    void on_pbConsiderForms_clicked();
    void on_pbConfirmForms_clicked();

signals:
    void addNewForm();
    void viewForms();
    void viewAllForms();
    void viewAccounts();
    void allocateForms();
    void considerForms();
    void confirmForms();
    void viewStatistics();

private:
    Ui::MainWindow *ui;

    QPushButton *pbAddForm;
    QPushButton *pbViewForm;
    QPushButton *pbAllForms;
    QPushButton *pbEditAccount;
    QPushButton *pbAllocateForms;
    QPushButton *pbConsiderForms;
    QPushButton *pbConfirmForms;
    QPushButton *pbStatistics;

};

#endif // MAINWINDOW_H
