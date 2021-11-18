// Copyright (c) 2020-2021 hors<horsicq@gmail.com>
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
#include "dialogmultidisasm.h"
#include "dialogdex.h"
#include "dialogarchive.h"
#include "dialogtextinfo.h"
#include "xshortcuts.h"

namespace Ui {
class FormatsWidget;
}

class FormatsWidget : public XShortcutsWidget
{
    Q_OBJECT

    enum TABINFO
    {
        TABINFO_BINARY=0,
        TABINFO_COM,
        TABINFO_ZIP,
        TABINFO_DEX,
        TABINFO_NE,
        TABINFO_LE,
        TABINFO_MSDOS,
        TABINFO_PE,
        TABINFO_ELF,
        TABINFO_MACH
    };

    enum SE
    {
        SE_AUTO=0,
        SE_DIE,
        SE_NFD
    };   

public:

    explicit FormatsWidget(QWidget *pParent=nullptr);
    ~FormatsWidget();
    void setFileName(QString sFileName,bool bScan);
    void setScanEngine(QString sScanEngine);

private slots:
    void on_comboBoxFileType_currentIndexChanged(int nIndex);
    void on_comboBoxScanEngine_currentIndexChanged(int nIndex);
    void reload();
    void scan();
    void on_pushButtonDisasm_clicked();
    void on_pushButtonHexEntryPoint_clicked();
    void on_pushButtonMemoryMap_clicked();
    void on_pushButtonPE_clicked();
    void on_pushButtonPEExport_clicked();
    void on_pushButtonPEImport_clicked();
    void on_pushButtonPEResources_clicked();
    void on_pushButtonPEOverlay_clicked();
    void on_pushButtonPENET_clicked();
    void on_pushButtonPESections_clicked();
    void on_pushButtonPEManifest_clicked();
    void on_pushButtonPEVersion_clicked();
    void on_pushButtonMSDOSOverlay_clicked();
    void on_pushButtonMSDOS_clicked();
    void on_pushButtonPETLS_clicked();
    void on_pushButtonELF_clicked();
    void on_pushButtonELFSections_clicked();
    void on_pushButtonELFPrograms_clicked();
    void on_pushButtonLE_clicked();
    void on_pushButtonNE_clicked();
    void on_pushButtonMACH_clicked();
    void on_pushButtonMACHSegments_clicked();
    void on_pushButtonMACHSections_clicked();
    void on_pushButtonMACHCommands_clicked();
    void on_pushButtonMACHLibraries_clicked();
    void on_pushButtonDEX_clicked();
    void on_pushButtonZIP_clicked();

    void showMSDOS(SMSDOS::TYPE type);
    void showLE(SLE::TYPE type);
    void showNE(SNE::TYPE type);
    void showPE(SPE::TYPE type);
    void showELF(SELF::TYPE type);
    void showMACH(SMACH::TYPE type);
    void showDEX(SDEX::TYPE type);

    XBinary::FT getCurrentFileType();

    SE getScanEngine(SE seIndex);
    void adjustScanTab(SE seIndex);

    void onScanStarted();
    void onScanFinished();

protected:
    virtual void registerShortcuts(bool bState);

private:
    Ui::FormatsWidget *ui;
    QString g_sFileName;
    bool g_bScan;
};

#endif // FORMATSWIDGET_H
