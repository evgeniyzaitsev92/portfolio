#include "taskitem.h"
#include "ui_taskitem.h"

//#include "mainwindow.h"

TaskItem::TaskItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TaskItem)
{
    ui->setupUi(this);
    ui->line->setVisible(false);

    // timer
    ui->Lb_Timer->setText("00:00:00");

    // update timer
    pTimer = new QTimer;
    connect(pTimer,SIGNAL(timeout()),this,SLOT(updateDisplay()));
    ui->Lb_Progress->setVisible(false);

    // task type pic
    if(ui->Cb_Type->currentText() == "Request"){
        QPixmap pixmapReq("://Request.ico");
        ui->Lb_Pic->setPixmap(pixmapReq);
    } else if(ui->Cb_Type->currentText() == "Other"){
        QPixmap pixmapOther("://Other.ico");
        ui->Lb_Pic->setPixmap(pixmapOther);
    }
}

TaskItem::~TaskItem()
{
    delete ui;
}

// set ID when creating a task
void TaskItem::SetTaskID(int ID, QString dbpath){
    str_ID = QString::number(ID);
    ui->Lb_TaskName->setText(str_ID);
    m_iID = ID;
    str_dbpath = dbpath;
}

// get index of started task
void TaskItem::GetIndexStarted(int ID){
    m_iIndexStarted = ID;
}

// update timer
void TaskItem::updateDisplay()
{
    m_iSpentTime += 1000;
    QTime showTime(0,0,0);
    showTime = showTime.addMSecs(m_iSpentTime);
    timeStr = showTime.toString("hh:mm:ss");
    ui->Lb_Timer->setText(timeStr);
}

// *** TASK TIMER ***
// task timer start button
void TaskItem::on_Btn_Start_clicked()
{
    if(m_iIndexStarted == -1){
        m_bTaskStarted = true;
        pTimer->start(1000);

        ui->Lb_Timer->setStyleSheet("color: rgb(255, 0, 0);background-color: rgb(255, 255, 255);");
        ui->Btn_Start->setEnabled(false);
        ui->Btn_Pause->setEnabled(true);
        ui->Lb_Progress->setVisible(true);

    }
}

// task timer pause button
void TaskItem::on_Btn_Pause_clicked()
{
    pTimer->stop();
    m_bTaskPaused = true;
    m_bTaskStarted = false;

    this->ui->Lb_Timer->setStyleSheet("color: rgb(0, 0, 0);background-color: rgb(255, 255, 255);");
    ui->Btn_Start->setEnabled(true);
    ui->Btn_Pause->setEnabled(false);
    ui->Lb_Progress->setVisible(false);

    str_SpentTime = QString::number(m_iSpentTime);

    JsonOpen();
    QJsonValue DayValue = mainObject.value("Day");
    if (DayValue.type() == QJsonValue::Array) {
        QJsonArray TaskArray = DayValue.toArray();
        QJsonObject Task = TaskArray[m_iID-1].toObject();
        Task["SpentTime"] = m_iSpentTime;
        TaskArray.replace(m_iID-1, Task);
        mainObject["Day"] = TaskArray;
    }
    JsonClose();

    m_iDelta = m_iDeltaOld;
    m_iDelta = m_iSpentTime - m_iDelta;
    m_iDeltaOld += m_iDelta;
}

// task timer reset button
void TaskItem::ResetTimer()
{
    m_bTaskReseted = true;
    //pTimer->stop();
    ui->Lb_Timer->setText("00:00:00");
    m_iSpentTime = 0;

    JsonOpen();
    QJsonValue DayValue = mainObject.value("Day");
    if (DayValue.type() == QJsonValue::Array) {
        QJsonArray TaskArray = DayValue.toArray();
        QJsonObject Task = TaskArray[m_iID-1].toObject();
        Task["SpentTime"] = 0;
        TaskArray.replace(m_iID-1, Task);
        mainObject["Day"] = TaskArray;
    }
    JsonClose();
}
// *** TASK TIMER END ***

// task name
void TaskItem::on_Le_TaskName_textChanged(const QString &arg1)
{
    str_TaskName = arg1;
    JsonOpen();
    QJsonValue DayValue = mainObject.value("Day");
    if (DayValue.type() == QJsonValue::Array) {
        QJsonArray TaskArray = DayValue.toArray();
        QJsonObject Task = TaskArray[m_iID-1].toObject();
        Task["Name"] = arg1;
        TaskArray.replace(m_iID-1, Task);
        mainObject["Day"] = TaskArray;
    }
    JsonClose();
}

