#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->Le_Attachment->setVisible(false);

    // tasks context menu
    ui->Lw_Tasks->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->Lw_Tasks, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));

    // update total time
    QTimer *TimerForUpdate = new QTimer(this);
    QObject::connect(TimerForUpdate, SIGNAL(timeout()), this, SLOT(UpdateTotalTime()));
    TimerForUpdate->start(1000);

    // update calls
    pTimer = new QTimer;
    connect(this->pTimer,SIGNAL(timeout()),this,SLOT(updateDisplayCalls()));

    // timers
    ui->Lb_TimerTotal->setText("00:00:00");
    QPalette palette = ui->Lb_TimerTotal->palette();
    palette.setColor(ui->Lb_TimerTotal->foregroundRole(), Qt::darkGreen);
    ui->Lb_TimerTotal->setPalette(palette);

    ui->Lb_TimerCalls->setText("00:00:00");

    // Knowledge DB Online
    if(ui->Le_SearchOnline->text() == ""){
         ui->Lw_SearchOnline->setVisible(false);
    }

    ui->Lw_KBOnline->setSpacing(2);
    ui->Lw_KBOnline->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->Lw_SearchOnline->setSpacing(2);
 //   LoadKBOnline();

 //   JsonOpenKBOnline();
    mainObjectCopy = mainObjectKBOnline;

    connect(ui->Lw_KBOnline->model(), SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)), this, SLOT(RefreshKBdbOnline()));
    connect(ui->Lw_KBOnline, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenuKBOnline(QPoint)));

    QNetworkRequest requestKB((QUrl("https://api.npoint.io/66f22abaa99085ff9759")));
    requestKB.setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));
    m_namLoad = new QNetworkAccessManager();
    connect(m_namLoad, &QNetworkAccessManager::finished, this, &MainWindow::LoadKBOnline);
    m_namLoad->get(requestKB);

    // Notes
    notesfile.setFileName("Notes.txt");
    notesfile.open(QFile::ReadOnly | QIODevice::Text);
    QTextStream in(&notesfile);
    QString notes = in.readAll();
    ui->pTE_Notes->setPlainText(notes);
    notesfile.close();
    m_searchHighLight = new SearchHighLight(ui->pTE_Notes->document());
}

MainWindow::~MainWindow()
{
    delete ui;
}

// update total time
void MainWindow::UpdateTotalTime()
{
    int ItemsCount = ui->Lw_Tasks->model()->rowCount();

    for (int index = 0; index < ItemsCount; index++) {

        QListWidgetItem* item = ui->Lw_Tasks->item(index);
        TaskItem *TaskWidget = qobject_cast<TaskItem*>(ui->Lw_Tasks->itemWidget(item)); // dynamic_cast
        if(TaskWidget->m_bTaskPaused == true){
            TaskWidget->m_bTaskPaused = false;
            m_iIndexStarted = -1;
            UpdateTotalTimeinJson();
        }
        if(TaskWidget->m_bTaskReseted == true){
            TaskWidget->m_bTaskReseted = false;
            UpdateTotalTimeinJson();
        }
        if(TaskWidget->m_bTaskStarted == true){
            m_iIndexStarted = TaskWidget->m_iID;
            m_iTotalSpentTime += 1000;
            QTime showTime(0,0,0);
            showTime = showTime.addMSecs(m_iTotalSpentTime);
            timeStr = showTime.toString("hh:mm:ss");
            ui->Lb_TimerTotal->setText(timeStr);      
        } else {
           TaskWidget->GetIndexStarted(m_iIndexStarted);
        }

        QString str_Task = "";

        str_Task = TaskWidget->str_TaskName;
        ui->cB_Tasks->setItemText(index, str_Task);
        QFont fnt;
        if(TaskWidget->m_bTaskStarted == true){
            fnt.setWeight(QFont::Bold);
            ui->cB_Tasks->setItemData(index, QBrush(Qt::red), Qt::TextColorRole);
        } else {
            fnt.setWeight(QFont::Normal);
            ui->cB_Tasks->setItemData(index, QBrush(Qt::black), Qt::TextColorRole);
        }
        ui->cB_Tasks->setItemData(index, fnt, Qt::FontRole);
    }
  //  UpdateKBOnline();
}

void MainWindow::UpdateTotalTimeinJson()
{ 
    JsonOpenDay();
    int sum = 0;
    QJsonValue jv = mainObjectDay.value("Day");
            if(jv.isArray()){
                QJsonArray ja = jv.toArray();
                for(int i = 0; i < ja.count(); i++){
                    QJsonObject subtree = ja.at(i).toObject();
                    sum += subtree.value("SpentTime").toInt();
                }
            }

    QJsonValue DayValue = mainObjectDay.value("Total");
    if (DayValue.type() == QJsonValue::Array) {
        QJsonArray TotalArray = DayValue.toArray();
        QJsonObject Total = TotalArray[1].toObject();
        Total["TotalTime"] = sum;
        TotalArray.replace(1, Total);
        mainObjectDay["Total"] = TotalArray;
    }
    JsonCloseDay();

    m_iTotalSpentTime = sum;

    QTime showTime(0,0,0);
    showTime = showTime.addMSecs(sum);
    timeStr = showTime.toString("hh:mm:ss");
    ui->Lb_TimerTotal->setText(timeStr);
}

