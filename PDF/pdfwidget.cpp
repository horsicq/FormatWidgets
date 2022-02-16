/* Copyright (c) 2022 hors<horsicq@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "pdfwidget.h"
#include "ui_pdfwidget.h"

PDFWidget::PDFWidget(QWidget *pParent) :
    FormatWidget(pParent),
    ui(new Ui::PDFWidget)
{
    ui->setupUi(this);

    initWidget();
}

PDFWidget::~PDFWidget()
{
    delete ui;
}

void PDFWidget::clear()
{

}

void PDFWidget::cleanup()
{

}

void PDFWidget::reload()
{

}

FormatWidget::SV PDFWidget::_setValue(QVariant vValue, int nStype, int nNdata, int nVtype, int nPosition, qint64 nOffset)
{
    return SV_NONE;
}

void PDFWidget::setReadonly(bool bState)
{

}

void PDFWidget::blockSignals(bool bState)
{

}

void PDFWidget::reloadData()
{

}
