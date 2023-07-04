#include "kbeditor.h"
#include "ui_kbeditor.h"

KBEditor::KBEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KBEditor)
{
    ui->setupUi(this);
    setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    ui->Lw_KB->setSpacing(2);
    ui->Lw_KB->setSelectionMode(QAbstractItemView::ExtendedSelection);
    connect(ui->Lw_KB->model(), SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)), this, SLOT(ReorderKBdb()));
}

KBEditor::~KBEditor()
{
    delete ui;
}

// close
void KBEditor::closeEvent (QCloseEvent *event)
{
    QString connection;
    connection = kbDB.connectionName();
    kbDB.close();
    kbDB = QSqlDatabase();
    kbDB.removeDatabase(connection);
    event->accept();
}

// create KB
void KBEditor::on_Btn_CreateKB_clicked()
{
    QString str_KBDefaultName = "KB";
    str_kbDBPath = QFileDialog::getSaveFileName(this, "Create a knowledge base", "KBs/" +str_KBDefaultName+ ".db", tr("KB DB (*.db)"));

    if(!str_kbDBPath.isEmpty() && !str_kbDBPath.isNull()){
        ui->Le_KBName->setEnabled(true);

        QFile testfile(str_kbDBPath);
        QFileInfo fileInfo(testfile.fileName());
        QString str_KBNameforLineEdit = fileInfo.fileName();
        ui->Le_KBName->setText(str_KBNameforLineEdit);

        ui->Btn_AddSample->setEnabled(true);
        ui->Btn_DeleteSample->setEnabled(true);
        ui->Btn_SaveSample->setEnabled(true);
        ui->Btn_SendSamples->setEnabled(true);
        m_iKBRowTotal = 0;
        ui->Lw_KB->clear();
    } else {
        return;
    }

    kbDB = QSqlDatabase::database("kbDB");
    kbDB.setDatabaseName(str_kbDBPath);
    kbDB.open();
    if (!kbDB.open()) {
          qDebug() << kbDB.lastError().text();
    }

    kbDB = QSqlDatabase::database("kbDB");
    QSqlQuery query(kbDB);

    query.exec("DELETE FROM 'Knowledge';");
    query.exec("DELETE FROM sqlite_sequence WHERE name = 'Knowledge';");

    query.exec("CREATE TABLE Knowledge ("
               "`ID` INTEGER PRIMARY KEY AUTOINCREMENT, " // 0
               "`Number` TEXT NOT NULL, "                 // 1
               "`Sample` TEXT NOT NULL "                  // 2
               ");");
}

// load KB
void KBEditor::on_Btn_LoadKB_clicked()
{
    str_kbDBPath = QFileDialog::getOpenFileName(this, tr("Load samples from DB"), "KBs/.db", tr("KB (*.db)"));

    if(!str_kbDBPath.isEmpty() && !str_kbDBPath.isNull()){
        ui->Le_KBName->setEnabled(true);

        QFile testfile(str_kbDBPath);
        QFileInfo fileInfo(testfile.fileName());
        QString str_KBNameforLineEdit = fileInfo.fileName();
        ui->Le_KBName->setText(str_KBNameforLineEdit);

        ui->Btn_AddSample->setEnabled(true);
        ui->Btn_DeleteSample->setEnabled(true);
        ui->Btn_SaveSample->setEnabled(true);
        ui->Btn_SendSamples->setEnabled(true);
        m_iKBRowTotal = 0;
        ui->Lw_KB->clear();
    } else {
        return;
    }

    kbDB = QSqlDatabase::database("kbDB");
    kbDB.setDatabaseName(str_kbDBPath);
    kbDB.open();

    QString str_Sample;
    m_iKBRowTotal = 0;

    kbDB = QSqlDatabase::database("kbDB");
    QSqlQuery query(kbDB);

    query.prepare("SELECT COUNT(*) FROM 'Knowledge';");
    query.exec();

    int count = 0;
    if (query.next()) {
        count = query.value(0).toInt();
    }

    for (int i=0; i<count; i++) {
        query.prepare("SELECT * FROM Knowledge;");
        query.exec();
        if (query.seek(i)){
            str_Sample = query.value(2).toString();
        }
        m_iKBRowTotal++;
        ui->Lw_KB->addItem(str_Sample);
        ui->Lw_KB->item(m_iKBRowTotal-1)->setBackground(Qt::lightGray);
        ui->Lw_KB->item(m_iKBRowTotal-1)->setTextAlignment(Qt::AlignBottom);

        // Heading
        QString checkforHeading = ui->Lw_KB->item(m_iKBRowTotal-1)->text();
        bool yes = checkforHeading.contains("//**");
        if(yes){
            ui->Lw_KB->item(m_iKBRowTotal-1)->setBackground(Qt::darkGreen);
            ui->Lw_KB->item(m_iKBRowTotal-1)->setForeground(Qt::white);
            QFont fnt = ui->Lw_KB->item(m_iKBRowTotal-1)->font();
            fnt.setWeight(QFont::Bold);
            fnt.setItalic(true);
            fnt.setPointSize(14);
            ui->Lw_KB->item(m_iKBRowTotal-1)->setFont(fnt);
        }
    }
    CheckinMainKB();
}

