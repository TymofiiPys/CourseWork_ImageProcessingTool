#include <QDir>
#include <QFile>

#include "iptconfigmanager.h"

IPTConfigManager *IPTConfigManager::instance = nullptr;

IPTConfigManager::IPTConfigManager() {
    this->settings = new QSettings(kConfigFile, QSettings::IniFormat);

    if (!QFile::exists(kConfigFile)) {
        setDefaultValues();
    }
}

IPTConfigManager *IPTConfigManager::getInstance() {
    if (instance == nullptr) {
        instance = new IPTConfigManager();
    }
    return instance;
}

void IPTConfigManager::setDefaultValues() {
    settings->setValue("recent_img_file", "recent.txt");
    settings->setValue("recent_img_dir", QDir::homePath());
    settings->setValue("max_recent_img", 15);
    settings->sync();
}

// void IPTConfigManager::setRecentImgFile(const QString &file) {
//     settings->setValue("recent_img_file", file);
//     settings->sync();
// }

void IPTConfigManager::setRecentImgDir(const QString &dir) {
    settings->setValue("recent_img_dir", dir);
    settings->sync();
}

QString IPTConfigManager::getRecentImgFile() {
    return settings->value("recent_img_file", "recent.txt").toString();
}

QString IPTConfigManager::getRecentImgDir() {
    return settings->value("recent_img_dir", QDir::homePath()).toString();
}

int IPTConfigManager::getMaxRecentImg() {
    return settings->value("max_recent_img", 15).toInt();
}
