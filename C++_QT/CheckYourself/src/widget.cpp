#include "widget.h"
#include "ui_widget.h"

#include <QListView>
#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->Button_OK->hide();
    ui->Button_Cancel->hide();
    ui->lineEdit->hide();
    /*
    model = new QFileSystemModel(this);
    model->setFilter(QDir::Files);
    model->setRootPath("Tests");
    model->setReadOnly(false);
    ui->listView->setModel(model);
    ui->listView->setRootIndex(model->index("Tests"));
    */

}

Widget::~Widget()
{
    delete ui;
}

//*****************//
// ADD TEST BUTTON //
//*****************//
// add item to qlistwidget
void Widget::on_Button_Add_Test_clicked()
{
    //   int ItemsCount = model->size();
  //  QModelIndex TestsFolder = model->index("Tests");
  //  int ItemsCount = model->rowCount(TestsFolder);
  //  QString str_ItemsCount = QString("Новый тест %1").arg(ItemsCount+1);
        //QString path = "Tests/" + str_ItemsCount + ".test";
    //QString fileName = QFileDialog::getOpenFileName(this, tr("Open Database"), "/home/yourhome", tr("SQLite Database Files (*.sqlite)"));

    str_TestPath = QFileDialog::getSaveFileName(this, "Создать тест", "Tests/.test", tr("Тесты (*.test)"));
    QSqlDatabase newDB = QSqlDatabase::addDatabase("QSQLITE");
    newDB.setDatabaseName(str_TestPath);
 //   newDB.setPassword("yes");
 //   newDB.setConnectOptions("QSQLITE_CREATE_KEY");
    newDB.open();
    if (!newDB.open()) {
          qDebug() << newDB.lastError().text();
    }
    QSqlQuery query;
 //   query.exec("PRAGMA key = 'y';");
    query.exec("CREATE TABLE part1 ("
               "`ID` INTEGER PRIMARY KEY AUTOINCREMENT, " // 0
              // "`type` INTEGER NOT NULL, "                // 1
               "`Quest` TEXT NOT NULL, "                  // 1
               "`Ans1` TEXT NOT NULL, "                   // 2
               "`Ans2` TEXT NOT NULL, "                   // 3
               "`Ans3` TEXT NOT NULL, "                   // 4
               "`Ans4` TEXT NOT NULL, "                   // 5
               "`AnsRight` INTEGER NOT NULL, "            // 6
               "`Quant` INTEGER NOT NULL "                // 7
               ");");

    query.exec("CREATE TABLE part2 ("
               "`ID` INTEGER PRIMARY KEY AUTOINCREMENT, " // 0
               "`Quest` TEXT NOT NULL, "                  // 1
               "`Word` TEXT NOT NULL, "                   // 2
               "`Word2` TEXT NOT NULL, "                  // 3
               "`Quant` INTEGER NOT NULL "                // 4
               ");");

    query.exec("CREATE TABLE part3 ("
               "`ID` INTEGER PRIMARY KEY AUTOINCREMENT, " // 0
               "`Quest` TEXT NOT NULL, "                  // 1
               "`Ans` TEXT NOT NULL, "                   // 2
               "`Wrong` TEXT NOT NULL "                // 3
               ");");

    query.exec("CREATE TABLE part1task ("
               "`ID` INTEGER PRIMARY KEY AUTOINCREMENT, " // 0
               "`Task` TEXT NOT NULL "                  // 1
               ");");

    query.exec("CREATE TABLE part2task ("
               "`ID` INTEGER PRIMARY KEY AUTOINCREMENT, " // 0
               "`Task` TEXT NOT NULL "                  // 1
               ");");

    query.exec("CREATE TABLE part3task ("
               "`ID` INTEGER PRIMARY KEY AUTOINCREMENT, " // 0
               "`Task` TEXT NOT NULL "                  // 1
               ");");

    query.exec("CREATE TABLE time ("
               "`ID` INTEGER PRIMARY KEY AUTOINCREMENT, " // 0
               "`Time` TEXT NOT NULL "                  // 1
               ");");

    query.exec("CREATE TABLE code ("
               "`ID` INTEGER PRIMARY KEY AUTOINCREMENT, " // 0
               "`code` TEXT NOT NULL "                // 1
               ");");

    query.exec("CREATE TABLE results1 ("
               "`ID` INTEGER PRIMARY KEY AUTOINCREMENT, " // 0
               "`Result` TEXT NOT NULL "                  // 1
               ");");

    query.exec("CREATE TABLE results2 ("
               "`ID` INTEGER PRIMARY KEY AUTOINCREMENT, " // 0
               "`Result` TEXT NOT NULL "                  // 1
               ");");

    query.exec("CREATE TABLE results3 ("
               "`ID` INTEGER PRIMARY KEY AUTOINCREMENT, " // 0
               "`Result` TEXT NOT NULL "                  // 1
               ");");

    query.exec("CREATE TABLE counttotal ("
               "`ID` INTEGER PRIMARY KEY AUTOINCREMENT, " // 0
               "`Counttotal` TEXT NOT NULL "                   // 2
               ");");

    query.exec("CREATE TABLE count1 ("
               "`ID` INTEGER PRIMARY KEY AUTOINCREMENT, " // 0
               "`Count1` TEXT NOT NULL, "                   // 2
               "`Quant` INTEGER NOT NULL "                // 4
               ");");

    query.exec("CREATE TABLE count2 ("
               "`ID` INTEGER PRIMARY KEY AUTOINCREMENT, " // 0
               "`Count2` TEXT NOT NULL, "                   // 2
               "`Quant` INTEGER NOT NULL "                // 4
               ");");

    query.exec("CREATE TABLE count3 ("
               "`ID` INTEGER PRIMARY KEY AUTOINCREMENT, " // 0
               "`Count3` TEXT NOT NULL, "                   // 2
               "`Quant` INTEGER NOT NULL "                // 4
               ");");

    query.exec("CREATE TABLE testname ("
               "`ID` INTEGER PRIMARY KEY AUTOINCREMENT, " // 0
               "`Testname` TEXT NOT NULL "                // 1
               ");");

    query.exec("CREATE TABLE email ("
               "`ID` INTEGER PRIMARY KEY AUTOINCREMENT, " // 0
               "`Email` TEXT NOT NULL "                // 1
               ");");

    newDB.close();

    // test path to text edit
    QFile testfile(str_TestPath);
    QFileInfo fileInfo(testfile.fileName());
    QString str_TestNameforTextEdit = fileInfo.fileName();
    ui->textBrowser->setText(str_TestNameforTextEdit);
}

