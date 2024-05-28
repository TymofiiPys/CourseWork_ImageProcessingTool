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
    QFile fileRecent(kRecentListFile);
    QString tempFilePath = "temp.txt";
    QFile fileTemp(tempFilePath);
    // Recent filenames amount
    int recentFNs = getRecentImages().size();
    bool filePathPresent = getRecentImages().contains(filePath);
    if (filePathPresent)
        recentFNs--;
    if (fileRecent.open(QIODevice::ReadOnly | QIODevice::Text)
        && fileTemp.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream in(&fileRecent);
        QTextStream out(&fileTemp);

        while (!in.atEnd()) {
            QString line = in.readLine();
            if (recentFNs >= kMaxRecentImgs) {
                recentFNs--;
            } else if (line != filePath) {
                out << line << "\n";
            }
        }
        fileRecent.close();
        fileTemp.close();
    }
    QFile::remove(kRecentListFile);
    QFile::rename(tempFilePath, kRecentListFile);
    if (fileRecent.open(QIODeviceBase::Append | QIODeviceBase::Text)) {
        QTextStream textStream(&fileRecent);
        textStream << filePath;
        fileRecent.close();
    }
}
