#ifndef EDITPART3_TEACHER_H
#define EDITPART3_TEACHER_H

#include <QWidget>
#include <QtSql>
#include "QtSql/QSqlDatabase"
#include "QSqlQuery"

namespace Ui {
class EditPart3_Teacher;
}

class EditPart3_Teacher : public QWidget
{
    Q_OBJECT

public:
    explicit EditPart3_Teacher(QWidget *parent = nullptr);
    ~EditPart3_Teacher();

public:
    int m_iOldID;
    QString str_Quest;
    QString str_Ans;

    void SetQuestID(int ID);
    void Delete();
    void ReorderID(int index);
    void Save(QString testpath);
    void Load(QString testpath);
    void ReceiveRightAns(QString rightans);

private:
    QSqlDatabase db_Test;

private:
    Ui::EditPart3_Teacher *ui;
};

#endif // EDITPART3_TEACHER_H
