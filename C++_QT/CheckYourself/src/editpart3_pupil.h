#ifndef EDITPART3_PUPIL_H
#define EDITPART3_PUPIL_H

#include <QWidget>
#include <QtSql>
#include "QtSql/QSqlDatabase"
#include "QSqlQuery"

namespace Ui {
class EditPart3_Pupil;
}

class EditPart3_Pupil : public QWidget
{
    Q_OBJECT

public:
    explicit EditPart3_Pupil(QWidget *parent = nullptr);
    ~EditPart3_Pupil();

public:
    int m_iOldID;
    QString str_Wrong;

    void SetQuestID(int ID);
    void Delete();
    void ReorderID(int index);
    void Save(QString testpath);
    void GetRightAns(QString &rightans);
    void Load(QString testpath);

private:
    QSqlDatabase db_Test;

private:
    Ui::EditPart3_Pupil *ui;
};

#endif // EDITPART3_PUPIL_H
