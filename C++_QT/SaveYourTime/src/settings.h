#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>

#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();

    QFile jsonSettings;
    QJsonDocument JsonDocument;
    QJsonObject mainObject;

private slots:
    void on_buttonBox_accepted();
    void JsonOpen();
    void JsonClose();

private:
    Ui::Settings *ui;
};

#endif // SETTINGS_H
