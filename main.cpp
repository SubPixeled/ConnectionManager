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
#include <QDirIterator>
#include "Directories.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QWidget>
#include <QString>



void copyFolder(const QString& srcFolder, const QString& subdir = "")

{

    QDir srcDir(srcFolder);
    QDir destDir(destFolder);

    if (!srcDir.exists())
        return;

    // create the destination directory if it doesn't exist
    if (!destDir.exists()) {
        destDir.mkpath(".");
    }
    // Get a list of all the files and directories in the directory
    QFileInfoList items = srcDir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QFileInfo& item : items)
    {
        if (item.isFile())
        {
            // copy file to destination folder
            QString srcFilePath = item.absoluteFilePath();
            QString destFilePath = destDir.absoluteFilePath(item.fileName());
            if(QFile::exists(destFilePath))
            {
                // file already exist, prompt the user to keep the existing file or overwrite it
                int choice = QMessageBox::question(nullptr, "File Exist", "A file with the same name already exist in the destination folder, do you want to overwrite it?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
                if(choice == QMessageBox::No) {
                    continue;
                }
            }

            QFile inputFile(item.absoluteFilePath());
                if (inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
                {
                    QTextStream in(&inputFile);

                    // Read each line of the input file
                    while (!in.atEnd())
                    {
                        QString line = in.readLine();

                        // Check for the Labels= line
                        if (line.startsWith("Labels="))
                        {
                            // Get the text after the = character
                            QString label = line.mid(7);

                            // Create a new directory with the label name
                            QDir().mkdir(destFolder + label);

                            // Copy the file into the new directory
                            inputFile.copy(destFolder + label + "/" + QFileInfo(item.absoluteFilePath()).fileName());
                        }
                    }

                    inputFile.close();
                }

        }

    }
}



void scanFolder(QTreeWidgetItem* parent, const QString& folderPath)
{
    QDir dir(folderPath);
    if (!dir.exists())
        return;

    // Get a list of all the files and directories in the directory
    QFileInfoList items = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QFileInfo& item : items)
    {
        if (item.isFile())
        {
            // Add the file as a leaf node in the tree
            QTreeWidgetItem* itemWidget = new QTreeWidgetItem(parent);
            itemWidget->setText(0, item.fileName());
            itemWidget->setData(0, Qt::UserRole, item.absoluteFilePath());
        }
        else if (item.isDir())
        {
            // Add the directory as a non-leaf node in the tree
            QTreeWidgetItem* itemWidget = new QTreeWidgetItem(parent);
            itemWidget->setText(0, item.fileName());
            itemWidget->setData(0, Qt::UserRole, item.absoluteFilePath());

            // Recursively scan the subdirectory
            scanFolder(itemWidget, item.absoluteFilePath());
        }
    }
}


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);




    copyFolder("C:\%USERPROFILE%\\Desktop\\Connection_files\\AddressBook", "");

    // Create the tree widget and set its header
    QTreeWidget treeWidget;
    treeWidget.setHeaderLabel("File Browser");

    // Create the root item for the tree
    QTreeWidgetItem* rootItem = new QTreeWidgetItem(&treeWidget);
    rootItem->setText(0, "Connections");

    // Scan the specified folder and add the filenames to the tree widget
    scanFolder(rootItem, destFolder);
    //"C:\%USERPROFILE%\\Documents\\Qt Projects\\ConnectionManager\\Connection_files\\AddressBook"

    QObject::connect(&treeWidget, &QTreeWidget::itemActivated, [&](QTreeWidgetItem* item, int column)
        {
            // Only open files, not directories
            if (!item->childCount())
            {
                QString filePath = item->data(column, Qt::UserRole).toString();
                QUrl vncUrl = QUrl::fromLocalFile(filePath);
                QDesktopServices::openUrl(vncUrl);
            }
        }

      );



    treeWidget.show();

    return app.exec();

}
