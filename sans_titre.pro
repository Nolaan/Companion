    #-------------------------------------------------
#
# Project created by QtCreator 2014-10-13T20:01:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sans_titre
TEMPLATE = app

INCLUDEPATH += /usr/include/kde4
INCLUDEPATH += gui, core, util

SOURCES += main.cpp\
        mainwindow.cpp \
    gui/partwidget.cpp \
    core/partitionnode.cpp \
    gui/partwidgetbase.cpp \
    gui/parttablewidget.cpp \
    core/device.cpp \
    core/partition.cpp \
    core/partitionrole.cpp \
    core/partitiontable.cpp \
    fs/ext2.cpp \
    fs/ext3.cpp \
    fs/ext4.cpp \
    fs/extended.cpp \
    fs/fat16.cpp \
    fs/fat32.cpp \
    fs/filesystem.cpp \
    fs/filesystemfactory.cpp \
    fs/hfs.cpp \
    fs/hfsplus.cpp \
    fs/jfs.cpp \
    fs/linuxswap.cpp \
    fs/ntfs.cpp \
    fs/reiser4.cpp \
    fs/reiserfs.cpp \
    fs/ufs.cpp \
    fs/unformatted.cpp \
    fs/unknown.cpp \
    fs/xfs.cpp \
    util/capacity.cpp \
    util/externalcommand.cpp \
    util/globallog.cpp \
    util/helpers.cpp \
    util/report.cpp \
    core/operationstack.cpp \
    jobs/backupfilesystemjob.cpp \
    jobs/checkfilesystemjob.cpp \
    jobs/copyfilesystemjob.cpp \
    jobs/createfilesystemjob.cpp \
    jobs/createpartitionjob.cpp \
    jobs/createpartitiontablejob.cpp \
    jobs/deletefilesystemjob.cpp \
    jobs/deletepartitionjob.cpp \
    jobs/job.cpp \
    jobs/movefilesystemjob.cpp \
    jobs/resizefilesystemjob.cpp \
    jobs/restorefilesystemjob.cpp \
    jobs/setfilesystemlabeljob.cpp \
    jobs/setpartflagsjob.cpp \
    jobs/setpartgeometryjob.cpp \
    ops/backupoperation.cpp \
    ops/checkoperation.cpp \
    ops/copyoperation.cpp \
    ops/createfilesystemoperation.cpp \
    ops/createpartitiontableoperation.cpp \
    ops/deleteoperation.cpp \
    ops/newoperation.cpp \
    ops/operation.cpp \
    ops/resizeoperation.cpp \
    ops/restoreoperation.cpp \
    ops/setfilesystemlabeloperation.cpp \
    ops/setpartflagsoperation.cpp \
    core/copysourcedevice.cpp \
    core/copytarget.cpp \
    core/copytargetdevice.cpp \
    core/copysource.cpp \
    core/copysourcefile.cpp \
    core/copytargetfile.cpp \
    core/libparted.cpp \
    core/operationrunner.cpp \
    core/devboard.cpp

HEADERS  += mainwindow.h \
    gui/partwidget.h \
    core/partitionnode.h \
    gui/partwidgetbase.h \
    gui/parttablewidget.h \
    core/device.h \
    core/partition.h \
    core/partitionrole.h \
    core/partitiontable.h \
    fs/ext2.h \
    fs/ext3.h \
    fs/ext4.h \
    fs/extended.h \
    fs/fat16.h \
    fs/fat32.h \
    fs/filesystem.h \
    fs/filesystemfactory.h \
    fs/hfs.h \
    fs/hfsplus.h \
    fs/jfs.h \
    fs/linuxswap.h \
    fs/ntfs.h \
    fs/reiser4.h \
    fs/reiserfs.h \
    fs/ufs.h \
    fs/unformatted.h \
    fs/unknown.h \
    fs/xfs.h \
    util/capacity.h \
    util/externalcommand.h \
    util/globallog.h \
    util/helpers.h \
    util/libpartitionmanagerexport.h \
    util/report.h \
    core/operationstack.h \
    jobs/backupfilesystemjob.h \
    jobs/checkfilesystemjob.h \
    jobs/copyfilesystemjob.h \
    jobs/createfilesystemjob.h \
    jobs/createpartitionjob.h \
    jobs/createpartitiontablejob.h \
    jobs/deletefilesystemjob.h \
    jobs/deletepartitionjob.h \
    jobs/job.h \
    jobs/movefilesystemjob.h \
    jobs/resizefilesystemjob.h \
    jobs/restorefilesystemjob.h \
    jobs/setfilesystemlabeljob.h \
    jobs/setpartflagsjob.h \
    jobs/setpartgeometryjob.h \
    ops/backupoperation.h \
    ops/checkoperation.h \
    ops/copyoperation.h \
    ops/createfilesystemoperation.h \
    ops/createpartitiontableoperation.h \
    ops/deleteoperation.h \
    ops/newoperation.h \
    ops/operation.h \
    ops/resizeoperation.h \
    ops/restoreoperation.h \
    ops/setfilesystemlabeloperation.h \
    ops/setpartflagsoperation.h \
    core/copysourcedevice.h \
    core/copytarget.h \
    core/copytargetdevice.h \
    core/copysource.h \
    core/copysourcefile.h \
    core/copytargetfile.h \
    core/libparted.h \
    core/operationrunner.h \
    core/devboard.h

FORMS    += mainwindow.ui

LIBS += -L/usr/lib64/kde4/devel/
LIBS += -L/lib64/
LIBS += -lkio
LIBS += -luuid
LIBS += -lblkid
LIBS += -lusb-1.0
LIBS += -lparted
LIBS += -lkdeui
LIBS += -lkdecore

