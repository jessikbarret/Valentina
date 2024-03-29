/************************************************************************
 **
 **  @file   vstandardtablecell.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013 Valentina project
 **  <https://bitbucket.org/dismine/valentina> All Rights Reserved.
 **
 **  Valentina is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Valentina is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 *************************************************************************/

#ifndef VSTANDARDTABLEROW_H
#define VSTANDARDTABLEROW_H

#include <QString>

/**
 * @brief The VMeasurement class keep data row of standard table
 */
class VMeasurement
{
public:
    VMeasurement();
    VMeasurement(const qreal &base, const qreal &ksize, const qreal &kheight,
                 const QString &gui_text = QString(), const QString &description = QString(),
                 const QString &TagName = QString());
    VMeasurement(const qreal &base, const QString &gui_text = QString(),
                 const QString &description = QString(), const QString &TagName = QString());
    VMeasurement(const VMeasurement &m);
    VMeasurement &operator=(const VMeasurement &m);
    ~VMeasurement(){}
    qreal   GetBase() const;
    void    setBase(const qreal &value);
    qreal   GetKsize() const;
    qreal   GetKheight() const;
    QString GetGuiText() const;
    QString GetDescription() const;
    qreal   GetValue() const;
    qreal   GetValue(const qreal &size, const qreal &height) const;
    QString TagName() const;
    void    setTagName(const QString &TagName);
private:
    /** @brief base value in base size and growth */
    qreal          base;

    /** @brief ksize increment in sizes */
    qreal          ksize;

    /** @brief kgrowth increment in growths */
    qreal          kheight;

    /** @brief description description measurement */
    QString        gui_text;

    QString        description;
    QString        _tagName;
};

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetBase return value in base size and growth
 * @return value
 */
inline qreal VMeasurement::GetBase() const
{
    return base;
}

//---------------------------------------------------------------------------------------------------------------------
inline void VMeasurement::setBase(const qreal &value)
{
    base = value;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetKsize return increment in sizes
 * @return increment
 */
inline qreal VMeasurement::GetKsize() const
{
    return ksize;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetKheight return increment in heights
 * @return increment
 */
inline qreal VMeasurement::GetKheight() const
{
    return kheight;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetGuiText measurement name for tooltip
 * @return measurement name
 */
inline QString VMeasurement::GetGuiText() const
{
    return gui_text;
}

//---------------------------------------------------------------------------------------------------------------------
inline QString VMeasurement::GetDescription() const
{
    return description;
}

//---------------------------------------------------------------------------------------------------------------------
inline qreal VMeasurement::GetValue() const
{
    return base;
}

//---------------------------------------------------------------------------------------------------------------------
inline QString VMeasurement::TagName() const
{
    return _tagName;
}

//---------------------------------------------------------------------------------------------------------------------
inline void VMeasurement::setTagName(const QString &tagName)
{
    _tagName = tagName;
}

#endif // VSTANDARDTABLEROW_H
