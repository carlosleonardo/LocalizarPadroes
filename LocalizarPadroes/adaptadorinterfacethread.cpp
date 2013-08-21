#include "adaptadorinterfacethread.h"

AdaptadorInterfaceThread::AdaptadorInterfaceThread()
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

AdaptadorInterfaceThread *AdaptadorInterfaceThread::threadAtual()
{
    return obterInstancia();
}

