#include "form.h"
#include "ui_form.h"

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
}

Form::~Form()
{
    delete ui;
}

QHBoxLayout* Form::setAdditionalLayout()
{
//    ui->wAdditional->layout()->addWidget(addWidget);
//    ui->wAdditional->setLayout(addLayout);
//    return ui->wAdditional;
    return ui->hblAdditional;
}

void Form::on_pbLoadPhoto_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
                0, "Открыть", "", "*.png *.jpg *.bmp");
//    QImage image(filename);
//    QPainter painter;
//    painter.begin(ui->canva);
//    painter.drawImage(0, 0, image);
//    painter.end();
    QPixmap pic(filename);
    QSize PicSize(ui->limage->size());
    pic = pic.scaled(PicSize,Qt::KeepAspectRatio);

    ui->limage->setPixmap(pic);
//    ui->limage->setScaledContents(true);
    picfile = new QFile(filename);
}

void Form::on_pbAdd_clicked()
{
    FormData form_data;
    form_data.last_name = ui->leLastName->text();
    form_data.first_name = ui->leFirstName->text();
    form_data.patronymic = ui->lePatronymic->text();
    form_data.sex = (ui->rbMale->isChecked() ? "Male" :
                        (ui->rbFemale->isChecked() ? "Female" : "Unknown"));
    form_data.birthday = ui->deBirthday->date();
    form_data.address = ui->pteAddress->toPlainText();
    form_data.activity = ui->pteActivity->toPlainText();
    form_data.experience = ui->sbExperience->value();
    form_data.phone = ui->lePhone->text();
    form_data.email = ui->leEmail->text();

    qDebug() << "on_pbAdd: birthday is " << form_data.birthday.toString();

    if (ui->limage->pixmap())
    {
        QBuffer buffer(&form_data.photo);
        buffer.open(QIODevice::WriteOnly);
        ui->limage->pixmap()->save(&buffer, "PNG");
//        if (!picfile->open(QIODevice::ReadOnly))
//            qDebug() << "on_Add: opening is fail";
//        form_data.photo = picfile->readAll();
//        qDebug() << "on_Add: photo length is " << form_data.photo.length();
//        delete picfile;
    }

    emit addFormData(form_data);
}

void Form::viewFormData(const FormData& form_data)
{
    qDebug() << "viewFormData: get form :)";
    ui->leLastName->setText(form_data.last_name);
    ui->leFirstName->setText(form_data.first_name);
    ui->lePatronymic->setText(form_data.patronymic);
    if (form_data.sex == "Male") ui->rbMale->setChecked(true);
    else if (form_data.sex == "Female") ui->rbFemale->setChecked(true);
    ui->deBirthday->setDate(form_data.birthday);
    ui->pteAddress->setPlainText(form_data.address);
    ui->pteActivity->setPlainText(form_data.activity);
    ui->sbExperience->setValue(form_data.experience);
    ui->lePhone->setText(form_data.phone);
    ui->leEmail->setText(form_data.email);
    QPixmap pixmap = QPixmap();
    pixmap.loadFromData(form_data.photo);
    ui->limage->setPixmap(pixmap);
    qDebug() << "\t\t\tBirthday is " << form_data.birthday.toString();
}
