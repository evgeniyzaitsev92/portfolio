#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QListWidgetItem>
#include <QFileSystemModel>
#include <QDir>
#include <QDebug>
#include <QSql>
#include <QtSql/QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include "edit.h"
#include "test.h"
#include "password.h"
#include "results.h"
#include "license.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    Ui::Widget *ui;

private slots:

    void on_Button_Add_Test_clicked();
    void on_Button_Exit_clicked();
    void on_Button_Del_clicked();
    void on_Button_Edit_clicked();
    void on_Button_Rename_clicked();
    void on_Button_Cancel_clicked();
    void on_Button_Select_clicked();
    void on_Button_Remove_clicked();
    void on_lineEdit_textChanged(/*const QString &arg1*/);
    void on_Button_OK_clicked();
    void on_Button_Start_clicked();
    void on_Button_Results_clicked();

    void on_Button_License_clicked();

public slots:
   // void ItemRenameTest(const QString);

signals:
 //   void RenameTest(const QString &newItemName);

public:
     QString str_TestPath;
     void NoTestMSG();
 //   void GetTestName();
  //  QString str_NewTestName;

private:
    QFileSystemModel* model;
    QSqlDatabase db_Test;
    QString inputStyle = "QInputDialog { font-family: MS Shell Dlg 2; font-style: normal; font-size: 16pt; font-color: transparent;"
                       "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(36, 113, 190, 255), stop:1 rgba(243, 243, 243, 255));"
                       "}"
                       "QInputDialog QLabel {background-color: transparent; }";

    bool Password = false;
    bool PasswordEntered = false;
    QString str_Password = "";

};
#endif // WIDGET_H
