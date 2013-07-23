#ifndef TELATESTAREXPRESSAOREGULAR_H
#define TELATESTAREXPRESSAOREGULAR_H

#include <QDialog>

namespace Ui {
class TelaTestarExpressaoRegular;
}

class TelaTestarExpressaoRegular : public QDialog
{
    Q_OBJECT
    
public:
    explicit TelaTestarExpressaoRegular(QWidget *parent = 0);
    ~TelaTestarExpressaoRegular();
    
private:
    Ui::TelaTestarExpressaoRegular *ui;
};

#endif // TELATESTAREXPRESSAOREGULAR_H