// load all from main kb
void KBEditor::on_Btn_LoadAllFromMainKB_clicked()
{
    on_Btn_CreateKB_clicked();

    ui->Lw_KB->clear();
    QString str_Sample;
    m_iKBRowTotal = 0;

    knowledgeDB = QSqlDatabase::database("knowledgeDB");
    QSqlQuery query(knowledgeDB);

    query.prepare("SELECT COUNT(*) FROM 'Knowledge';");
    query.exec();

    int count = 0;
    if (query.next()) {
        count = query.value(0).toInt();
    }

    for (int i=0; i<count; i++) {
        query.prepare("SELECT * FROM Knowledge;");
        query.exec();
        if (query.seek(i)){
            str_Sample = query.value(2).toString();
        }
        m_iKBRowTotal++;
        ui->Lw_KB->addItem(str_Sample);
    }

    SaveLoadedFromMainKB();
}

// send sample to main kb
void KBEditor::on_Btn_SendSamples_clicked()
{
    if(ui->Lw_KB->currentItem()){
        bool found = false;
        QString str_Sample = "";

        knowledgeDB = QSqlDatabase::database("knowledgeDB");
        QSqlQuery query(knowledgeDB);

        query.prepare("SELECT COUNT(*) FROM 'Knowledge';");
        query.exec();

        int count = 0;
        if (query.next()) {
            count = query.value(0).toInt();
        }

        for (int i=0; i<count; i++) {
            query.prepare("SELECT * FROM Knowledge;");
            query.exec();
            if (query.seek(i)){
                str_Sample = query.value(2).toString();
            }
            if(str_Sample == ui->Lw_KB->currentItem()->text()){
                found = true;
            }
        }

        if(found){
            QMessageBox::warning(this, "Warning!","This sample already exists!");
        } else {
            knowledgeDB = QSqlDatabase::database("knowledgeDB");
            QSqlQuery query(knowledgeDB);

            QString setRandomNumber = "";
            RandomNumber(setRandomNumber);

            query.prepare("INSERT INTO Knowledge (Number, Sample)"
                          "VALUES (:Number, :Sample);");
            query.bindValue(":Number", setRandomNumber);
            query.bindValue(":Sample", ui->Lw_KB->currentItem()->text());
            query.exec();

            CheckinMainKB();
        }
    }
}

// add sample
void KBEditor::on_Btn_AddSample_clicked()
{
    m_iKBRowTotal++;
    ui->Lw_KB->addItem("");
    ui->Lw_KB->item(m_iKBRowTotal-1)->setBackground(Qt::lightGray);
    ui->Lw_KB->item(m_iKBRowTotal-1)->setTextAlignment(Qt::AlignBottom);

    QString kbrowTotal = QString::number(m_iKBRowTotal);

    kbDB = QSqlDatabase::database("kbDB");
    QSqlQuery query(kbDB);

    query.prepare("SELECT COUNT(*) FROM 'Knowledge';");
    query.exec();

    QString setRandomNumber = "";
    RandomNumber(setRandomNumber);

    query.prepare("INSERT INTO Knowledge (Number, Sample)"
                  "VALUES (:Number, :Sample);");
    query.bindValue(":Number", setRandomNumber);
    query.bindValue(":Sample", "");
    query.exec();
}

