#ifndef RESULTS_H
#define RESULTS_H

#include <QDialog>
#include <QDebug>
#include <QSql>
#include <QtSql/QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPdfWriter>
#include <QtPrintSupport/QPrinter>
#include <QDate>
#include <QTime>
#include <qpainter.h>
#include <qtextdocument.h>

namespace Ui {
class Results;
}

class Results : public QDialog
{
    Q_OBJECT

public:
    explicit Results(QWidget *parent = nullptr);
    ~Results();

private:
    Ui::Results *ui;

public:
    QString str_TestPathInEdit;
    void GetTestPath(QString testpath);
    void Load();
    QSqlDatabase db_Test;

private slots:
    void on_lineEdit_Name_textChanged();
    void on_Button_PDF_clicked();
    void on_Button_Close_clicked();
};

#endif // RESULTS_H
