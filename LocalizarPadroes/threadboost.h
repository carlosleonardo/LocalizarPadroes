#ifndef THREADBOOST_H
#define THREADBOOST_H

#include "adaptadorinterfacethread.h"

#include <boost/thread.hpp>

/**
 * @brief The ThreadBoost class
 *
 * Classe que implementa thread boost
 */
class ThreadBoost : public AdaptadorInterfaceThread
{
public:
    ThreadBoost();

    // AdaptadorInterfaceThread interface
public:
    void iniciar();
    void concederTempo();
    AdaptadorInterfaceThread *obterInstancia();

private:
    boost::mutex meuMutex;
    boost::shared_ptr<boost::thread> subprocesso;
    boost::condition_variable condicao;

protected:
    void bloquear();
    void desbloquear();
    virtual void executar()=0;

    // AdaptadorInterfaceThread interface
public:
    void juntar();
    void esperar(int milisegundos);
    void dormir();
    void acordar();
};

#endif // THREADBOOST_H