// delete selected
void KBEditor::on_Btn_DeleteSample_clicked()
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "Deletion",
                                                                tr("Delete?\n"),
                                                                QMessageBox::No | QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        return;
    } else {
        if(ui->Lw_KB->selectedItems().count() > 1){
            QList<QListWidgetItem*> items = ui->Lw_KB->selectedItems();
            foreach(QListWidgetItem * item, items)
            {
                delete ui->Lw_KB->takeItem(ui->Lw_KB->row(item));
            }
            ReorderKBdb();

        } else if(ui->Lw_KB->selectedItems().count() == 1){
            if(ui->Lw_KB->currentItem()){
                m_iKBRowTotal--;
                int totalRows = ui->Lw_KB->count();
                QString newtotalRow = QString::number(totalRows-1);
                int currentRow = ui->Lw_KB->currentIndex().row();
                QString curRow = QString::number(currentRow+1);
                QString str_ident = "";
                delete ui->Lw_KB->currentItem();

                kbDB = QSqlDatabase::database("kbDB");
                QSqlQuery query(kbDB);
                query.prepare("DELETE FROM Knowledge WHERE ID = " + curRow);
                query.exec();
                query.exec("UPDATE sqlite_sequence SET seq = " +newtotalRow+" WHERE name = 'Knowledge'");

                for(int index = 0; index < totalRows-1; index++) {
                    query.prepare("SELECT * FROM Knowledge;");
                    query.exec();
                    if (query.seek(index)){
                        str_ident = query.value(1).toString();
                    }
                    QString str_ID = QString::number(index+1);
                    query.prepare("UPDATE Knowledge SET ID = " + str_ID + " WHERE Number = '" + str_ident+"'");
                    query.exec();
                }
            }
        }
    }
}

// save sample
void KBEditor::on_Btn_SaveSample_clicked()
{
    bool found = false;
    for (int i = 0; i < ui->Lw_KB->count(); ++i) {
        if (ui->Lw_KB->item(i)->text() == ui->pTE_EditSample->toPlainText()) {
            found = true;
            break;
        }
    }

    if(found){
        QMessageBox::warning(this, "Warning!","This sample already exists!");
    } else {
        if(ui->Lw_KB->currentItem()){
            ui->Lw_KB->currentItem()->setBackground(Qt::lightGray);
             ui->Lw_KB->currentItem()->setForeground(Qt::black);
             QFont fnt = ui->Lw_KB->currentItem()->font();
             fnt.setWeight(QFont::Normal);
             fnt.setItalic(false);
             fnt.setPointSize(9);
             ui->Lw_KB->currentItem()->setFont(fnt);

             ui->Lw_KB->currentItem()->setText(ui->pTE_EditSample->toPlainText());

             // Heading
             QString checkforHeading = ui->Lw_KB->currentItem()->text();
             bool yes = checkforHeading.contains("//**");
             if(yes){
                 ui->Lw_KB->currentItem()->setBackground(Qt::darkGreen);
                 ui->Lw_KB->currentItem()->setForeground(Qt::white);
                 QFont fnt = ui->Lw_KB->currentItem()->font();
                 fnt.setWeight(QFont::Bold);
                 fnt.setItalic(true);
                 fnt.setPointSize(14);
                 ui->Lw_KB->currentItem()->setFont(fnt);
             }


             kbDB = QSqlDatabase::database("kbDB");
             QSqlQuery query(kbDB);

             QString str_ID = QString::number(ui->Lw_KB->currentRow()+1);
             QString str_NewSample = ui->pTE_EditSample->toPlainText();

             query.prepare("UPDATE Knowledge SET Sample=:Sample WHERE ID=:ID");
             query.bindValue(":Sample", str_NewSample);
             query.bindValue(":ID", str_ID);
             query.exec();

             CheckinMainKB();
         }
    }
}

