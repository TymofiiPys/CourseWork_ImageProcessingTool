#include <QDir>
#include <QFile>

#include "iptconfigmanager.h"

IPTConfigManager::IPTConfigManager() {
    this->settings = std::make_unique<QSettings>(kConfigFile, QSettings::IniFormat);

    if (!QFile::exists(kConfigFile)) {
        setDefaultValues();
    }
}

IPTConfigManager &IPTConfigManager::getInstance() {
    static IPTConfigManager instance;
    return instance;
}

void IPTConfigManager::setDefaultValues() const {
    settings->setValue("recent_img_file", "recent.txt");
    settings->setValue("recent_img_dir", QDir::homePath());
    settings->setValue("max_recent_img", 15);
    settings->setValue("threads", IPTConfigManager::getDefaultThreadsNum());
    settings->sync();
}

void IPTConfigManager::setRecentImgDir(const QString &dir) const {
    settings->setValue("recent_img_dir", dir);
    settings->sync();
}

void IPTConfigManager::setThreads(const unsigned int &threads) const {
    settings->setValue("threads", threads);
    settings->sync();
}

QString IPTConfigManager::getRecentImgFile() const {
    return settings->value("recent_img_file", "recent.txt").toString();
}

QString IPTConfigManager::getRecentImgDir() const {
    return settings->value("recent_img_dir", QDir::homePath()).toString();
}

unsigned int IPTConfigManager::getMaxRecentImg() const {
    return settings->value("max_recent_img", 15).toInt();
}

unsigned int IPTConfigManager::getThreads() const {
    return settings->value("threads", IPTConfigManager::getDefaultThreadsNum()).toUInt();
}
