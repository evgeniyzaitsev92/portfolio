#include "test.h"
#include "ui_test.h"
#include "results.h"

Test::Test(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Test),
    timer(new QTimer()),
    time(new QTime(0,0))

{
    ui->setupUi(this);
    setWindowFlags(Qt::Window
        | Qt::WindowMinimizeButtonHint
        | Qt::WindowMaximizeButtonHint);

  //  connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(0);
    timer->start(1000);
    connect(timer,&QTimer::timeout,[this](){
             *time=time->addMSecs(1000);
            ui->label_Time->setText(time->toString("mm:ss"));
        });
}

Test::~Test()
{
    delete ui;
}

//*******//
// TIMER //
//*******//
void Test::updateTime()
{
  //   int a = 0;
  //   a = a
  //   ui->label_Time->setText(QString::number(a));
}

//***************//
// GET TEST PATH //
//***************//
void Test::GetTestPath(QString testpath){
    str_TestPathInEdit = testpath;
    Load();
}

void Test::on_tabWidget_currentChanged()
{
    if(m_bHasPart1 == true && m_bHasPart2 == true){
        if(count > 0 && !(ui->tab_2->isEnabled()) && !(ui->tab_3->isEnabled())){
            ui->tabWidget->setCurrentIndex(0);
        } else if(count2 > 0 && !(ui->tab->isEnabled()) && !(ui->tab_3->isEnabled())){
            ui->tabWidget->setCurrentIndex(1);
        } else if(count3 > 0 && !(ui->tab->isEnabled()) && !(ui->tab_2->isEnabled())){
            ui->tabWidget->setCurrentIndex(2);
        }
    } else if(m_bHasPart1 == true && m_bHasPart3 == true && m_bHasPart2 == false){
        if(count > 0 && !(ui->tab_3->isEnabled())){
            ui->tabWidget->setCurrentIndex(0);
        } else if(count3 > 0 && !(ui->tab->isEnabled())){
            ui->tabWidget->setCurrentIndex(1);
        }
    } else if(m_bHasPart1 == false && m_bHasPart2 == true && m_bHasPart3 == true){
        if(count2 > 0 && !(ui->tab_3->isEnabled())){
            ui->tabWidget->setCurrentIndex(0);
        } else if(count3 > 0 && !(ui->tab_2->isEnabled())){
            ui->tabWidget->setCurrentIndex(1);
        }
    }

}

