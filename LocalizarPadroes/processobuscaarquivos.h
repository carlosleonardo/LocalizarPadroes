#ifndef PROCESSOBUSCAARQUIVOS_H
#define PROCESSOBUSCAARQUIVOS_H

#include <QThread>
#include <string>

class ControleLocalizarPadroes;


class ProcessoBuscaArquivos : public QThread
{
    Q_OBJECT
public:
    explicit ProcessoBuscaArquivos(QObject *parent = 0);

protected:
    void run();
    
signals:

public slots:
    
};

#endif // PROCESSOBUSCAARQUIVOS_H
