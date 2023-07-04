#ifndef KBEDITSAMPLE_H
#define KBEDITSAMPLE_H

#include <QDialog>
#include <QMessageBox>
#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QCloseEvent>

namespace Ui {
class KBEditSample;
}

class KBEditSample : public QDialog
{
    Q_OBJECT

public:
    explicit KBEditSample(QWidget *parent = nullptr);
    ~KBEditSample();

    void GetTextandID(QString text, int ID, int tab);
    int m_iIDsample = 0;
    int m_iTabKB = 0;
    bool m_bEditSampleInProcess = false;

    QFile kbfile;
    QJsonDocument JsonDocumentKB;
    QJsonObject mainObjectKB;

    QFile kbonlinefile;
    QJsonDocument JsonDocumentKBOnline;
    QJsonObject mainObjectKBOnline;

private slots:
    void on_Btn_Link_clicked();
    void on_buttonBox_accepted();
    void on_Btn_Header_clicked();
    void on_buttonBox_rejected();
    void closeEvent(QCloseEvent *event) override;

    void JsonOpenKBOnline();
    void JsonCloseKBOnline();

    void on_pTE_Sample_textChanged();

private:
    Ui::KBEditSample *ui;
};

#endif // KBEDITSAMPLE_H
