#include "results.h"
#include "ui_results.h"

Results::Results(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Results)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() | Qt::WindowMinMaxButtonsHint);
    ui->Button_Send->setVisible(false);
}

Results::~Results()
{
    delete ui;
}

void Results::GetTestPath(QString testpath){
    str_TestPathInEdit = testpath;
    Load();
}

void Results::Load(){
    db_Test = QSqlDatabase::addDatabase("QSQLITE");
    db_Test.setDatabaseName(str_TestPathInEdit);
    db_Test.open();

    QSqlQuery query;

    // test name
    query.prepare("SELECT * FROM testname;");
    query.exec();
    if (query.seek(0)){
        ui->listWidget_Result->addItem(query.value(1).toString());
    }

    // total count
    query.prepare("SELECT * FROM counttotal;");
    query.exec();
    if (query.seek(0)){
        ui->listWidget_Result->addItem(query.value(1).toString());
    }

    // time
    query.prepare("SELECT * FROM time;");
    query.exec();
    if (query.seek(0)){
        ui->listWidget_Result->addItem(query.value(1).toString());
        ui->listWidget_Result->addItem("");
    }

    // part 1
    query.prepare("SELECT * FROM count1;");
    query.exec();
    int count = 0;
    if (query.last()) {
        count = query.value(0).toInt();
    }
    if (count>0){
        if (query.seek(0)){
            ui->listWidget_Result->addItem(query.value(1).toString());
        }

        query.prepare("SELECT * FROM results1;");
        query.exec();
        while (query.next()) {
            ui->listWidget_Result->addItem(query.value(1).toString());
        }
        ui->listWidget_Result->addItem("");
    }

    // part 2
    query.prepare("SELECT * FROM count2;");
    query.exec();
    int count2 = 0;
    if (query.last()) {
        count2 = query.value(0).toInt();
    }
    if (count2>0){
        if (query.seek(0)){
            ui->listWidget_Result->addItem(query.value(1).toString());
        }

        query.prepare("SELECT * FROM results2;");
        query.exec();
        while (query.next()) {
            ui->listWidget_Result->addItem(query.value(1).toString());
        }
        ui->listWidget_Result->addItem("");
    }

    // part 3
    query.prepare("SELECT * FROM count3;");
    query.exec();
    int count3 = 0;
    if (query.last()) {
        count3 = query.value(0).toInt();
    }
    if (count3>0){
        if (query.seek(0)){
            ui->listWidget_Result->addItem(query.value(1).toString());
        }

        query.prepare("SELECT * FROM results3;");
        query.exec();
        while (query.next()) {
            ui->listWidget_Result->addItem(query.value(1).toString());
        }
    }

    db_Test.close();
}

//******//
// NAME //
//******//
void Results::on_lineEdit_Name_textChanged()
{
    if (ui->lineEdit_Name->text() == "") {
        ui->Button_PDF->setEnabled(false);
        ui->Button_Send->setEnabled(false);
    } else {
        ui->Button_PDF->setEnabled(true);
        ui->Button_Send->setEnabled(true);
    }
}

