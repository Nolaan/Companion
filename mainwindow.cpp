#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "core/devboard.h"
#include <QDir>
#include <QSettings>
#include <QLayout>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{



    ui->setupUi(this);


    // Create local dir to hold app config
    m_configDir = QDir::homePath();
    m_configDir += "/.companion/";
    if(!QDir(m_configDir).exists())
    {
         QDir().mkdir(m_configDir);
    }

    // Check for configuration file
    //QSettings mysettings( QString(m_configDir+"/settings"),QSettings::NativeFormat, this);
    QSettings m_settings( QString(m_configDir+"/settings"),QSettings::NativeFormat, this);
    QString currConf(m_settings.value("toolPath").toString());
    ui->kurlrequester->setText(currConf);


    // Device* Jetson = new Device("Jetson", "file", 5, 6,512, 100);
    // Maybe use a dialog prompt to ask which board to
    // load from config file...?
    DevBoard* Jetson = new DevBoard(this,"Jetson", "32 bits", "TK1", "ARMV7");
    m_Board = Jetson;
    ui->m_PartTableWidget->setPartitionTable(Jetson->partitionTable());
    refreshConnectionStatus(m_Board);

    m_timer = new QTimer(this);
    m_timer->setInterval(3000);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(refreshConnectionStatus()));
    m_timer->start();

    QString text = ui->filewarning->text();
    text.append(getUrlToolPath()+"/bootloader");
    ui->filewarning->setText(text);



}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::setKLedGreen()
{
    ui->kled->setColor(QColor(0,255,0));
}

void MainWindow::setKLedRed()
{
    ui->kled->setColor(QColor(255,0,0));
}
void MainWindow::resetPalette()
{
    ui->kurlrequester->setPalette(QPalette());
}

void MainWindow::setRedPalette()
{
    QPalette palette;
    QBrush brush(QColor(255, 0, 4, 102));
    brush.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Base, brush);
    palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
    QBrush brush1(QColor(244, 244, 244, 255));
    brush1.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
    ui->kurlrequester->setPalette(palette);

}


void MainWindow::on_Refresh_clicked()
{
    DevBoard* myboard;
    myboard = (DevBoard*)m_Board;
    refreshConnectionStatus(m_Board);
    myboard->on_conf_refresh(this);
}

QString MainWindow::getUrlToolPath()
{
    QString path;
    path = ui->kurlrequester->text();
    return path;
}

QString MainWindow::getUrlConfPath()
{
    return m_configDir;
}

void MainWindow::on_Reload_clicked()
{
    DevBoard* myboard;
    myboard = (DevBoard*)m_Board;
    myboard->getParttable(this);
}

void MainWindow::on_pushButton_clicked()
{
    DevBoard* myboard;
    myboard = (DevBoard*)m_Board;
    myboard->saveConfTab(this);
}

void MainWindow::setConf(const QString& key, const QVariant& value)
{
    m_settings.setValue(key,value);
}

void MainWindow::refreshConnectionStatus(void *Board)
{
    DevBoard* mBoard = (DevBoard*)Board;
    mBoard->isConnected();
    if(mBoard->m_ConnectionStatus)
    {
        ui->label->setText(tr("Jetson plugged"));
        MainWindow::setKLedGreen();
    }
    else
    {
        ui->label->setText(tr("Jetson unplugged"));
        MainWindow::setKLedRed();
        // MainWindow::resetPalette();
    }
}

void MainWindow::setFuturePartTable(PartitionTable *ptable)
{
    ui->m_PartTableWidgetFuture->setPartitionTable(ptable);
}

void MainWindow::refreshMandatoryFiles(QHash<QString, QString> hash,int a=0)
{
    int number = ui->verticalLayout_5->count();
    if( number > 2 )
    {
        //Save title
        QString warning(ui->filewarning->text());
        //ui->verticalLayout_5->addWidget(ui->filewarning);

        // Clear widgets
        QLayoutItem *child;
        while ( (child = ui->verticalLayout_5->takeAt(0)) != 0 )
        {
            child->widget()->setParent(NULL);
            delete child;
        }


        // Restore title
        ui->filewarning = new QLabel(ui->Files);
        ui->filewarning->setObjectName(QString::fromUtf8("filewarning"));
        ui->filewarning->setWordWrap(true);
        ui->filewarning->setText(warning);
        ui->verticalLayout_5->addWidget(ui->filewarning);
    }
    if(!a)
    {
        foreach(const QString &key, hash.keys())
        {

            QLabel* titre = new QLabel();
            titre->setText("Partition "+key);
            ui->verticalLayout_5->addWidget(titre);
            titre->show();

            KUrlRequester* systimg;
            systimg = new KUrlRequester();
            systimg->setText(hash.value(key));
            ui->verticalLayout_5->addWidget(systimg);
            systimg->show();

        }
    }
}

void MainWindow::on_flashRoot_clicked()
{
    flashPart("LNX",m_Board);
}

void MainWindow::flashPart(QString partName, DevBoard* Board)
{

    Board->flashPart(partName,this);

}

void MainWindow::flashAll(DevBoard* Board)
{
    Board->flashAll(this);
}

void MainWindow::on_flashall_clicked()
{
    flashAll(m_Board);
}

void MainWindow::on_flashSystem_clicked()
{
    flashPart("SYS",m_Board);
}
