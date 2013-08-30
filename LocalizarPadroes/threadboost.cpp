#include "threadboost.h"

ThreadBoost::ThreadBoost():AdaptadorInterfaceThread()
{
}

void ThreadBoost::iniciar()
{
    subprocesso.reset(new boost::thread(boost::bind(&ThreadBoost::executar, this)));
}

void ThreadBoost::bloquear()
{
    meuMutex.lock();
}

void ThreadBoost::desbloquear()
{
    meuMutex.unlock();
}

AdaptadorInterfaceThread *ThreadBoost::obterInstancia()
{
    return this;
}

void ThreadBoost::concederTempo()
{
    boost::this_thread::yield();
}
