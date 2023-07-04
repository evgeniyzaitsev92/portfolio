#include "edit.h"
#include "ui_edit.h"
#include <QListWidget>

Edit::Edit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Edit)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() | Qt::WindowMinMaxButtonsHint);
    setAcceptDrops(true);
    connect(ui->listWidget->model(), SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)), this, SLOT(Reorder()));
    connect(ui->listWidget_2->model(), SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)), this, SLOT(Reorder2()));
    connect(ui->listWidget_3Teacher->model(), SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)), this, SLOT(Reorder3T()));
    connect(ui->listWidget_3Pupil->model(), SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)), this, SLOT(Reorder3P()));
    connect(ui->Button_Save, SIGNAL(clicked(bool)), this, SLOT(SaveAll()));
    connect(ui->Button_Save_2, SIGNAL(clicked(bool)), this, SLOT(SaveAll()));
    connect(ui->Button_Save_3, SIGNAL(clicked(bool)), this, SLOT(SaveAll()));

    // password
      // current
 //   ui->checkBox_PassCurrent->setVisible(false);
 //   ui->label_PassCurrent->setVisible(false);

      // create
    ui->label_PassCreate->setVisible(false);
    ui->checkBox_PassCreate->setVisible(false);
    ui->lineEdit_PassCreate->setVisible(false);
    ui->Button_OK_PassCreate->setVisible(false);
    ui->Button_Cancel_PassCreate->setVisible(false);

      // change
    ui->label_PassChange_Old->setVisible(false);
    ui->label_PassChange_New->setVisible(false);
    ui->checkBox_PassChange_Old->setVisible(false);
    ui->checkBox_PassChange_New->setVisible(false);
    ui->lineEdit_PassChange_Old->setVisible(false);
    ui->lineEdit_PassChange_New->setVisible(false);
    ui->Button_OK_PassChange->setVisible(false);
    ui->Button_Cancel_PassChange->setVisible(false);

      // delete
    ui->label_PassDel->setVisible(false);
    ui->checkBox_PassDel->setVisible(false);
    ui->lineEdit_PassDel->setVisible(false);
    ui->Button_OK_PassDel->setVisible(false);
    ui->Button_Cancel_PassDel->setVisible(false);

}

Edit::~Edit()
{
    delete ui;
}

/*
void Edit::closeEvent(QCloseEvent *event){
    on_Button_Save_clicked();
    event->accept();
}
*/

//***************//
// GET TEST PATH //
//***************//
void Edit::GetTestPath(QString testpath){
    str_TestPathInEdit = testpath;
    Load();

    // check password
    db_Test = QSqlDatabase::addDatabase("QSQLITE");
    db_Test.setDatabaseName(str_TestPathInEdit);
    db_Test.open();
    QSqlQuery query;
    query.prepare("SELECT * FROM code;");
    query.exec();
    if (query.seek(0)){
        str_Password = query.value(1).toString(); // get password
    }

    ui->lineEdit_PassCurrent->setText(str_Password);

    if(str_Password == ""){
        ui->Button_PassCreate->setEnabled(true);
        ui->Button_PassChange->setEnabled(false);
        ui->Button_PassDel->setEnabled(false);
    } else {
        ui->Button_PassCreate->setEnabled(false);
        ui->Button_PassChange->setEnabled(true);
        ui->Button_PassDel->setEnabled(true);
    }

    // test name
    query.prepare("SELECT * FROM testname;");
    query.exec();
    if (query.seek(0)){
        ui->lineEdit_TestName->setText(query.value(1).toString());
    }


}

//********//
// PART 1 //
//********//

// add question
void Edit::on_Button_Add_clicked()
{

    m_iQuestId++;
    QListWidgetItem *QuestItem = new QListWidgetItem(ui->listWidget);
    ui->listWidget->addItem(QuestItem);

    EditPart1 *QuestWidget = new EditPart1;
    QuestWidget->SetQuestID(m_iQuestId);

    QuestItem->setSizeHint(QSize(ui->listWidget->width()-25, QuestWidget->height()));
    ui->listWidget->setItemWidget (QuestItem, QuestWidget);

/*
    db_Test = QSqlDatabase::addDatabase("QSQLITE");
    db_Test.setDatabaseName(str_TestPathInEdit);
    db_Test.open();
    if (!db_Test.open()) {
          qDebug() << db_Test.lastError().text();
    }
    QSqlQuery query;
    QString str_ID = "1";
 //   int Index = str_ID.toInt();
    query.prepare("SELECT * FROM part1;");
 //   query.bindValue(":Index", Index);
 //   query.prepare("SELECT * FROM 'part1';");
    query.exec();
//int idName = query.record().indexOf("Quest");
if (query.seek(1))
    {
    ui->label111->setText(query.value(1).toString());}
    db_Test.close();
*/

}

