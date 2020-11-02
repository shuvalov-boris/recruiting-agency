#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QFileDialog>
#include <QImage>
#include <QPainter>
#include <QPixmap>
#include <QPicture>
#include <QDate>
#include <QMessageBox>
#include <QByteArray>
#include <QBuffer>
#include <QDebug>
#include <QLayout>
#include <QHBoxLayout>

//enum Sex
//{
//    MaleSex,
//    FemaleSex,
//    UnknownSex
//};

struct FormData
{
    QByteArray photo;
    QString last_name;
    QString first_name;
    QString patronymic;
    QString sex;
    QDate birthday;
    QString address;
    QString activity;
    int experience;
    QString phone;
    QString email;
};

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

private:
    QFile *picfile;

public:
    explicit Form(QWidget *parent = 0);
    ~Form();

    QHBoxLayout *setAdditionalLayout();

private slots:
    void on_pbLoadPhoto_clicked();
    void on_pbAdd_clicked();

    void viewFormData(const FormData& form_data);

signals:
    void addFormData(const FormData&);

private:
    Ui::Form *ui;
};

#endif // FORM_H