//********************//
// SELECT TEST BUTTON //
//********************//
void Widget::on_Button_Select_clicked()
{
    str_TestPath = QFileDialog::getOpenFileName(this, tr("Выбрать тест"), "Tests/.test", tr("Тесты (*.test)"));
    QFile testfile(str_TestPath);
    QFileInfo fileInfo(testfile.fileName());
    QString str_TestNameforTextEdit = fileInfo.fileName();
    ui->textBrowser->setText(str_TestNameforTextEdit);
    PasswordEntered = false;
}

//********************//
// REMOVE TEST BUTTON //
//********************//
void Widget::on_Button_Remove_clicked()
{
    str_TestPath = "";
    ui->textBrowser->setText("");

}

//******************//
// EDIT TEST BUTTON //
//******************//
void Widget::on_Button_Edit_clicked()
{
    if(str_TestPath == ""){
        NoTestMSG();
    } else {
        if(PasswordEntered == false){
            db_Test = QSqlDatabase::addDatabase("QSQLITE");
            db_Test.setDatabaseName(str_TestPath);
            db_Test.open();
            QSqlQuery query;
            query.prepare("SELECT COUNT(*) FROM code;");
            query.exec();
            int count = 0;
           if (query.next()) {
                count = query.value(0).toInt();
            }

           query.prepare("SELECT * FROM code;");
           query.exec();
           if (query.seek(0)){
               str_Password = query.value(1).toString(); // get password
           }

           if(count == 0){
               Edit edit;
               edit.GetTestPath(str_TestPath);
               edit.exec();
           } else {
               bool bOk;
               QString str_Enterpassword = QInputDialog::getText( 0, "Внимание!", "Введите пароль:", QLineEdit::Password, "", &bOk);
               if (!bOk){
                   // Cancel
                //   on_Button_Remove_clicked();
               } else {
                   if(str_Enterpassword == str_Password && str_Enterpassword != ""){
                       Edit edit;
                       edit.GetTestPath(str_TestPath);
                       edit.exec();
                       PasswordEntered = true;
                   } else if(str_Enterpassword != str_Password || str_Enterpassword == ""){
                       on_Button_Edit_clicked();
                   }
               }
           }
        } else {
            Edit edit;
            edit.GetTestPath(str_TestPath);
            edit.exec();
        }

    }
}

