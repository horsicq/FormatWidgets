/* Copyright (c) 2019-2025 hors<horsicq@gmail.com>
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
#ifndef DIALOGNE_H
#define DIALOGNE_H

#include <QDialog>

#include "newidget.h"

namespace Ui {
class DialogNE;
}

class DialogNE : public XShortcutsDialog {
    Q_OBJECT

public:
    explicit DialogNE(QWidget *pParent = nullptr);
    ~DialogNE();

    virtual void adjustView();

    void setData(QIODevice *pDevice, FW_DEF::OPTIONS options);
    void setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions);

protected:
    virtual void registerShortcuts(bool bState);

private:
    Ui::DialogNE *ui;
};

#endif  // DIALOGNE_H
