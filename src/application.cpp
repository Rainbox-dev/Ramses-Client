#include "application.h"
#ifdef QT_DEBUG
#include <QtDebug>
#endif

Application::Application(int& argc, char** argv) : QApplication(argc, argv)
{
    timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(idleTimeOut()));
    timeout=600000;
    timer->start(timeout);
}

void Application::idleTimeOut()
{
    emit idle();
}

void Application::setTimeOut(int to)
{
    timeout = to;
}

bool Application::notify(QObject *receiver, QEvent *ev) {

    if(ev->type() == QEvent::KeyPress || ev->type() == QEvent::MouseButtonPress)
    {
        //reset timer
        timer->start(timeout);
    }
    return QApplication::notify(receiver,ev);
}
