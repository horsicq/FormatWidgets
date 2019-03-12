#ifndef DIALOGELF_H
#define DIALOGELF_H

#include <QDialog>
#include "elfwidget.h"

namespace Ui {
class DialogELF;
}

class DialogELF : public QDialog
{
    Q_OBJECT

public:
    explicit DialogELF(QWidget *parent = nullptr);
    ~DialogELF();

     void setData(QIODevice *pDevice,FormatWidget::OPTIONS *pOptions);
private:
    Ui::DialogELF *ui;
};

#endif // DIALOGELF_H
