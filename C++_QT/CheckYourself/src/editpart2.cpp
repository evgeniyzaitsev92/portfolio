#include "editpart2.h"
#include "ui_editpart2.h"

EditPart2::EditPart2(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditPart2)
{
    ui->setupUi(this);

    m_iQuantIndex = 0;
    ui->comboBox->addItem("1");
    ui->comboBox->addItem("2");
    ui->comboBox->setCurrentIndex(0);
}

EditPart2::~EditPart2()
{
    delete ui;
}

// set ID when creating a question
void EditPart2::SetQuestID(int ID){
    QString str_ID = QString::number(ID);
    ui->label->setText(str_ID);
}

// reset order after drag and drop
void EditPart2::ReorderID(int index){
    QString str_GottenID = QString::number(index);
    ui->label->setText(str_GottenID);
}

// delete question
void EditPart2::Delete(){
    QString str_ID = ui->label->text();
    int ID = str_ID.toInt();
    if(ID > m_iOldID){
        ID--;
        str_ID = QString::number(ID);
        ui->label->setText(str_ID);
    }
}

// save part 2 to database
void EditPart2::Save(QString testpath){
    db_Test = QSqlDatabase::addDatabase("QSQLITE");
    db_Test.setDatabaseName(testpath);
 //   db_Test.open();
    if (!db_Test.open()) {
          qDebug() << db_Test.lastError().text();
    }
    QSqlQuery query;
    query.prepare("INSERT INTO part2 (Quest, Word, Word2, Quant)"
                  "VALUES (:Quest, :Word, :Word2, :Quant);");
    query.bindValue(":Quest", ui->textEdit_Quest->toPlainText());
    query.bindValue(":Word", ui->textEdit_Ans1->toPlainText());
    query.bindValue(":Word2", ui->textEdit_Ans2->toPlainText());
    query.bindValue(":Quant", m_iQuantIndex);
    query.exec();
 //   db_Test.close();
}

void EditPart2::Load(QString testpath){
    db_Test = QSqlDatabase::addDatabase("QSQLITE");
    db_Test.setDatabaseName(testpath);
    db_Test.open();
    if (!db_Test.open()) {
          qDebug() << db_Test.lastError().text();
    }
    QSqlQuery query;
    QString str_ID = ui->label->text();
    int Index = str_ID.toInt();

    query.prepare("SELECT * FROM part2;");
    query.exec();
    if (query.seek(Index-1)){
        str_Quest = query.value(1).toString(); // 1 - Quest
        str_Ans1 = query.value(2).toString(); // 2 - Ans 1
        str_Ans2 = query.value(3).toString(); // 3 - Ans 2
        m_iQuantIndex = query.value(4).toInt(); // 7 - Quantity of variants
    }

    ui->textEdit_Quest->setText(str_Quest);
    ui->textEdit_Ans1->setText(str_Ans1);
    ui->textEdit_Ans2->setText(str_Ans2);


    if(m_iQuantIndex == 0){
        ui->comboBox->setCurrentIndex(0);
    } else if(m_iQuantIndex == 1){
        ui->comboBox->setCurrentIndex(1);
    }

    db_Test.close();
}

// quantity of variants m_iQuant
void EditPart2::on_comboBox_currentIndexChanged(int index)
{
    if(index == 0){
        m_iQuantIndex = 0;

        ui->label_2->setEnabled(true);
        ui->label_3->setEnabled(false);
        ui->textEdit_Ans1->setEnabled(true);
        ui->textEdit_Ans2->setEnabled(false);
        ui->textEdit_Ans2->setText("");

    } else if(index == 1){
        m_iQuantIndex = 1;

        ui->label_2->setEnabled(true);
        ui->label_3->setEnabled(true);
        ui->textEdit_Ans1->setEnabled(true);
        ui->textEdit_Ans2->setEnabled(true);

    }
}