// delete question
void Edit::on_Button_Delete_clicked()
{
    if(ui->listWidget->currentItem()){
        m_iQuestId--;
        int ItemsCount = ui->listWidget->model()->rowCount();
        for (int index = 1; index < ItemsCount; index++) {
            QListWidgetItem* item = ui->listWidget->item(index);
            EditPart1 *QuestWidget = qobject_cast<EditPart1*>(ui->listWidget->itemWidget(item)); // or dynamic_cast
            QuestWidget->m_iOldID = ui->listWidget->currentRow();
            QuestWidget->Delete();
        }

        delete ui->listWidget->currentItem();
    }
}

// reorder
void Edit::Reorder()
{
        int ItemsCount = ui->listWidget->model()->rowCount();
        for (int index = 0; index < ItemsCount; index++) {
            QListWidgetItem* item = ui->listWidget->item(index);
            EditPart1 *QuestWidget = qobject_cast<EditPart1*>(ui->listWidget->itemWidget(item)); // or dynamic_cast
            QuestWidget->ReorderID(index+1);
        }
}
/*
// save
void Edit::on_Button_Save_clicked()
{
    db_Test = QSqlDatabase::addDatabase("QSQLITE");
    db_Test.setDatabaseName(str_TestPathInEdit);
    db_Test.open();
    if (!db_Test.open()) {
          qDebug() << db_Test.lastError().text();
    }
    QSqlQuery query;

    query.exec("DELETE FROM 'part1';");
    query.exec("DELETE FROM sqlite_sequence WHERE name = 'part1';");
    int ItemsCount = ui->listWidget->model()->rowCount();
    for (int index = 0; index < ItemsCount; index++) {
        QListWidgetItem* item = ui->listWidget->item(index);
        EditPart1 *QuestWidget = qobject_cast<EditPart1*>(ui->listWidget->itemWidget(item)); // or dynamic_cast
        QuestWidget->Save(str_TestPathInEdit);
    }

    db_Test.close();
}
*/
/*
// activate save button
void Edit::SaveBtnActivate()
{
        ui->Button_Save->setEnabled(true);
}

bool Edit::GetChanges(bool changes){
    QListWidgetItem* item = ui->listWidget->currentItem();
    EditPart1 *QuestWidget = qobject_cast<EditPart1*>(ui->listWidget->itemWidget(item)); // or dynamic_cast
    changes = QuestWidget->m_bChanges;
    return changes;
}
*/


//********//
// PART 2 //
//********//

// add question
void Edit::on_Button_Add_2_clicked()
{
    m_iQuest2Id++;
    QListWidgetItem *QuestItem2 = new QListWidgetItem(ui->listWidget_2);
    ui->listWidget_2->addItem(QuestItem2);

    EditPart2 *QuestWidget2 = new EditPart2;
    QuestWidget2->SetQuestID(m_iQuest2Id);

    QuestItem2->setSizeHint(QSize(ui->listWidget_2->width()-25, QuestWidget2->height()));
    ui->listWidget_2->setItemWidget(QuestItem2, QuestWidget2);
}

// delete question
void Edit::on_Button_Delete_2_clicked()
{
    if(ui->listWidget_2->currentItem()){
        m_iQuest2Id--;
        int ItemsCount = ui->listWidget_2->model()->rowCount();
        for (int index = 1; index < ItemsCount; index++) {
            QListWidgetItem* item2 = ui->listWidget_2->item(index);
            EditPart2 *QuestWidget2 = qobject_cast<EditPart2*>(ui->listWidget_2->itemWidget(item2)); // or dynamic_cast
            QuestWidget2->m_iOldID = ui->listWidget_2->currentRow();
            QuestWidget2->Delete();
        }

        delete ui->listWidget_2->currentItem();
    }

}

