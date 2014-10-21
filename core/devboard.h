#ifndef DEVBOARD_H
#define DEVBOARD_H

#include "core/device.h"
#include <QMainWindow>
#include "mainwindow.h"
#include <QTimer>

class DevBoard : public Device
{
public:
    DevBoard(MainWindow* parent,const QString& name, const QString& arch,const QString& cpu_name, const QString& cpu_arch);
    ~DevBoard();

public:
    void getParttable(MainWindow* parent);
    bool isConnected();
    void on_conf_refresh(MainWindow* parent);
    bool isConfPathValid(MainWindow* parent);
    void saveConfTab(MainWindow* parent);
    void parseFlashConfig(MainWindow* parent);

public:
    QString m_Arch;
    QString m_BoardName;
    QString m_CpuName;
    QString m_CpuArch;
    bool m_ConnectionStatus;
    MainWindow* m_parent;
    bool m_ConfPathValid;
    // Ugly timer for board's connection status
    QTimer *m_timer;

protected:
    QString m_partitionfile;

};

#endif // DEVBOARD_H
