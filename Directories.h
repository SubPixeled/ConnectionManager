#ifndef DIRECTORIES_H
#define DIRECTORIES_H

#include <QDir>
#include <QFileInfo>
#include <QTreeWidget>
#include <QApplication>
#include <QUrl>
#include <QDesktopServices>
#include <QFile>
#include <QDataStream>
#include <QMessageBox>
#include <QStandardPaths>


QString roamingPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
QString destFolder = roamingPath+"\\ConnectionManager\\connectionFiles\\";

#endif // DIRECTORIES_H
