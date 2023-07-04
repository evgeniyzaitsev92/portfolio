#ifndef KBJSONFILEEDIT_H
#define KBJSONFILEEDIT_H

#include "searchhighlight.h"

#include <QDialog>
#include <QTextStream>
#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

class SearchHighLight;

namespace Ui {
class KBJsonFileEdit;
}

class KBJsonFileEdit : public QDialog
{
    Q_OBJECT

public:
    explicit KBJsonFileEdit(QWidget *parent = nullptr);
    ~KBJsonFileEdit();

    void JsonKBFileOpen();

private slots:
    void on_Btn_Save_clicked();
    void on_Le_Search_textChanged(const QString &arg1);

private:
    Ui::KBJsonFileEdit *ui;

    // Наследованный класс для создания логики подсветки синтаксиса
    SearchHighLight* m_searchHighLight;
};

#endif // KBJSONFILEEDIT_H
