#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

#include <QWidget>
#include <QDialog>

namespace Ui {
class Authorization;
}

class Authorization : public QDialog
{
    Q_OBJECT

public:
    explicit Authorization(QWidget *parent = 0);
    ~Authorization();

private:
    Ui::Authorization *ui;

signals:
    void authorize(const QString&, const QString&);

private slots:
    void on_pbSignIn_clicked();

public slots:
    void incorrectAuthorizationData();
};

#endif // AUTHORIZATION_H
