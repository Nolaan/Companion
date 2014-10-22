#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "util/libpartitionmanagerexport.h"
#include <QSettings>
#include <QTimer>
#include "core/partition.h"

 namespace Ui {
 class MainWindow;
 }

class DevBoard;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setKLedGreen();
    void setKLedRed();
    void resetPalette();
    void setRedPalette();
    QString getUrlConfPath();
    QString getUrlToolPath();
    void setConf(const QString& key, const QVariant& value);
    void refreshConnectionStatus(void* Board);
    void setFuturePartTable(PartitionTable *ptable);
    void refreshMandatoryFiles(QHash<QString,QString> hash,int a);
    void flashPart(QString partName, DevBoard* Board);
    void flashAll(DevBoard* Board);

public:
    QTimer* m_timer;
    QString m_configDir;
    DevBoard* m_Board;
    QSettings m_settings;

private slots:

    void on_Refresh_clicked();

    void on_Reload_clicked();

    void on_pushButton_clicked();

    void on_flashRoot_clicked();

    void on_flashall_clicked();

    void on_flashSystem_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
