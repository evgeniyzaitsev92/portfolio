#ifndef EDITPART1_H
#define EDITPART1_H

#include <QWidget>
#include <QtSql>
#include "QtSql/QSqlDatabase"
#include "QSqlQuery"

namespace Ui {
class EditPart1;
}

class EditPart1 : public QWidget
{
    Q_OBJECT

public:
    explicit EditPart1(QWidget *parent = nullptr);
    ~EditPart1();

private:
    Ui::EditPart1 *ui;

private:
    QSqlDatabase db_Test;
    void AnsRight1();
    void AnsRight2();
    void AnsRight3();
    void AnsRight4();

public:
    int m_iAnsRight = 0;
    int m_iQuantIndex = 0;
    int m_iOldID;

    QString str_Quest;
    QString str_Ans1;
    QString str_Ans2;
    QString str_Ans3;
    QString str_Ans4;

    void SetQuestID(int ID);
    void ReorderID(int index);
    void Delete();
    void Save(QString testpath);
    void Load(QString testpath);
    void SetQuantIndex();

private slots:
    void on_radioButton_1_clicked(bool checked);
    void on_radioButton_2_clicked(bool checked);
    void on_radioButton_3_clicked(bool checked);
    void on_radioButton_4_clicked(bool checked);
    void on_comboBox_currentIndexChanged(int index);
};

#endif // EDITPART1_H