// reorder
void Edit::Reorder2()
{
        int ItemsCount = ui->listWidget_2->model()->rowCount();
        for (int index = 0; index < ItemsCount; index++) {
            QListWidgetItem* item2 = ui->listWidget_2->item(index);
            EditPart2 *QuestWidget2 = qobject_cast<EditPart2*>(ui->listWidget_2->itemWidget(item2)); // or dynamic_cast
            QuestWidget2->ReorderID(index+1);
        }
}

//********//
// PART 3 //
//********//

// add question
void Edit::on_Button_Add_3_clicked()
{
    m_iQuest3IdT++;
    QListWidgetItem *QuestItem3T = new QListWidgetItem(ui->listWidget_3Teacher);
    ui->listWidget_3Teacher->addItem(QuestItem3T);
    EditPart3_Teacher *QuestWidget3T = new EditPart3_Teacher;
    QuestWidget3T->SetQuestID(m_iQuest3IdT);
    QuestItem3T->setSizeHint(QSize(ui->listWidget_3Teacher->width()-25, QuestWidget3T->height()));
    ui->listWidget_3Teacher->setItemWidget(QuestItem3T, QuestWidget3T);
}

void Edit::on_Button_Add_4_clicked()
{
    m_iQuest3IdP++;
    QListWidgetItem *QuestItem3P = new QListWidgetItem(ui->listWidget_3Pupil);
    ui->listWidget_3Pupil->addItem(QuestItem3P);
    EditPart3_Pupil *QuestWidget3P = new EditPart3_Pupil;
    QuestWidget3P->SetQuestID(m_iQuest3IdP);
    QuestItem3P->setSizeHint(QSize(ui->listWidget_3Pupil->width()-25, QuestWidget3P->height()));
    ui->listWidget_3Pupil->setItemWidget(QuestItem3P, QuestWidget3P);
}

// delete question
void Edit::on_Button_Delete_3_clicked()
{
    if(ui->listWidget_3Teacher->currentItem()){
        m_iQuest3IdT--;
        int ItemsCount = ui->listWidget_3Teacher->model()->rowCount();
        for (int index = 1; index < ItemsCount; index++) {
            QListWidgetItem* item3T = ui->listWidget_3Teacher->item(index);
            EditPart3_Teacher *QuestWidget3T = qobject_cast<EditPart3_Teacher*>(ui->listWidget_3Teacher->itemWidget(item3T)); // or dynamic_cast
            QuestWidget3T->m_iOldID = ui->listWidget_3Teacher->currentRow();
            QuestWidget3T->Delete();
        }
        delete ui->listWidget_3Teacher->currentItem();
    }

    else if(ui->listWidget_3Pupil->currentItem()){
        m_iQuest3IdP--;
        int ItemsCountP = ui->listWidget_3Pupil->model()->rowCount();
        for (int index = 1; index < ItemsCountP; index++) {
            QListWidgetItem* item3P = ui->listWidget_3Pupil->item(index);
            EditPart3_Pupil *QuestWidget3P = qobject_cast<EditPart3_Pupil*>(ui->listWidget_3Pupil->itemWidget(item3P)); // or dynamic_cast
            QuestWidget3P->m_iOldID = ui->listWidget_3Pupil->currentRow();
            QuestWidget3P->Delete();
        }
        delete ui->listWidget_3Pupil->currentItem();
    }
}

// for teacher
void Edit::on_listWidget_3Teacher_currentItemChanged()
{
    ui->listWidget_3Pupil->selectionModel()->clear();
}
// for pupil
void Edit::on_listWidget_3Pupil_currentItemChanged()
{
    ui->listWidget_3Teacher->selectionModel()->clear();
}

// reorder
void Edit::Reorder3T()
{
        int ItemsCount = ui->listWidget_3Teacher->model()->rowCount();
        for (int index = 0; index < ItemsCount; index++) {
            QListWidgetItem* item3T = ui->listWidget_3Teacher->item(index);
            EditPart3_Teacher *QuestWidget3T = qobject_cast<EditPart3_Teacher*>(ui->listWidget_3Teacher->itemWidget(item3T)); // or dynamic_cast
            QuestWidget3T->ReorderID(index+1);
        }
}

// reorder
void Edit::Reorder3P()
{
        int ItemsCount = ui->listWidget_3Pupil->model()->rowCount();
        for (int index = 0; index < ItemsCount; index++) {
            QListWidgetItem* item3P = ui->listWidget_3Pupil->item(index);
            EditPart3_Pupil *QuestWidget3P = qobject_cast<EditPart3_Pupil*>(ui->listWidget_3Pupil->itemWidget(item3P)); // or dynamic_cast
            QuestWidget3P->ReorderID(index+1);
        }
}