//*****//
// PDF //
//*****//
void Results::on_Button_PDF_clicked()
{

    /*
    const QString filename("Results/test.pdf");
    QPdfWriter pdfwriter(filename);
    pdfwriter.setPageSize(QPageSize(QPageSize::A4));
    pdfwriter.setPageMargins(QMargins(30, 30, 30, 30));

    QString testData = "";
    QPainter painter( &pdfwriter);
    for (int i = 0; i < ui->listWidget_Result->count(); i++) {
        testData += ui->listWidget_Result->item(i)->text();
        testData += "\n\n";

     //   QRect r = painter.viewport();
        painter.drawText(0, Qt::AlignLeft, testData);
     //   pdfwriter.newPage();
    }
    painter.end();
    */

    db_Test = QSqlDatabase::addDatabase("QSQLITE");
    db_Test.setDatabaseName(str_TestPathInEdit);
    db_Test.open();

    QSqlQuery query;
    QString str_Testname = "";

    // test name
    query.prepare("SELECT * FROM testname;");
    query.exec();
    if (query.seek(0)){
        str_Testname = query.value(1).toString();
    }

    QDate date;
    date = QDate::currentDate();
    QString str_Date = date.toString("dd.MM.yyyy");

    int count = 0;
    int count2 = 0;
    int count3 = 0;
    int linecount = 5;

    query.prepare("SELECT COUNT(*) FROM results1;");
    query.exec();
    if (query.next()){
        count = query.value(0).toInt();
    }

    query.prepare("SELECT COUNT(*) FROM results2;");
    query.exec();
    if (query.next()){
        count2 = query.value(0).toInt();
    }

    query.prepare("SELECT COUNT(*) FROM results3;");
    query.exec();
    if (query.next()){
        count3 = query.value(0).toInt();
    }

    QString html = "";

    html += "<h1 align=center><font size=6>";
    html += ui->listWidget_Result->item(0)->text(); // test name
    html += "<br>";
    html += ui->listWidget_Result->item(1)->text(); // total right
    html += "<br>";
    html += ui->listWidget_Result->item(2)->text(); // time
    html += "</font></h1>";

    // part 1
    if(count>0 || count2>0 || count3>0){
    html += "<p align=justify><font size=5><b>";
    html += ui->listWidget_Result->item(4)->text();
    html += "</b></font>";
    html += "<br>";
    }

    html += "<font size=5>";
    if(count>0){
        for (int i = 5; i < linecount+count; i++) {
            html += ui->listWidget_Result->item(i)->text();
            html += "<br>";
        }
        linecount += count;
        count = 0;
    } else if(count2>0){
        for (int i = 5; i < linecount+count2; i++) {
            html += ui->listWidget_Result->item(i)->text();
            html += "<br>";
        }
        linecount += count2;
        count2 = 0;
    } else if(count3>0){
        for (int i = 5; i < linecount+count3; i++) {
            html += ui->listWidget_Result->item(i)->text();
            html += "<br>";
        }
        linecount += count3;
        count3 = 0;
    }
    html += "</font>";
    html += "</p>";

    // part 2
    if(count2>0 || count3>0){
    html += "<p align=justify><font size=5><b>";
    html += ui->listWidget_Result->item(linecount+1)->text();
    html += "</b></font>";
    html += "<br>";
    linecount += 2;
    }

    html += "<font size=5>";
    if(count2>0){
        for (int i = linecount; i < linecount+count2; i++) {
            html += ui->listWidget_Result->item(i)->text();
            html += "<br>";
        }
        linecount += count2;
        count2 = 0;
    } else if(count3>0){
        for (int i = linecount; i < linecount+count3; i++) {
            html += ui->listWidget_Result->item(i)->text();
            html += "<br>";
        }
        linecount += count3;
        count3 = 0;
    }
    html += "</font>";
    html += "</p>";

    // part 3
    if(count3>0){
    html += "<p align=justify><font size=5><b>";
    html += ui->listWidget_Result->item(linecount+1)->text();
    html += "</b></font>";
    html += "<br>";
    linecount += 2;
    }

    html += "<font size=5>";
    if(count3>0){
        for (int i = linecount; i < linecount+count3; i++) {
            html += ui->listWidget_Result->item(i)->text();
            html += "<br>";
        }
        linecount += count3;
        count3 = 0;
    }
    html += "</font>";
    html += "</p>";

    QTextDocument document;
    document.setHtml(html);

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QPrinter::A4);
    printer.setOutputFileName("Results/" +str_Testname+ " - " +ui->lineEdit_Name->text()+ " - " +str_Date+ ".pdf");
    printer.setPageMargins(QMarginsF(15, 5, 15, 15));

    document.print(&printer);

    db_Test.close();

}

/*
    "<div align=right>"
       "City, 11/11/2015"
    "</div>"
    "<div align=left>"
       "Sender Name<br>"
       "street 34/56A<br>"
       "121-43 city"
    "</div>"
 */
/*
"<h1 align=center>DOCUMENT TITLE</h1>"
"<p align=justify>"
   "document content document content document content document content document content document content document content document content document content document content "
   "document content document content document content document content document content document content document content document content document content document content "
"</p>"
"<div align=right>sincerly</div>";
*/

void Results::on_Button_Close_clicked()
{
    this->close();
}
