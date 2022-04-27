/* Copyright (c) 2017-2022 hors<horsicq@gmail.com>
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
#include "searchsignaturesoptionswidget.h"
#include "ui_searchsignaturesoptionswidget.h"

SearchSignaturesOptionsWidget::SearchSignaturesOptionsWidget(QWidget *pParent) :
    QWidget(pParent),
    ui(new Ui::SearchSignaturesOptionsWidget)
{
    ui->setupUi(this);

    g_pOptions=nullptr;
}

SearchSignaturesOptionsWidget::~SearchSignaturesOptionsWidget()
{
    delete ui;
}

void SearchSignaturesOptionsWidget::setOptions(XOptions *pOptions)
{
    g_pOptions=pOptions;

    reload();
}

void SearchSignaturesOptionsWidget::save()
{
    g_pOptions->getLineEdit(ui->lineEditSearchSignaturesPath,XOptions::ID_SIGNATURES_PATH);
}

void SearchSignaturesOptionsWidget::setDefaultValues(XOptions *pOptions)
{
    pOptions->addID(XOptions::ID_SIGNATURES_PATH,"$data/signatures");
}

void SearchSignaturesOptionsWidget::reload()
{
    g_pOptions->setLineEdit(ui->lineEditSearchSignaturesPath,XOptions::ID_SIGNATURES_PATH);
}

void SearchSignaturesOptionsWidget::on_toolButtonSearchSignaturesPath_clicked()
{
    QString sText=ui->lineEditSearchSignaturesPath->text();
    QString sInitDirectory=XBinary::convertPathName(sText);

    QString sDirectoryName=QFileDialog::getExistingDirectory(this,tr("Open directory")+QString("..."),sInitDirectory,QFileDialog::ShowDirsOnly);

    if(!sDirectoryName.isEmpty())
    {
        ui->lineEditSearchSignaturesPath->setText(sDirectoryName);
    }
}