//******//
// SAVE //
//******//
void Edit::Save1()
{
    QSqlQuery query;
    query.exec("DELETE FROM 'part1';");
    query.exec("DELETE FROM sqlite_sequence WHERE name = 'part1';");
    query.exec("DELETE FROM 'part2';");
    query.exec("DELETE FROM sqlite_sequence WHERE name = 'part2';");
    query.exec("DELETE FROM 'part3';");
    query.exec("DELETE FROM sqlite_sequence WHERE name = 'part3';");
    query.exec("DELETE FROM 'part1task';");
    query.exec("DELETE FROM sqlite_sequence WHERE name = 'part1task';");
    query.exec("DELETE FROM 'part2task';");
    query.exec("DELETE FROM sqlite_sequence WHERE name = 'part2task';");
    query.exec("DELETE FROM 'part3task';");
    query.exec("DELETE FROM sqlite_sequence WHERE name = 'part3task';");
    query.exec("DELETE FROM 'testname';");
    query.exec("DELETE FROM sqlite_sequence WHERE name = 'testname';");
    query.exec("DELETE FROM 'email';");
    query.exec("DELETE FROM sqlite_sequence WHERE name = 'email';");

    query.prepare("INSERT INTO testname (Testname)"
                  "VALUES (:Testname);");
    query.bindValue(":Testname", ui->lineEdit_TestName->text());
    query.exec();

    query.prepare("INSERT INTO part1task (Task)"
                  "VALUES (:Task);");
    query.bindValue(":Task", ui->lineEdit_Task->text());
    query.exec();


    int ItemsCount = ui->listWidget->model()->rowCount();
    for (int index = 0; index < ItemsCount; index++) {
        QListWidgetItem* item = ui->listWidget->item(index);
        EditPart1 *QuestWidget = qobject_cast<EditPart1*>(ui->listWidget->itemWidget(item)); // or dynamic_cast
        QuestWidget->Save(str_TestPathInEdit);
    }
}

void Edit::Save2()
{
    db_Test = QSqlDatabase::addDatabase("QSQLITE");
    db_Test.setDatabaseName(str_TestPathInEdit);
    db_Test.open();
    QSqlQuery query;
    query.prepare("INSERT INTO part2task (Task)"
                  "VALUES (:Task);");
    query.bindValue(":Task", ui->lineEdit_Task_2->text());
    query.exec();
    db_Test.close();

    int ItemsCount2 = ui->listWidget_2->model()->rowCount();
    for (int index = 0; index < ItemsCount2; index++) {
        QListWidgetItem* item2 = ui->listWidget_2->item(index);
        EditPart2 *QuestWidget2 = qobject_cast<EditPart2*>(ui->listWidget_2->itemWidget(item2)); // or dynamic_cast
        QuestWidget2->Save(str_TestPathInEdit);
    }
}
void Edit::Save3()
{
    db_Test = QSqlDatabase::addDatabase("QSQLITE");
    db_Test.setDatabaseName(str_TestPathInEdit);
    db_Test.open();
    QSqlQuery query;
    query.prepare("INSERT INTO part3task (Task)"
                  "VALUES (:Task);");
    query.bindValue(":Task", ui->lineEdit_Task_3->text());
    query.exec();
    db_Test.close();

    int ItemsCountT = ui->listWidget_3Teacher->model()->rowCount();
    for (int index = 0; index < ItemsCountT; index++) {
        QListWidgetItem* item3 = ui->listWidget_3Teacher->item(index);
        EditPart3_Teacher *QuestWidget3 = qobject_cast<EditPart3_Teacher*>(ui->listWidget_3Teacher->itemWidget(item3)); // or dynamic_cast
        QuestWidget3->Save(str_TestPathInEdit);
    }

    int ItemsCountP = ui->listWidget_3Pupil->model()->rowCount();
    for (int index = 0; index < ItemsCountP; index++) {
        QListWidgetItem* itemP = ui->listWidget_3Pupil->item(index);
        EditPart3_Pupil *QuestWidgetP = qobject_cast<EditPart3_Pupil*>(ui->listWidget_3Pupil->itemWidget(itemP)); // or dynamic_cast
        QuestWidgetP->Save(str_TestPathInEdit);
    }

}

