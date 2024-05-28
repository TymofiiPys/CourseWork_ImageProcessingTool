#ifndef RECENTIMAGES_H
#define RECENTIMAGES_H

#include <QString>
#include <QStringList>

#include "../config/iptconfigmanager.h"

const QString kRecentListFile = IPTConfigManager::getInstance()->getRecentImgFile();
const int kMaxRecentImgs = IPTConfigManager::getInstance()->getMaxRecentImg();

QStringList getRecentImages();

void savePathToFile(const QString &filePath);

#endif // RECENTIMAGES_H
