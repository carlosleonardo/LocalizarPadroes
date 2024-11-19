#include "adaptadorinterfacethread.h"

AdaptadorInterfaceThread::AdaptadorInterfaceThread() : cancelado(false)
{
}

AdaptadorInterfaceThread::~AdaptadorInterfaceThread()
{
}

void AdaptadorInterfaceThread::interromper()
{
    bloquear();
    cancelado = true;
    desbloquear();
}

bool AdaptadorInterfaceThread::interrompido()
{
    return cancelado;
}