// comment
void TaskItem::on_pTE_Comment_textChanged()
{
    JsonOpen();
    QJsonValue DayValue = mainObject.value("Day");
    if (DayValue.type() == QJsonValue::Array) {
        QJsonArray TaskArray = DayValue.toArray();
        QJsonObject Task = TaskArray[m_iID-1].toObject();
        Task["Comment"] = ui->pTE_Comment->toPlainText();
        TaskArray.replace(m_iID-1, Task);
        mainObject["Day"] = TaskArray;
    }
    JsonClose();
}

// save
void TaskItem::Save(){
    JsonOpen();
    QJsonObject Day;
        Day["Name"] = ui->Le_TaskName->text();
        Day["SpentTime"] = m_iSpentTime;
        Day["Type"] = ui->Cb_Type->currentText();
        Day["Comment"] = ui->pTE_Comment->toPlainText();
        QJsonArray DayArray = mainObject["Day"].toArray();
        DayArray.append(Day);
    mainObject["Day"] = DayArray;
    JsonClose();
}

// delete
void TaskItem::Delete(){
    QString str_ID = ui->Lb_TaskName->text();
    int ID = str_ID.toInt();
    if(ID > m_iOldID){
        ID--;
        str_ID = QString::number(ID);
        ui->Lb_TaskName->setText(str_ID);
    }
}

// task type
void TaskItem::on_Cb_Type_currentTextChanged(const QString &arg1)
{
    JsonOpen();
    QJsonValue DayValue = mainObject.value("Day");
    if (DayValue.type() == QJsonValue::Array) {
        QJsonArray TaskArray = DayValue.toArray();
        QJsonObject Task = TaskArray[m_iID-1].toObject();
        Task["Type"] = arg1;
        TaskArray.replace(m_iID-1, Task);
        mainObject["Day"] = TaskArray;
    }
    JsonClose();

    if(arg1 == "Request"){
        QPixmap pixmapReq("://Request.ico");
        ui->Lb_Pic->setPixmap(pixmapReq);
    } else if(arg1 == "Other"){
        QPixmap pixmapOther("://Other.ico");
        ui->Lb_Pic->setPixmap(pixmapOther);
    }
}

// load task
void TaskItem::Load()
{
    JsonOpen();
    QJsonValue jv = mainObject.value("Day");
                QJsonArray ja = jv.toArray();
                  QJsonObject subtree = ja.at(m_iID-1).toObject();

                   QJsonValue TaskNameValue = subtree.value("Name");
                    str_TaskName = TaskNameValue.toString();

                   QJsonValue SpentTimeValue = subtree.value("SpentTime");
                    m_iSpentTime = SpentTimeValue.toInt();

                   QJsonValue TypeValue = subtree.value("Type");
                    str_Type = TypeValue.toString();

                   QJsonValue CommentValue = subtree.value("Comment");
                    str_Comment = CommentValue.toString();


    ui->Le_TaskName->setText(str_TaskName);
    ui->pTE_Comment->setPlainText(str_Comment);

    QTime time = time.addMSecs(m_iSpentTime);
    QString str_time = time.toString("hh:mm:ss");
    ui->Lb_Timer->setText(str_time);

    if(str_Type == "Request"){
        ui->Cb_Type->setCurrentText("Request");
        QPixmap pixmapReq("://Request.ico");
        ui->Lb_Pic->setPixmap(pixmapReq);
    } else if(str_Type == "Other"){
        ui->Cb_Type->setCurrentText("Other");
        QPixmap pixmapOther("://Other.ico");
        ui->Lb_Pic->setPixmap(pixmapOther);
    }
}

// open json
void TaskItem::JsonOpen()
{
    dayfile.setFileName(str_dbpath);
    dayfile.open(QIODevice::ReadOnly | QIODevice::Text);
    QJsonParseError JsonParseError;
    JsonDocument = QJsonDocument::fromJson(dayfile.readAll(), &JsonParseError);
    dayfile.close();
    mainObject = JsonDocument.object();
}

// open json
void TaskItem::JsonClose()
{
    JsonDocument.setObject(mainObject);
    dayfile.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
    dayfile.write(JsonDocument.toJson());
    dayfile.close();
}