//******//
// LOAD //
//******//
void Test::Load()
{
    db_Test = QSqlDatabase::addDatabase("QSQLITE");
    db_Test.setDatabaseName(str_TestPathInEdit);
    db_Test.open();
    if (!db_Test.open()) {
          qDebug() << db_Test.lastError().text();
    }
    QSqlQuery query;
    query.exec("DELETE FROM 'results1';");
    query.exec("DELETE FROM sqlite_sequence WHERE name = 'results1';");
    query.exec("DELETE FROM 'results2';");
    query.exec("DELETE FROM sqlite_sequence WHERE name = 'results2';");
    query.exec("DELETE FROM 'results3';");
    query.exec("DELETE FROM sqlite_sequence WHERE name = 'results3';");

    query.prepare("SELECT * FROM part1task;");
    query.exec();
    if (query.seek(0)){
        str_Task = query.value(1).toString();
    }

    query.prepare("SELECT * FROM part2task;");
    query.exec();
    if (query.seek(0)){
        str_Task2 = query.value(1).toString();
    }

    query.prepare("SELECT * FROM part3task;");
    query.exec();
    if (query.seek(0)){
        str_Task3 = query.value(1).toString();
    }

    query.prepare("SELECT * FROM part1;");
    query.exec();
  //  count = 0;
    if (query.last()) {
        count = query.value(0).toInt();
    }
    countR = count;

    query.prepare("SELECT * FROM part2;");
    query.exec();
  //  count2 = 0;
    if (query.last()) {
        count2 = query.value(0).toInt();
    }
    count2R = count2;

    query.prepare("SELECT * FROM part3;");
    query.exec();
  //  count3 = 0;
    if (query.last()) {
        count3 = query.value(0).toInt();
    }
    count3R = count3;

    if(count>0){
        m_bHasPart1 = true;
    }
    if(count2>0){
        m_bHasPart2 = true;
    }
    if(count3>0){
        m_bHasPart3 = true;
    }

    // check if there are questions
    if(count3==0){
        ui->tabWidget->removeTab(2);
    }
    if(count2==0){
        ui->tabWidget->removeTab(1);
    }
    if(count==0){
        ui->tabWidget->removeTab(0);
    }

    // set names
    if(count>0 && count2==0 && count3>0){
        ui->tabWidget->setTabText(1, "Часть 2");
        str_Part3Name = "ЧАСТЬ 2";
    } else if(count==0 && count2>0 && count3>0){
        ui->tabWidget->setTabText(0, "Часть 1");
        ui->tabWidget->setTabText(1, "Часть 2");
        str_Part2Name = "ЧАСТЬ 1";
        str_Part3Name = "ЧАСТЬ 2";
    } else if(count==0 && count2==0 && count3>0){
        ui->tabWidget->setTabText(0, "Часть 1");
        str_Part3Name = "ЧАСТЬ 1";
    } else if(count>0 && count2>0 && count3>0){
        str_Part2Name = "ЧАСТЬ 2";
        str_Part3Name = "ЧАСТЬ 3";
    } else if(count>0 && count2>0 && count3==0){
        str_Part2Name = "ЧАСТЬ 2";
    } else if(count==0 && count2>0 && count3==0){
        ui->tabWidget->setTabText(0, "Часть 1");
        str_Part2Name = "ЧАСТЬ 1";
    }

    // set current tab
    ui->tabWidget->setCurrentIndex(0);
    if(m_bHasPart1 == true){
        ui->tab->setEnabled(true);
        ui->tab_2->setEnabled(false);
        ui->tab_3->setEnabled(false);
    } else if(m_bHasPart1 == false && m_bHasPart2 == true){
        ui->tab_2->setEnabled(true);
        ui->tab->setEnabled(false);
        ui->tab_3->setEnabled(false);
    } else if(m_bHasPart1 == false && m_bHasPart2 == false && m_bHasPart3 == true){
        ui->tab_3->setEnabled(true);
        ui->tab->setEnabled(false);
        ui->tab_2->setEnabled(false);
        ui->Button_Add->setVisible(false);
    }

    // load available part
    if(count>0){
        Load1();
    } else if(count2>0 && count==0){
        Load2();
    } else if(count3>0 && count2==0 && count==0){
        Load3();
    }
    db_Test.close();
}

void Test::Load1()
{
    ui->radioButton_Ans1->setAutoExclusive(false);
    ui->radioButton_Ans2->setAutoExclusive(false);
    ui->radioButton_Ans3->setAutoExclusive(false);
    ui->radioButton_Ans4->setAutoExclusive(false);
    ui->radioButton_Ans1->setChecked(false);
    ui->radioButton_Ans2->setChecked(false);
    ui->radioButton_Ans3->setChecked(false);
    ui->radioButton_Ans4->setChecked(false);

    ui->radioButton_Ans1->setAutoExclusive(true);
    ui->radioButton_Ans2->setAutoExclusive(true);
    ui->radioButton_Ans3->setAutoExclusive(true);
    ui->radioButton_Ans4->setAutoExclusive(true);

    db_Test = QSqlDatabase::addDatabase("QSQLITE");
    db_Test.setDatabaseName(str_TestPathInEdit);
    db_Test.open();

    QSqlQuery query;

    ui->lineEdit_Task->setText(str_Task);

    query.prepare("SELECT * FROM part1;");
    query.exec();

    if (query.seek(m_iPart1Quest)){
        m_iAnsRight = query.value(6).toInt(); // 6 - AnsRight
        m_iQuantIndex = query.value(7).toInt(); // 7 - Quantity of variants

        if(m_iQuantIndex == 0){
            ui->radioButton_Ans3->setVisible(false);
            ui->radioButton_Ans4->setVisible(false);
        }
        if(m_iQuantIndex == 1){
            ui->radioButton_Ans4->setVisible(false);
        }

        ui->label_Num->setText(query.value(0).toString());
        ui->label_Quest->setText(query.value(1).toString());
        ui->radioButton_Ans1->setText(query.value(2).toString());
        ui->radioButton_Ans2->setText(query.value(3).toString());
        if(m_iQuantIndex == 1 || m_iQuantIndex == 2){
            ui->radioButton_Ans3->setText(query.value(4).toString());
        }
        if(m_iQuantIndex == 2){
            ui->radioButton_Ans4->setText(query.value(5).toString());
        }

        if(m_iAnsRight == 1){
            str_QuestRight = query.value(2).toString();
        } else if(m_iAnsRight == 2){
            str_QuestRight = query.value(3).toString();
        } else if(m_iAnsRight == 3){
            str_QuestRight = query.value(4).toString();
        } else if(m_iAnsRight == 4){
            str_QuestRight = query.value(5).toString();
        }
    }

    db_Test.close();
}

