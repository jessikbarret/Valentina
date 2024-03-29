/************************************************************************
 **
 **  @file   vtoollineintersect.cpp
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

#include "vtoollineintersect.h"
#include "../../dialogs/tools/dialoglineintersect.h"
#include "../../geometry/vpointf.h"

const QString VToolLineIntersect::ToolType = QStringLiteral("lineIntersect");

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VToolLineIntersect constructor.
 * @param doc dom document container.
 * @param data container with variables.
 * @param id object id in container.
 * @param p1Line1 id first point first line.
 * @param p2Line1 id second point first line.
 * @param p1Line2 id first point second line.
 * @param p2Line2 id second point second line.
 * @param typeCreation way we create this tool.
 * @param parent parent object.
 */
VToolLineIntersect::VToolLineIntersect(VPattern *doc, VContainer *data, const quint32 &id,
                                       const quint32 &p1Line1, const quint32 &p2Line1, const quint32 &p1Line2,
                                       const quint32 &p2Line2, const Source &typeCreation,
                                       QGraphicsItem *parent)
    :VToolPoint(doc, data, id, parent), p1Line1(p1Line1), p2Line1(p2Line1), p1Line2(p1Line2),
    p2Line2(p2Line2)
{
    ignoreFullUpdate = true;
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
void VToolLineIntersect::setDialog()
{
    SCASSERT(dialog != nullptr);
    DialogLineIntersect *dialogTool = qobject_cast<DialogLineIntersect*>(dialog);
    SCASSERT(dialogTool != nullptr);
    const VPointF *p = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    dialogTool->setP1Line1(p1Line1);
    dialogTool->setP2Line1(p2Line1);
    dialogTool->setP1Line2(p1Line2);
    dialogTool->setP2Line2(p2Line2);
    dialogTool->setPointName(p->name());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool from GUI.
 * @param dialog dialog.
 * @param scene pointer to scene.
 * @param doc dom document container.
 * @param data container with variables.
 */
void VToolLineIntersect::Create(DialogTool *dialog, VMainGraphicsScene *scene, VPattern *doc, VContainer *data)
{
    SCASSERT(dialog != nullptr);
    DialogLineIntersect *dialogTool = qobject_cast<DialogLineIntersect*>(dialog);
    SCASSERT(dialogTool != nullptr);
    const quint32 p1Line1Id = dialogTool->getP1Line1();
    const quint32 p2Line1Id = dialogTool->getP2Line1();
    const quint32 p1Line2Id = dialogTool->getP1Line2();
    const quint32 p2Line2Id = dialogTool->getP2Line2();
    const QString pointName = dialogTool->getPointName();
    Create(0, p1Line1Id, p2Line1Id, p1Line2Id, p2Line2Id, pointName, 5, 10, scene, doc, data,
           Document::FullParse, Source::FromGui);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool.
 * @param _id tool id, 0 if tool doesn't exist yet.
 * @param p1Line1Id id first point first line.
 * @param p2Line1Id id second point first line.
 * @param p1Line2Id id first point second line.
 * @param p2Line2Id id second point second line.
 * @param pointName point name.
 * @param mx label bias x axis.
 * @param my label bias y axis.
 * @param scene pointer to scene.
 * @param doc dom document container.
 * @param data container with variables.
 * @param parse parser file mode.
 * @param typeCreation way we create this tool.
 */
void VToolLineIntersect::Create(const quint32 _id, const quint32 &p1Line1Id, const quint32 &p2Line1Id,
                                const quint32 &p1Line2Id, const quint32 &p2Line2Id, const QString &pointName,
                                const qreal &mx, const qreal &my, VMainGraphicsScene *scene,
                                VPattern *doc, VContainer *data, const Document &parse,
                                const Source &typeCreation)
{
    const VPointF *p1Line1 = data->GeometricObject<const VPointF *>(p1Line1Id);
    const VPointF *p2Line1 = data->GeometricObject<const VPointF *>(p2Line1Id);
    const VPointF *p1Line2 = data->GeometricObject<const VPointF *>(p1Line2Id);
    const VPointF *p2Line2 = data->GeometricObject<const VPointF *>(p2Line2Id);

    QLineF line1(p1Line1->toQPointF(), p2Line1->toQPointF());
    QLineF line2(p1Line2->toQPointF(), p2Line2->toQPointF());
    QPointF fPoint;
    QLineF::IntersectType intersect = line1.intersect(line2, &fPoint);
    if (intersect == QLineF::UnboundedIntersection || intersect == QLineF::BoundedIntersection)
    {
        quint32 id = _id;
        if (typeCreation == Source::FromGui)
        {
            id = data->AddGObject(new VPointF(fPoint.x(), fPoint.y(), pointName, mx, my));
            data->AddLine(p1Line1Id, id);
            data->AddLine(id, p2Line1Id);
            data->AddLine(p1Line2Id, id);
            data->AddLine(id, p2Line2Id);
        }
        else
        {
            data->UpdateGObject(id, new VPointF(fPoint.x(), fPoint.y(), pointName, mx, my));
            data->AddLine(p1Line1Id, id);
            data->AddLine(id, p2Line1Id);
            data->AddLine(p1Line2Id, id);
            data->AddLine(id, p2Line2Id);
            if (parse != Document::FullParse)
            {
                doc->UpdateToolData(id, data);
            }
        }
        VDrawTool::AddRecord(id, Tool::LineIntersectTool, doc);
        if (parse == Document::FullParse)
        {
            VToolLineIntersect *point = new VToolLineIntersect(doc, data, id, p1Line1Id, p2Line1Id, p1Line2Id,
                                                               p2Line2Id, typeCreation);
            scene->addItem(point);
            connect(point, &VToolLineIntersect::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
            connect(scene, &VMainGraphicsScene::NewFactor, point, &VToolLineIntersect::SetFactor);
            connect(scene, &VMainGraphicsScene::DisableItem, point, &VToolPoint::Disable);
            doc->AddTool(id, point);
            doc->IncrementReferens(p1Line1Id);
            doc->IncrementReferens(p2Line1Id);
            doc->IncrementReferens(p1Line2Id);
            doc->IncrementReferens(p2Line2Id);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FullUpdateFromFile update tool data form file.
 */
void VToolLineIntersect::FullUpdateFromFile()
{
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        p1Line1 = domElement.attribute(AttrP1Line1, "").toUInt();
        p2Line1 = domElement.attribute(AttrP2Line1, "").toUInt();
        p1Line2 = domElement.attribute(AttrP1Line2, "").toUInt();
        p2Line2 = domElement.attribute(AttrP2Line2, "").toUInt();
    }
    RefreshPointGeometry(*VAbstractTool::data.GeometricObject<const VPointF *>(id));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetFactor set current scale factor of scene.
 * @param factor scene scale factor.
 */
void VToolLineIntersect::SetFactor(qreal factor)
{
    VDrawTool::SetFactor(factor);
    RefreshPointGeometry(*VAbstractTool::data.GeometricObject<const VPointF *>(id));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ShowContextMenu show context menu.
 * @param event context menu event.
 */
void VToolLineIntersect::ShowContextMenu(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogLineIntersect>(this, event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief contextMenuEvent handle context menu events.
 * @param event context menu event.
 */
void VToolLineIntersect::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogLineIntersect>(this, event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddToFile add tag with informations about tool into file.
 */
void VToolLineIntersect::AddToFile()
{
    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    QDomElement domElement = doc->createElement(TagName);

    doc->SetAttribute(domElement, VDomDocument::AttrId, id);
    doc->SetAttribute(domElement, AttrType, ToolType);
    doc->SetAttribute(domElement, AttrName, point->name());
    doc->SetAttribute(domElement, AttrMx, qApp->fromPixel(point->mx()));
    doc->SetAttribute(domElement, AttrMy, qApp->fromPixel(point->my()));

    doc->SetAttribute(domElement, AttrP1Line1, p1Line1);
    doc->SetAttribute(domElement, AttrP2Line1, p2Line1);
    doc->SetAttribute(domElement, AttrP1Line2, p1Line2);
    doc->SetAttribute(domElement, AttrP2Line2, p2Line2);

    AddToCalculation(domElement);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RefreshDataInFile refresh attributes in file. If attributes don't exist create them.
 */
void VToolLineIntersect::RefreshDataInFile()
{
    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        doc->SetAttribute(domElement, AttrName, point->name());
        doc->SetAttribute(domElement, AttrMx, qApp->fromPixel(point->mx()));
        doc->SetAttribute(domElement, AttrMy, qApp->fromPixel(point->my()));
        doc->SetAttribute(domElement, AttrP1Line1, p1Line1);
        doc->SetAttribute(domElement, AttrP2Line1, p2Line1);
        doc->SetAttribute(domElement, AttrP1Line2, p1Line2);
        doc->SetAttribute(domElement, AttrP2Line2, p2Line2);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RemoveReferens decrement value of reference.
 */
void VToolLineIntersect::RemoveReferens()
{
    doc->DecrementReferens(p1Line1);
    doc->DecrementReferens(p2Line1);
    doc->DecrementReferens(p1Line2);
    doc->DecrementReferens(p2Line2);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SaveDialog save options into file after change in dialog.
 */
void VToolLineIntersect::SaveDialog(QDomElement &domElement)
{
    SCASSERT(dialog != nullptr);
    DialogLineIntersect *dialogTool = qobject_cast<DialogLineIntersect*>(dialog);
    SCASSERT(dialogTool != nullptr);
    doc->SetAttribute(domElement, AttrName, dialogTool->getPointName());
    doc->SetAttribute(domElement, AttrP1Line1, QString().setNum(dialogTool->getP1Line1()));
    doc->SetAttribute(domElement, AttrP2Line1, QString().setNum(dialogTool->getP2Line1()));
    doc->SetAttribute(domElement, AttrP1Line2, QString().setNum(dialogTool->getP1Line2()));
    doc->SetAttribute(domElement, AttrP2Line2, QString().setNum(dialogTool->getP2Line2()));
}