// update total time
void MainWindow::UpdateKBOnline()
{
    // KB online
  //  JsonOpenKBOnline();

    int ItemsCountKBOnline = ui->Lw_KBOnline->model()->rowCount();
    for (int index = 0; index < ItemsCountKBOnline; index++) {

        // get data
        QString record = QString::number(index+1);
        QJsonObject ID = mainObjectKBOnline.value(record).toObject();
        QJsonValue checkCanEdit = ID.value("CanEdit");

        if(checkCanEdit.toString() == "Yes"){
            if(KBOnlineCannotEdit.contains(ui->Lw_KBOnline->item(index)->text(), Qt::CaseInsensitive)){
                KBOnlineCannotEdit.removeAll(ui->Lw_KBOnline->item(index)->text());
            }
            ui->Lw_KBOnline->item(index)->setBackground(Qt::lightGray);
        } else if(checkCanEdit.toString() == "No"){
            if(!(KBOnlineCannotEdit.contains(ui->Lw_KBOnline->item(index)->text(), Qt::CaseInsensitive))){
                KBOnlineCannotEdit.append(ui->Lw_KBOnline->item(index)->text());
            }
            ui->Lw_KBOnline->item(index)->setFlags(ui->Lw_KBOnline->item(index)->flags() & ~Qt::ItemIsSelectable);
            ui->Lw_KBOnline->item(index)->setBackground(Qt::red); 
        }

        ui->Lw_KBOnline->item(index)->setTextAlignment(Qt::AlignBottom);
        QFont fnt = ui->Lw_KBOnline->item(index)->font();
        fnt.setWeight(QFont::Normal);
        fnt.setItalic(false);
        fnt.setPointSize(9);
        ui->Lw_KBOnline->item(index)->setFont(fnt);

        // Heading
        QString checkforHeading = ui->Lw_KBOnline->item(index)->text();
        bool yes = checkforHeading.contains("//**");
        if(yes){
            if(checkCanEdit.toString() == "Yes"){
                ui->Lw_KBOnline->item(index)->setBackground(Qt::darkGreen);
            } else if(checkCanEdit.toString() == "No"){
                ui->Lw_KBOnline->item(index)->setBackground(Qt::red);
            }
            ui->Lw_KBOnline->item(index)->setForeground(Qt::white);
            QFont fnt = ui->Lw_KBOnline->item(index)->font();
            fnt.setWeight(QFont::Bold);
            fnt.setItalic(true);
            fnt.setPointSize(14);
            ui->Lw_KBOnline->item(index)->setFont(fnt);
        }
    }

    // update listwidget content
    if(mainObjectCopy != mainObjectKBOnline){
        mainObjectCopy = mainObjectKBOnline;
     //   LoadKBOnline();
        ui->Lw_KBOnline->setCurrentRow(m_iKBOnlineCurrentItem);
    }
}

// update calls
void MainWindow::updateDisplayCalls()
{
    m_iTotalCalls += 1000;
    QTime showTime(0,0,0);
    showTime = showTime.addMSecs(m_iTotalCalls);
    timeStr = showTime.toString("hh:mm:ss");
    ui->Lb_TimerCalls->setText(timeStr);
}

// *** WORKING DAY ***
// open json
void MainWindow::JsonOpenDay()
{
    dayfile.setFileName(str_DayDBPath);
    dayfile.open(QIODevice::ReadOnly | QIODevice::Text);
    QJsonParseError JsonParseError;
    JsonDocumentDay = QJsonDocument::fromJson(dayfile.readAll(), &JsonParseError);
    dayfile.close();
    mainObjectDay = JsonDocumentDay.object();
}

void MainWindow::JsonOpenDayOld()
{
    dayfile.setFileName(str_DayDBPathOld);
    dayfile.open(QIODevice::ReadOnly | QIODevice::Text);
    QJsonParseError JsonParseError;
    JsonDocumentDay = QJsonDocument::fromJson(dayfile.readAll(), &JsonParseError);
    dayfile.close();
    mainObjectDay = JsonDocumentDay.object();
}

// open json
void MainWindow::JsonCloseDay()
{
    JsonDocumentDay.setObject(mainObjectDay);
    dayfile.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
    dayfile.write(JsonDocumentDay.toJson());
    dayfile.close();
}

// add working day
void MainWindow::on_Btn_AddWorkingDay_clicked()
{
    pTimer->stop();
    QDateTime date = QDateTime::currentDateTime();
    str_DateName = date.toString("dd.MM.yyyy");

    str_DayDBPath = QFileDialog::getSaveFileName(this, "Add working day", "Days/" +str_DateName+ ".json", tr("Day file (*.json)"));
    QFile dayfile(str_DayDBPath);

    if(!str_DayDBPath.isEmpty() && !str_DayDBPath.isNull()){
        UnexpectedSaving();
        QFileInfo fileInfo(dayfile.fileName());
        QString str_DateNameforLineEdit = fileInfo.fileName();
        ui->Le_DateName->setText(str_DateNameforLineEdit);
        m_iTotalSpentTime = 0;
        m_iTotalCalls = 0;
        ui->Lb_TimerTotal->setText("00:00:00");
        ui->Lb_TimerCalls->setText("00:00:00");
        str_DayDBPathOld = str_DayDBPath;
    } else {
        return;
    }

    JsonOpenDay();
    mainObjectDay = QJsonObject();
    QJsonObject Day;
    mainObjectDay.insert("Day", Day);

    QJsonObject TotalName;
    TotalName.insert("Name", str_DateName);

    QJsonObject TotalTime;
    TotalTime.insert("TotalTime", 0);

    QJsonObject TotalCalls;
    TotalCalls.insert("TotalCalls", 0);

    QJsonArray TotalArray;
    TotalArray.append(TotalName);
    TotalArray.append(TotalTime);
    TotalArray.append(TotalCalls);

    mainObjectDay["Total"] = TotalArray;

    JsonCloseDay();
}

