/************************************************************************
 **
 **  @file   vnodespline.h
 **  @author Roman Telezhinsky <dismine@gmail.com>
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

#ifndef VNODESPLINE_H
#define VNODESPLINE_H

#include "vabstractnode.h"
#include <QGraphicsPathItem>

/**
 * @brief The VNodeSpline class
 */
class VNodeSpline:public VAbstractNode, public QGraphicsPathItem
{
    Q_OBJECT
public:
                  /**
                   * @brief VNodeSpline
                   * @param doc dom document container
                   * @param data
                   * @param id
                   * @param idSpline
                   * @param typeCreation
                   * @param parent
                   */
                  VNodeSpline(VDomDocument *doc, VContainer *data, qint64 id, qint64 idSpline,
                              const Tool::Sources &typeCreation, QGraphicsItem * parent = 0);
    /**
     * @brief Create
     * @param doc dom document container
     * @param data
     * @param id
     * @param idSpline
     * @param parse
     * @param typeCreation
     * @return
     */
    static VNodeSpline *Create(VDomDocument *doc, VContainer *data, qint64 id, qint64 idSpline,
                               const Document::Documents &parse, const Tool::Sources &typeCreation);
    /**
     * @brief TagName
     */
    static const QString TagName;
    /**
     * @brief ToolType
     */
    static const QString ToolType;
public slots:
    /**
     * @brief FullUpdateFromFile
     */
    virtual void FullUpdateFromFile ();
protected:
    /**
     * @brief AddToFile
     */
    virtual void AddToFile ();
    /**
     * @brief mouseReleaseEvent
     * @param event
     */
    virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    /**
     * @brief hoverMoveEvent
     * @param event
     */
    virtual void hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    /**
     * @brief hoverLeaveEvent
     * @param event
     */
    virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
private:
    /**
     * @brief RefreshGeometry
     */
    void         RefreshGeometry ();
};

#endif // VNODESPLINE_H
