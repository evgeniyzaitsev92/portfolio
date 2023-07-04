#include "kbjsonfileedit.h"
#include "ui_kbjsonfileedit.h"

KBJsonFileEdit::KBJsonFileEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KBJsonFileEdit)
{
    ui->setupUi(this);
    setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    // Создаём и инициализируем подсветку поиска
    m_searchHighLight = new SearchHighLight(ui->pTE_Json->document());
    JsonKBFileOpen();
}

KBJsonFileEdit::~KBJsonFileEdit()
{
    delete ui;
}

// open kb json file
void KBJsonFileEdit::JsonKBFileOpen()
{
    QFile jsonKBOnline("KnowledgeBaseOnline.json");
    jsonKBOnline.open(QIODevice::ReadOnly | QIODevice::Text);
    QJsonParseError JsonParseError;
    QJsonDocument JsonDocument = QJsonDocument::fromJson(jsonKBOnline.readAll(), &JsonParseError);
    jsonKBOnline.close();
    QJsonObject mainObject = JsonDocument.object();

    QJsonDocument text(mainObject);
    QByteArray ba = text.toJson();
    QString JsonToText = QString(ba);

    ui->pTE_Json->setPlainText(JsonToText);
}

// save kb json file
void KBJsonFileEdit::on_Btn_Save_clicked()
{
    QString TextToJson = ui->pTE_Json->toPlainText();

    QJsonDocument JsonDocument = QJsonDocument::fromJson(TextToJson.toUtf8());
    QFile jsonKBOnline("KnowledgeBaseOnline.json");
    jsonKBOnline.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
    jsonKBOnline.write(JsonDocument.toJson());
    jsonKBOnline.close();
}

// search
void KBJsonFileEdit::on_Le_Search_textChanged(const QString &arg1)
{
    // Загружаем текст для поиска в подсветку синтаксиса
        m_searchHighLight->searchText(arg1);
}