void Edit::SaveAll()
{
    int ItemsCountT = ui->listWidget_3Teacher->model()->rowCount();
    int ItemsCountP = ui->listWidget_3Pupil->model()->rowCount();
    if(ItemsCountT != ItemsCountP){
        QMessageBox msgBox;
        QPushButton *OKButton = msgBox.addButton(QMessageBox::Ok);

        OKButton->setStyleSheet(btnStyle);
        OKButton->setText("OK");

        msgBox.setWindowTitle("Внимание!");
        msgBox.setText("Количество вопросов и ответов должно совпадать (Matching)!");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStyleSheet(boxStyle);
        msgBox.exec();
    } else {
        db_Test = QSqlDatabase::addDatabase("QSQLITE");
        db_Test.setDatabaseName(str_TestPathInEdit);
        db_Test.open();
        Save1();
        Save2();
        Save3();
        db_Test.close();
    }
}

// fix answers in 1st column
void Edit::on_Button_Fix_clicked()
{
    int ItemsCountT = ui->listWidget_3Teacher->model()->rowCount();
    int ItemsCountP = ui->listWidget_3Pupil->model()->rowCount();

    if(ItemsCountT != ItemsCountP){
        QMessageBox msgBox;
        QPushButton *OKButton = msgBox.addButton(QMessageBox::Ok);

        OKButton->setStyleSheet(btnStyle);
        OKButton->setText("OK");

        msgBox.setWindowTitle("Внимание!");
        msgBox.setText("Количество вопросов и ответов должно совпадать!");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStyleSheet(boxStyle);
        msgBox.exec();
    } else {
        QMessageBox msgBox;
        QPushButton *YesButton = msgBox.addButton(QMessageBox::Yes);
        QPushButton *NoButton = msgBox.addButton(QMessageBox::No);

        YesButton->setStyleSheet(btnStyle);
        YesButton->setText("Да");
        NoButton->setStyleSheet(btnStyle);
        NoButton->setText("Нет");

        msgBox.setWindowTitle("Внимание!");
        msgBox.setText("Скопировать правильные ответы?");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStyleSheet(boxStyle);
        msgBox.exec();

        if (msgBox.clickedButton() == YesButton) {
            int ItemsCount3P = ui->listWidget_3Pupil->model()->rowCount();
            for (int index = 0; index < ItemsCount3P; index++) {
                QListWidgetItem* item3P = ui->listWidget_3Pupil->item(index);
                EditPart3_Pupil *QuestWidget3P = qobject_cast<EditPart3_Pupil*>(ui->listWidget_3Pupil->itemWidget(item3P)); // or dynamic_cast

                QListWidgetItem* item3T = ui->listWidget_3Teacher->item(index);
                EditPart3_Teacher *QuestWidget3T = qobject_cast<EditPart3_Teacher*>(ui->listWidget_3Teacher->itemWidget(item3T)); // or dynamic_cast

                QuestWidget3P->GetRightAns(str_CopyRightAns);
                QuestWidget3T->ReceiveRightAns(str_CopyRightAns);
            }
        }
    }
}

//******//
// LOAD //
//******//
void Edit::Load1()
{
    db_Test = QSqlDatabase::addDatabase("QSQLITE");
    db_Test.setDatabaseName(str_TestPathInEdit);
    db_Test.open();
    QSqlQuery query;

    query.prepare("SELECT * FROM part1task;");
    query.exec();
    QString str_Task1 = "";
    if (query.seek(0)){
        str_Task1 = query.value(1).toString();
    }
    ui->lineEdit_Task->setText(str_Task1);

    query.prepare("SELECT COUNT(*) FROM 'part1';");
    query.exec();
    int count = 0;
   if (query.next()) {
        count = query.value(0).toInt();
    }

    for (int i=0; i<count; i++) {
        on_Button_Add_clicked();
    }
    int ItemsCount = ui->listWidget->model()->rowCount();
    for (int index = 0; index < ItemsCount; index++) {
        QListWidgetItem* item = ui->listWidget->item(index);
        EditPart1 *QuestWidget = qobject_cast<EditPart1*>(ui->listWidget->itemWidget(item)); // or dynamic_cast
        QuestWidget->Load(str_TestPathInEdit);
    }
    db_Test.close();
}

