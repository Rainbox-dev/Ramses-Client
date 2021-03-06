#ifndef APPUTILS_H
#define APPUTILS_H

#include <QtDebug>
#include <QApplication>
#include <QIcon>
#include <QSplashScreen>
#include <QProgressBar>
#include <QLabel>
#include <QFileInfo>
#include <QFontDatabase>
#include <QRegularExpression>
#include <QTimer>

#include "app-version.h"
#include "app-style.h"
#include "../duqf-utils/utils.h"
#include "../duqf-utils/duqflogger.h"

#ifdef Q_OS_WIN
#include "windows.h"
#endif

class MessageHandler
{
public:
    static void messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
    {
        QByteArray localMsg = msg.toLocal8Bit();
        const char *file = context.file ? context.file : "";
        const char *function = context.function ? context.function : "";
        switch (type) {
        case QtDebugMsg:
    #ifdef QT_DEBUG
            fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
    #endif
            break;
        case QtInfoMsg:
            fprintf(stdout, "%s\n", localMsg.constData());
            break;
        case QtWarningMsg:
            fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
            break;
        case QtCriticalMsg:
            fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
            break;
        case QtFatalMsg:
            fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
            break;
        }
    }
};

class DuSplashScreen : public QSplashScreen
{
    Q_OBJECT
public:
    DuSplashScreen(const QPixmap &pixmap = QPixmap(), Qt::WindowFlags f = Qt::WindowFlags()) : QSplashScreen(pixmap, f)
    {
        //add a progress bar
        _progressBar = new QProgressBar(this);
        _progressBar->setMinimumWidth( pixmap.width() );
        _progressBar->setMaximumWidth( pixmap.width() );
        _progressBar->move( 0, pixmap.height() - 30);
        _progressBar->setAlignment(Qt::AlignVCenter);
        //add a label for the version
        _versionLabel = new QLabel("v" + QString(STR_VERSION), this);
        _versionLabel->setMinimumWidth( pixmap.width() );
        _versionLabel->setMaximumWidth( pixmap.width() );
        _versionLabel->move( 0, pixmap.height() - 45);
    }

public slots:
    void newMessage(QString message, DuQFLog::LogType lt = DuQFLog::Information)
    {
        if (!this->isVisible()) return;
        if (lt == DuQFLog::Debug) return;
        _progressBar->setFormat( "%p% - " + message );
#ifdef QT_DEBUG
    qDebug().noquote() << message;
#endif
        repaint();
    }
    void progressMax(int max)
    {
        if (!this->isVisible()) return;
        _progressBar->setMaximum( max );
    }
    void progress(int val)
    {
        if (!this->isVisible()) return;
        _progressBar->setValue(val);
        repaint();
    }

private:

    QProgressBar *_progressBar;
    QLabel *_versionLabel;
};

class DuApplication : public QApplication
{
    Q_OBJECT
public:
    explicit DuApplication(int &argc, char *argv[]);

    DuSplashScreen *splashScreen() const;

    void showSplashScreen();

    void setIdleTimeOut(int to);

    // Process the CLI arguments
    bool processArgs(QStringList examples = QStringList(), QStringList helpStrings = QStringList());

signals:
    void idle();

protected:
    virtual bool notify(QObject *receiver, QEvent *ev);

private slots:
    void idleTimeOut();

private:
    DuSplashScreen *_splashScreen;
    QTimer *_idleTimer;
    int _idleTimeout;
    int _argc;
    char *_argv[];
};

#endif // APPUTILS_H