void Test::Load2()
{
    ui->lineEdit_Ans->clear();


    db_Test = QSqlDatabase::addDatabase("QSQLITE");
    db_Test.setDatabaseName(str_TestPathInEdit);
    db_Test.open();

    QSqlQuery query;

    ui->lineEdit_Task_2->setText(str_Task2);

    query.prepare("SELECT * FROM part2;");
    query.exec();

    if (query.seek(m_iPart2Quest)){
        str_AnsRight2_1 = query.value(2).toString(); // 2 - AnsRight 1
        str_AnsRight2_2 = query.value(3).toString(); // 3 - AnsRight 2
        m_iQuantIndex2 = query.value(4).toInt(); // 4 - Quantity of variants

        ui->label_Num_2->setText(query.value(0).toString());
        ui->label_Quest_2->setText(query.value(1).toString());
    }

    db_Test.close();
}

void Test::Load3()
{
    m_bPart3Started = true;

    db_Test = QSqlDatabase::addDatabase("QSQLITE");
    db_Test.setDatabaseName(str_TestPathInEdit);
    db_Test.open();

    QSqlQuery query;

    ui->lineEdit_Task_3->setText(str_Task3);

    query.prepare("SELECT * FROM part3;");
    query.exec();
    QString str_Wrong = "";
    QString str_Question = "";

    for (int i=0; i<count3; i++) {
        if (query.seek(i)){
            str_Wrong = query.value(3).toString();
            ui->listWidget_Ans->addItem(str_Wrong);
            str_Question = query.value(1).toString();
            ui->listWidget_Quest->addItem(str_Question);
        //    ui->listWidget_Quest->setTextAlignment(Qt::AlignCenter);
        }

    }

    db_Test.close();
}