void Edit::Load2()
{
    db_Test = QSqlDatabase::addDatabase("QSQLITE");
    db_Test.setDatabaseName(str_TestPathInEdit);
    db_Test.open();
    QSqlQuery query;

    query.prepare("SELECT * FROM part2task;");
    query.exec();
    QString str_Task2 = "";
    if (query.seek(0)){
        str_Task2 = query.value(1).toString();
    }
    ui->lineEdit_Task_2->setText(str_Task2);

    query.prepare("SELECT COUNT(*) FROM 'part2';");
    query.exec();
    int count = 0;
   if (query.next()) {
        count = query.value(0).toInt();
    }

    for (int i=0; i<count; i++) {
        on_Button_Add_2_clicked();
    }
    int ItemsCount2 = ui->listWidget_2->model()->rowCount();
    for (int index = 0; index < ItemsCount2; index++) {
        QListWidgetItem* item2 = ui->listWidget_2->item(index);
        EditPart2 *QuestWidget2 = qobject_cast<EditPart2*>(ui->listWidget_2->itemWidget(item2)); // or dynamic_cast
        QuestWidget2->Load(str_TestPathInEdit);
    }
    db_Test.close();
}

void Edit::Load3()
{
    db_Test = QSqlDatabase::addDatabase("QSQLITE");
    db_Test.setDatabaseName(str_TestPathInEdit);
    db_Test.open();
    QSqlQuery query;
    query.prepare("SELECT * FROM part3task;");
    query.exec();
    QString str_Task3 = "";
    if (query.seek(0)){
        str_Task3 = query.value(1).toString();
    }
    ui->lineEdit_Task_3->setText(str_Task3);

    query.prepare("SELECT COUNT(*) FROM 'part3';");
    query.exec();
    int count = 0;
   if (query.next()) {
        count = query.value(0).toInt();
    }

    for (int i=0; i<count; i++) {
        on_Button_Add_3_clicked();
        on_Button_Add_4_clicked();
    }
    int ItemsCount3T = ui->listWidget_3Teacher->model()->rowCount();
    for (int index = 0; index < ItemsCount3T; index++) {
        QListWidgetItem* item3T = ui->listWidget_3Teacher->item(index);
        EditPart3_Teacher *QuestWidget3T = qobject_cast<EditPart3_Teacher*>(ui->listWidget_3Teacher->itemWidget(item3T)); // or dynamic_cast
        QuestWidget3T->Load(str_TestPathInEdit);
    }
    int ItemsCount3P = ui->listWidget_3Pupil->model()->rowCount();
    for (int index = 0; index < ItemsCount3P; index++) {
        QListWidgetItem* item3P = ui->listWidget_3Pupil->item(index);
        EditPart3_Pupil *QuestWidget3P = qobject_cast<EditPart3_Pupil*>(ui->listWidget_3Pupil->itemWidget(item3P)); // or dynamic_cast
        QuestWidget3P->Load(str_TestPathInEdit);
    }
    query.exec();

    db_Test.close();
}

void Edit::Load()
{
    Load1();
    Load2();
    Load3();
}


//******//
// EXIT //
//******//
void Edit::on_Button_Exit_clicked()
{
    QMessageBox msgBox;
    QPushButton *YesButton = msgBox.addButton(QMessageBox::Yes);
    QPushButton *NoButton = msgBox.addButton(QMessageBox::No);
    QPushButton *CancelButton = msgBox.addButton(QMessageBox::Cancel);

    YesButton->setStyleSheet(btnStyle);
    YesButton->setText("Да");
    NoButton->setStyleSheet(btnStyle);
    NoButton->setText("Нет");
    CancelButton->setStyleSheet(btnStyle);
    CancelButton->setText("Отмена");

    msgBox.setWindowTitle("Внимание!");
    msgBox.setText("Сохранить тест?");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setStyleSheet(boxStyle);
    msgBox.exec();

    if (msgBox.clickedButton() == YesButton) {
        int ItemsCountT = ui->listWidget_3Teacher->model()->rowCount();
        int ItemsCountP = ui->listWidget_3Pupil->model()->rowCount();
        if(ItemsCountT != ItemsCountP){
            QMessageBox msgBox;
            QPushButton *OKButton = msgBox.addButton(QMessageBox::Ok);

            OKButton->setStyleSheet(btnStyle);
            OKButton->setText("OK");

            msgBox.setWindowTitle("Внимание!");
            msgBox.setText("Количество вопросов и ответов должно совпадать (Matching)!");
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setStyleSheet(boxStyle);
            msgBox.exec();
        } else {
        SaveAll();
        this->close();
        }
    } else if (msgBox.clickedButton() == NoButton) {
        this->close();
    }
}

