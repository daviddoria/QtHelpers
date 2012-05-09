/*=========================================================================
 *
 *  Copyright David Doria 2011 daviddoria@gmail.com
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#ifndef QtHelpers_H
#define QtHelpers_H

// Qt
#include <QColor>
#include <QImage>
#include <QMetaType>
class QGraphicsView;
class QTableWidget;

namespace QtHelpers
{
/** Convert a QColor to an unsigned char[3] */
void QColorToUCharColor(const QColor& color, unsigned char outputColor[3]);

/** Get a columns location in the table based on its header string */
bool GetColumnIdByHeader(const QTableWidget* table, const std::string& header, int& columnId);

/** Scale an image so that it fits in a QGraphicsView */
QImage FitToGraphicsView(const QImage qimage, const QGraphicsView* gfx);

/** Change the center pixel (in-place) to the specified 'color' */
void HighlightCenterPixel(QImage& qimage, const QColor& color);

/** Set all of the pixels in 'qimage' to 'color' */
void SetImageToConstant(QImage& qimage, const QColor& color);

} // end namespace

#include "QtHelpers.hpp"

#endif
