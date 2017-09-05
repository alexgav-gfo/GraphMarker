#ifndef SETTINGS_H
#define SETTINGS_H

#include <QApplication>
#include <QSettings>

const QString ORG_NAME = "GML";
const QString APP_NAME = "Graph Marker";

class Settings {
public:
    // Singleton
    static Settings &instance() {
        static Settings instance;
        return instance;
    }
    Settings(const Settings &) = delete;
    Settings(Settings &&) = delete;
    Settings &operator= (const Settings &) = delete;
    Settings &operator= (Settings &&) = delete;

    void setCurrentDir(const QString &currentDir);
    QString getCurrentDir();

protected:
    Settings();
};

#endif // SETTINGS_H
