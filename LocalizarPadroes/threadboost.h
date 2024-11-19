#ifndef THREADBOOST_H
#define THREADBOOST_H

#include "adaptadorinterfacethread.h"

#include <thread>
#include <memory>
#include <condition_variable>
#include <mutex>

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
    void iniciar() override;
    void concederTempo() override;
    AdaptadorInterfaceThread *obterInstancia() override;

private:
    std::mutex meuMutex;
    std::shared_ptr<std::thread> subprocesso;
    std::condition_variable condicao;

protected:
    void bloquear() override;
    void desbloquear() override;
    virtual void executar() = 0;

    // AdaptadorInterfaceThread interface
public:
    void juntar() override;
    void esperar(int milisegundos) override;
    void dormir() override;
    void acordar() override;
};

#endif // THREADBOOST_H
