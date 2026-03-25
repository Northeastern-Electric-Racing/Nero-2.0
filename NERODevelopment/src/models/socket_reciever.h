#ifndef SOCKET_RECEIVER_H
#define SOCKET_RECEIVER_H

#include <QObject>
#include <QSocketNotifier>
#include <QString>

class ButtonSocketReceiver : public QObject {
    Q_OBJECT

public:
    explicit ButtonSocketReceiver(const QString &socketPath,
                                  QObject *parent = nullptr);
    ~ButtonSocketReceiver();

    bool start();
    void stop();

signals:
    void buttonDataReceived(const QString &topic, float value);
    void buttonEventReceived(const int buttonNumber, const QString &state);
    void errorOccurred(const QString &error);

private slots:
    void handleClientData();

private:
    void processMessage(const int buttonNumber);

    QString          m_socketPath;
    int              m_fd;
    QSocketNotifier *m_notifier;
    bool             m_running;
};

#endif