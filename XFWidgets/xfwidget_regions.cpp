/* Copyright (c) 2025-2026 hors<horsicq@gmail.com>
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

#include "xfwidget_regions.h"

#include "ui_xfwidget_regions.h"

XFWidget_Regions::XFWidget_Regions(QWidget *pParent) : QWidget(pParent), ui(new Ui::XFWidget_Regions)
{
    ui->setupUi(this);

    m_pCurrentDevice = nullptr;
    m_pXInfoDB = nullptr;
    m_bIsReadonly = false;
}

XFWidget_Regions::~XFWidget_Regions()
{
    clear();

    delete ui;
}

void XFWidget_Regions::setData(const XBinary::INDATA &inData)
{
    XRegionsModel::OPTIONS options = {};
    options.fileType = inData.fileType;
    options.bIsImage = inData.bIsImage;
    options.nModuleAddress = inData.nModuleAddress;

    setData(inData, options);
}

void XFWidget_Regions::setData(const XBinary::INDATA &inData, const XRegionsModel::OPTIONS &options)
{
    clear();

    m_inData = inData;
    m_options = options;

    m_pCurrentDevice = XFormats::createDevice(m_inData);

    if (m_pCurrentDevice) {
        m_pXInfoDB = new XInfoDB(this);
        m_pXInfoDB->setData(m_pCurrentDevice, m_options.fileType);

        ui->widgetRegions->setData(m_pCurrentDevice, m_pXInfoDB, m_options, true);
    }
}

void XFWidget_Regions::clear()
{
    if (m_pXInfoDB) {
        delete m_pXInfoDB;
        m_pXInfoDB = nullptr;
    }

    if (m_pCurrentDevice) {
        XFormats::removeDevice(m_pCurrentDevice, m_inData);
        m_pCurrentDevice = nullptr;
    }
}

void XFWidget_Regions::reload()
{
    if (m_pCurrentDevice && m_pXInfoDB) {
        ui->widgetRegions->reloadData(false);
    }
}

void XFWidget_Regions::setReadonly(bool bIsReadonly)
{
    m_bIsReadonly = bIsReadonly;
}

XRegionsWidget *XFWidget_Regions::getRegionsWidget()
{
    return ui->widgetRegions;
}
