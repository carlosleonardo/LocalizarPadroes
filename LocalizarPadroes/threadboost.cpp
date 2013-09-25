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


void ThreadBoost::juntar()
{
    boost::thread* meuThread = subprocesso.get();

    meuThread->join();
}

void ThreadBoost::esperar(int milisegundos)
{
    boost::this_thread::sleep_for(boost::chrono::milliseconds(milisegundos));
}

void ThreadBoost::dormir()
{
    boost::unique_lock<boost::mutex> bloqueio;
    condicao.wait(bloqueio);
}

void ThreadBoost::acordar()
{
    condicao.notify_one();
}
