#include "editpart1.h"
#include "ui_editpart1.h"

EditPart1::EditPart1(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditPart1)
{
    ui->setupUi(this);
    ui->radioButton_1->setChecked(true);
    ui->radioButton_1->setText("Правильный ответ");
    m_iAnsRight = 1;
    m_iQuantIndex = 2;

    ui->comboBox->addItem("2");
    ui->comboBox->addItem("3");
    ui->comboBox->addItem("4");
    ui->comboBox->setCurrentIndex(2);
}

EditPart1::~EditPart1()
{
    delete ui;
}

// set ID when creating a question
void EditPart1::SetQuestID(int ID){
    QString str_ID = QString::number(ID);
    ui->label->setText(str_ID);
}

// reset order after drag and drop
void EditPart1::ReorderID(int index){
    QString str_GottenID = QString::number(index);
    ui->label->setText(str_GottenID);
}

// delete question
void EditPart1::Delete(){
    QString str_ID = ui->label->text();
    int ID = str_ID.toInt();
    if(ID > m_iOldID){
        ID--;
        str_ID = QString::number(ID);
        ui->label->setText(str_ID);
    }
}

// save part 1 to database
void EditPart1::Save(QString testpath){
    db_Test = QSqlDatabase::addDatabase("QSQLITE");
    db_Test.setDatabaseName(testpath);
    db_Test.open();
    QSqlQuery query;
    query.prepare("INSERT INTO part1 (Quest, Ans1, Ans2, Ans3, Ans4, AnsRight, Quant)"
                  "VALUES (:Quest, :Ans1, :Ans2, :Ans3, :Ans4, :AnsRight, :Quant);");
    query.bindValue(":Quest", ui->textEdit_Quest->toPlainText());
    query.bindValue(":Ans1", ui->textEdit_Ans1->toPlainText());
    query.bindValue(":Ans2", ui->textEdit_Ans2->toPlainText());
    query.bindValue(":Ans3", ui->textEdit_Ans3->toPlainText());
    query.bindValue(":Ans4", ui->textEdit_Ans4->toPlainText());
    query.bindValue(":AnsRight", m_iAnsRight);
    query.bindValue(":Quant", m_iQuantIndex);
    query.exec();
 //   db_Test.close();
}

void EditPart1::Load(QString testpath){
    db_Test = QSqlDatabase::addDatabase("QSQLITE");
    db_Test.setDatabaseName(testpath);
    db_Test.open();
    if (!db_Test.open()) {
          qDebug() << db_Test.lastError().text();
    }
    QSqlQuery query;
    QString str_ID = ui->label->text();
    int Index = str_ID.toInt();

    query.prepare("SELECT * FROM part1;");
    query.exec();
    if (query.seek(Index-1)){
        str_Quest = query.value(1).toString(); // 1 - Quest
        str_Ans1 = query.value(2).toString(); // 2 - Ans 1
        str_Ans2 = query.value(3).toString(); // 3 - Ans 2
        str_Ans3 = query.value(4).toString(); // 4 - Ans 3
        str_Ans4 = query.value(5).toString(); // 5 - Ans 4
        m_iAnsRight = query.value(6).toInt(); // 6 - AnsRight
        m_iQuantIndex = query.value(7).toInt(); // 7 - Quantity of variants
    }

    ui->textEdit_Quest->setText(str_Quest);
    ui->textEdit_Ans1->setText(str_Ans1);
    ui->textEdit_Ans2->setText(str_Ans2);
    ui->textEdit_Ans3->setText(str_Ans3);
    ui->textEdit_Ans4->setText(str_Ans4);

    if(m_iAnsRight == 1){
        SetQuantIndex();
        AnsRight1();
        ui->radioButton_1->setChecked(true);
    } else if(m_iAnsRight == 2){
        SetQuantIndex();
        AnsRight2();
        ui->radioButton_2->setChecked(true);
       }
    else if(m_iAnsRight == 3){
        SetQuantIndex();
        AnsRight3();
        ui->radioButton_3->setChecked(true);
       }
    else if(m_iAnsRight == 4){
        SetQuantIndex();
        AnsRight4();
        ui->radioButton_4->setChecked(true);
       }

    db_Test.close();
}

