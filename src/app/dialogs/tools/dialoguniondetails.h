/************************************************************************
 **
 **  @file   dialoguniondetails.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   23 12, 2013
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

#ifndef DIALOGUNIONDETAILS_H
#define DIALOGUNIONDETAILS_H

#include "dialogtool.h"

namespace Ui
{
    class DialogUnionDetails;
}

/**
 * @brief The DialogUnionDetails class dialog for ToolUnionDetails.
 */
class DialogUnionDetails : public DialogTool
{
    Q_OBJECT
public:
    DialogUnionDetails(const VContainer *data, QWidget *parent = nullptr);
    ~DialogUnionDetails();

    quint32          getD1() const;
    quint32          getD2() const;
    ptrdiff_t        getIndexD1() const;
    ptrdiff_t        getIndexD2() const;
public slots:
    void             ChoosedObject(quint32 id, const SceneObject &type);
    virtual void     DialogAccepted();
    /** TODO ISSUE 79 : create real function
     * @brief DialogApply apply data and emit signal about applied dialog.
     */
    virtual void      DialogApply(){}
private:
    Q_DISABLE_COPY(DialogUnionDetails)

    /** @brief ui keeps information about user interface */
    Ui::DialogUnionDetails *ui;

    /** @brief indexD1 index edge first detail */
    ptrdiff_t        indexD1;

    /** @brief indexD2 index edge second detail */
    ptrdiff_t        indexD2;

    /** @brief d1 id first detail */
    quint32           d1;

    /** @brief d2 id second detail */
    quint32           d2;

    /** @brief numberD number of detail, what we already have */
    qint32           numberD;

    /** @brief numberP number of points, what we already have */
    qint32           numberP;

    /** @brief p1 id first point of detail */
    quint32           p1;

    /** @brief p2 id second point of detail */
    quint32           p2;

    bool             CheckObject(const quint32 &id, const quint32 &idDetail) const;
    void             ChoosedDetail(const quint32 &id, const SceneObject &type, quint32 &idDetail,
                                   ptrdiff_t &index);
};

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getD1 return id first detail
 * @return id
 */
inline quint32 DialogUnionDetails::getD1() const
{
    return d1;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getD2 return id second detail
 * @return id
 */
inline quint32 DialogUnionDetails::getD2() const
{
    return d2;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getIndexD1 return index edge first detail
 * @return index
 */
inline ptrdiff_t DialogUnionDetails::getIndexD1() const
{
    return indexD1;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getIndexD2 return index edge second detail
 * @return index
 */
inline ptrdiff_t DialogUnionDetails::getIndexD2() const
{
    return indexD2;
}

#endif // DIALOGUNIONDETAILS_H