// load working day
void MainWindow::on_Btn_LoadWorkingDay_clicked()
{

    m_bLoadDayClicked = true;
    str_DayDBPath = QFileDialog::getOpenFileName(this, tr("Load working day"), "Days/.json", tr("Day file (*.json)"));
    QFile dayfile(str_DayDBPath);
    QFileInfo fileInfo(dayfile.fileName());
    QString str_DateNameforLineEdit = fileInfo.fileName();

    if(!str_DayDBPath.isEmpty() && !str_DayDBPath.isNull()){
        UnexpectedSaving();
        ui->Le_DateName->setText(str_DateNameforLineEdit);
        ui->Lb_TimerCalls->setStyleSheet("color: rgb(0, 0, 0);background-color: rgb(255, 255, 255);");
        str_DayDBPathOld = str_DayDBPath;
    } else {
        m_bLoadDayClicked = false;
        return;
    }

    LoadFromDayTable();
    LoadFromTotal();

    m_bLoadDayClicked = false;
}

void MainWindow::LoadFromDayTable()
{
    JsonOpenDay();
    QJsonArray ja;
    QJsonValue jv = mainObjectDay.value("Day");
            if(jv.isArray()){
                ja = jv.toArray();
            }

    int count = ja.count();
    for (int i=0; i<count; i++) {
        on_Btn_AddTask_clicked();
    }

    int ItemsCount = ui->Lw_Tasks->model()->rowCount();
    for (int index = 0; index < ItemsCount; index++) {
        QListWidgetItem* item = ui->Lw_Tasks->item(index);
        TaskItem *TaskWidget = qobject_cast<TaskItem*>(ui->Lw_Tasks->itemWidget(item));
        TaskWidget->Load();
    }
}

void MainWindow::LoadFromTotal()
{
    JsonOpenDay();
    QJsonValue jv = mainObjectDay.value("Total");
                QJsonArray ja = jv.toArray();
                  QJsonObject subtree = ja.at(1).toObject();

                   QJsonValue SpentTimeValue = subtree.value("TotalTime");
                    m_iTotalSpentTime = SpentTimeValue.toInt();

                    QTime timeTotal = timeTotal.addMSecs(m_iTotalSpentTime);
                    QString str_timeTotal = timeTotal.toString("hh:mm:ss");
                    ui->Lb_TimerTotal->setText(str_timeTotal);

    LoadFromTotalCalls();
}

void MainWindow::LoadFromTotalCalls()
{
    JsonOpenDay();
    QJsonValue jv = mainObjectDay.value("Total");
                QJsonArray ja = jv.toArray();
                  QJsonObject subtree = ja.at(2).toObject();

                   QJsonValue SpentCallsValue = subtree.value("TotalCalls");
                    m_iTotalCalls = SpentCallsValue.toInt();

                    QTime timeCalls = timeCalls.addMSecs(m_iTotalCalls);
                    QString str_timeCalls = timeCalls.toString("hh:mm:ss");
                    ui->Lb_TimerCalls->setText(str_timeCalls);
}

// unexpected saving
void MainWindow::UnexpectedSaving()
{
    int ItemsCount = ui->Lw_Tasks->model()->rowCount();
    for (int index = 0; index < ItemsCount; index++) {
        QListWidgetItem* item = ui->Lw_Tasks->item(index);
        TaskItem *TaskWidget = qobject_cast<TaskItem*>(ui->Lw_Tasks->itemWidget(item)); // or dynamic_cast
        if(TaskWidget->m_bTaskStarted == true){
            m_iIndexStarted = -1;
            TaskWidget->m_bTaskStarted = false;
        }
    }

    if(ui->Le_DateName->text() != ""){
        Save();
        UpdateTotalTimeinJson();
    }

    ui->Le_DateName->setEnabled(true);
    ui->Btn_AddTask->setEnabled(true);
    ui->Btn_Start->setEnabled(true);
    ui->Btn_Reset->setEnabled(true);
    ui->Btn_Pause->setEnabled(false);
    m_iTaskItemId = 0;
    ui->Lw_Tasks->clear();
    ui->cB_Tasks->clear();
    pTimer->stop();
}

// add task
void MainWindow::on_Btn_AddTask_clicked()
{
    m_iTaskItemId++;
    QListWidgetItem *LwTaskItem = new QListWidgetItem(ui->Lw_Tasks);
    ui->Lw_Tasks->addItem(LwTaskItem);

    TaskItem *TaskWidget = new TaskItem;
    TaskWidget->SetTaskID(m_iTaskItemId, str_DayDBPath);

    LwTaskItem->setSizeHint(ui->Lw_Tasks->sizeHint());
    ui->Lw_Tasks->setItemWidget (LwTaskItem, TaskWidget);

    ui->cB_Tasks->addItem("");
    ui->Lw_Tasks->setCurrentRow(m_iTaskItemId-1);

    if(m_bLoadDayClicked == false){
        Save();
    }
}

