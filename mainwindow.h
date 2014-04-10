#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFtp>
#include <QUrl>
#include <QFile>
#include <QList>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QFtp *ftp;
    QUrl *url;
    QList<QFile*> files;
    int nbFileDld, nbDldFinished;

public slots:
    void connectToFtp();
    void dlFile(QUrlInfo);
    void cmdFinished(int, bool);
    void closeFic();

signals:
    void allDldFinished();

};

#endif // MAINWINDOW_H
