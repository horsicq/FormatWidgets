/* Copyright (c) 2019-2022 hors<horsicq@gmail.com>
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
#ifndef DIALOGMODELINFO_H
#define DIALOGMODELINFO_H

#include <QDialog>
#include <QFileDialog>
#include <QStandardItemModel>

#include "xbinary.h"
#include "xoptions.h"

namespace Ui {
class DialogModelInfo;
}

class DialogModelInfo : public QDialog {
    Q_OBJECT

   public:
    explicit DialogModelInfo(QWidget *pParent = nullptr);
    ~DialogModelInfo();

    void setData(QIODevice *pDevice, QString sTitle,
                 QStandardItemModel *pModel);

   private slots:
    void on_pushButtonOK_clicked();
    void on_pushButtonSave_clicked();

   private:
    Ui::DialogModelInfo *ui;
    QIODevice *g_pDevice;
    QString g_sTitle;
};

#endif  // DIALOGMODELINFO_H