//******//
// NEXT //
//******//
void Test::on_Button_Add_clicked()
{
    db_Test = QSqlDatabase::addDatabase("QSQLITE");
    db_Test.setDatabaseName(str_TestPathInEdit);
    db_Test.open();


    if(count > 0){
     // Part 1
        if(m_iAnsRight == m_iAnsRightChoose){
         // result right
            m_iAnsRightCount++;

            QSqlQuery query;
            query.prepare("INSERT INTO results1 (Result)"
                          "VALUES (:Result);");
            query.bindValue(":Result", "Вопрос №" + ui->label_Num->text() + ": \"" + ui->label_Quest->text() + "\". "
                            + "Выбранный ответ верный: \"" + str_QuestRight + "\".");
            query.exec();

        } else if(m_iAnsRightChoose == 0){
            // no answer
               QSqlQuery query;
               query.prepare("INSERT INTO results1 (Result)"
                             "VALUES (:Result);");
               query.bindValue(":Result", "Вопрос №" + ui->label_Num->text() + ": \""
                               + ui->label_Quest->text() + "\". " + "Ответ выбран не был. "
                               + "Правильный ответ: \"" + str_QuestRight + "\".");
               query.exec();

           } else if(m_iAnsRight != m_iAnsRightChoose){
         // result wrong
            QSqlQuery query;
            query.prepare("INSERT INTO results1 (Result)"
                          "VALUES (:Result);");
            query.bindValue(":Result", "Вопрос №" + ui->label_Num->text() + ": \""
                            + ui->label_Quest->text() + "\". " + "Выбранный ответ неверный: \"" + str_AnsChoose + "\". "
                            + "Правильный ответ: \"" + str_QuestRight + "\".");
            query.exec();

        }
        db_Test.close();

        m_iPart1Quest++;
        count--;

        if(count==1 && count2==0 && count3==0){
            ui->Button_Add->setVisible(false);
        }

        if(count2>0 && count==0){
            ui->tab_2->setEnabled(true);
            ui->tab->setEnabled(false);
            ui->tab_3->setEnabled(false);
            ui->tabWidget->setCurrentIndex(1);
            Load2();
        } else if(count3>0 && count2==0 && count==0){
            ui->tab_3->setEnabled(true);
            ui->tab->setEnabled(false);
            ui->tab_2->setEnabled(false);
            ui->tabWidget->setCurrentIndex(1);
            ui->Button_Add->setVisible(false);
            Load3();
        } else if(count > 0){
            Load1();
        } else if(count == 0 && count2==0 && count3==0){
            // RESULTS
            on_Button_Exit_clicked();
        }

    } else if(count2 > 0 && count == 0){
      // Part 2
        if((ui->lineEdit_Ans->text() == str_AnsRight2_1 || ui->lineEdit_Ans->text() == str_AnsRight2_2) && ui->lineEdit_Ans->text() != "" ){
         // result right
            m_iAnsRightCount2++;
            QSqlQuery query;

            if(m_iQuantIndex2 == 0){
                query.prepare("INSERT INTO results2 (Result)"
                              "VALUES (:Result);");
                query.bindValue(":Result", "Вопрос №" + ui->label_Num_2->text() + ": \"" + ui->label_Quest_2->text() + "\". "
                                + "Выбранный ответ верный: \"" + str_AnsRight2_1 + "\".");
            } else if(m_iQuantIndex2 == 1){
                if(ui->lineEdit_Ans->text() == str_AnsRight2_1){
                    query.prepare("INSERT INTO results2 (Result)"
                                  "VALUES (:Result);");
                    query.bindValue(":Result", "Вопрос №" + ui->label_Num_2->text() + ": \"" + ui->label_Quest_2->text() + "\". "
                                    + "Выбранный ответ верный: \"" + str_AnsRight2_1 + "\". " + "Альтернативный ответ: \"" + str_AnsRight2_2 + "\".");
                } else if(ui->lineEdit_Ans->text() == str_AnsRight2_2){
                    query.prepare("INSERT INTO results2 (Result)"
                                  "VALUES (:Result);");
                    query.bindValue(":Result", "Вопрос №" + ui->label_Num_2->text() + ": \"" + ui->label_Quest_2->text() + "\". "
                                    + "Выбранный ответ верный: \"" + str_AnsRight2_2 + "\". " + "Альтернативный ответ: \"" + str_AnsRight2_1 + "\".");
                }
            }
            query.exec();

        } else if(ui->lineEdit_Ans->text() == ""){
            // no answer
               QSqlQuery query;
               if(m_iQuantIndex2 == 0){
                   query.prepare("INSERT INTO results2 (Result)"
                                 "VALUES (:Result);");
                   query.bindValue(":Result", "Вопрос №" + ui->label_Num_2->text() + ": \""
                                   + ui->label_Quest_2->text() + "\". " + "Ответ выбран не был. "
                                   + "Правильный ответ: \"" + str_AnsRight2_1 + "\".");
               } else if(m_iQuantIndex2 == 1){
                   query.prepare("INSERT INTO results2 (Result)"
                                 "VALUES (:Result);");
                   query.bindValue(":Result", "Вопрос №" + ui->label_Num_2->text() + ": \""
                                   + ui->label_Quest_2->text() + "\". " + "Ответ выбран не был. "
                                   + "Правильные ответы: \"" + str_AnsRight2_1 + "\" и \"" + str_AnsRight2_2 + "\".");
               }
               query.exec();

           } else if(ui->lineEdit_Ans->text() != str_AnsRight2_1 && ui->lineEdit_Ans->text() != str_AnsRight2_2){
         // result wrong
            QSqlQuery query;
            if(m_iQuantIndex2 == 0){
                query.prepare("INSERT INTO results2 (Result)"
                              "VALUES (:Result);");
                query.bindValue(":Result", "Вопрос №" + ui->label_Num_2->text() + ": "
                                + ui->label_Quest_2->text() + ". " + "Выбранный ответ неверный: \"" + ui->lineEdit_Ans->text() + "\". "
                                + "Правильный ответ: \"" + str_AnsRight2_1 + "\".");
            } else if(m_iQuantIndex2 == 1){
                query.prepare("INSERT INTO results2 (Result)"
                              "VALUES (:Result);");
                query.bindValue(":Result", "Вопрос №" + ui->label_Num_2->text() + ": \""
                                + ui->label_Quest_2->text() + "\". " + "Выбранный ответ неверный: \"" + ui->lineEdit_Ans->text() + "\". "
                                + "Правильные ответы: \"" + str_AnsRight2_1 + "\" и \"" + str_AnsRight2_2 + "\".");
            }
            query.exec();
        }

        m_iPart2Quest++;
        count2--;

        if(count2==1 && count3==0){
            ui->Button_Add->setVisible(false);
        }

        if(count3>0 && count2==0){
            ui->tab_3->setEnabled(true);
            ui->tab->setEnabled(false);
            ui->tab_2->setEnabled(false);
            if(m_bHasPart1 == true && m_bHasPart2 == true){
                ui->tabWidget->setCurrentIndex(2);
            } else if(m_bHasPart1 == false && m_bHasPart2 == true){
                ui->tabWidget->setCurrentIndex(1);
            }
            ui->Button_Add->setVisible(false);
            Load3();
        } else if(count2 > 0){
            Load2();
        } else if(count2==0 && count3==0){
            // RESULTS
            on_Button_Exit_clicked();
        }

    }
    //else if(count3 > 0 && count == 0 && count2 == 0){
      // Part 3
  //  }

}