//**********//
// PASSWORD //
//**********//
// current
void Edit::on_checkBox_PassCurrent_clicked(bool checked)
{
    if(checked){
        ui->lineEdit_PassCurrent->setEchoMode(QLineEdit::Normal);
    } else {
        ui->lineEdit_PassCurrent->setEchoMode(QLineEdit::Password);
    }
}

// create
void Edit::on_Button_PassCreate_clicked()
{
    ui->label_PassCreate->setVisible(true);
    ui->checkBox_PassCreate->setVisible(true);
    ui->lineEdit_PassCreate->setVisible(true);
    ui->Button_OK_PassCreate->setVisible(true);
    ui->Button_Cancel_PassCreate->setVisible(true);
}

void Edit::on_checkBox_PassCreate_clicked(bool checked)
{
    if(checked){
        ui->lineEdit_PassCreate->setEchoMode(QLineEdit::Normal);
    } else {
        ui->lineEdit_PassCreate->setEchoMode(QLineEdit::Password);
    }
}

void Edit::on_lineEdit_PassCreate_textChanged(const QString &arg1)
{
    if(arg1 != ""){
       ui->Button_OK_PassCreate->setEnabled(true);
    } else {
       ui->Button_OK_PassCreate->setEnabled(false);
    }
}

void Edit::on_Button_Cancel_PassCreate_clicked()
{
    ui->label_PassCreate->setVisible(false);
    ui->checkBox_PassCreate->setVisible(false);
    ui->lineEdit_PassCreate->setVisible(false);
    ui->Button_OK_PassCreate->setVisible(false);
    ui->Button_Cancel_PassCreate->setVisible(false);
    ui->lineEdit_PassCreate->clear();
}

void Edit::on_Button_OK_PassCreate_clicked()
{
    db_Test = QSqlDatabase::addDatabase("QSQLITE");
    db_Test.setDatabaseName(str_TestPathInEdit);
    db_Test.open();

    QSqlQuery query;
    query.exec("DELETE FROM 'code';");
    query.exec("DELETE FROM sqlite_sequence WHERE name = 'code';");

    query.prepare("INSERT INTO code (code)"
                  "VALUES (:code);");
    query.bindValue(":code", ui->lineEdit_PassCreate->text());
    query.exec();

    ui->lineEdit_PassCurrent->setText(ui->lineEdit_PassCreate->text());
    on_Button_Cancel_PassCreate_clicked();

    ui->Button_PassCreate->setEnabled(false);
    ui->Button_PassChange->setEnabled(true);
    ui->Button_PassDel->setEnabled(true);
    str_Password = ui->lineEdit_PassCreate->text();

    db_Test.close();
}

// change
void Edit::on_Button_PassChange_clicked()
{
    ui->label_PassChange_Old->setVisible(true);
    ui->checkBox_PassChange_Old->setVisible(true);
    ui->lineEdit_PassChange_Old->setVisible(true);
    ui->label_PassChange_New->setVisible(true);
    ui->checkBox_PassChange_New->setVisible(true);
    ui->lineEdit_PassChange_New->setVisible(true);
    ui->Button_OK_PassChange->setVisible(true);
    ui->Button_Cancel_PassChange->setVisible(true);
    ui->Button_PassDel->setEnabled(false);
}

void Edit::on_checkBox_PassChange_Old_clicked(bool checked)
{
    if(checked){
        ui->lineEdit_PassChange_Old->setEchoMode(QLineEdit::Normal);
    } else {
        ui->lineEdit_PassChange_Old->setEchoMode(QLineEdit::Password);
    }
}

void Edit::on_checkBox_PassChange_New_clicked(bool checked)
{
    if(checked){
        ui->lineEdit_PassChange_New->setEchoMode(QLineEdit::Normal);
    } else {
        ui->lineEdit_PassChange_New->setEchoMode(QLineEdit::Password);
    }
}

void Edit::on_lineEdit_PassChange_Old_textChanged(const QString &arg1)
{
    if(arg1 != "" && ui->lineEdit_PassChange_New->text() != "" && ui->lineEdit_PassChange_New->text() != ui->lineEdit_PassChange_Old->text()){
       ui->Button_OK_PassChange->setEnabled(true);
    } else {
       ui->Button_OK_PassChange->setEnabled(false);
    }
}

