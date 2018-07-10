#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "masterthread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void showResponse(const QString &s);
    void downFinish(const QString &s);
    void processError(const QString &s);
    void processTimeout(const QString &s);
    void showFactoryReadResponse(const QString &s);
    void on_toolButtonStop_clicked();
    void on_toolButtonOpenFile_clicked();
    void on_toolButtonDownload_clicked();

    void on_toolButtonWrite_clicked();

    void on_toolButtonReadInfo_clicked();

private:
    Ui::MainWindow *ui;
    MasterThread m_thread;
};

#endif // MAINWINDOW_H