// save
void MainWindow::Save()
{
    if(ui->Le_DateName->text() != ""){
        JsonOpenDayOld();
    } else {
        JsonOpenDay();
    }

    mainObjectDay.remove("Day");
    QJsonObject Day;
    mainObjectDay.insert("Day", Day);
    JsonCloseDay();

    int ItemsCount = ui->Lw_Tasks->model()->rowCount();
    for (int index = 0; index < ItemsCount; index++) {
        QListWidgetItem* item = ui->Lw_Tasks->item(index);
        TaskItem *TaskWidget = qobject_cast<TaskItem*>(ui->Lw_Tasks->itemWidget(item)); // or dynamic_cast
        TaskWidget->Save();
    }
}

// context menu
void MainWindow::showContextMenu(const QPoint &pos)
{
    // *** TASKS
    // Handle global position
    QPoint globalPos = ui->Lw_Tasks->mapToGlobal(pos);

    // Create menu and insert some actions
    QMenu myMenu;
    myMenu.addAction("Reset timer",  this, SLOT(ResetTimer()));
    myMenu.addAction("Delete",  this, SLOT(DeleteItem()));

    // Show context menu at handling position
    myMenu.exec(globalPos);
}

// reset task timer
void MainWindow::ResetTimer()
{
    if(ui->Lw_Tasks->currentItem()){
        int current = ui->Lw_Tasks->currentRow();
        QListWidgetItem* item = ui->Lw_Tasks->item(current);
        TaskItem *TaskWidget = qobject_cast<TaskItem*>(ui->Lw_Tasks->itemWidget(item)); // or dynamic_cast
        TaskWidget->ResetTimer();
    }
}

// delete task
void MainWindow::DeleteItem()
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "Deletion",
                                                                tr("Delete it?"),
                                                                QMessageBox::No | QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        return;
    } else {
        if(ui->Lw_Tasks->currentItem()){
            int GetCurrentIndex = ui->Lw_Tasks->currentRow();
            m_iTaskItemId--;
            int ItemsCount = ui->Lw_Tasks->model()->rowCount();
            for (int index = 1; index < ItemsCount; index++) {
                QListWidgetItem* item = ui->Lw_Tasks->item(index);
                TaskItem *TaskWidget = qobject_cast<TaskItem*>(ui->Lw_Tasks->itemWidget(item)); // or dynamic_cast
                TaskWidget->m_iOldID = ui->Lw_Tasks->currentRow();
                TaskWidget->Delete();
            }
            delete ui->Lw_Tasks->currentItem();
            ui->cB_Tasks->removeItem(GetCurrentIndex);
        }
        Save();
        UpdateTotalTimeinJson();
    }
}

// tasks combobox
void MainWindow::on_Lw_Tasks_currentRowChanged(int currentRow)
{
    ui->cB_Tasks->setCurrentIndex(currentRow);
}

void MainWindow::on_cB_Tasks_currentIndexChanged(int index)
{
    ui->Lw_Tasks->setCurrentRow(index);
}
// *** WORKING DAY END ***


// *** CALLS TIMER ***
// calls timer start button
void MainWindow::on_Btn_Start_clicked()
{
    pTimer->start(1000);
    ui->Lb_TimerCalls->setStyleSheet("color: rgb(255, 0, 0);background-color: rgb(255, 255, 255);");
    ui->Btn_Start->setEnabled(false);
    ui->Btn_Pause->setEnabled(true);
}

// calls timer pause button
void MainWindow::on_Btn_Pause_clicked()
{
    pTimer->stop();

    ui->Lb_TimerCalls->setStyleSheet("color: rgb(0, 0, 0);background-color: rgb(255, 255, 255);");
    ui->Btn_Start->setEnabled(true);
    ui->Btn_Pause->setEnabled(false);

    JsonOpenDay();
    QJsonValue DayValue = mainObjectDay.value("Total");
    if (DayValue.type() == QJsonValue::Array) {
        QJsonArray CallsArray = DayValue.toArray();
        QJsonObject Calls = CallsArray[2].toObject();
        Calls["TotalCalls"] = m_iTotalCalls;
        CallsArray.replace(2, Calls);
        mainObjectDay["Total"] = CallsArray;
    }
    JsonCloseDay();

    m_iDeltaCalls = m_iDeltaOldCalls;
    m_iDeltaCalls = m_iTotalCalls - m_iDeltaCalls;
    m_iDeltaOldCalls += m_iDeltaCalls;
}

// calls timer reset button
void MainWindow::on_Btn_Reset_clicked()
{
    ui->Btn_Start->setEnabled(true);
    ui->Btn_Pause->setEnabled(false);
    ui->Lb_TimerCalls->setStyleSheet("color: rgb(0, 0, 0);background-color: rgb(255, 255, 255);");
    pTimer->stop();
    ui->Lb_TimerCalls->setText("00:00:00");
    m_iTotalCalls = 0;

    JsonOpenDay();
    QJsonValue DayValue = mainObjectDay.value("Total");
    if (DayValue.type() == QJsonValue::Array) {
        QJsonArray CallsArray = DayValue.toArray();
        QJsonObject Calls = CallsArray[2].toObject();
        Calls["TotalCalls"] = 0;
        CallsArray.replace(2, Calls);
        mainObjectDay["Total"] = CallsArray;
    }
    JsonCloseDay();

}
// *** CALLS TIMER END ***


