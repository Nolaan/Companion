#include "devboard.h"
#include <libusb-1.0/libusb.h>
#include <core/partition.h>
#include <fs/filesystemfactory.h>
#include <QProcess>
#include <kurlrequester.h>
#include <QFile>
#include <qtextstream.h>


DevBoard::DevBoard(MainWindow* parent,const QString& name, const QString& arch,const QString& cpu_name, const QString& cpu_arch)
    : Device("Jetson", "emmc", 5, 3849216,512, 4096)
{
    m_parent = parent;
    m_BoardName = name;
    m_Arch = arch;
    m_CpuName = cpu_name;
    m_CpuArch = cpu_arch;
    //m_Memory = new Device(m_BoardName, "file", 5, 6,512, 100);
    m_ConnectionStatus = this->isConnected();
    m_partitionfile = parent->getUrlConfPath() + "partitionfile";
    getParttable(m_parent);


    // Timer
    m_timer = new QTimer(this);
    m_timer->setInterval(3000);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(this->isConnected()));
    m_timer->start();
        
}

DevBoard::~DevBoard()
{
 delete(this);
}


bool DevBoard::isConnected()
{
    //pointer to pointer of device used to retrieve a list of devices
    libusb_device **devs;
    libusb_device_handle *dev_handle; //a device handle
    libusb_context *ctx = NULL; //A LIBUSB session
    int r;// for return values
    ssize_t cnt; //holding number of devices in list
    r = libusb_init(&ctx); // initialize the library for the session we just declared

    if(r < 0){
        // printf("init error\n");
        return 1;
    }

    libusb_set_debug(ctx, 3); // set verbosity level to 3, as suggested in the documentation
    cnt = libusb_get_device_list(ctx, &devs); //get the list of devices

    if (cnt < 0) {
        // printf("Get Device Error \n");
        return 1;
    }


    uint16_t vendor_id = 0x0955;
    uint16_t product_id = 0x7140;
    dev_handle = libusb_open_device_with_vid_pid(ctx,vendor_id, product_id);

    if(dev_handle){
        // ("WE HAVE A HANDLE => device connected\n");
        this->m_ConnectionStatus = 1;
    } else
    {
        // printf("Nothing there!!!\n");
        this->m_ConnectionStatus = 0;
    }
    
    return m_ConnectionStatus;
}

void DevBoard::getParttable(MainWindow* parent)
{
    PartitionTable* ptable = new PartitionTable();
    PartitionRole::Roles(r);
    r = PartitionRole::Primary;
    QProcess process;
    QString cmdline;
    QString toolPath;
    QString flasher;
    QString options;
    QString name;

    int start;
    int last;
    int bytepersector=4096;
    QRegExp rx("\\d+");
    QRegExp nm("^\\w+=(\\w+)");

    QList<int>sectorsVals;
    QList<QString>partNames;
    // Fetch the partition table text file from the board
    if(this->m_ConnectionStatus && isConfPathValid(parent))
    {

        flasher = parent->getUrlConfPath();
        flasher.append("/bootloader/fastboot.bin --go");

        toolPath = parent->getUrlConfPath();
        toolPath.append("/bootloader/nvflash");

        options = " --getpartitiontable ";
        options += parent->m_configDir;
        options.append(this->m_partitionfile);
        options.append(" --bl ");
        options.append(flasher);

        cmdline = "";
        cmdline.append(toolPath);
        cmdline.append(options);
        //cmdline = "/home/nolaan/Travail/Projets/Auto/Dev_env/Linux_for_Tegra2/bootloader/nvflash  --getpartitiontable /home/nolaan/partitionfile --bl fastboot.bin --go";
        process.start(cmdline);
        process.waitForFinished(-1); // will wait forever until finished

        QString stdout = process.readAllStandardOutput();
        QString stderr = process.readAllStandardError();
        cmdline.clear();
    }
    else
    {
        // Alert!!!

    }
    // Parse the text file to extract partition
    if(QFile(m_partitionfile).exists())
    {
        QFile layout(m_partitionfile);
        QString line;
        if(layout.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream ts(&layout);

            while(!ts.atEnd()){

                line = ts.readLine();

                if(line.contains("PartitionId"))
                {
                    while(!line.contains("Name") && ts.pos())
                    {
                        line = ts.readLine();
                    }
                    nm.indexIn(line);
                    partNames.append(nm.cap(1));

                    while(!line.contains("StartSector") && ts.pos())
                    {
                        line = ts.readLine();
                    }
                    rx.indexIn(line);
                    start = rx.cap().toInt();

                    while(!line.contains("NumSectors") && ts.pos())
                    {
                        line = ts.readLine();
                    }

                    rx.indexIn(line);
                    last = start + rx.cap().toInt();
                    sectorsVals.append(start);
                    sectorsVals.append(last);
                }
            }
        }

    }
    // Generate Layout consitent with partitionmanager way

    while(!sectorsVals.empty())
    {
        start = sectorsVals.takeFirst();
        last  = sectorsVals.takeFirst()-1;
        name = partNames.takeFirst();
        this->m_DeviceNode = name;
        Partition* p = new Partition(ptable, *this, PartitionRole(r), FileSystemFactory::create(FileSystem::Ext4, 0, 4096), start, last, -1);
        this->m_DeviceNode = "emmc";

        ptable->append(p);
    }

    // Create partition and insert them in ptable
    int i=0;

    this->setPartitionTable(ptable);
}

void DevBoard::on_conf_refresh(MainWindow* parent)
{
    if(this->isConfPathValid(parent))
        this->parseFlashConfig(parent);

}

bool DevBoard::isConfPathValid(MainWindow* parent)
{
    QFile flashFile;
    QString toolPath;
    toolPath = parent->getUrlToolPath();
    // Now let's check if the directory is correct
    toolPath.append("/flash.sh");
    flashFile.setFileName(toolPath);
    if(flashFile.exists())
    {
        m_ConfPathValid = 1;
        parent->resetPalette();

    } else {
        m_ConfPathValid = 0;
        parent->setRedPalette();
    }

    return m_ConfPathValid;
}

void DevBoard::saveConfTab(MainWindow* parent)
{
    //Retrieve current confPath
    QString path(parent->getUrlConfPath());
    QSettings m_settings( QString(path+"/settings"),QSettings::NativeFormat, this);
    m_settings.setValue("toolPath",parent->getUrlToolPath());
}

void DevBoard::parseFlashConfig(MainWindow* parent)
{
    QString flashcfg;
    flashcfg = parent->getUrlToolPath() + "/bootloader/flash.cfg";
    if(QFile(flashcfg).exists())
    {

        QSettings sets(flashcfg,QSettings::IniFormat);

//        QFile layout(flashcfg);
//        QString line;
//        if(layout.open(QIODevice::ReadOnly | QIODevice::Text))
//        {
//            QSettings
//        }
    }
}