void Edit::on_lineEdit_PassChange_New_textChanged(const QString &arg1)
{
    if(arg1 != "" && ui->lineEdit_PassChange_Old->text() != "" && ui->lineEdit_PassChange_Old->text() != ui->lineEdit_PassChange_New->text()){
       ui->Button_OK_PassChange->setEnabled(true);
    } else {
       ui->Button_OK_PassChange->setEnabled(false);
    }
}

void Edit::on_Button_Cancel_PassChange_clicked()
{
    ui->label_PassChange_Old->setVisible(false);
    ui->checkBox_PassChange_Old->setVisible(false);
    ui->lineEdit_PassChange_Old->setVisible(false);
    ui->label_PassChange_New->setVisible(false);
    ui->checkBox_PassChange_New->setVisible(false);
    ui->lineEdit_PassChange_New->setVisible(false);
    ui->Button_OK_PassChange->setVisible(false);
    ui->Button_Cancel_PassChange->setVisible(false);
    ui->Button_PassDel->setEnabled(true);
    ui->lineEdit_PassChange_Old->clear();
    ui->lineEdit_PassChange_New->clear();
}

void Edit::on_Button_OK_PassChange_clicked()
{
    db_Test = QSqlDatabase::addDatabase("QSQLITE");
    db_Test.setDatabaseName(str_TestPathInEdit);
    db_Test.open();

    if(ui->lineEdit_PassCurrent->text() == ui->lineEdit_PassChange_Old->text()){
        QSqlQuery query;
        query.exec("DELETE FROM 'code';");
        query.exec("DELETE FROM sqlite_sequence WHERE name = 'code';");

        query.prepare("INSERT INTO code (code)"
                      "VALUES (:code);");
        query.bindValue(":code", ui->lineEdit_PassChange_New->text());
        query.exec();

        ui->lineEdit_PassCurrent->setText(ui->lineEdit_PassChange_New->text());
        on_Button_Cancel_PassChange_clicked();
        ui->Button_PassDel->setEnabled(true);
        str_Password = ui->lineEdit_PassChange_New->text();
    } else {
        ui->lineEdit_PassChange_Old->clear();
        ui->lineEdit_PassChange_New->clear();
    }

    db_Test.close();
}

// delete
void Edit::on_Button_PassDel_clicked()
{
    ui->label_PassDel->setVisible(true);
    ui->checkBox_PassDel->setVisible(true);
    ui->lineEdit_PassDel->setVisible(true);
    ui->Button_OK_PassDel->setVisible(true);
    ui->Button_Cancel_PassDel->setVisible(true);
    ui->Button_PassChange->setEnabled(false);
}

void Edit::on_checkBox_PassDel_clicked(bool checked)
{
    if(checked){
        ui->lineEdit_PassDel->setEchoMode(QLineEdit::Normal);
    } else {
        ui->lineEdit_PassDel->setEchoMode(QLineEdit::Password);
    }
}

void Edit::on_lineEdit_PassDel_textChanged(const QString &arg1)
{
    if(arg1 != ""){
       ui->Button_OK_PassDel->setEnabled(true);
    } else {
       ui->Button_OK_PassDel->setEnabled(false);
    }
}

void Edit::on_Button_Cancel_PassDel_clicked()
{
    ui->label_PassDel->setVisible(false);
    ui->checkBox_PassDel->setVisible(false);
    ui->lineEdit_PassDel->setVisible(false);
    ui->Button_OK_PassDel->setVisible(false);
    ui->Button_Cancel_PassDel->setVisible(false);
    ui->lineEdit_PassDel->clear();
}

void Edit::on_Button_OK_PassDel_clicked()
{
    db_Test = QSqlDatabase::addDatabase("QSQLITE");
    db_Test.setDatabaseName(str_TestPathInEdit);
    db_Test.open();

    if(ui->lineEdit_PassCurrent->text() == ui->lineEdit_PassDel->text()){
        QSqlQuery query;
        query.exec("DELETE FROM 'code';");
        query.exec("DELETE FROM sqlite_sequence WHERE name = 'code';");

        on_Button_Cancel_PassDel_clicked();
        ui->lineEdit_PassCurrent->setText("");
        ui->Button_PassChange->setEnabled(false);
        ui->Button_PassDel->setEnabled(false);
        ui->Button_PassCreate->setEnabled(true);
        str_Password = "";
    } else {
        ui->lineEdit_PassDel->clear();
    }

    db_Test.close();
}