// *** MESSAGE COMPILER ***
// copy text
void MainWindow::on_Btn_CopyText_clicked()
{
    QString str_Attach = "";
    QString str_Greeting = ui->LE_Greeting->text();
    QString str_LetterBody= ui->pTE_LetterBody->toPlainText();
    QString str_Signature = ui->pTE_Sign->toPlainText();
    if(ui->cB_Attachment->isChecked()){
        str_Attach = "Attachments:" + ui->Le_Attachment->text();
    }

    QClipboard* clipboard = QApplication::clipboard();
    if(ui->cB_Attachment->isChecked()){
        clipboard->setText(str_Greeting + "\n\n" +
                           str_LetterBody + "\n\n" +
                           str_Signature + "\n" +
                           str_Attach);
    } else {
        clipboard->setText(str_Greeting + "\n\n" +
                           str_LetterBody + "\n\n" +
                           str_Signature);
    }

}

// renew fields
void MainWindow::on_Btn_Renew_clicked()
{
    QString str_Greeting;
    QString str_Signature;

    QFile jsonKBOnline("Settings.json");
    jsonKBOnline.open(QIODevice::ReadOnly | QIODevice::Text);
    QJsonParseError JsonParseError;
    QJsonDocument JsonDocument = QJsonDocument::fromJson(jsonKBOnline.readAll(), &JsonParseError);
    jsonKBOnline.close();
    QJsonObject mainObject = JsonDocument.object();
    QJsonValue jv = mainObject.value("MessageCompiler");
                QJsonArray ja = jv.toArray();
                  QJsonObject Greeting = ja.at(0).toObject();
                   QJsonValue GreetingValue = Greeting.value("Greeting");
                    str_Greeting = GreetingValue.toString();

                  QJsonObject Signature = ja.at(1).toObject();
                   QJsonValue SignatureValue = Signature.value("Signature");
                    str_Signature = SignatureValue.toString();

    ui->LE_Greeting->setText(str_Greeting);
    ui->pTE_Sign->setPlainText(str_Signature);
    ui->Le_Attachment->setText("");
    ui->pTE_LetterBody->clear();
}

// link sample
void MainWindow::on_Btn_Link_clicked()
{
    QTextCursor cursor(ui->pTE_LetterBody->textCursor());
    cursor.movePosition(QTextCursor::EndOfWord, QTextCursor::MoveAnchor, 1);
    cursor.insertText(" []()");
}

// attachment
void MainWindow::on_cB_Attachment_toggled(bool checked)
{
    if(!checked){
        ui->Le_Attachment->setVisible(false);
        //ui->Le_Attachment->setText("");
    } else{
        ui->Le_Attachment->setVisible(true);
    }
    Preview();
}

// preview
void MainWindow::Preview()
{
    QString str_Attach = "";
    QString str_Greeting = ui->LE_Greeting->text();
    QString str_LetterBody= ui->pTE_LetterBody->toPlainText();
    QString str_Signature = ui->pTE_Sign->toPlainText();
    if(ui->cB_Attachment->isChecked()){
        str_Attach = ui->Le_Attachment->text();
    }

    if(ui->cB_Attachment->isChecked()){
        ui->pTE_PreviewLetter->setPlainText(str_Greeting + "\n\n" +
                           str_LetterBody + "\n\n" +
                           str_Signature + "\n" +
                           "Attachments:" + str_Attach);
    } else {
        ui->pTE_PreviewLetter->setPlainText(str_Greeting + "\n\n" +
                           str_LetterBody + "\n\n" +
                           str_Signature);
    }
}

void MainWindow::on_pTE_LetterBody_textChanged()
{
    Preview();
}

void MainWindow::on_LE_Greeting_textChanged(/*const QString &arg1*/)
{
    Preview();
}

void MainWindow::on_pTE_Sign_textChanged()
{
    Preview();
}

void MainWindow::on_Le_Attachment_textChanged(/*const QString &arg1*/)
{
    Preview();
}

// *** MESSAGE COMPILER END ***


// *** KNOWLEDGE BASE ONLINE ***
// open json
void MainWindow::JsonOpenKBOnline(QNetworkReply *reply)
{
    /*    kbonlinefile.setFileName("KnowledgeBaseOnline.json");
    kbonlinefile.open(QIODevice::ReadOnly | QIODevice::Text);
    QJsonParseError JsonParseError;
    JsonDocumentKBOnline = QJsonDocument::fromJson(kbonlinefile.readAll(), &JsonParseError);
    kbonlinefile.close();
    mainObjectKBOnline = JsonDocumentKBOnline.object();
    */
    JsonDocumentKBOnline = QJsonDocument::fromJson(reply->readAll());
    mainObjectKBOnline = JsonDocumentKBOnline.object();
}

// open json
void MainWindow::JsonCloseKBOnline()
{
    JsonDocumentKBOnline.setObject(mainObjectKBOnline);
    kbonlinefile.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
    kbonlinefile.write(JsonDocumentKBOnline.toJson());
    kbonlinefile.close();
}