//********************//
// RENAME TEST BUTTON //
//********************//
void Widget::on_Button_Rename_clicked()
{
    if(str_TestPath == ""){
        NoTestMSG();
    } else {
        ui->Button_Add_Test->setEnabled(false);
        ui->Button_Select->setEnabled(false);
        ui->Button_Remove->setEnabled(false);
        ui->Button_Edit->setEnabled(false);
        ui->Button_Del->setEnabled(false);
        ui->Button_Start->setEnabled(false);

        ui->lineEdit->show();
        ui->textBrowser->hide();

        ui->Button_OK->show();
        ui->Button_Cancel->show();
        ui->Button_Rename->hide();

        QFile testfile(str_TestPath);
        QFileInfo fileInfo(testfile.fileName());
        QString str_TestNameforTextEdit = fileInfo.fileName();
        str_TestNameforTextEdit.replace(QString(".test"), QString(""));
        ui->lineEdit->setText(str_TestNameforTextEdit);

            //file.rename(fileInfo.absolutePath() + "/" + newName);
    }
}

// set new name
void Widget::on_lineEdit_textChanged(/*const QString &arg1*/)
{
    QString str_NewTestPath = "Tests/" + ui->lineEdit->text() + ".test";
    bool fileExists = QFileInfo::exists(str_NewTestPath) && QFileInfo(str_NewTestPath).isFile();

    if (ui->lineEdit->text() == "" || fileExists == true) {
        ui->Button_OK->setEnabled(false);
    } else {
        ui->Button_OK->setEnabled(true);
    }
}

// click OK
void Widget::on_Button_OK_clicked()
{
    QFile file(str_TestPath);
    file.rename("Tests/" + ui->lineEdit->text() + ".test");
    QFileInfo newfileInfo(file.fileName());
    QString str_NewTestNameforTextEdit = newfileInfo.fileName();
    QString str_NewTestPath = newfileInfo.absoluteFilePath();

    ui->Button_Add_Test->setEnabled(true);
    ui->Button_Select->setEnabled(true);
    ui->Button_Remove->setEnabled(true);
    ui->Button_Edit->setEnabled(true);
    ui->Button_Del->setEnabled(true);
    ui->Button_Start->setEnabled(true);

    ui->lineEdit->hide();
    ui->textBrowser->show();

    ui->Button_OK->hide();
    ui->Button_Cancel->hide();
    ui->Button_Rename->show();

    ui->textBrowser->setText(str_NewTestNameforTextEdit);
    str_TestPath = str_NewTestPath;
}

// click Cancel
void Widget::on_Button_Cancel_clicked()
{
    ui->Button_Add_Test->setEnabled(true);
    ui->Button_Select->setEnabled(true);
    ui->Button_Remove->setEnabled(true);
    ui->Button_Edit->setEnabled(true);
    ui->Button_Del->setEnabled(true);
    ui->Button_Start->setEnabled(true);

    ui->lineEdit->hide();
    ui->textBrowser->show();

    ui->Button_OK->hide();
    ui->Button_Cancel->hide();
    ui->Button_Rename->show();
}

/*
// set new name
void Widget::ItemRenameTest(const QString newname){
//  ui->listWidget->currentItem()->setText(newname);
}
*/

