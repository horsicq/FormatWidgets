#ifndef DIALOGPE_H
#define DIALOGPE_H

#include <QDialog>
#include "pewidget.h"

namespace Ui {
class DialogPE;
}

class DialogPE : public QDialog
{
    Q_OBJECT

public:
    explicit DialogPE(QWidget *parent = nullptr);
    ~DialogPE();
    void setData(QIODevice *pDevice,FormatWidget::OPTIONS *pOptions);
private:
    Ui::DialogPE *ui;
};

#endif // DIALOGPE_H
