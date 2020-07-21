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
#include "dialogdisasm.h"

namespace Ui {
class FormatsWidget;
}

class FormatsWidget : public QWidget
{
    Q_OBJECT

    enum TABINFO
    {
        TABINFO_BINARY=0,
        TABINFO_COM,
        TABINFO_NE,
        TABINFO_LE,
        TABINFO_MSDOS,
        TABINFO_PE,
        TABINFO_ELF,
        TABINFO_MACH
    };

    enum TABSE
    {
        TABSE_DIE=0,
        TABSE_NFD
    };

public:
    explicit FormatsWidget(QWidget *parent=nullptr);
    void setFileName(QString sFileName,bool bScan);
    void setBackupFileName(QString sBackupFileName);
    ~FormatsWidget();
    void setDIEDatabase(QString sDatabasePath);

private slots:
    void on_comboBoxType_currentIndexChanged(int nIndex);
    void on_comboBoxScanEngine_currentIndexChanged(int nIndex);
    void reload();
    void scan();
    void on_pushButtonDisasm_clicked();
    void on_pushButtonMemoryMap_clicked();
    void on_pushButtonPEExport_clicked();
    void on_pushButtonPEImport_clicked();
    void on_pushButtonPEResource_clicked();
    void on_pushButtonPEOverlay_clicked();
    void on_pushButtonPENET_clicked();
    void on_pushButtonPE_clicked();
    void on_pushButtonPESections_clicked();
    void on_pushButtonMSDOSOverlay_clicked();
    void on_pushButtonMSDOS_clicked();
    void on_pushButtonPETLS_clicked();
    void on_pushButtonELF_clicked();
    void on_pushButtonLE_clicked();
    void on_pushButtonNE_clicked();

    void showMSDOS(SMSDOS::TYPE type);
    void showLE(SLE::TYPE type);
    void showNE(SNE::TYPE type);
    void showPE(SPE::TYPE type);
    void showELF(SELF::TYPE type);
    void showMACH(SMACH::TYPE type);

private:
    Ui::FormatsWidget *ui;
    QString sFileName;
    bool bScan;
    QString sBackupFilename;
};

#endif // FORMATSWIDGET_H
