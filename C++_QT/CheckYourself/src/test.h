#ifndef TEST_H
#define TEST_H

#include <QDialog>

#include <QDebug>
#include <QSql>
#include <QtSql/QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QTimer>
#include <QTime>

namespace Ui {
class Test;
}

class Test : public QDialog
{
    Q_OBJECT

public:
    explicit Test(QWidget *parent = nullptr);
    ~Test();

private:
    Ui::Test *ui;

public:
    QString str_TestPathInEdit;
    void GetTestPath(QString testpath);

    QSqlDatabase db_Test;
    bool m_bHasPart1 = false;
    bool m_bHasPart2 = false;
    bool m_bHasPart3 = false;
    bool m_bPart3Started = false;

    // part 1
    int m_iPart1Quest = 0;
    int m_iQuestNum = 0;
    int m_iAnsRight = 0;
    int m_iAnsRightChoose = 0;
    int m_iAnsRightCount = 0;
    int m_iQuantIndex = 0;

    QString str_Quest;
    QString str_Ans1;
    QString str_Ans2;
    QString str_Ans3;
    QString str_Ans4;
    QString str_QuestRight;
    QString str_AnsChoose;

    // part 2 & 3
    int m_iPart2Quest = 0;
    int m_iQuantIndex2 = 0;
    int m_iAnsRightCount2 = 0;
    int m_iAnsRightCount3 = 0;

    QString str_AnsRight2_1 = 0;
    QString str_AnsRight2_2 = 0;

    void Load();
    void Load1();
    void Load2();
    void Load3();

    void FinishNotComplete();

    int count = 0;
    int count2 = 0;
    int count3 = 0;

    int countR = 0;
    int count2R = 0;
    int count3R = 0;

    QTimer *timer = new QTimer(this);
    QTime *time;

    QString str_Part2Name = "";
    QString str_Part3Name = "";

        QString str_Task = "";
        QString str_Task2 = "";
        QString str_Task3 = "";

private slots:
    void on_Button_Add_clicked();
    void on_radioButton_Ans1_clicked();
    void on_radioButton_Ans2_clicked();
    void on_radioButton_Ans3_clicked();
    void on_radioButton_Ans4_clicked();
    void on_tabWidget_currentChanged();
    void on_Button_Exit_clicked();
    void on_listWidget_Quest_itemClicked();

    void updateTime();

private:


};

#endif // TEST_H
