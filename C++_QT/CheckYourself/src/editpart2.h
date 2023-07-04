#ifndef EDITPART2_H
#define EDITPART2_H

#include <QWidget>
#include <QtSql>
#include "QtSql/QSqlDatabase"
#include "QSqlQuery"

namespace Ui {
class EditPart2;
}

class EditPart2 : public QWidget
{
    Q_OBJECT

public:
    explicit EditPart2(QWidget *parent = nullptr);
    ~EditPart2();

private:
    Ui::EditPart2 *ui;

private:
    QSqlDatabase db_Test;

public:
    //int m_iAnsRight = 0;
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

private slots:
    void on_comboBox_currentIndexChanged(int index);
};

#endif // EDITPART2_H