// random number for kb
QString KBEditor::RandomNumber(QString &randomString)
{
    const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
    const int randomStringLength = 12; // assuming you want random strings of 12 characters

       for(int i=0; i<randomStringLength; ++i)
       {
           int index = qrand() % possibleCharacters.length();
           QChar nextChar = possibleCharacters.at(index);
           randomString.append(nextChar);
       }

       return randomString;
}

// reorder kb db
void KBEditor::ReorderKBdb()
{
    kbDB = QSqlDatabase::database("kbDB");
    QSqlQuery query(kbDB);

    query.exec("DELETE FROM 'Knowledge';");
    query.exec("DELETE FROM sqlite_sequence WHERE name = 'Knowledge';");

    int ItemsCount = ui->Lw_KB->model()->rowCount();
    for (int index = 0; index < ItemsCount; index++) {
        QString str_NewSample = ui->Lw_KB->item(index)->text();
        QString setRandomNumber = "";
        RandomNumber(setRandomNumber);
        query.prepare("INSERT INTO Knowledge (Number, Sample)"
                      "VALUES (:Number, :Sample);");
        query.bindValue(":Number", setRandomNumber);
        query.bindValue(":Sample", str_NewSample);
        query.exec();
    }
}

// choose sample for editing
void KBEditor::on_Lw_KB_currentTextChanged(const QString &currentText)
{
    ui->pTE_EditSample->setPlainText(currentText);
}

// add header
void KBEditor::on_Btn_Header_clicked()
{
    QTextCursor cursor(ui->pTE_EditSample->textCursor());
    cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor, 1);
    cursor.insertText("//** ");

    cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor, 1);
    cursor.insertText(" **\\\\");
}

// check in main kb
void KBEditor::CheckinMainKB()
{
    QString str_Sample = "";
    knowledgeDB = QSqlDatabase::database("knowledgeDB");
    QSqlQuery query(knowledgeDB);

    for (int y = 0; y < ui->Lw_KB->count(); ++y) {
        query.prepare("SELECT COUNT(*) FROM 'Knowledge';");
        query.exec();

        int count = 0;
        if (query.next()) {
            count = query.value(0).toInt();
        }

        for (int i=0; i<count; i++) {
            query.prepare("SELECT * FROM Knowledge;");
            query.exec();
            if (query.seek(i)){
                str_Sample = query.value(2).toString();
            }
            if(str_Sample == ui->Lw_KB->item(y)->text()){
                ui->Lw_KB->item(y)->setBackground(Qt::lightGray);
                ui->Lw_KB->item(y)->setForeground(Qt::red);
                QFont fnt = ui->Lw_KB->item(y)->font();
                fnt.setWeight(QFont::Bold);
                ui->Lw_KB->item(y)->setFont(fnt);

                // Heading
                QString checkforHeading = ui->Lw_KB->item(y)->text();
                bool yes = checkforHeading.contains("//**");
                if(yes){
                    ui->Lw_KB->item(y)->setBackground(Qt::yellow);
                    ui->Lw_KB->item(y)->setForeground(Qt::red);
                    QFont fnt = ui->Lw_KB->item(y)->font();
                    fnt.setWeight(QFont::Bold);
                    fnt.setItalic(true);
                    fnt.setPointSize(14);
                    ui->Lw_KB->item(y)->setFont(fnt);
                }
            }
        }
    }
}

// save loaded from mian kb
void KBEditor::SaveLoadedFromMainKB()
{

    CheckinMainKB();

    kbDB = QSqlDatabase::database("kbDB");
    QSqlQuery query(kbDB);

    query.exec("DELETE FROM 'Knowledge';");
    query.exec("DELETE FROM sqlite_sequence WHERE name = 'Knowledge';");

    int ItemsCount = ui->Lw_KB->model()->rowCount();
    for (int index = 0; index < ItemsCount; index++) {
        QString str_NewSample = ui->Lw_KB->item(index)->text();
        QString setRandomNumber = "";
        RandomNumber(setRandomNumber);
        query.prepare("INSERT INTO Knowledge (Number, Sample)"
                      "VALUES (:Number, :Sample);");
        query.bindValue(":Number", setRandomNumber);
        query.bindValue(":Sample", str_NewSample);
        query.exec();
    }


}
