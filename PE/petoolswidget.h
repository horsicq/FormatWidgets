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
#ifndef PETOOLSWIDGET_H
#define PETOOLSWIDGET_H

#include "xpe.h"
#include "xshortcutswidget.h"
#include "xdialogprocess.h"
#include "dumpprocess.h"

namespace Ui {
class PEToolsWidget;
}

class PEToolsWidget : public XShortcutsWidget {
    Q_OBJECT

public:
    explicit PEToolsWidget(QWidget *pParent = nullptr);
    ~PEToolsWidget();

    void setData(QIODevice *pDevice, bool bIsImage = false, XADDR nModuleAddress = -1);
    void reload();
    void setReadonly(bool bState);
    bool saveBackup();
    virtual void adjustView();
    virtual void reloadData(bool bSaveSelection);

    static void dumpRegion(QWidget *pParent, QIODevice *pDevice, qint64 nOffset, qint64 nSize, const QString &sName);
    static void dumpOverlay(QWidget *pParent, QIODevice *pDevice, bool bIsImage = false, XADDR nModuleAddress = -1);
    static void dumpDosStub(QWidget *pParent, QIODevice *pDevice, bool bIsImage = false, XADDR nModuleAddress = -1);

protected:
    virtual void registerShortcuts(bool bState);

private slots:
    void on_pushButtonDosStubAdd_clicked();
    void on_pushButtonDosStubRemove_clicked();
    void on_pushButtonDosStubDump_clicked();
    void on_pushButtonOverlayAdd_clicked();
    void on_pushButtonOverlayRemove_clicked();
    void on_pushButtonOverlayDump_clicked();

private:
    Ui::PEToolsWidget *ui;
    QIODevice *m_pDevice;
    bool m_bIsImage;
    XADDR m_nModuleAddress;
    bool m_bReadonly;
};

#endif  // PETOOLSWIDGET_H
