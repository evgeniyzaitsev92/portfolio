#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "taskitem.h"
#include "settings.h"
#include "kbeditsample.h"
#include "kbjsonfileedit.h"
#include "searchhighlight.h"

#include <QMainWindow>
#include <QListWidgetItem>
#include <QFileSystemModel>
#include <QDir>
#include <QDebug>
#include <QListView>
#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>
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
#include <QCheckBox>
#include <QWidget>
#include <QTextStream>

#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

#include <QNetworkAccessManager>
#include <QUrlQuery>
#include <QNetworkReply>
#include <QUrl>

class SearchHighLight;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Ui::MainWindow *ui;

    QString str_DayDBPath;
    QString str_DateName = "";
    QString str_DayDBPathOld = "";
    QString str_SpentTimeCalls;
    QString str_SpentTimeTotal;
    QString timeStr;
    QString str_KB = "";
    QString str_KBOnlineCurrentItem = "";
    QStringList KBOnlineCannotEdit;

    QListWidgetItem *KBOnlineCurrentItem;
    QListWidgetItem *KBOnlinePreviousItem;


    QTime pauseTime;
    QTimer *pTimer;
    QTime baseTime;

    QJsonObject mainObjectCopy;

    int m_iTaskItemId = 0;
    int m_iTotalSpentTime = 0;
    int m_iTotalCalls = 0;
    int m_iDeltaCalls = 0;
    int m_iDeltaOldCalls = 0;
    int m_iDeltaPar = 0;
    int m_iKBRowTotal = 0;
    int m_iKBCurrentRow = 0;
    int m_iIndexStarted = -1;
    int m_iKBOnlineRowTotal = 0;
    int m_iKBOnlineCurrentItem = 0;

    bool m_bDelTask = false;
    bool m_bLoadDayClicked = false;
    bool m_bCallStarted = false;
    bool m_bLoadKBOnline = false;
    bool m_bAddDayClicked = false;

    QFile dayfile;
    QJsonDocument JsonDocumentDay;
    QJsonObject mainObjectDay;

    QFile kbfile;
    QJsonDocument JsonDocumentKB;
    QJsonObject mainObjectKB;

    QFile kbonlinefile;
    QJsonDocument JsonDocumentKBOnline;
    QJsonObject mainObjectKBOnline;

    QFile notesfile;

public slots:
    void LoadKBOnline(QNetworkReply *reply);

    void JsonOpenDay();
    void JsonOpenDayOld();
    void JsonCloseDay();

    void JsonOpenKBOnline(QNetworkReply *reply);
    void JsonCloseKBOnline();

private slots:

    void on_Btn_AddWorkingDay_clicked();
    void on_Btn_AddTask_clicked();
    void on_Btn_LoadWorkingDay_clicked();
    void on_Btn_Start_clicked();
    void on_Btn_Pause_clicked();
    void on_Btn_Reset_clicked();
    void on_Btn_Link_clicked();
    void on_Btn_Settings_clicked();
    void on_Btn_Renew_clicked();
    void on_Btn_CopyText_clicked();
    void DeleteItem();
    void ResetTimer();
    void showContextMenu(const QPoint&);
    void showContextMenuKBOnline(const QPoint&);
    void EditKBSample();
    void Save();
    void UpdateTotalTime();
    void UpdateTotalTimeinJson();
    void updateDisplayCalls();  
    void LoadFromDayTable();
    void LoadFromTotal();
    void LoadFromTotalCalls();
    void closeEvent(QCloseEvent *event) override;
    void on_Btn_SaveSample_clicked();
    void DelSampleKBOnline();
    void Preview();
    void on_cB_Attachment_toggled(bool checked);
    void on_pTE_LetterBody_textChanged();
    void on_pTE_Sign_textChanged();
    void on_Le_Attachment_textChanged(/*const QString &arg1*/);
    void on_LE_Greeting_textChanged(/*const QString &arg1*/);
    void on_Lw_Tasks_currentRowChanged(int currentRow);
    void on_cB_Tasks_currentIndexChanged(int index);
    void UnexpectedSaving();
    void AddSampleKBOnline();
    void DelSelectedKBOnline();
    void RefreshKBdbOnline();
    void UpdateKBOnline();
    void on_Lw_KBOnline_itemDoubleClicked(/*QListWidgetItem *item*/);
    void on_Le_SearchOnline_textChanged(const QString &arg1);
    void on_Lw_SearchOnline_currentTextChanged(const QString &currentText);
    void on_Btn_EditSampleOnline_clicked();
    void on_Btn_AddSampleOnline_clicked();
    void EditKBJsonFile();
    void on_Lw_SearchOnline_itemDoubleClicked(/*QListWidgetItem *item*/);
    void on_pTE_Notes_textChanged();
    void on_Le_Search_textChanged(const QString &arg1);




private:
    QStringList nowList_KBOnline;
    SearchHighLight* m_searchHighLight;
    QNetworkAccessManager * m_namLoad;
    QNetworkRequest m_nreq;

public slots:



};
#endif // MAINWINDOW_H