// load KB
void MainWindow::LoadKBOnline(QNetworkReply *reply)
{
    // save current item
    m_iKBOnlineCurrentItem = ui->Lw_KBOnline->currentRow();

    m_bLoadKBOnline = true;
    ui->Lw_KBOnline->clear();
    m_iKBOnlineRowTotal = 0;


    JsonOpenKBOnline(reply);

    QStringList allrecords = mainObjectKBOnline.keys();
    int count = allrecords.count();
    for (int i=1; i<count+1; i++) {

        // get data
        QString record = QString::number(i);
        QJsonObject ID = mainObjectKBOnline.value(record).toObject();
        QJsonValue addSample = ID.value("Sample");

        m_iKBOnlineRowTotal++;
        QString str_m_iKBOnlineRowTotal = QString::number(m_iKBOnlineRowTotal);
        ui->Lw_KBOnline->addItem(addSample.toString());

        ui->Lw_KBOnline->item(m_iKBOnlineRowTotal-1)->setBackground(Qt::lightGray);
        ui->Lw_KBOnline->item(m_iKBOnlineRowTotal-1)->setTextAlignment(Qt::AlignBottom);

        // Heading
        QString checkforHeading = ui->Lw_KBOnline->item(m_iKBOnlineRowTotal-1)->text();
        bool yes = checkforHeading.contains("//**");
        if(yes){
            ui->Lw_KBOnline->item(m_iKBOnlineRowTotal-1)->setBackground(Qt::darkGreen);
            ui->Lw_KBOnline->item(m_iKBOnlineRowTotal-1)->setForeground(Qt::white);
            QFont fnt = ui->Lw_KBOnline->item(m_iKBOnlineRowTotal-1)->font();
            fnt.setWeight(QFont::Bold);
            fnt.setItalic(true);
            fnt.setPointSize(14);
            ui->Lw_KBOnline->item(m_iKBOnlineRowTotal-1)->setFont(fnt);
        }
    }
    m_bLoadKBOnline = false;
}

// add sample
void MainWindow::AddSampleKBOnline()
{
    int currentRow = ui->Lw_KBOnline->currentRow();
    m_iKBOnlineRowTotal++;
    QString str_m_iKBOnlineRowTotal = QString::number(m_iKBOnlineRowTotal);
    ui->Lw_KBOnline->addItem("");

    ui->Lw_KBOnline->item(m_iKBOnlineRowTotal-1)->setBackground(Qt::lightGray);
    ui->Lw_KBOnline->item(m_iKBOnlineRowTotal-1)->setTextAlignment(Qt::AlignBottom);
    QListWidgetItem* itemNew = ui->Lw_KBOnline->item( m_iKBOnlineRowTotal-1 );
    ui->Lw_KBOnline->takeItem(m_iKBOnlineRowTotal-1);
    ui->Lw_KBOnline->insertItem( currentRow+1, itemNew );
    ui->Lw_KBOnline->setCurrentRow(currentRow+1);

    RefreshKBdbOnline();
}

// delete sample
void MainWindow::DelSampleKBOnline()
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "Deletion",
                                                                tr("Delete it?\n"),
                                                                QMessageBox::No | QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        return;
    } else {
        if(ui->Lw_KBOnline->currentItem()){
            m_iKBOnlineRowTotal--;
            int totalRows = ui->Lw_KBOnline->count();
            QString newtotalRow = QString::number(totalRows-1);
            int currentRow = ui->Lw_KBOnline->currentIndex().row();
            QString curRow = QString::number(currentRow+1);
            delete ui->Lw_KBOnline->currentItem();
            RefreshKBdbOnline();
        }
    }
}

// delete selected
void MainWindow::DelSelectedKBOnline()
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "Deletion",
                                                                tr("Delete samples?\n"),
                                                                QMessageBox::No | QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        return;
    } else {
        QList<QListWidgetItem*> items = ui->Lw_KBOnline->selectedItems();
        foreach(QListWidgetItem * item, items)
        {
            delete ui->Lw_KBOnline->takeItem(ui->Lw_KBOnline->row(item));
        }
        RefreshKBdbOnline();
    }
}

// reorder kb db
void MainWindow::RefreshKBdbOnline()
{
 //   JsonOpenKBOnline();
    mainObjectKBOnline = QJsonObject();
    //create an object for inner object of main object
    QJsonObject ID;

    int ItemsCount = ui->Lw_KBOnline->model()->rowCount();
    m_iKBOnlineRowTotal = ItemsCount;
    for (int index = 0; index < ItemsCount; index++) {
        ID.insert("Sample", ui->Lw_KBOnline->item(index)->text());
        if(!(KBOnlineCannotEdit.contains(ui->Lw_KBOnline->item(index)->text(), Qt::CaseInsensitive))){
            ID.insert("CanEdit", "Yes");
        } else if(KBOnlineCannotEdit.contains(ui->Lw_KBOnline->item(index)->text(), Qt::CaseInsensitive)){
            ID.insert("CanEdit", "No");
        }

        //insert the inner json object inside main object
        mainObjectKBOnline.insert(QString::number(index+1), ID);
    }

    JsonCloseKBOnline();
}

