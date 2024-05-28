#ifndef IPTCONFIGMANAGER_H
#define IPTCONFIGMANAGER_H

#include <memory>

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

  public:
    IPTConfigManager(const IPTConfigManager &) = delete;
    void operator=(const IPTConfigManager &) = delete;
    static IPTConfigManager &getInstance();
    void setRecentImgDir(const QString &dir);
    QString getRecentImgFile() const;
    QString getRecentImgDir() const;
    int getMaxRecentImg() const;
};

#endif // IPTCONFIGMANAGER_H
