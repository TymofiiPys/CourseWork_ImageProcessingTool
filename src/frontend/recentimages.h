#ifndef RECENTIMAGES_H
#define RECENTIMAGES_H

#include <QString>
#include <QStringList>

const QString kRecentListFile = "recent.txt";

QStringList getRecentImages();

void savePathToFile(QString &filePath);

#endif // RECENTIMAGES_H
