#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QString>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    nbFileDld = 0;
    nbDldFinished = 0 ;
    ui->setupUi(this);
    connect(ui->connectButton, SIGNAL(clicked()), this, SLOT(connectToFtp()));
    connect(this, SIGNAL(allDldFinished()), this, SLOT(closeFic()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectToFtp()
{
    ftp = new QFtp(this);
    connect(ftp, SIGNAL(commandFinished(int,bool)), this, SLOT(cmdFinished(int,bool)));
    connect(ftp, SIGNAL(listInfo(QUrlInfo)), this, SLOT(dlFile(QUrlInfo)));
    url = new QUrl(ui->adrEdit->text());

    if(!url->isValid() || url->scheme().toLower() != QLatin1String("ftp")){

        ftp->connectToHost(ui->adrEdit->text());
        ftp->login(ui->loginEdit->text(), ui->pwdEdit->text());

    } else {

        ftp->connectToHost(url->host(), url->port());

        ftp->login(ui->loginEdit->text(), ui->pwdEdit->text());
    }


}

void MainWindow::dlFile(QUrlInfo info)
{

    if(info.isFile()){


        files.append(new QFile(info.name()));

        if(!files.last()->open(QIODevice::WriteOnly)){
            delete files.last();
            QMessageBox msgBox;
             msgBox.setText("Erreur lors de la creation de fichier");
             msgBox.exec();

            return;
        }
        nbFileDld++;
        ftp->get(info.name(), files.last());
    }
}

void MainWindow::cmdFinished(int i, bool error)
{
    if(ftp->currentCommand() == QFtp::Get){
        if(error){
            nbFileDld--;
            QMessageBox msgBox;
             msgBox.setText("Erreur lors du telechargement,\n des fichiers peuvent etre corrompus");
             msgBox.exec();
            return;
        }
        nbDldFinished++;
        if(nbDldFinished == nbFileDld && nbDldFinished!=0)
            emit(allDldFinished());
    }

    if(ftp->currentCommand() == QFtp::Login){
        if (error) {
            QMessageBox msgBox;
             msgBox.setText("Erreur lors de l'authentification");
             msgBox.exec();
            return;
        }


         if(ui->dirEdit->text().length() > 2){
             ftp->cd(ui->dirEdit->text());
         }
        ftp->list();
    }

    if (ftp->currentCommand() == QFtp::ConnectToHost) {
             if (error) {
                 QMessageBox msgBox;
                  msgBox.setText("Erreur lors de la connexion");
                  msgBox.exec();
                 return;
             }
             return;
    }
}

void MainWindow::closeFic()
{
    foreach(QFile *f , files){
        f->close();
        delete f;
    }
    files.clear();
    QMessageBox msgBox;
    msgBox.setText("Telechargement termine\nNombre de fichiers : "+QString::number(nbFileDld));
    msgBox.exec();
    nbFileDld = 0 ;
    nbDldFinished = 0 ;
}

