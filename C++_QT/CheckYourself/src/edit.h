#ifndef EDIT_H
#define EDIT_H

#include <QDialog>
#include <QListWidgetItem>
#include <QDragEnterEvent>
#include <QMessageBox>

#include "editpart1.h"
#include "editpart2.h"
#include "editpart3_teacher.h"
#include "editpart3_pupil.h"

namespace Ui {
class Edit;
}

class Edit : public QDialog
{
    Q_OBJECT

public:
    explicit Edit(QWidget *parent = nullptr);
    ~Edit();

private:
    Ui::Edit *ui;
    QSqlDatabase db_Test;
 //   void closeEvent(QCloseEvent *event);

public:
    QString str_TestPathInEdit;
    QString str_CopyRightAns;

    void GetTestPath(QString testpath);
    void Load();
    void Load1();
    void Load2();
    void Load3();

    void Save1();
    void Save2();
    void Save3();

    QString btnStyle = "QPushButton {"
                       "width: 75px; height: 38px; font-family: MS Shell Dlg 2; font-style: normal; font-size: 12pt; font-weight: bold;"
                       "border-radius: 20px;"
                       "border: 3px solid rgb(0, 135, 0); "
                       "background-color: qlineargradient(spread:pad, x1:1, y1:0, x2:1, y2:1, stop:0 rgba(0, 180, 0, 255), stop:1 rgba(152, 255, 152, 255));"
                       "color: rgb(25, 25, 25);}"

                       "QPushButton:hover {"
                       "border-radius: 20px;"
                       "border: 3px solid rgb(0, 135, 0); 	"
                       "background-color: qlineargradient(spread:pad, x1:1, y1:0, x2:1, y2:1, stop:0 rgba(36, 232, 36, 255), stop:1 rgba(236, 250, 236, 255));"
                       "color: rgb(25, 25, 25);}"

                       "QPushButton:pressed {"
                       "border-radius: 20px;"
                       "border: 3px solid rgb(0, 135, 0); "
                       "background-color: qlineargradient(spread:pad, x1:1, y1:0, x2:1, y2:1, stop:0 rgba(236, 250, 236, 255), stop:1 rgba(36, 232, 36, 255));"
                       "color: rgb(25, 25, 25);}";
    QString boxStyle = "QMessageBox { font-family: MS Shell Dlg 2; font-style: normal; font-size: 16pt; font-color: transparent;"
                       "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(36, 113, 190, 255), stop:1 rgba(243, 243, 243, 255));"
                       "}"
                       "QMessageBox QLabel {background-color: transparent; }";

    // part 1
    int m_iQuestId = 0;
    int m_iGetOldID = 0;
    int m_iGetNewID = 0;

    // part 2
    int m_iQuest2Id = 0;
    int m_iGetOld2ID = 0;
    int m_iGetNew2ID = 0;

    // part 3
    int m_iQuest3IdT = 0;
    int m_iQuest3IdP = 0;

    QString str_Password = "";

private slots:
    void on_Button_Exit_clicked();
    void SaveAll();

    // part 1
    void on_Button_Add_clicked();
    void on_Button_Delete_clicked();
    void Reorder();
 //   void on_Button_Save_clicked();
 //   bool GetChanges(bool changes);

    // part 2
    void on_Button_Add_2_clicked();
    void on_Button_Delete_2_clicked();
    void Reorder2();
 //   void on_Button_Save_2_clicked();

        // part 3
    void on_Button_Add_3_clicked();
    void on_Button_Add_4_clicked();
    void on_Button_Delete_3_clicked();
    void on_listWidget_3Teacher_currentItemChanged();
    void on_listWidget_3Pupil_currentItemChanged();
    void Reorder3T();
    void Reorder3P();
    void on_Button_Fix_clicked();

    // password
      // create
    void on_Button_PassCreate_clicked();    
    void on_checkBox_PassCreate_clicked(bool checked);
    void on_lineEdit_PassCreate_textChanged(const QString &arg1);
    void on_Button_Cancel_PassCreate_clicked();
    void on_Button_OK_PassCreate_clicked();
    void on_checkBox_PassCurrent_clicked(bool checked);

    // change
    void on_Button_PassChange_clicked();
    void on_checkBox_PassChange_Old_clicked(bool checked);
    void on_checkBox_PassChange_New_clicked(bool checked);
    void on_lineEdit_PassChange_Old_textChanged(const QString &arg1);
    void on_lineEdit_PassChange_New_textChanged(const QString &arg1);
    void on_Button_Cancel_PassChange_clicked();
    void on_Button_OK_PassChange_clicked();

    // delete
    void on_Button_PassDel_clicked();
    void on_checkBox_PassDel_clicked(bool checked);
    void on_lineEdit_PassDel_textChanged(const QString &arg1);
    void on_Button_Cancel_PassDel_clicked();
    void on_Button_OK_PassDel_clicked();

public slots:
 //   void SaveBtnActivate();

protected:
  //  void dragEnterEvent(QDragEnterEvent *event) override;
  //  void dropEvent(QDropEvent *event) override;

signals:


};

#endif // EDIT_H