// kb context menu
void MainWindow::showContextMenuKBOnline(const QPoint &pos)
{
    // *** KB   
  //  JsonOpenKBOnline();

    // get data
    QString record = QString::number(ui->Lw_KBOnline->currentRow()+1);
    QJsonObject ID = mainObjectKBOnline.value(record).toObject();
    QJsonValue checkCanEdit = ID.value("CanEdit");

    // Handle global position
    QPoint globalPosKBOnline = ui->Lw_KBOnline->mapToGlobal(pos);

    // Create menu and insert some actions
    QMenu myMenuKBOnline;
    myMenuKBOnline.addAction("Add sample",  this, SLOT(AddSampleKBOnline()));
    if(checkCanEdit.toString() == "Yes"){
        myMenuKBOnline.addAction("Edit sample",  this, SLOT(EditKBSample()));
    }
    if(ui->Lw_KBOnline->selectedItems().count() > 1){
        myMenuKBOnline.addAction("Delete selected",  this, SLOT(DelSelectedKBOnline()));
    } else if(ui->Lw_KBOnline->selectedItems().count() == 1){
        if(checkCanEdit.toString() == "Yes"){
            myMenuKBOnline.addAction("Delete sample",  this, SLOT(DelSampleKBOnline()));
        }
    }
    myMenuKBOnline.addAction("Edit KB Json file manually",  this, SLOT(EditKBJsonFile()));

    // Show context menu at handling position
    myMenuKBOnline.exec(globalPosKBOnline);
}

// send to letter body
void MainWindow::on_Lw_KBOnline_itemDoubleClicked(/*QListWidgetItem *item*/)
{
    if(ui->pTE_LetterBody->toPlainText() != ""){
        QTextCursor cursor(ui->pTE_LetterBody->textCursor());
        cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor, 1);
        cursor.insertText(" "+ui->Lw_KBOnline->currentItem()->text());
    } else {
        ui->pTE_LetterBody->setPlainText(ui->Lw_KBOnline->currentItem()->text());
    }
}

void MainWindow::on_Lw_SearchOnline_itemDoubleClicked(/*QListWidgetItem *item*/)
{
    if(ui->pTE_LetterBody->toPlainText() != ""){
        QTextCursor cursor(ui->pTE_LetterBody->textCursor());
        cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor, 1);
        cursor.insertText(" "+ui->Lw_SearchOnline->currentItem()->text());
    } else {
        ui->pTE_LetterBody->setPlainText(ui->Lw_SearchOnline->currentItem()->text());
    }
}

// search
void MainWindow::on_Le_SearchOnline_textChanged(const QString &arg1)
{
    if(ui->Le_SearchOnline->text() == ""){
         ui->Lw_SearchOnline->setVisible(false);
    } else {
        ui->Lw_SearchOnline->setVisible(true);
    }

    ui->Lw_SearchOnline->clear();
    nowList_KBOnline.clear();
    if(arg1 != ""){
        QList<QListWidgetItem *> listKBOnline = ui->Lw_KBOnline->findItems(arg1, Qt::MatchContains);
      //  QList<QListWidgetItem *> listKB = ui->Lw_KB->findItems(arg1, Qt::MatchFixedString);
        for(int i=0; i<listKBOnline.count(); i++){
            nowList_KBOnline << listKBOnline[i]->text();
        }
        QRegExp regExp(arg1, Qt::CaseInsensitive, QRegExp::Wildcard);

        ui->Lw_SearchOnline->addItems(nowList_KBOnline.filter(regExp));
    } else {
        ui->Lw_SearchOnline->clear();
    }
}

void MainWindow::on_Lw_SearchOnline_currentTextChanged(const QString &currentText)
{
    for(int i = 0; i < ui->Lw_KBOnline->count(); i++)
    {
        if(ui->Lw_KBOnline->item(i)->text() == currentText){
            ui->Lw_KBOnline->setCurrentRow(i);
        }
    }
}

// edit sample
void MainWindow::on_Btn_EditSampleOnline_clicked()
{
    EditKBSample();
}

// add sample
void MainWindow::on_Btn_AddSampleOnline_clicked()
{
    AddSampleKBOnline();
}

// edit KB Json file manually
void MainWindow::EditKBJsonFile()
{
    KBJsonFileEdit kbjsonfileedit;
    kbjsonfileedit.exec();
}

// save/add sample
void MainWindow::on_Btn_SaveSample_clicked()
{
    if(ui->pTE_LetterBody->toPlainText() != ""){
        bool found = false;
        for (int i = 0; i < ui->Lw_KBOnline->count(); ++i) {
            if (ui->Lw_KBOnline->item(i)->text() == ui->pTE_LetterBody->toPlainText()) {
                found = true;
                break;
            }
        }

        if(found){
            QMessageBox::warning(this, "Warning!","This sample already exists!");
        } else {
            if(ui->tabKB->currentIndex() == 0){ // online
                int currentRow = ui->Lw_KBOnline->currentRow();
                m_iKBOnlineRowTotal++;
                QString str_m_iKBOnlineRowTotal = QString::number(m_iKBOnlineRowTotal);
                ui->Lw_KBOnline->addItem(ui->pTE_LetterBody->toPlainText());

                ui->Lw_KBOnline->item(m_iKBOnlineRowTotal-1)->setBackground(Qt::lightGray);
                ui->Lw_KBOnline->item(m_iKBOnlineRowTotal-1)->setTextAlignment(Qt::AlignBottom);
                QListWidgetItem* itemNew = ui->Lw_KBOnline->item( m_iKBOnlineRowTotal-1 );
                ui->Lw_KBOnline->takeItem(m_iKBOnlineRowTotal-1);
                ui->Lw_KBOnline->insertItem( currentRow+1, itemNew );

                RefreshKBdbOnline();

            }
        }
    }
}

