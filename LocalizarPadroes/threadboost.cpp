#include "threadboost.h"

#include <chrono>

using namespace std;

ThreadBoost::ThreadBoost():AdaptadorInterfaceThread()
{
}

void ThreadBoost::iniciar()
{
    subprocesso.reset(new thread(bind(&ThreadBoost::executar, this)));
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
    this_thread::yield();
}


void ThreadBoost::juntar()
{
    thread* meuThread = subprocesso.get();

    meuThread->join();
}

void ThreadBoost::esperar(int milisegundos)
{
    this_thread::sleep_for(chrono::milliseconds(milisegundos));
}

void ThreadBoost::dormir()
{
    unique_lock<mutex> bloqueio;
    condicao.wait(bloqueio);
}

void ThreadBoost::acordar()
{
    condicao.notify_one();
}
