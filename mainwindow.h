#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "util/libpartitionmanagerexport.h"
#include <QSettings>
#include <QTimer>


 namespace Ui {
 class MainWindow;
 }


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
    QString m_configDir;
    void* m_Board;
    QSettings m_settings;
    void setConf(const QString& key, const QVariant& value);
    void refreshConnectionStatus(void* Board);
    QTimer* m_timer;

private slots:

    void on_Refresh_clicked();

    void on_Reload_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