// edit sample
void MainWindow::EditKBSample()
{
 //   JsonOpenKBOnline();

    // get data
    QString record = QString::number(ui->Lw_KBOnline->currentRow()+1);
    QJsonObject ID = mainObjectKBOnline.value(record).toObject();
    QJsonValue checkCanEdit = ID.value("CanEdit");
    if(ui->tabKB->currentIndex() == 0 && checkCanEdit.toString() == "No"){
        return;
    }

    KBEditSample kdeditsample;
    if(ui->tabKB->currentIndex() == 0){ // online

        QJsonValue IDValue = mainObjectKBOnline.value(QString::number(ui->Lw_KBOnline->currentRow()+1));
        if (IDValue.type() == QJsonValue::Object) {
            QJsonObject IDObject = IDValue.toObject();
            IDObject["CanEdit"] = "No";
            mainObjectKBOnline[QString::number(ui->Lw_KBOnline->currentRow()+1)] = IDObject;
        }

        JsonCloseKBOnline();
        kdeditsample.GetTextandID(ui->Lw_KBOnline->currentItem()->text(), ui->Lw_KBOnline->currentRow(), 0);

    }

    kdeditsample.exec();

    if(ui->tabKB->currentIndex() == 0){ // online

        QString record = QString::number(ui->Lw_KBOnline->currentRow()+1);
        QJsonObject ID = mainObjectKBOnline.value(record).toObject();
        QJsonValue changeSample = ID.value("Sample");

        ui->Lw_KBOnline->currentItem()->setText(changeSample.toString());
        ui->Lw_KBOnline->currentItem()->setBackground(Qt::lightGray);
        ui->Lw_KBOnline->currentItem()->setForeground(Qt::black);
        QFont fnt = ui->Lw_KBOnline->currentItem()->font();
        fnt.setWeight(QFont::Normal);
        fnt.setItalic(false);
        fnt.setPointSize(9);
        ui->Lw_KBOnline->currentItem()->setFont(fnt);

        // Heading
        QString checkforHeading = ui->Lw_KBOnline->currentItem()->text();
        bool yes = checkforHeading.contains("//**");
        if(yes){
            ui->Lw_KBOnline->currentItem()->setBackground(Qt::darkGreen);
            ui->Lw_KBOnline->currentItem()->setForeground(Qt::white);
            QFont fnt = ui->Lw_KBOnline->currentItem()->font();
            fnt.setWeight(QFont::Bold);
            fnt.setItalic(true);
            fnt.setPointSize(14);
            ui->Lw_KBOnline->currentItem()->setFont(fnt);
        }
    }
}
// *** KNOWLEDGE BASE ONLINE END ***

// *** NOTES ***
// save changes
void MainWindow::on_pTE_Notes_textChanged()
{
    notesfile.open(QFile::WriteOnly | QIODevice::Text);
    QTextStream out(&notesfile);
    out<<ui->pTE_Notes->document()->toPlainText();
    notesfile.close();
}

// search
void MainWindow::on_Le_Search_textChanged(const QString &arg1)
{
    m_searchHighLight->searchText(arg1);
}

// *** NOTES END ***

// settings
void MainWindow::on_Btn_Settings_clicked()
{
    Settings setting;
    setting.exec();
}

// close app
void MainWindow::closeEvent (QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "Exit",
                                                                tr("Are you sure?"),
                                                                QMessageBox::No | QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        Save();
        UpdateTotalTimeinJson();
        on_Btn_Pause_clicked();


        /*
        // online
        KBEditSample kdeditsample; //m_iIDsample
        if(kdeditsample.m_bEditSampleInProcess){
            int IDinProcess = kdeditsample.m_iIDsample;
            QFile jsonKBOnline("CSKnowledgeBase.json");
            jsonKBOnline.open(QIODevice::ReadOnly | QIODevice::Text);
            QJsonParseError JsonParseError;
            QJsonDocument JsonDocument = QJsonDocument::fromJson(jsonKBOnline.readAll(), &JsonParseError);
            jsonKBOnline.close();
            QJsonObject mainObject = JsonDocument.object();

            QString record = QString::number(IDinProcess+1);
            QJsonObject ID = mainObject.value(record).toObject();
            QJsonValue checkCanEdit = ID.value("CanEdit");

            if(checkCanEdit.toString() == "No"){
                QJsonValue IDValue = mainObject.value(QString::number(IDinProcess+1));
                if (IDValue.type() == QJsonValue::Object) {
                    QJsonObject IDObject = IDValue.toObject();
                    IDObject["CanEdit"] = "Yes";
                    mainObject[QString::number(IDinProcess+1)] = IDObject;
                }

                JsonDocument.setObject(mainObject); // set to json document
                jsonKBOnline.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
                jsonKBOnline.write(JsonDocument.toJson());
                jsonKBOnline.close();
            }
        }
        */

        event->accept();
    }
}
