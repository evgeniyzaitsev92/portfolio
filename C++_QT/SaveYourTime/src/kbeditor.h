#ifndef KBEDITOR_H
#define KBEDITOR_H

#include <QDialog>
#include <QListWidgetItem>
#include <QFileSystemModel>
#include <QDir>
#include <QDebug>
#include <QListView>
#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QSql>
#include <QtSql/QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>
#include <QWidget>
#include <QInputDialog>
#include <QClipboard>
#include <QCloseEvent>
#include <QTableWidget>
#include <QHeaderView>
#include <QTextEdit>
#include <QRadioButton>
#include <QRegExp>


namespace Ui {
class KBEditor;
}

class KBEditor : public QDialog
{
    Q_OBJECT

public:
    explicit KBEditor(QWidget *parent = nullptr);
    ~KBEditor();

   // QSqlDatabase kbDB;
    QSqlDatabase kbDB = QSqlDatabase::addDatabase("QSQLITE", "kbDB");
    QSqlDatabase knowledgeDB;
    QString str_kbDBPath = "";
    QString RandomNumber(QString &randomString);

private slots:
    void on_Btn_LoadKB_clicked();
    void on_Btn_CreateKB_clicked();
    void on_Btn_AddSample_clicked();
    void closeEvent(QCloseEvent *event) override;
    void on_Btn_DeleteSample_clicked();
    void on_Lw_KB_currentTextChanged(const QString &currentText);
    void on_Btn_SaveSample_clicked();
    void on_Btn_SendSamples_clicked();
    void CheckinMainKB();
    void ReorderKBdb();
    void on_Btn_Header_clicked();
    void on_Btn_LoadAllFromMainKB_clicked();
    void SaveLoadedFromMainKB();

private:
    Ui::KBEditor *ui;

    int m_iKBRowTotal = 0;

signals:
    void RefreshMainKB();

};

#endif // KBEDITOR_H
