#ifndef APPLICATION_H
#define APPLICATION_H
#include <QObject>
#include <QGuiApplication>
#include <QSharedMemory>
class Application:public QGuiApplication
{

    Q_OBJECT
public:
   Application(int &argc, char **argv);
    ~Application();

    bool lock();

private:
    QSharedMemory *_singular; // shared memory !! SINGLE ACCESS
};

#endif // APPLICATION_H
