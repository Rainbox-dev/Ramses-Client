#ifndef RAMSES_H
#define RAMSES_H

#include "duqf-app/app-utils.h"
#include "dbinterface.h"
#include "ramuser.h"
#include "dbisuspender.h"

#include <QObject>

class Ramses : public QObject
{
    Q_OBJECT
public:
    static Ramses *instance();
    void login(QString username, QString password);
    void logout();

    QList<RamUser *> users() const;
    RamUser *currentUser() const;
    RamUser *defaultUser() const;
    RamUser *createUser();
    void removeUser(QString uuid);

    bool isConnected() const;

signals:
    void loggedIn(RamUser*);
    void loggedOut();
    void newUser(RamUser *user);

protected:
    static Ramses *_instance;

private slots:
    void update();
    void newData(QJsonObject data);
    void gotUsers(QJsonArray users);
    //TODO This should be modified when implementing offline version
    void dbiConnectionStatusChanged(NetworkUtils::NetworkStatus s);
private:
    /**
     * @brief Ramses is the class managing all data. It is a singleton
     * @param parent
     */
    explicit Ramses(QObject *parent = nullptr);

    DBInterface *_dbi;
    void login(QJsonObject user);
    /**
     * @brief True when loogged in, false otherwise.
     */
    bool _connected;

    QList<RamUser *> _users;
    RamUser *_currentUser;
    RamUser *_defaultUser;
    QString _currentUserShortName;
};

#endif // RAMSES_H
