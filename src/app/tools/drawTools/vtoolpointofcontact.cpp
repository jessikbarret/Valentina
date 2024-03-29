/************************************************************************
 **
 **  @file   vtoolpointofcontact.cpp
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

#include "vtoolpointofcontact.h"
#include "../../container/calculator.h"
#include "../../dialogs/tools/dialogpointofcontact.h"
#include "../../geometry/vpointf.h"

const QString VToolPointOfContact::ToolType = QStringLiteral("pointOfContact");

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VToolPointOfContact constructor.
 * @param doc dom document container.
 * @param data container with variables.
 * @param id object id in container.
 * @param arcRadius string with formula radius arc.
 * @param center id center arc point.
 * @param firstPointId id first line point.
 * @param secondPointId id second line point.
 * @param typeCreation way we create this tool.
 * @param parent parent object.
 */
VToolPointOfContact::VToolPointOfContact(VPattern *doc, VContainer *data, const quint32 &id,
                                         const QString &radius, const quint32 &center,
                                         const quint32 &firstPointId, const quint32 &secondPointId,
                                         const Source &typeCreation, QGraphicsItem *parent)
    : VToolPoint(doc, data, id, parent), arcRadius(radius), center(center), firstPointId(firstPointId),
      secondPointId(secondPointId)
{
    if (typeCreation == Source::FromGui)
    {
        AddToFile();
    }
    else
    {
        RefreshDataInFile();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setDialog set dialog when user want change tool option.
 */
void VToolPointOfContact::setDialog()
{
    SCASSERT(dialog != nullptr);
    DialogPointOfContact *dialogTool = qobject_cast<DialogPointOfContact*>(dialog);
    SCASSERT(dialogTool != nullptr);
    const VPointF *p = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    dialogTool->setRadius(arcRadius);
    dialogTool->setCenter(center, id);
    dialogTool->setFirstPoint(firstPointId, id);
    dialogTool->setSecondPoint(secondPointId, id);
    dialogTool->setPointName(p->name());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FindPoint return point intersection line and arc.
 * @param arcRadius string with formula radius arc.
 * @param center center arc point.
 * @param firstPoint first line point.
 * @param secondPoint second line point.
 * @return point intersection.
 */
QPointF VToolPointOfContact::FindPoint(const qreal &radius, const QPointF &center, const QPointF &firstPoint,
                                       const QPointF &secondPoint)
{
    QPointF pArc;
    qreal s = 0.0, s_x, s_y, step = 0.01, distans;
    while ( s < 1)
    {
        s_x = secondPoint.x()-(qAbs(secondPoint.x()-firstPoint.x()))*s;
        s_y = secondPoint.y()-(qAbs(secondPoint.y()-firstPoint.y()))*s;
        distans = QLineF(center.x(), center.y(), s_x, s_y).length();
        if (fabs(distans*10 - radius*10) < 0.1)
        {
            pArc.rx() = s_x;
            pArc.ry() = s_y;
            break;
        }
        if (distans<radius)
        {
            pArc.rx() = s_x;
            pArc.ry() = s_y;
        }
        s = s + step;
    }
    return pArc;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool from GUI.
 * @param dialog dialog.
 * @param scene pointer to scene.
 * @param doc dom document container.
 * @param data container with variables.
 */
VToolPointOfContact* VToolPointOfContact::Create(DialogTool *dialog, VMainGraphicsScene *scene, VPattern *doc,
                                                 VContainer *data)
{
    SCASSERT(dialog != nullptr);
    DialogPointOfContact *dialogTool = qobject_cast<DialogPointOfContact*>(dialog);
    SCASSERT(dialogTool != nullptr);
    QString radius = dialogTool->getRadius();
    const quint32 center = dialogTool->getCenter();
    const quint32 firstPointId = dialogTool->getFirstPoint();
    const quint32 secondPointId = dialogTool->getSecondPoint();
    const QString pointName = dialogTool->getPointName();
    VToolPointOfContact *point = nullptr;
    point=Create(0, radius, center, firstPointId, secondPointId, pointName, 5, 10, scene, doc, data,
           Document::FullParse, Source::FromGui);
    if (point != nullptr)
    {
        point->dialog=dialogTool;
    }
    return point;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool.
 * @param _id tool id, 0 if tool doesn't exist yet.
 * @param arcRadius string with formula radius arc.
 * @param center id center arc point.
 * @param firstPointId id first line point.
 * @param secondPointId id second line point.
 * @param pointName point name.
 * @param mx label bias x axis.
 * @param my label bias y axis.
 * @param scene pointer to scene.
 * @param doc dom document container.
 * @param data container with variables.
 * @param parse parser file mode.
 * @param typeCreation way we create this tool.
 */
VToolPointOfContact* VToolPointOfContact::Create(const quint32 _id, QString &radius, const quint32 &center,
                                                 const quint32 &firstPointId, const quint32 &secondPointId,
                                                 const QString &pointName, const qreal &mx, const qreal &my,
                                                 VMainGraphicsScene *scene, VPattern *doc, VContainer *data,
                                                 const Document &parse, const Source &typeCreation)
{
    const VPointF *centerP = data->GeometricObject<const VPointF *>(center);
    const VPointF *firstP = data->GeometricObject<const VPointF *>(firstPointId);
    const VPointF *secondP = data->GeometricObject<const VPointF *>(secondPointId);

    const qreal result = CheckFormula(radius, data);

    QPointF fPoint = VToolPointOfContact::FindPoint(qApp->toPixel(result), centerP->toQPointF(),
                                                     firstP->toQPointF(), secondP->toQPointF());
    quint32 id =  _id;
    if (typeCreation == Source::FromGui)
    {
        id = data->AddGObject(new VPointF(fPoint.x(), fPoint.y(), pointName, mx, my));
        data->AddLine(firstPointId, id);
        data->AddLine(secondPointId, id);
        data->AddLine(center, id);
    }
    else
    {
        data->UpdateGObject(id, new VPointF(fPoint.x(), fPoint.y(), pointName, mx, my));
        data->AddLine(firstPointId, id);
        data->AddLine(secondPointId, id);
        data->AddLine(center, id);
        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }
    VDrawTool::AddRecord(id, Tool::PointOfContact, doc);
    if (parse == Document::FullParse)
    {
        VToolPointOfContact *point = new VToolPointOfContact(doc, data, id, radius, center,
                                                             firstPointId, secondPointId, typeCreation);
        scene->addItem(point);
        connect(point, &VToolPointOfContact::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
        connect(scene, &VMainGraphicsScene::NewFactor, point, &VToolPointOfContact::SetFactor);
        connect(scene, &VMainGraphicsScene::DisableItem, point, &VToolPoint::Disable);
        doc->AddTool(id, point);
        doc->IncrementReferens(center);
        doc->IncrementReferens(firstPointId);
        doc->IncrementReferens(secondPointId);
        return point;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FullUpdateFromFile update tool data form file.
 */
void VToolPointOfContact::FullUpdateFromFile()
{
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        arcRadius = domElement.attribute(AttrRadius, "");
        center = domElement.attribute(AttrCenter, "").toUInt();
        firstPointId = domElement.attribute(AttrFirstPoint, "").toUInt();
        secondPointId = domElement.attribute(AttrSecondPoint, "").toUInt();
    }
    RefreshPointGeometry(*VAbstractTool::data.GeometricObject<const VPointF *>(id));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetFactor set current scale factor of scene.
 * @param factor scene scale factor.
 */
void VToolPointOfContact::SetFactor(qreal factor)
{
    VDrawTool::SetFactor(factor);
    RefreshPointGeometry(*VAbstractTool::data.GeometricObject<const VPointF *>(id));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ShowContextMenu show context menu.
 * @param event context menu event.
 */
void VToolPointOfContact::ShowContextMenu(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogPointOfContact>(this, event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief contextMenuEvent handle context menu events.
 * @param event context menu event.
 */
void VToolPointOfContact::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogPointOfContact>(this, event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddToFile add tag with informations about tool into file.
 */
void VToolPointOfContact::AddToFile()
{
    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    QDomElement domElement = doc->createElement(TagName);

    doc->SetAttribute(domElement, VDomDocument::AttrId, id);
    doc->SetAttribute(domElement, AttrType, ToolType);
    doc->SetAttribute(domElement, AttrName, point->name());
    doc->SetAttribute(domElement, AttrMx, qApp->fromPixel(point->mx()));
    doc->SetAttribute(domElement, AttrMy, qApp->fromPixel(point->my()));

    doc->SetAttribute(domElement, AttrRadius, arcRadius);
    doc->SetAttribute(domElement, AttrCenter, center);
    doc->SetAttribute(domElement, AttrFirstPoint, firstPointId);
    doc->SetAttribute(domElement, AttrSecondPoint, secondPointId);

    AddToCalculation(domElement);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RefreshDataInFile refresh attributes in file. If attributes don't exist create them.
 */
void VToolPointOfContact::RefreshDataInFile()
{
    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        doc->SetAttribute(domElement, AttrName, point->name());
        doc->SetAttribute(domElement, AttrMx, qApp->fromPixel(point->mx()));
        doc->SetAttribute(domElement, AttrMy, qApp->fromPixel(point->my()));
        doc->SetAttribute(domElement, AttrRadius, arcRadius);
        doc->SetAttribute(domElement, AttrCenter, center);
        doc->SetAttribute(domElement, AttrFirstPoint, firstPointId);
        doc->SetAttribute(domElement, AttrSecondPoint, secondPointId);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RemoveReferens decrement value of reference.
 */
void VToolPointOfContact::RemoveReferens()
{
    doc->DecrementReferens(center);
    doc->DecrementReferens(firstPointId);
    doc->DecrementReferens(secondPointId);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SaveDialog save options into file after change in dialog.
 */
void VToolPointOfContact::SaveDialog(QDomElement &domElement)
{
    SCASSERT(dialog != nullptr);
    DialogPointOfContact *dialogTool = qobject_cast<DialogPointOfContact*>(dialog);
    SCASSERT(dialogTool != nullptr);
    doc->SetAttribute(domElement, AttrName, dialogTool->getPointName());
    doc->SetAttribute(domElement, AttrRadius, dialogTool->getRadius());
    doc->SetAttribute(domElement, AttrCenter, QString().setNum(dialogTool->getCenter()));
    doc->SetAttribute(domElement, AttrFirstPoint, QString().setNum(dialogTool->getFirstPoint()));
    doc->SetAttribute(domElement, AttrSecondPoint, QString().setNum(dialogTool->getSecondPoint()));
}
