/************************************************************************
 **
 **  @file   vdetail.h
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

#ifndef VDETAIL_H
#define VDETAIL_H

#include "vnodedetail.h"

#include <QString>
#include <QVector>

namespace Detail
{
    /**
     * @brief The Contour enum OpenContour - first and last points contour don't match,
     * CloseContour - first and last points contour match.
     */
    enum Contour { OpenContour, CloseContour };
    Q_DECLARE_FLAGS(Contours, Contour)

    /**
     * @brief The Equidistant enum OpenEquidistant - first and last points equidistant don't match,
     * CloseEquidistant - first and last points equidistant match.
     */
    enum Equidistant { OpenEquidistant, CloseEquidistant };
    Q_DECLARE_FLAGS(Equidistants, Equidistant)
}
Q_DECLARE_OPERATORS_FOR_FLAGS(Detail::Contours)
Q_DECLARE_OPERATORS_FOR_FLAGS(Detail::Equidistants)

/**
 * @brief The VDetail class for path of object (points, arcs, splines).
 */
class VDetail
{
public:
                   /**
                    * @brief VDetail default contructor. Create empty detail.
                    */
                   VDetail();
                   /**
                    * @brief VDetail constructor.
                    * @param name detail name.
                    * @param nodes list of nodes.
                    */
                   VDetail(const QString &name, const QVector<VNodeDetail> &nodes);
                   /**
                    * @brief VDetail copy constructor.
                    * @param detail detail.
                    */
                   VDetail(const VDetail &detail);
                   /**
                    * @brief operator = assignment operator.
                    * @param detail detail.
                    * @return new detail.
                    */
                   VDetail &operator=(const VDetail &detail);
    /**
     * @brief append append in the end of list node.
     * @param node new node.
     */
    inline void    append(const VNodeDetail &node) {nodes.append(node);}
    /**
     * @brief Clear detail full clear.
     */
    void           Clear();
    /**
     * @brief ClearNodes clear list of nodes.
     */
    void           ClearNodes();
    /**
     * @brief CountNode return count nodes.
     * @return count.
     */
    inline qint32  CountNode() const {return nodes.size();}
    /**
     * @brief Containes check if detail containe this id.
     * @param id object id.
     * @return true if containe.
     */
    bool           Containes(const qint64 &id)const;
    /**
     * @brief operator [] find node by index in list.
     * @param indx index node in list.
     * @return node
     */
    VNodeDetail &  operator[](ptrdiff_t indx);
    /**
     * @brief at find node by index in list.
     * @param indx index node in list.
     * @return const node.
     */
    const VNodeDetail & at ( ptrdiff_t indx ) const;
    /**
     * @brief getName return detail name.
     * @return name.
     */
    inline QString getName() const {return name;}
    /**
     * @brief setName set detail name.
     * @param value new name.
     */
    inline void    setName(const QString &value) {name = value;}
    /**
     * @brief getMx return bias for X axis.
     * @return x bias.
     */
    inline qreal   getMx() const {return mx;}
    /**
     * @brief setMx set bias for X axis.
     * @param value new x bias.
     */
    inline void    setMx(const qreal &value) {mx = value;}
    /**
     * @brief getMy get bias for y axis.
     * @return y axis.
     */
    inline qreal   getMy() const {return my;}
    /**
     * @brief setMy set bias for y axis.
     * @param value new y bias.
     */
    inline void    setMy(const qreal &value) {my = value;}
    /**
     * @brief getSeamAllowance keep status for seam allowance detail.
     * @return true - need seam allowance, false - no need seam allowance.
     */
    inline bool    getSeamAllowance() const {return seamAllowance;}
    /**
     * @brief setSeamAllowance set status for seam allowance detail.
     * @param value true - need seam allowance, false - no need seam allowance.
     */
    inline void    setSeamAllowance(bool value) {seamAllowance = value;}
    /**
     * @brief getClosed keep close status for detail equdistant.
     * @return true - close equdistant, false - don't close equdistant.
     */
    inline bool    getClosed() const {return closed;}
    /**
     * @brief setClosed set close status for detail equdistant.
     * @param value true - close equdistant, false - don't close equdistant.
     */
    inline void    setClosed(bool value) {closed = value;}
    /**
     * @brief getWidth return value detail seam allowance.
     * @return value in mm.
     */
    inline qreal   getWidth() const {return width;}
    /**
     * @brief setWidth set value detail seam allowance.
     * @param value width in mm.
     */
    inline void    setWidth(const qreal &value) {width = value;}
    /**
     * @brief getNodes return list of nodes.
     * @return list of nodes.
     */
    inline QVector<VNodeDetail> getNodes() const {return nodes;}
    /**
     * @brief setNodes set list of nodes
     * @param value list of nodes
     */
    inline void    setNodes(const QVector<VNodeDetail> &value) {nodes = value;}
    /**
     * @brief indexOfNode return index in list node using id object.
     * @param id object (arc, point, spline, splinePath) id.
     * @return index in list or -1 id can't find.
     */
    ptrdiff_t      indexOfNode(const qint64 &id) const;
    /**
     * @brief id return id detail in list data.
     * @return id.
     */
    qint64         id() const;
    /**
     * @brief setId set id detail in list data.
     * @param id detail id.
     */
    void           setId(const qint64 &id);
    /**
     * @brief OnEdge checks if two poins located on the edge. Edge is line between two points. If between two points
     * located arcs or splines ignore this.
     * @param p1 id first point.
     * @param p2 id second point.
     * @return true - on edge, false - no.
     */
    bool           OnEdge(const qint64 &p1, const qint64 &p2)const;
    /**
     * @brief Edge return edge index in detail. Edge is line between two points. If between two points
     * located arcs or splines ignore this.
     * @param p1 id first point.
     * @param p2 id second point.
     * @return edge index or -1 if points don't located on edge
     */
    ptrdiff_t      Edge(const qint64 &p1, const qint64 &p2)const;
    /**
     * @brief NodeOnEdge return nodes located on edge with index.
     * @param index index of edge.
     * @param p1 first node.
     * @param p2 second node.
     */
    void           NodeOnEdge(const ptrdiff_t &index, VNodeDetail &p1, VNodeDetail &p2)const;
    /**
     * @brief RemoveEdge return detail without edge with index.
     * @param index idex of edge.
     * @return
     */
    VDetail        RemoveEdge(const ptrdiff_t &index) const;
    /**
     * @brief Missing find missing ids in detail. When we deleted object in detail and return this detail need
     * understand, what nodes need make invisible.
     * @param det changed detail.
     * @return  list with missing detail.
     */
    QList<qint64>  Missing(const VDetail &det) const;
private:
    /**
     * @brief _id id detail.
     */
    qint64         _id;
    /**
     * @brief nodes list detail nodes.
     */
    QVector<VNodeDetail> nodes;
    /**
     * @brief name detail name.
     */
    QString        name;
    /**
     * @brief mx bias x axis.
     */
    qreal          mx;
    /**
     * @brief my bias y axis.
     */
    qreal          my;
    /**
     * @brief seamAllowance status seamAllowance detail.
     */
    bool           seamAllowance;
    /**
     * @brief closed status equdistant detail.
     */
    bool           closed;
    /**
     * @brief width value seamAllowance in mm.
     */
    qreal          width;
    /**
     * @brief listNodePoint return list nodes only with points.
     * @return list points node.
     */
    QVector<VNodeDetail> listNodePoint()const;
    /**
     * @brief indexOfNode return index in list node using id object.
     * @param list list nodes detail.
     * @param id object (arc, point, spline, splinePath) id.
     * @return index in list or -1 id can't find.
     */
    ptrdiff_t      indexOfNode(const QVector<VNodeDetail> &list, const qint64 &id) const;
};

#endif // VDETAIL_H
