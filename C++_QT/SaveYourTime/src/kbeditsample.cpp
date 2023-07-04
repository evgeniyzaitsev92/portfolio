#include "kbeditsample.h"
#include "ui_kbeditsample.h"

KBEditSample::KBEditSample(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KBEditSample)
{
    ui->setupUi(this);
    setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

  //  m_bEditSampleInProcess = true;
}

KBEditSample::~KBEditSample()
{
    delete ui;
}

// OK pressed
void KBEditSample::GetTextandID(QString text, int ID, int tab)
{
    ui->pTE_Sample->setPlainText(text);
    m_iIDsample = ID;
    m_iTabKB = tab;
}


// link sample
void KBEditSample::on_Btn_Link_clicked()
{
    if(ui->pTE_Sample->toPlainText() != ""){
        QTextCursor cursor(ui->pTE_Sample->textCursor());
        cursor.movePosition(QTextCursor::EndOfWord, QTextCursor::MoveAnchor, 1);
        cursor.insertText(" []()");
    } else {
        ui->pTE_Sample->setPlainText("[]()");
    }
}

// check sample in KB
void KBEditSample::on_pTE_Sample_textChanged()
{
    bool found = false;
    //int checkingID = 0;

    if(m_iTabKB == 0){ // online KB

        JsonOpenKBOnline();

        QStringList allrecords = mainObjectKBOnline.keys();
        int count = allrecords.count();
        for (int i=1; i<count+1; i++) {
            QString record = QString::number(i);
            QJsonObject ID = mainObjectKBOnline.value(record).toObject();
            QJsonValue checkSample = ID.value("Sample");

            if(ui->pTE_Sample->toPlainText() == checkSample.toString()){
             //   checkingID = i;
                found = true;
                break;
            }
        }

      //  if((m_iIDsample+1) != checkingID){
            if(found){
                ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
                ui->Lb_Exists->setVisible(true);
            } else {
                ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
                ui->Lb_Exists->setVisible(false);
            }
      //  }

    }
}

// OK pressed
void KBEditSample::on_buttonBox_accepted()
{
    if(m_iTabKB == 0){ // online KB

        JsonOpenKBOnline();

        QString str_NewSample = ui->pTE_Sample->toPlainText();
        QJsonValue IDValue = mainObjectKBOnline.value(QString::number(m_iIDsample+1));
        if (IDValue.type() == QJsonValue::Object) {
            QJsonObject IDObject = IDValue.toObject();
            IDObject["Sample"] = str_NewSample;
           // IDObject["CanEdit"] = "Yes";
            mainObjectKBOnline[QString::number(m_iIDsample+1)] = IDObject;
        }

        QJsonValue IDValue2 = mainObjectKBOnline.value(QString::number(m_iIDsample+1));
        if (IDValue2.type() == QJsonValue::Object) {
            QJsonObject IDObject = IDValue2.toObject();
            IDObject["CanEdit"] = "Yes";
            mainObjectKBOnline[QString::number(m_iIDsample+1)] = IDObject;
        }

        JsonCloseKBOnline();

    }
}

// Cancel
void KBEditSample::on_buttonBox_rejected()
{
  //  m_bEditSampleInProcess = false;

    if(m_iTabKB == 0){ // online KB
        JsonOpenKBOnline();

        QJsonValue IDValue = mainObjectKBOnline.value(QString::number(m_iIDsample+1));
        if (IDValue.type() == QJsonValue::Object) {
            QJsonObject IDObject = IDValue.toObject();
            IDObject["CanEdit"] = "Yes";
            mainObjectKBOnline[QString::number(m_iIDsample+1)] = IDObject;
        }

        JsonCloseKBOnline();
    }
}

// header
void KBEditSample::on_Btn_Header_clicked()
{
    QTextCursor cursor(ui->pTE_Sample->textCursor());
    cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor, 1);
    cursor.insertText("//** ");

    cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor, 1);
    cursor.insertText(" **\\\\");
}

// close
void KBEditSample::closeEvent (QCloseEvent *event)
{
    if(m_iTabKB == 0){ // online KB
        JsonOpenKBOnline();

        QJsonValue IDValue = mainObjectKBOnline.value(QString::number(m_iIDsample+1));
        if (IDValue.type() == QJsonValue::Object) {
            QJsonObject IDObject = IDValue.toObject();
            IDObject["CanEdit"] = "Yes";
            mainObjectKBOnline[QString::number(m_iIDsample+1)] = IDObject;
        }

        JsonCloseKBOnline();
    }

    event->accept();
}

// open json
void KBEditSample::JsonOpenKBOnline()
{
    kbonlinefile.setFileName("KnowledgeBaseOnline.json");
    kbonlinefile.open(QIODevice::ReadOnly | QIODevice::Text);
    QJsonParseError JsonParseError;
    JsonDocumentKBOnline = QJsonDocument::fromJson(kbonlinefile.readAll(), &JsonParseError);
    kbonlinefile.close();
    mainObjectKBOnline = JsonDocumentKBOnline.object();
}

// open json
void KBEditSample::JsonCloseKBOnline()
{
    JsonDocumentKBOnline.setObject(mainObjectKBOnline);
    kbonlinefile.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
    kbonlinefile.write(JsonDocumentKBOnline.toJson());
    kbonlinefile.close();
}
