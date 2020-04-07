#include "application.h"

Application::Application(int &argc, char **argv):QGuiApplication(argc, argv, true)
{
    _singular = new QSharedMemory("SharedMemoryForOnlyOneInstanceOfYourBeautifulApplication", this);
}

Application::~Application()
{
    if(_singular->isAttached())
        _singular->detach();
}

bool Application::lock()
{
    if(_singular->attach(QSharedMemory::ReadOnly)){
        _singular->detach();
        return false;
    }

    if(_singular->create(1))
        return true;

    return false;
}
