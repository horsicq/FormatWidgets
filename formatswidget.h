// Copyright (c) 2020 hors<horsicq@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
#ifndef FORMATSWIDGET_H
#define FORMATSWIDGET_H

#include <QWidget>
#include "xformats.h"
#include "dialogmemorymap.h"
#include "dialogmsdos.h"
#include "dialogne.h"
#include "dialogle.h"
#include "dialogpe.h"
#include "dialogelf.h"
#include "dialogmach.h"

namespace Ui {
class FormatsWidget;
}

class FormatsWidget : public QWidget
{
    Q_OBJECT

    enum TAB
    {
        TAB_BINARY=0,
        TAB_COM,
        TAB_MSDOS,
        TAB_PE,
        TAB_ELF,
        TAB_MACH
    };

public:
    explicit FormatsWidget(QWidget *parent=nullptr);
    void setFileName(QString sFileName);
    void setBackupFileName(QString sBackupFileName);
    ~FormatsWidget();

private slots:
    void on_comboBoxType_currentIndexChanged(int index);
    void reload();
    void on_pushButtonDisasm_clicked();
    void on_pushButtonMemoryMap_clicked();
    void on_pushButtonPEExport_clicked();
    void on_pushButtonPEImport_clicked();
    void on_pushButtonPEResource_clicked();
    void on_pushButtonPEOverlay_clicked();
    void on_pushButtonPENET_clicked();
    void on_pushButtonPE_clicked();
    void on_pushButtonPESections_clicked();
    void showMSDOS(SMSDOS::TYPE type);
    void showLE(SLE::TYPE type);
    void showNE(SNE::TYPE type);
    void showPE(SPE::TYPE type);
    void showELF(SELF::TYPE type);
    void showMACH(SMACH::TYPE type);
    void on_pushButtonMSDOSOverlay_clicked();
    void on_pushButtonMSDOS_clicked();

private:
    Ui::FormatsWidget *ui;
    QString sFileName;
    QString sBackupFilename;
};

#endif // FORMATSWIDGET_H
