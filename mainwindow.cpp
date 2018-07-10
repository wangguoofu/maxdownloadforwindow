#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "win_qextserialport.h"
#include <QFileDialog>
#include <QPlainTextEdit>
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    int i;
    QString com;
    ui->setupUi(this);

    for(i=0; i<50; i++)
        ui->comboBox->addItem(com.sprintf("COM%d", i));

    connect(&m_thread, &MasterThread::response, this, &MainWindow::showResponse);
    connect(&m_thread, &MasterThread::finish, this, &MainWindow::downFinish);
    connect(&m_thread, &MasterThread::error, this, &MainWindow::processError);
    connect(&m_thread, &MasterThread::timeout, this, &MainWindow::processTimeout);
    connect(&m_thread, &MasterThread::factoryReadResponse, this, &MainWindow::showFactoryReadResponse);


    ui->toolButtonStop->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showResponse(const QString &s)
{
    qDebug()<<s<<endl;
    ui->textResponse->append(s);
}

void MainWindow::downFinish(const QString &s)
{
    qDebug()<<s<<endl;
    QMessageBox::about(NULL, "finish", s);
     ui->toolButtonDownload->setEnabled(true);
     ui->toolButtonWrite->setEnabled(true);
     ui->toolButtonReadInfo->setEnabled(true);
     ui->toolButtonStop->setEnabled(false);
}

void MainWindow::processError(const QString &s)
{
    qDebug()<<s<<endl;
}

void MainWindow::processTimeout(const QString &s)
{
    qDebug()<<s<<endl;
}

void MainWindow::showFactoryReadResponse(const QString &s)
{
    qDebug()<<s<<endl;
    ui->textEditReadInfo->setText(s);
}

void MainWindow::on_toolButtonStop_clicked()
{
    m_thread.stopdown();
}

void MainWindow::on_toolButtonOpenFile_clicked()
{
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle(tr("Open Image"));
    fileDialog->setDirectory(".");
    if(fileDialog->exec() == QDialog::Accepted) {
           QString path = fileDialog->selectedFiles()[0];
           qDebug() <<"select:"<<path<<endl;
           ui->filePath->setText(path);
    } else {
            qDebug() <<"do not select"<<endl;
    }
}

void MainWindow::on_toolButtonDownload_clicked()
{
    qDebug() <<"start to download bin"<<endl;
    qDebug() <<"serial port is:"<<ui->comboBox->currentText()<<endl;
    qDebug() <<"bin file is:"<<ui->filePath->toPlainText()<<endl;
    ui->toolButtonDownload->setEnabled(false);
    ui->toolButtonWrite->setEnabled(false);
    ui->toolButtonReadInfo->setEnabled(false);
    ui->toolButtonStop->setEnabled(true);
     ui->textResponse->clear();
    m_thread.startdown(ui->comboBox->currentText(), ui->filePath->toPlainText(), ui->radioButtonCrk->isChecked(), ui->radioButtonBin->isChecked());
}

void MainWindow::on_toolButtonWrite_clicked()
{
    m_thread.factoryWrite(ui->comboBox->currentText(), ui->lineEditSn->text(), ui->lineEditDeviceId->text(), ui->lineEditApiSecret->text());
}

void MainWindow::on_toolButtonReadInfo_clicked()
{
    m_thread.factoryRead(ui->comboBox->currentText());
}
