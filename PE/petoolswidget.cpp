/* Copyright (c) 2017-2025 hors<horsicq@gmail.com>
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
#include "petoolswidget.h"

#include "ui_petoolswidget.h"

PEToolsWidget::PEToolsWidget(QWidget *pParent) : XShortcutsWidget(pParent), ui(new Ui::PEToolsWidget)
{
    ui->setupUi(this);

    g_pDevice = nullptr;
    g_bIsImage = false;
    g_nModuleAddress = -1;
    g_bReadonly = true;
}

PEToolsWidget::~PEToolsWidget()
{
    delete ui;
}

void PEToolsWidget::setData(QIODevice *pDevice, bool bIsImage, XADDR nModuleAddress)
{
    g_pDevice = pDevice;
    g_bIsImage = bIsImage;
    g_nModuleAddress = nModuleAddress;

    reload();
}

void PEToolsWidget::reload()
{
    if (g_pDevice) {
        XPE pe(g_pDevice, g_bIsImage, g_nModuleAddress);

        if (pe.isValid()) {
            bool bIsDosStubPresent = pe.isDosStubPresent();
            bool bIsOverlayPresent = pe.isOverlayPresent();

            ui->pushButtonDosStubAdd->setEnabled((!bIsDosStubPresent) && (!g_bReadonly));
            ui->pushButtonDosStubRemove->setEnabled(bIsDosStubPresent && (!g_bReadonly));
            ui->pushButtonDosStubDump->setEnabled(bIsDosStubPresent);

            ui->pushButtonOverlayAdd->setEnabled((!bIsOverlayPresent) && (!g_bReadonly));
            ui->pushButtonOverlayRemove->setEnabled(bIsOverlayPresent && (!g_bReadonly));
            ui->pushButtonOverlayDump->setEnabled(bIsOverlayPresent);
        }
    }
}

void PEToolsWidget::setReadonly(bool bState)
{
    g_bReadonly = bState;

    reload();
}

bool PEToolsWidget::saveBackup()
{
    bool bResult = true;

    if (getGlobalOptions()->isSaveBackup()) {
        // Save backup
        bResult = XBinary::saveBackup(g_pDevice);
    }

    return bResult;
}

void PEToolsWidget::adjustView()
{
}

void PEToolsWidget::reloadData(bool bSaveSelection)
{
    Q_UNUSED(bSaveSelection)
    reload();
}

void PEToolsWidget::dumpRegion(QWidget *pParent, QIODevice *pDevice, qint64 nOffset, qint64 nSize, const QString &sName)
{
    QString _sName = sName;
    if (_sName == "") {
        _sName = tr("Dump");
    }

    QString sSaveFileName = XBinary::getResultFileName(pDevice, QString("%1.bin").arg(_sName));
    QString sFileName = QFileDialog::getSaveFileName(pParent, tr("Save dump"), sSaveFileName, QString("%1 (*.bin)").arg(tr("Raw data")));

    if (!sFileName.isEmpty()) {
        DumpProcess dumpProcess;
        XDialogProcess dd(pParent, &dumpProcess);
        // dd.setGlobal(getShortcuts(), getGlobalOptions());
        dumpProcess.setData(pDevice, nOffset, nSize, sFileName, DumpProcess::DT_DUMP_DEVICE_OFFSET, dd.getPdStruct());
        dd.start();
        dd.showDialogDelay();
    }
}

void PEToolsWidget::dumpOverlay(QWidget *pParent, QIODevice *pDevice, bool bIsImage, XADDR nModuleAddress)
{
    XPE pe(pDevice, bIsImage, nModuleAddress);

    if (pe.isValid()) {
        qint64 nOffset = pe.getOverlayOffset();
        qint64 nSize = pe.getOverlaySize();

        dumpRegion(pParent, pDevice, nOffset, nSize, tr("Overlay"));
    }
}

void PEToolsWidget::dumpDosStub(QWidget *pParent, QIODevice *pDevice, bool bIsImage, XADDR nModuleAddress)
{
    XPE pe(pDevice, bIsImage, nModuleAddress);

    if (pe.isValid()) {
        qint64 nOffset = pe.getDosStubOffset();
        qint64 nSize = pe.getDosStubSize();

        dumpRegion(pParent, pDevice, nOffset, nSize, QString("DOSStub"));
    }
}

void PEToolsWidget::registerShortcuts(bool bState)
{
    Q_UNUSED(bState)
}

void PEToolsWidget::on_pushButtonDosStubAdd_clicked()
{
    if (saveBackup()) {
        QString sFileName = XShortcutsWidget::getOpenFileName(XBinary::getDeviceDirectory(g_pDevice));

        if (!sFileName.isEmpty()) {
            if (!sFileName.isEmpty()) {
                XPE pe(g_pDevice, g_bIsImage, g_nModuleAddress);

                if (pe.isValid()) {
                    if (pe.addDosStub(sFileName)) {
                        emit dataChanged(0, g_pDevice->size());
                    }
                }
            }
        }

        reload();
    }
}

void PEToolsWidget::on_pushButtonDosStubRemove_clicked()
{
    if (saveBackup()) {
        XPE pe(g_pDevice, g_bIsImage, g_nModuleAddress);

        if (pe.isValid()) {
            if (pe.removeDosStub()) {
                emit dataChanged(0, g_pDevice->size());
            }
        }

        reload();
    }
}

void PEToolsWidget::on_pushButtonDosStubDump_clicked()
{
    dumpDosStub(this, g_pDevice, g_bIsImage, g_nModuleAddress);
}

void PEToolsWidget::on_pushButtonOverlayAdd_clicked()
{
    if (saveBackup()) {
        QString sFileName = XShortcutsWidget::getOpenFileName(XBinary::getDeviceDirectory(g_pDevice));

        if (!sFileName.isEmpty()) {
            XPE pe(g_pDevice, g_bIsImage, g_nModuleAddress);

            if (pe.isValid()) {
                if (pe.addOverlay(sFileName)) {
                    emit dataChanged(0, g_pDevice->size());
                }
            }
        }

        void reload();
    }
}

void PEToolsWidget::on_pushButtonOverlayRemove_clicked()
{
    if (saveBackup()) {
        XPE pe(g_pDevice, g_bIsImage, g_nModuleAddress);

        if (pe.isValid()) {
            if (pe.removeOverlay()) {
                emit dataChanged(0, g_pDevice->size());
            }
        }

        reload();
    }
}

void PEToolsWidget::on_pushButtonOverlayDump_clicked()
{
    dumpOverlay(this, g_pDevice, g_bIsImage, g_nModuleAddress);
}
