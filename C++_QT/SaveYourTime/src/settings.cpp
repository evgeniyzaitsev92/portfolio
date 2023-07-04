#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
    setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    QString str_Greeting;
    QString str_Signature;

    JsonOpen();
    QJsonValue jv = mainObject.value("MessageCompiler");
                QJsonArray ja = jv.toArray();
                  QJsonObject Greeting = ja.at(0).toObject();
                   QJsonValue GreetingValue = Greeting.value("Greeting");
                    str_Greeting = GreetingValue.toString();

                  QJsonObject Signature = ja.at(1).toObject();
                   QJsonValue SignatureValue = Signature.value("Signature");
                    str_Signature = SignatureValue.toString();

    ui->Le_Greeting->setText(str_Greeting);
    ui->pTE_Sign->setPlainText(str_Signature);
}

Settings::~Settings()
{
    delete ui;
}

void Settings::on_buttonBox_accepted()
{
    JsonOpen();

    QJsonValue MessageCompilerValue = mainObject.value("MessageCompiler");
    if (MessageCompilerValue.type() == QJsonValue::Array) {
        QJsonArray MessageCompilerArray = MessageCompilerValue.toArray();

        QJsonObject Greeting = MessageCompilerArray[0].toObject();
        Greeting["Greeting"] = ui->Le_Greeting->text();
        QJsonObject Signature = MessageCompilerArray[1].toObject();
        Signature["Signature"] = ui->pTE_Sign->toPlainText();

        // Replace overlay
        MessageCompilerArray.replace(0, Greeting);
        MessageCompilerArray.replace(1, Signature);

        mainObject["MessageCompiler"] = MessageCompilerArray;
    }

    JsonClose();
}

// open json
void Settings::JsonOpen()
{
    jsonSettings.setFileName("Settings.json");
    jsonSettings.open(QIODevice::ReadOnly | QIODevice::Text);
    QJsonParseError JsonParseError;
    JsonDocument = QJsonDocument::fromJson(jsonSettings.readAll(), &JsonParseError);
    jsonSettings.close();
    mainObject = JsonDocument.object();
}

// close json
void Settings::JsonClose()
{
    JsonDocument.setObject(mainObject);
    jsonSettings.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
    jsonSettings.write(JsonDocument.toJson());
    jsonSettings.close();
}
