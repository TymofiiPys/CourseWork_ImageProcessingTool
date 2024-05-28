#include "recentimages.h"

#include <QDebug>
#include <QFile>
#include <QString>
#include <QTextStream>

QStringList getRecentImages() {
    QFile file(kRecentListFile);
    if (file.open(QIODeviceBase::ReadOnly | QIODeviceBase::Text)) {
        QTextStream textStream(&file);
        QStringList filenameList = textStream.readAll().split("\n");
        qDebug() << "Opened and read successfully";
        file.close();
        return filenameList;
    } else {
        file.open(QIODeviceBase::NewOnly);
        qDebug() << "Created new file";
        file.close();
        QStringList list("oopsie");
        return list;
    }
}

void savePathToFile(QString &filePath) {
    QFile file(kRecentListFile);
    if (file.open(QIODeviceBase::Append | QIODeviceBase::Text)) {
        QTextStream textStream(&file);
        textStream << Qt::endl << filePath;
        file.close();
    }
}
