#include "settings.h"

Settings::Settings() {
}

/**
 * @brief Set current directory path
 * @param currentDir
 */
void Settings::setCurrentDir(const QString &currentDir) {
    QString dir;
    if(currentDir.isEmpty()) {
        dir = qApp->applicationDirPath();
    } else {
        dir = currentDir.mid(0, currentDir.lastIndexOf('/'));
    }
    QSettings set(ORG_NAME, APP_NAME);
    set.setValue("current_dir", dir);
}

/**
 * Get current directory path
 * @return
 */
QString Settings::getCurrentDir() {
    QSettings set(ORG_NAME, APP_NAME);
    QString currentDir =  set.value("current_dir", "").toString();
    if(currentDir.isEmpty()) {
        currentDir = qApp->applicationDirPath();
    }
    return currentDir;
}
