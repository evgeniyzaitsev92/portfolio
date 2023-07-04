#ifndef TASKITEM_H
#define TASKITEM_H

//#include "mainwindow.h"
//#include "ui_mainwindow.h"

#include <QWidget>
#include <QTimer>
#include <QTime>
#include <QInputDialog>
#include <QPushButton>

#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

namespace Ui {
class TaskItem;
}

class TaskItem : public QWidget
{
    Q_OBJECT

public:
   // explicit TaskItem(QWidget *parent = nullptr);
    explicit TaskItem(QWidget *parent = {});
    ~TaskItem();

    Ui::TaskItem *ui;

    QTimer *timer = new QTimer(this);
    QTime *time;
    QTimer *pTimer;
    QString timeStr;

    void SetTaskID(int ID, QString dbpath);
    void Delete();
    void Save();
    void ResetTimer();
    void Load();
    void GetIndexStarted(int ID);

    bool m_bTaskStarted = false;
    bool m_bCanBeStarted = true;
    bool m_bTaskPaused = false;
    bool m_bTaskReseted = false;
    bool m_bTaskDeleted = false;
    bool m_bTypeRequest = true;
    bool m_bTypeOther = false;
    bool m_bLoaded = false;

    int m_iID = 0;
    int m_iOldID = 0;
    int m_iSpentTime = 0;
    int m_iSpentTimeEnd = 0;
    int m_iSpentTimeDelta = 0;
    int m_iDelta = 0;
    int m_iDeltaOld = 0;
    int m_iIndexStarted = -1;

    QString str_dbpath;
    QString str_ID;
    QString str_SpentTime;
    QString str_TaskName;
    QString str_Type;
    QString str_Comment;

    QFile dayfile;
    QJsonDocument JsonDocument;
    QJsonObject mainObject;

private slots:
    void updateDisplay();
    void on_Btn_Start_clicked();
    void on_Btn_Pause_clicked();
    void on_Cb_Type_currentTextChanged(const QString &arg1);
    void on_Le_TaskName_textChanged(const QString &arg1);
    void on_pTE_Comment_textChanged();

    void JsonOpen();
    void JsonClose();

private:

signals:
    void UpdateTotalChild(int spentTime);

};

#endif // TASKITEM_H
