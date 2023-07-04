#include "editpart3_teacher.h"
#include "ui_editpart3_teacher.h"

EditPart3_Teacher::EditPart3_Teacher(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditPart3_Teacher)
{
    ui->setupUi(this);
}

EditPart3_Teacher::~EditPart3_Teacher()
{
    delete ui;
}

// set ID when creating a question
void EditPart3_Teacher::SetQuestID(int ID){
    QString str_ID = QString::number(ID);
    ui->label->setText(str_ID);
}

// delete question
void EditPart3_Teacher::Delete(){
    QString str_ID = ui->label->text();
    int ID = str_ID.toInt();
    if(ID > m_iOldID){
        ID--;
        str_ID = QString::number(ID);
        ui->label->setText(str_ID);
    }
}

// reset order after drag and drop
void EditPart3_Teacher::ReorderID(int index){
    QString str_GottenID = QString::number(index);
    ui->label->setText(str_GottenID);
}

// save part 3 to database
void EditPart3_Teacher::Save(QString testpath){
    db_Test = QSqlDatabase::addDatabase("QSQLITE");
    db_Test.setDatabaseName(testpath);
 //   db_Test.open();
    if (!db_Test.open()) {
          qDebug() << db_Test.lastError().text();
    }
    QSqlQuery query;
//    query.prepare("UPDATE part3 (Quest) VALUES (:Quest);");
//    query.bindValue(":Quest", "ui->textEdit_Quest->toPlainText()");
    query.prepare("INSERT INTO part3 (Quest, Ans, Wrong)"
                  "VALUES (:Quest, :Ans, :Wrong);");
    query.bindValue(":Quest", ui->textEdit_Quest->toPlainText());
    query.bindValue(":Ans", ui->textEdit_Ans->toPlainText());
    query.bindValue(":Wrong", "");
    query.exec();
  //  db_Test.close();
}

// load
void EditPart3_Teacher::Load(QString testpath){
    db_Test = QSqlDatabase::addDatabase("QSQLITE");
    db_Test.setDatabaseName(testpath);
    db_Test.open();
    if (!db_Test.open()) {
          qDebug() << db_Test.lastError().text();
    }
    QSqlQuery query;
    QString str_ID = ui->label->text();
    int Index = str_ID.toInt();

    query.prepare("SELECT * FROM part3;");
    query.exec();
    if (query.seek(Index-1)){
        str_Quest = query.value(1).toString(); // 1 - Quest
        str_Ans = query.value(2).toString(); // 2 - Ans 1
    }

    ui->textEdit_Quest->setText(str_Quest);
    ui->textEdit_Ans->setText(str_Ans);

    db_Test.close();
}

// load
void EditPart3_Teacher::ReceiveRightAns(QString rightans){
    ui->textEdit_Ans->setText(rightans);
}