// send old name
/*
void Widget::on_Button_Rename_Test_clicked()
{
    QModelIndex index = ui->listView->currentIndex();
    QFileInfo fileInfo = model->fileInfo(index);
    QString name = model->fileName(index);
    QFile file(name);

    QString newName = "123";

    file.rename(fileInfo.absolutePath() + "/" + newName);



    if(ui->listView->currentIndex()){
        DialogRename dialogRename;
    //    QString CurItemName = ui->listWidget->currentItem()->text();
    //    dialogRename.GetTestName(CurItemName);
        connect( &dialogRename, SIGNAL(RenameTest(const QString)), this, SLOT(ItemRenameTest(const QString)));
        dialogRename.exec();
    }

}
*/

//************//
// DEL BUTTON //
//************//
void Widget::on_Button_Del_clicked()
{
    if(str_TestPath == ""){
        NoTestMSG();
    } else {
        QFile file(str_TestPath);
        file.remove();
        on_Button_Remove_clicked();
    }
    /*
    QModelIndex index = ui->listView->currentIndex();
    QFileInfo fileInfo = model->fileInfo(index);
    QString pathandname = fileInfo.absoluteFilePath();
    QFile file(pathandname);
    file.remove();
    */

}

//**************//
// START BUTTON //
//**************//
void Widget::on_Button_Start_clicked()
{
    if(str_TestPath == ""){
        NoTestMSG();
    } else {
        Test test;
        test.GetTestPath(str_TestPath);
        test.exec();
    }
}


//*************//
// EXIT BUTTON //
//*************//
void Widget::on_Button_Exit_clicked()
{
    this->close();
}

//*************//
// NO TEST MSG //
//*************//
void Widget::NoTestMSG()
{
        QMessageBox msgBox;
        QPushButton *OKButton = msgBox.addButton(QMessageBox::Ok);
        OKButton->setStyleSheet("QPushButton {"
                                "width: 155px; height: 38px; font-family: MS Shell Dlg 2; font-style: normal; font-size: 12pt; font-weight: bold;"
                                "border-radius: 20px;"
                                "border: 3px solid rgb(0, 135, 0); "
                                "background-color: qlineargradient(spread:pad, x1:1, y1:0, x2:1, y2:1, stop:0 rgba(0, 180, 0, 255), stop:1 rgba(152, 255, 152, 255));"
                                "color: rgb(25, 25, 25);}"

                                "QPushButton:hover {"
                                "border-radius: 20px;"
                                "border: 3px solid rgb(0, 135, 0); 	"
                                "background-color: qlineargradient(spread:pad, x1:1, y1:0, x2:1, y2:1, stop:0 rgba(36, 232, 36, 255), stop:1 rgba(236, 250, 236, 255));"
                                "color: rgb(25, 25, 25);}"

                                "QPushButton:pressed {"
                                "border-radius: 20px;"
                                "border: 3px solid rgb(0, 135, 0); "
                                "background-color: qlineargradient(spread:pad, x1:1, y1:0, x2:1, y2:1, stop:0 rgba(236, 250, 236, 255), stop:1 rgba(36, 232, 36, 255));"
                                "color: rgb(25, 25, 25);}");

        msgBox.setWindowTitle("Внимание!");
        msgBox.setText("Тест не выбран!");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStyleSheet("QMessageBox { font-family: MS Shell Dlg 2; font-style: normal; font-size: 16pt; font-color: transparent;"
                             "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(36, 113, 190, 255), stop:1 rgba(243, 243, 243, 255));"
                             "}"
                             "QMessageBox QLabel {background-color: transparent; }");
        msgBox.exec();
}



//*********//
// RESULTS //
//*********//
void Widget::on_Button_Results_clicked()
{
    if(str_TestPath == ""){
        NoTestMSG();
    } else {
        Results results;
        results.GetTestPath(str_TestPath);
        results.exec();
    }
}

//*********//
// LICENSE //
//*********//
void Widget::on_Button_License_clicked()
{
    License license;
    license.exec();
}
