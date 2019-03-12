#ifndef DIALOGSECTIONHEADER_H
#define DIALOGSECTIONHEADER_H

#include <QDialog>
#include "sectionheaderwidget.h"

namespace Ui {
class DialogSectionHeader;
}

class DialogSectionHeader : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSectionHeader(QWidget *parent = nullptr);
    ~DialogSectionHeader();
    void setData(QIODevice *pDevice, FormatWidget::OPTIONS *pOptions, quint32 nNumber);
private:
    Ui::DialogSectionHeader *ui;
};

#endif // DIALOGSECTIONHEADER_H
