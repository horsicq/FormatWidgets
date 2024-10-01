/* Copyright (c) 2020-2024 hors<horsicq@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef FORMATSWIDGET_H
#define FORMATSWIDGET_H

#include "dialogarchive.h"
#include "dialogbinary.h"
#include "dialogdex.h"
#include "dialogelf.h"
#include "dialogentropy.h"
#include "dialoghash.h"
#include "dialoghexview.h"
#include "dialogle.h"
#include "dialogmach.h"
#include "dialogmachofat.h"
#include "dialogmemorymap.h"
#include "dialogmime.h"
#include "dialogmsdos.h"
#include "dialogmultidisasm.h"
#include "dialogne.h"
#include "dialogpe.h"
#include "dialogsearchsignatures.h"
#include "dialogsearchstrings.h"
#include "dialogtextinfo.h"
#include "dialogxextractor.h"
#include "dialogxfileinfo.h"
#include "dialogxvirustotal.h"
#include "xformats.h"
#include "xshortcuts.h"

namespace Ui {
class FormatsWidget;
}

class FormatsWidget : public XShortcutsWidget {
    Q_OBJECT

    enum TABINFO {
        TABINFO_BINARY = 0,
        TABINFO_ARCHIVE,
        TABINFO_COM,
        TABINFO_DEX,
        TABINFO_NE,
        TABINFO_LE,
        TABINFO_MSDOS,
        TABINFO_PE,
        TABINFO_ELF,
        TABINFO_MACH,
    };

public:
    explicit FormatsWidget(QWidget *pParent = nullptr);
    ~FormatsWidget();

    void setName(const QString &sFileName, bool bScan);
    void setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions);
    virtual void adjustView();
    void setAdvanced(bool bState);

private slots:
    void on_comboBoxFileType_currentIndexChanged(int nIndex);
    void on_comboBoxScanEngine_currentIndexChanged(int nIndex);
    void reload();
    void scan();
    void on_toolButtonEntryPoint_clicked();
    void on_toolButtonMemoryMap_clicked();
    void on_toolButtonPE_clicked();
    void on_toolButtonPEExport_clicked();
    void on_toolButtonPEImport_clicked();
    void on_toolButtonPEResources_clicked();
    void on_toolButtonPEOverlay_clicked();
    void on_toolButtonPENET_clicked();
    void on_toolButtonPESections_clicked();
    void on_toolButtonPEManifest_clicked();
    void on_toolButtonPEVersion_clicked();
    void on_toolButtonMSDOSOverlay_clicked();
    void on_toolButtonMSDOS_clicked();
    void on_toolButtonPETLS_clicked();
    void on_toolButtonELF_clicked();
    void on_toolButtonELFSections_clicked();
    void on_toolButtonELFPrograms_clicked();
    void on_toolButtonLE_clicked();
    void on_toolButtonNE_clicked();
    void on_toolButtonMACH_clicked();
    void on_toolButtonMACHSegments_clicked();
    void on_toolButtonMACHSections_clicked();
    void on_toolButtonMACHCommands_clicked();
    void on_toolButtonMACHLibraries_clicked();
    void on_toolButtonDEX_clicked();
    void on_toolButtonArchive_clicked();
    void on_toolButtonBinary_clicked();
    void showType(SBINARY::TYPE type);
    static qint32 convertType(XBinary::FT fileType, SBINARY::TYPE type);
    void showMSDOS(SMSDOS::TYPE type);
    void showLE(SLE::TYPE type);
    void showNE(SNE::TYPE type);
    void showPE(SPE::TYPE type);
    void showELF(SELF::TYPE type);
    void showMACH(SMACH::TYPE type);
    void showDEX(SDEX::TYPE type);
    void showBinary(SBINARY::TYPE type);
    XBinary::FT getCurrentFileType();
    QString getScanEngine(const QString &sDefault);
    void adjustScanTab(const QString &sIndex);
    void onScanStarted();
    void onScanFinished();
    void on_toolButtonFileInfo_clicked();
    void on_toolButtonMIME_clicked();
    void on_toolButtonStrings_clicked();
    void on_toolButtonSignatures_clicked();
    void on_toolButtonHex_clicked();
    void on_toolButtonEntropy_clicked();
    void on_toolButtonVirusTotal_clicked();
    void on_toolButtonHash_clicked();
    void on_toolButtonExtractor_clicked();
    void on_toolButtonSearch_clicked();
    void on_toolButtonUnpack_clicked();
    void on_toolButtonFiles_clicked();
    void on_toolButtonYARA_clicked();
    void on_toolButtonDisasm_clicked();
    void on_toolButtonVisualization_clicked();
    void _showNfdInfo();
    void _showYaraInfo();
    void _currentFileType(qint32 nFT);
    void on_toolButtonMANIFESTMF_clicked();
    void on_toolButtonAndroidManifest_clicked();

protected:
    virtual void registerShortcuts(bool bState);

private:
    Ui::FormatsWidget *ui;
    QString g_sFileName;
    bool g_bScan;
};

#endif  // FORMATSWIDGET_H
