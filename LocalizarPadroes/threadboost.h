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

private:
    boost::mutex meuMutex;
    boost::shared_ptr<boost::thread> subprocesso;

protected:
    void bloquear();
    void desbloquear();
    virtual void executar()=0;

    // AdaptadorInterfaceThread interface
protected:
    AdaptadorInterfaceThread *obterInstancia();

    // AdaptadorInterfaceThread interface
public:
    void concederTempo();
};

#endif // THREADBOOST_H
