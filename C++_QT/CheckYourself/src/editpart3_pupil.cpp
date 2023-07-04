#include "editpart3_pupil.h"
#include "ui_editpart3_pupil.h"

EditPart3_Pupil::EditPart3_Pupil(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditPart3_Pupil)
{
    ui->setupUi(this);
}

EditPart3_Pupil::~EditPart3_Pupil()
{
    delete ui;
}

// set ID when creating a question
void EditPart3_Pupil::SetQuestID(int ID){
    QString str_ID = QString::number(ID);
    ui->label->setText(str_ID);
}

// delete question
void EditPart3_Pupil::Delete(){
    QString str_ID = ui->label->text();
    int ID = str_ID.toInt();
    if(ID > m_iOldID){
        ID--;
        str_ID = QString::number(ID);
        ui->label->setText(str_ID);
    }
}

// reset order after drag and drop
void EditPart3_Pupil::ReorderID(int index){
    QString str_GottenID = QString::number(index);
    ui->label->setText(str_GottenID);
}

// save part 3 to database
void EditPart3_Pupil::Save(QString testpath){
    QString str_ID = ui->label->text();
    int ID = str_ID.toInt();
    db_Test = QSqlDatabase::addDatabase("QSQLITE");
    db_Test.setDatabaseName(testpath);
    db_Test.open();
    if (!db_Test.open()) {
          qDebug() << db_Test.lastError().text();
    }
    QSqlQuery query;
    query.prepare("UPDATE part3 SET Wrong=:Wrong WHERE ID=:ID;");
    query.bindValue(":Wrong", ui->textEdit_Ans->toPlainText());
    query.bindValue(":ID", ID);
    query.exec();
    db_Test.close();
}

// fix
void EditPart3_Pupil::GetRightAns(QString &rightans){
    rightans = ui->textEdit_Ans->toPlainText();
}

// load
void EditPart3_Pupil::Load(QString testpath){
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
        str_Wrong = query.value(3).toString(); // 1 - Quest
    }

    ui->textEdit_Ans->setText(str_Wrong);

    db_Test.close();
}
