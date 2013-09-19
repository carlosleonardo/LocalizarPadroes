#ifndef ADAPTADORINTERFACETHREAD_H
#define ADAPTADORINTERFACETHREAD_H

#include <functional>

/**
 * @brief The AdaptadorInterfaceThread class
 *
 * Classe para adaptação de interface de thread
 */
class AdaptadorInterfaceThread
{
public:
    AdaptadorInterfaceThread();
    virtual ~AdaptadorInterfaceThread();

    virtual void iniciar()=0;
    virtual void concederTempo()=0;
    void interromper();
    bool interrompido();

    virtual AdaptadorInterfaceThread* obterInstancia()=0;
    virtual void bloquear()=0;
    virtual void desbloquear()=0;
    virtual void juntar()=0;
    virtual void esperar(int milisegundos)=0;
    virtual void dormir()=0;
    virtual void acordar()=0;
protected:

    volatile bool cancelado;
};

#endif // ADAPTADORINTERFACETHREAD_H
