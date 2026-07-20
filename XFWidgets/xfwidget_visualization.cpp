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

#include "xfwidget_visualization.h"

#include <QVBoxLayout>

XFWidget_Visualization::XFWidget_Visualization(QWidget *pParent) : XShortcutsWidget(pParent)
{
    m_pVisualizationWidget = new XVisualizationWidget(this);

    QVBoxLayout *pLayout = new QVBoxLayout(this);
    pLayout->setContentsMargins(0, 0, 0, 0);
    pLayout->addWidget(m_pVisualizationWidget);
}

XFWidget_Visualization::~XFWidget_Visualization()
{
    clear();
}

void XFWidget_Visualization::setData(const XBinary::INDATA &inData)
{
    m_pVisualizationWidget->setData(inData, false);
}

void XFWidget_Visualization::clear()
{
    m_pVisualizationWidget->clear();
}

void XFWidget_Visualization::reload()
{
    m_pVisualizationWidget->reload();
}

void XFWidget_Visualization::setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions)
{
    XShortcutsWidget::setGlobal(pShortcuts, pXOptions);
    m_pVisualizationWidget->setGlobal(pShortcuts, pXOptions);
}

XVisualizationWidget *XFWidget_Visualization::getVisualizationWidget()
{
    return m_pVisualizationWidget;
}
