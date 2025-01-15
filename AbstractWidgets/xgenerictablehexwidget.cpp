/* Copyright (c) 2025 hors<horsicq@gmail.com>
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
#include "xgenerictablehexwidget.h"
#include "ui_xgenerictablehexwidget.h"

XGenericTableHexWidget::XGenericTableHexWidget(QWidget *parent) :
    XFormatWidget(parent),
    ui(new Ui::XGenericTableHexWidget)
{
    ui->setupUi(this);

    connect(ui->widgetMain, SIGNAL(currentLocationChanged(quint64, qint32, qint64)), this, SIGNAL(currentLocationChanged(quint64, qint32, qint64)));
    connect(ui->widgetMain, SIGNAL(dataChanged(qint64, qint64)), this, SIGNAL(dataChanged(qint64, qint64)));
    connect(ui->widgetMain, SIGNAL(followLocation(quint64, qint32, qint64, qint32)), this, SLOT(followLocationSlot(quint64, qint32, qint64, qint32)));
    connect(ui->pageHex, SIGNAL(currentLocationChanged(quint64, qint32, qint64)), this, SIGNAL(currentLocationChanged(quint64, qint32, qint64)));
    connect(ui->pageHex, SIGNAL(dataChanged(qint64, qint64)), this, SIGNAL(dataChanged(qint64, qint64)));
    connect(ui->pageHex, SIGNAL(followLocation(quint64, qint32, qint64, qint32)), this, SIGNAL(followLocation(quint64, qint32, qint64, qint32)));
}

XGenericTableHexWidget::~XGenericTableHexWidget()
{
    delete ui;
}

void XGenericTableHexWidget::reloadData(bool bSaveSelection)
{
    ui->widgetMain->reloadData(bSaveSelection);
    ui->pageHex->reloadData(bSaveSelection);
}

void XGenericTableHexWidget::adjustView()
{
    ui->widgetMain->adjustView();
    ui->pageHex->adjustView();
}

void XGenericTableHexWidget::setCwOptions(const XFW_DEF::CWOPTIONS &cwOptions, bool bReload)
{
    ui->widgetMain->setCwOptions(cwOptions, bReload);
    // ui->pageHex->setCwOptions(cwOptions, bReload);
}

void XGenericTableHexWidget::followLocationSlot(quint64 nLocation, qint32 nLocationType, qint64 nSize, qint32 nWidgetType)
{
    if (nWidgetType == XOptions::WIDGETTYPE_CUSTOM) {
        if (nSize) {
            XFW_DEF::CWOPTIONS cwOptions = *(ui->widgetMain->getCwOptions());
            cwOptions.nDataOffset = nLocation;
            cwOptions.nDataSize = nSize;
            ui->pageHex->setCwOptions(cwOptions, true);
            ui->stackedWidgetHex->setCurrentIndex(1);
        } else {
            ui->stackedWidgetHex->setCurrentIndex(0);
        }
    } else {
        emit followLocation(nLocation, nLocationType, nSize, nWidgetType);
    }
}
