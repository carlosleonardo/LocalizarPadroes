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
    void interromper();
    bool interrompido();

    virtual AdaptadorInterfaceThread* obterInstancia()=0;
protected:

    virtual void bloquear()=0;
    virtual void desbloquear()=0;
    volatile bool cancelado;
};

#endif // ADAPTADORINTERFACETHREAD_H
