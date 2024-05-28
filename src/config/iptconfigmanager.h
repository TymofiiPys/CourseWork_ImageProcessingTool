#ifndef IPTCONFIGMANAGER_H
#define IPTCONFIGMANAGER_H

#include <QSettings>
#include <QString>

/**
 * @brief The IPTConfigManager class parses and stores application configuration.
 * Initialized at project startup, is singleton
 */
class IPTConfigManager {
  private:
    IPTConfigManager();
    static IPTConfigManager *instance;
    const QString kConfigFile = "app.config";
    QSettings *settings;
    void setDefaultValues() const;

  public:
    static IPTConfigManager *getInstance();
    // void setRecentImgFile(const QString &file);
    void setRecentImgDir(const QString &dir);
    QString getRecentImgFile() const;
    QString getRecentImgDir() const;
    int getMaxRecentImg() const;
};

#endif // IPTCONFIGMANAGER_H