void Test::on_radioButton_Ans1_clicked()
{
    m_iAnsRightChoose = 1;
    str_AnsChoose = ui->radioButton_Ans1->text();
}

void Test::on_radioButton_Ans2_clicked()
{
    m_iAnsRightChoose = 2;
    str_AnsChoose = ui->radioButton_Ans2->text();
}

void Test::on_radioButton_Ans3_clicked()
{
    m_iAnsRightChoose = 3;
    str_AnsChoose = ui->radioButton_Ans3->text();
}

void Test::on_radioButton_Ans4_clicked()
{
    m_iAnsRightChoose = 4;
    str_AnsChoose = ui->radioButton_Ans4->text();
}

//*********************//
// FINISH NOT COMPLETE //
//*********************//
void Test::FinishNotComplete()
{
    db_Test = QSqlDatabase::addDatabase("QSQLITE");
    db_Test.setDatabaseName(str_TestPathInEdit);
    db_Test.open();
    if(count > 0){
     // Part 1
        if(m_iAnsRight == m_iAnsRightChoose){
         // result right
            m_iAnsRightCount++;

            QSqlQuery query;
            query.prepare("INSERT INTO results1 (Result)"
                          "VALUES (:Result);");
            query.bindValue(":Result", "Вопрос №" + ui->label_Num->text() + ": \"" + ui->label_Quest->text() + "\". "
                            + "Выбранный ответ верный: \"" + str_QuestRight + "\".");
            query.exec();

        } else if(m_iAnsRightChoose == 0){
            // no answer
               QSqlQuery query;
               query.prepare("INSERT INTO results1 (Result)"
                             "VALUES (:Result);");
               query.bindValue(":Result", "Вопрос №" + ui->label_Num->text() + ": \""
                               + ui->label_Quest->text() + "\". " + "Ответ выбран не был. "
                               + "Правильный ответ: \"" + str_QuestRight + "\".");
               query.exec();

           } else if(m_iAnsRight != m_iAnsRightChoose){
         // result wrong
            QSqlQuery query;
            query.prepare("INSERT INTO results1 (Result)"
                          "VALUES (:Result);");
            query.bindValue(":Result", "Вопрос №" + ui->label_Num->text() + ": \""
                            + ui->label_Quest->text() + "\". " + "Выбранный ответ неверный: \"" + str_AnsChoose + "\". "
                            + "Правильный ответ: \"" + str_QuestRight + "\".");
            query.exec();

        }

        m_iPart1Quest++;
        count--;
    } else if(count2 > 0 && count == 0){
        // Part 2
          if((ui->lineEdit_Ans->text() == str_AnsRight2_1 || ui->lineEdit_Ans->text() == str_AnsRight2_2) && ui->lineEdit_Ans->text() != "" ){
           // result right
              m_iAnsRightCount2++;
              QSqlQuery query;

              if(m_iQuantIndex2 == 0){
                  query.prepare("INSERT INTO results2 (Result)"
                                "VALUES (:Result);");
                  query.bindValue(":Result", "Вопрос №" + ui->label_Num_2->text() + ": \"" + ui->label_Quest_2->text() + "\". "
                                  + "Выбранный ответ верный: \"" + str_AnsRight2_1 + "\".");
              } else if(m_iQuantIndex2 == 1){
                  if(ui->lineEdit_Ans->text() == str_AnsRight2_1){
                      query.prepare("INSERT INTO results2 (Result)"
                                    "VALUES (:Result);");
                      query.bindValue(":Result", "Вопрос №" + ui->label_Num_2->text() + ": \"" + ui->label_Quest_2->text() + "\". "
                                      + "Выбранный ответ верный: \"" + str_AnsRight2_1 + "\". " + "Альтернативный ответ: \"" + str_AnsRight2_2 + "\".");
                  } else if(ui->lineEdit_Ans->text() == str_AnsRight2_2){
                      query.prepare("INSERT INTO results2 (Result)"
                                    "VALUES (:Result);");
                      query.bindValue(":Result", "Вопрос №" + ui->label_Num_2->text() + ": \"" + ui->label_Quest_2->text() + "\". "
                                      + "Выбранный ответ верный: \"" + str_AnsRight2_2 + "\". " + "Альтернативный ответ: \"" + str_AnsRight2_1 + "\".");
                  }
              }
              query.exec();

          } else if(ui->lineEdit_Ans->text() == ""){
              // no answer
                 QSqlQuery query;
                 if(m_iQuantIndex2 == 0){
                     query.prepare("INSERT INTO results2 (Result)"
                                   "VALUES (:Result);");
                     query.bindValue(":Result", "Вопрос №" + ui->label_Num_2->text() + ": \""
                                     + ui->label_Quest_2->text() + "\". " + "Ответ выбран не был. "
                                     + "Правильный ответ: \"" + str_AnsRight2_1 + "\".");
                 } else if(m_iQuantIndex2 == 1){
                     query.prepare("INSERT INTO results2 (Result)"
                                   "VALUES (:Result);");
                     query.bindValue(":Result", "Вопрос №" + ui->label_Num_2->text() + ": \""
                                     + ui->label_Quest_2->text() + "\". " + "Ответ выбран не был. "
                                     + "Правильные ответы: \"" + str_AnsRight2_1 + "\" и \"" + str_AnsRight2_2 + "\".");
                 }
                 query.exec();

             } else if(ui->lineEdit_Ans->text() != str_AnsRight2_1 && ui->lineEdit_Ans->text() != str_AnsRight2_2){
           // result wrong
              QSqlQuery query;
              if(m_iQuantIndex2 == 0){
                  query.prepare("INSERT INTO results2 (Result)"
                                "VALUES (:Result);");
                  query.bindValue(":Result", "Вопрос №" + ui->label_Num_2->text() + ": "
                                  + ui->label_Quest_2->text() + ". " + "Выбранный ответ неверный: \"" + ui->lineEdit_Ans->text() + "\". "
                                  + "Правильный ответ: \"" + str_AnsRight2_1 + "\".");
              } else if(m_iQuantIndex2 == 1){
                  query.prepare("INSERT INTO results2 (Result)"
                                "VALUES (:Result);");
                  query.bindValue(":Result", "Вопрос №" + ui->label_Num_2->text() + ": "
                                  + ui->label_Quest_2->text() + ". " + "Выбранный ответ неверный: \"" + ui->lineEdit_Ans->text() + "\". "
                                  + "Правильные ответы: \"" + str_AnsRight2_1 + "\" и \"" + str_AnsRight2_2 + "\".");
              }
              query.exec();
          }

          m_iPart2Quest++;
          count2--;
      }
    db_Test.close();
}

