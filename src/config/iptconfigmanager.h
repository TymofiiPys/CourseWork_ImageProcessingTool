#ifndef IPTCONFIGMANAGER_H
#define IPTCONFIGMANAGER_H

#include <algorithm>
#include <memory>
#include <thread>

#include <QSettings>
#include <QString>

/**
 * @brief The IPTConfigManager class parses and stores application configuration.
 * Initialized at project startup, is singleton
 */
class IPTConfigManager {
  private:
    IPTConfigManager();
    ~IPTConfigManager() = default;
    const QString kConfigFile = "app.config";
    std::unique_ptr<QSettings> settings;
    void setDefaultValues() const;
    static unsigned int getDefaultThreadsNum();

  public:
    IPTConfigManager(const IPTConfigManager &) = delete;
    void operator=(const IPTConfigManager &) = delete;
    static IPTConfigManager &getInstance();
    void setRecentImgDir(const QString &dir) const;
    void setThreads(const unsigned int &threads) const;
    QString getRecentImgFile() const;
    QString getRecentImgDir() const;
    unsigned int getMaxRecentImg() const;
    unsigned int getThreads() const;
};

inline unsigned int IPTConfigManager::getDefaultThreadsNum() {
    return std::clamp(std::thread::hardware_concurrency(), 0u, 4u);
}

#endif // IPTCONFIGMANAGER_H
