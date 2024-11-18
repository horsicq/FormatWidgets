/* Copyright (c) 2017-2024 hors<horsicq@gmail.com>
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
#ifndef FORMATWIDGET_DEF_H
#define FORMATWIDGET_DEF_H

#include <QString>
#include "xbinary.h"
#include "xshortcuts.h"
#include "xoptions.h"

namespace FW_DEF {
struct OPTIONS {
    bool bIsImage;
    qint64 nImageBase;  // TODO default_const
    // TODO Last directory
    qint32 nStartType;
    XBinary::FT fileType;
    bool bOffset;        // Show Offset title no Address TODO remove make auto if Address != nOffset
    QString sTitle;      // For dialogs
    bool bFilter;        // For Archive_widget
    bool bNoWindowOpen;  // For Archive_widget
};

struct CWOPTIONS {
    QWidget *pParent;
    XBinary::FT fileType;
    QIODevice *pDevice;
    qint32 nType;
    XShortcuts *pShortcuts;
    XOptions *pOptions;
    qint64 nDataOffset; // TODO location
    qint64 nDataSize;
    XBinary::ENDIAN endian;
    XBinary::MODE mode;
};

enum TYPE {
    TYPE_INFO = 0,
    TYPE_VISUALIZATION,
    TYPE_VIRUSTOTAL,
    TYPE_HEX,
    TYPE_DISASM,
    TYPE_HASH,
    TYPE_STRINGS,
    TYPE_SIGNATURES,
    TYPE_MEMORYMAP,
    TYPE_ENTROPY,
    TYPE_NFDSCAN,
    TYPE_EXTRACTOR,
    TYPE_SEARCH,
    TYPE_DIESCAN,
    TYPE_YARASCAN,
    TYPE_TOOLS,
    TYPE_MACH_mach_header,
};

enum VAL_TYPE {
    VAL_TYPE_UNKNOWN = 0,
    VAL_TYPE_DATA,
    VAL_TYPE_FLAGS,
    VAL_TYPE_RELADDRESS,
    VAL_TYPE_ADDRESS,
    VAL_TYPE_OFFSET,
    VAL_TYPE_SIZE,
    VAL_TYPE_TEXT,
    VAL_TYPE_LABEL,
    VAL_TYPE_UNIXTIME,  // TODO
    VAL_TYPE_UUID
};

enum INFO {
    INFO_NONE = 0,
    INFO_COMBOBOX,
};

struct HEADER_RECORD {
    qint32 nPosition;
    QString sName;
    qint32 nOffset;
    qint32 nSize;
    QString sType;
    VAL_TYPE vtype;
    INFO info;
};

enum SECTION_DATA {
    SECTION_DATA_TYPE = 0,
    SECTION_DATA_NUMBER,
    SECTION_DATA_NAME,
    SECTION_DATA_OFFSET,
    SECTION_DATA_SIZE,
    SECTION_DATA_ADDRESS,
    //        SECTION_DATA_RELADDRESS,
    SECTION_DATA_VSIZE,
    SECTION_DATA_EXTRAOFFSET,
    SECTION_DATA_EXTRASIZE,
    SECTION_DATA_STRINGTABLEOFFSET,
    SECTION_DATA_STRINGTABLESIZE,
    SECTION_DATA_HEADEROFFSET,
    SECTION_DATA_VALUE1,
    SECTION_DATA_VALUE2,
    SECTION_DATA_VALUE3
};

enum WIDGET_DATA {
    WIDGET_DATA_TYPE = 0,
    WIDGET_DATA_OFFSET,
    WIDGET_DATA_SIZE,
    WIDGET_DATA_MODE,
    WIDGET_DATA_ENDIAN,
    WIDGET_DATA_EXTRAOFFSET,
    WIDGET_DATA_EXTRASIZE,
    WIDGET_DATA_NAME,
};
}  // namespace FW_DEF

#endif  // FORMATWIDGET_DEF_H