//********//
// FINISH //
//********//
void Test::on_Button_Exit_clicked()
{
    FinishNotComplete();

    timer->stop();
    db_Test = QSqlDatabase::addDatabase("QSQLITE");
    db_Test.setDatabaseName(str_TestPathInEdit);
    db_Test.open();
    QSqlQuery query;

    // not selected questions QString::number(countR+count2R+count3R)
    if(countR>0 && count != 0){
        query.prepare("INSERT INTO results1 (Result)"
                      "VALUES (:Result);");
        query.bindValue(":Result", "Вопросы с/со " + QString::number(m_iPart1Quest+1) + " по "
                        + QString::number(countR) + " были пропущены.");
        query.exec();
    }

    if(count2R>0 && count2 != 0){
        query.prepare("INSERT INTO results2 (Result)"
                      "VALUES (:Result);");
        query.bindValue(":Result", "Вопросы с/со " + QString::number(m_iPart2Quest+1) + " по "
                        + QString::number(count2R) + " были пропущены.");
        query.exec();
    }

    query.exec("DELETE FROM 'counttotal';");
    query.exec("DELETE FROM sqlite_sequence WHERE name = 'counttotal';");
    query.exec("DELETE FROM 'count1';");
    query.exec("DELETE FROM sqlite_sequence WHERE name = 'count1';");
    query.exec("DELETE FROM 'count2';");
    query.exec("DELETE FROM sqlite_sequence WHERE name = 'count2';");
    query.exec("DELETE FROM 'count3';");
    query.exec("DELETE FROM sqlite_sequence WHERE name = 'count3';");
    query.exec("DELETE FROM 'time';");
    query.exec("DELETE FROM sqlite_sequence WHERE name = 'time';");

    query.prepare("INSERT INTO time (Time)"
                  "VALUES (:Time);");
    query.bindValue(":Time", "ВРЕМЯ ВЫПОЛНЕНИЯ ТЕСТА: " + ui->label_Time->text() +".");
    query.exec();

    // Part 3 result
    if(m_bHasPart3 == true){

        int ItemsCount = ui->listWidget_Ans->model()->rowCount();
        QString str_AnsR = "";
        QString str_Q = "";
        for (int index = 0; index < ItemsCount; index++) {
            QListWidgetItem* item = ui->listWidget_Ans->item(index);
            QString str_WordNum = QString::number(index+1);


            query.prepare("SELECT * FROM part3;");
            query.exec();
            if (query.seek(index)){
                str_AnsR = query.value(2).toString();
                str_Q = query.value(1).toString();
            }

            if(item->text() == str_AnsR){
                m_iAnsRightCount3++;
                query.prepare("INSERT INTO results3 (Result)"
                              "VALUES (:Result);");
                query.bindValue(":Result", "Пункт №" + str_WordNum + ": \"" + str_Q + "\". "
                                + "Выбранное соответствие верное: \"" + str_AnsR + "\".");
            } else {
                query.prepare("INSERT INTO results3 (Result)"
                              "VALUES (:Result);");
                query.bindValue(":Result", "Пункт №" + str_WordNum + ": \"" + str_Q + "\". "
                                + "Выбранное соответствие неверное: \"" + item->text() + "\". "
                                + "Правильный вариант: \"" + str_AnsR + "\".");
            }
            query.exec();

        }

        query.prepare("INSERT INTO count3 (Count3, Quant)"
                      "VALUES (:Count3, :Quant);");
        query.bindValue(":Count3",  str_Part3Name + ": \"" + str_Task3 + "\". Правильно выполнено " + QString::number(m_iAnsRightCount3)
                         + " из " + QString::number(count3R) +".");
        query.bindValue(":Quant",  count3R);
        query.exec();

    }

            if(count3R>0 && m_bPart3Started==false){
                query.prepare("INSERT INTO results3 (Result)"
                              "VALUES (:Result);");
                query.bindValue(":Result", "Задание было пропущено.");
                query.exec();
            }

    if(m_bHasPart1 == true){
        query.prepare("INSERT INTO count1 (Count1, Quant)"
                      "VALUES (:Count1, :Quant);");
        query.bindValue(":Count1", "ЧАСТЬ 1: \"" + str_Task + "\". Правильно выполнено " + QString::number(m_iAnsRightCount)
                         + " из " + QString::number(countR) +".");
        query.bindValue(":Quant",  countR);
        query.exec();
    }

    if(m_bHasPart2 == true){
        query.prepare("INSERT INTO count2 (Count2, Quant)"
                      "VALUES (:Count2, :Quant);");
        query.bindValue(":Count2", str_Part2Name + ": \"" + str_Task2 + "\". Правильно выполнено " + QString::number(m_iAnsRightCount2)
                         + " из " + QString::number(count2R) +".");
        query.bindValue(":Quant",  count2R);
        query.exec();
    }

    query.prepare("INSERT INTO counttotal (Counttotal)"
                  "VALUES (:Counttotal);");
    query.bindValue(":Counttotal", "ПРАВИЛЬНО ВЫПОЛНЕНО " + QString::number(m_iAnsRightCount+m_iAnsRightCount2+m_iAnsRightCount3)
                     + " ИЗ " + QString::number(countR+count2R+count3R) +".");
    query.exec();

    db_Test.close();

    Results results;
    results.GetTestPath(str_TestPathInEdit);
    results.exec();

    this->close();
}



void Test::on_listWidget_Quest_itemClicked()
{
//    ui->listWidget_Quest->selectionModel()->clear();
}