void EditPart1::SetQuantIndex()
{
    if(m_iQuantIndex == 0){
        ui->comboBox->setCurrentIndex(0);
    } else if(m_iQuantIndex == 1){
        ui->comboBox->setCurrentIndex(1);
    } else if(m_iQuantIndex == 2){
        ui->comboBox->setCurrentIndex(2);
    }
}

// choosing the right answer
void EditPart1::AnsRight1()
{
        ui->radioButton_1->setText("Правильный ответ");
        ui->radioButton_2->setText("");
        ui->radioButton_3->setText("");
        ui->radioButton_4->setText("");
}
void EditPart1::AnsRight2()
{
    ui->radioButton_2->setText("Правильный ответ");
    ui->radioButton_1->setText("");
    ui->radioButton_3->setText("");
    ui->radioButton_4->setText("");
}
void EditPart1::AnsRight3()
{
    ui->radioButton_3->setText("Правильный ответ");
    ui->radioButton_1->setText("");
    ui->radioButton_2->setText("");
    ui->radioButton_4->setText("");
}
void EditPart1::AnsRight4()
{
    ui->radioButton_4->setText("Правильный ответ");
    ui->radioButton_1->setText("");
    ui->radioButton_2->setText("");
    ui->radioButton_3->setText("");
}

void EditPart1::on_radioButton_1_clicked(bool checked)
{
    if(checked){
        AnsRight1();
        m_iAnsRight = 1;
    }
}

void EditPart1::on_radioButton_2_clicked(bool checked)
{
    if(checked){
        AnsRight2();
        m_iAnsRight = 2;
    }
}

void EditPart1::on_radioButton_3_clicked(bool checked)
{
    if(checked){
        AnsRight3();
        m_iAnsRight = 3;
    }
}

void EditPart1::on_radioButton_4_clicked(bool checked)
{
    if(checked){
        AnsRight4();
        m_iAnsRight = 4;
    }
}

// quantity of variants m_iQuant
void EditPart1::on_comboBox_currentIndexChanged(int index)
{
    if(index == 2){
        m_iQuantIndex = 2;
        ui->radioButton_1->setChecked(true);
        AnsRight1();
        m_iAnsRight = 1;

        ui->label_4->setEnabled(true);
        ui->label_5->setEnabled(true);
        ui->textEdit_Ans3->setEnabled(true);
        ui->textEdit_Ans4->setEnabled(true);
        ui->radioButton_3->setEnabled(true);
        ui->radioButton_4->setEnabled(true);

    } else if(index == 1){
        m_iQuantIndex = 1;
        ui->radioButton_1->setChecked(true);
        AnsRight1();
        m_iAnsRight = 1;
        ui->textEdit_Ans4->setText("");

        ui->label_4->setEnabled(true);
        ui->label_5->setEnabled(false);
        ui->textEdit_Ans3->setEnabled(true);
        ui->textEdit_Ans4->setEnabled(false);
        ui->radioButton_3->setEnabled(true);
        ui->radioButton_4->setEnabled(false);

    } else if(index == 0){
        m_iQuantIndex = 0;
        ui->radioButton_1->setChecked(true);
        AnsRight1();
        m_iAnsRight = 1;
        ui->textEdit_Ans3->setText("");
        ui->textEdit_Ans4->setText("");

        ui->label_4->setEnabled(false);
        ui->label_5->setEnabled(false);
        ui->textEdit_Ans3->setEnabled(false);
        ui->textEdit_Ans4->setEnabled(false);
        ui->radioButton_3->setEnabled(false);
        ui->radioButton_4->setEnabled(false);

    }
}

/*
void EditPart1::on_textEdit_Quest_textChanged()
{
    m_bChanges = true;
}

void EditPart1::SetChanges(){
    m_bChanges = true;
}

bool EditPart1::GetChanges(){
    return m_bChanges;
}


*/

