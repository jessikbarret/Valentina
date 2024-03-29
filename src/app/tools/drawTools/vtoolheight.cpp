/************************************************************************
 **
 **  @file   vtoolheight.cpp
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

#include "vtoolheight.h"
#include "../../dialogs/tools/dialogheight.h"
#include "../../geometry/vpointf.h"

const QString VToolHeight::ToolType = QStringLiteral("height");

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VToolHeight constructor.
 * @param doc dom document container.
 * @param data container with variables.
 * @param id object id in container.
 * @param typeLine line type.
 * @param basePointId id base point of projection.
 * @param p1LineId id first point of line.
 * @param p2LineId id second point of line.
 * @param typeCreation way we create this tool.
 * @param parent parent object.
 */
VToolHeight::VToolHeight(VPattern *doc, VContainer *data, const quint32 &id, const QString &typeLine,
                         const quint32 &basePointId, const quint32 &p1LineId, const quint32 &p2LineId,
                         const Source &typeCreation, QGraphicsItem * parent)
    :VToolLinePoint(doc, data, id, typeLine, QString(), basePointId, 0, parent), p1LineId(p1LineId), p2LineId(p2LineId)
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
void VToolHeight::setDialog()
{
    SCASSERT(dialog != nullptr);
    DialogHeight *dialogTool = qobject_cast<DialogHeight*>(dialog);
    SCASSERT(dialogTool != nullptr);
    const VPointF *p = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    dialogTool->setTypeLine(typeLine);
    dialogTool->setBasePointId(basePointId, id);
    dialogTool->setP1LineId(p1LineId, id);
    dialogTool->setP2LineId(p2LineId, id);
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
void VToolHeight::Create(DialogTool *dialog, VMainGraphicsScene *scene, VPattern *doc,
                         VContainer *data)
{
    SCASSERT(dialog != nullptr);
    DialogHeight *dialogTool = qobject_cast<DialogHeight*>(dialog);
    SCASSERT(dialogTool != nullptr);
    disconnect(doc, &VPattern::FullUpdateFromFile, dialogTool, &DialogHeight::UpdateList);
    const QString pointName = dialogTool->getPointName();
    const QString typeLine = dialogTool->getTypeLine();
    const quint32 basePointId = dialogTool->getBasePointId();
    const quint32 p1LineId = dialogTool->getP1LineId();
    const quint32 p2LineId = dialogTool->getP2LineId();
    Create(0, pointName, typeLine, basePointId, p1LineId, p2LineId, 5, 10, scene, doc, data,
           Document::FullParse, Source::FromGui);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool
 * @param _id tool id, 0 if tool doesn't exist yet.
 * @param pointName point name.
 * @param typeLine line type.
 * @param basePointId id base point of projection.
 * @param p1LineId id first point of line.
 * @param p2LineId id second point of line.
 * @param mx label bias x axis.
 * @param my label bias y axis.
 * @param scene pointer to scene.
 * @param doc dom document container.
 * @param data container with variables.
 * @param parse parser file mode.
 * @param typeCreation way we create this tool.
 */
void VToolHeight::Create(const quint32 _id, const QString &pointName, const QString &typeLine,
                         const quint32 &basePointId, const quint32 &p1LineId, const quint32 &p2LineId,
                         const qreal &mx, const qreal &my, VMainGraphicsScene *scene, VPattern *doc,
                         VContainer *data, const Document &parse, const Source &typeCreation)
{
    const VPointF *basePoint = data->GeometricObject<const VPointF *>(basePointId);
    const VPointF *p1Line = data->GeometricObject<const VPointF *>(p1LineId);
    const VPointF *p2Line = data->GeometricObject<const VPointF *>(p2LineId);

    QPointF pHeight = FindPoint(QLineF(p1Line->toQPointF(), p2Line->toQPointF()), basePoint->toQPointF());
    quint32 id = _id;
    if (typeCreation == Source::FromGui)
    {
        id = data->AddGObject(new VPointF(pHeight.x(), pHeight.y(), pointName, mx, my));
        data->AddLine(basePointId, id);
        data->AddLine(p1LineId, id);
        data->AddLine(p2LineId, id);
    }
    else
    {
        data->UpdateGObject(id, new VPointF(pHeight.x(), pHeight.y(), pointName, mx, my));
        data->AddLine(basePointId, id);
        data->AddLine(p1LineId, id);
        data->AddLine(p2LineId, id);
        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }
    VDrawTool::AddRecord(id, Tool::Height, doc);
    if (parse == Document::FullParse)
    {
        VToolHeight *point = new VToolHeight(doc, data, id, typeLine, basePointId, p1LineId, p2LineId,
                                             typeCreation);
        scene->addItem(point);
        connect(point, &VToolPoint::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
        connect(scene, &VMainGraphicsScene::NewFactor, point, &VToolPoint::SetFactor);
        connect(scene, &VMainGraphicsScene::DisableItem, point, &VToolPoint::Disable);
        doc->AddTool(id, point);
        doc->IncrementReferens(basePointId);
        doc->IncrementReferens(p1LineId);
        doc->IncrementReferens(p2LineId);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FindPoint find projection base point onto line.
 * @param line line
 * @param point base point.
 * @return point onto line.
 */
QPointF VToolHeight::FindPoint(const QLineF &line, const QPointF &point)
{
    return VAbstractTool::ClosestPoint(line, point);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FullUpdateFromFile update tool data form file.
 */
void VToolHeight::FullUpdateFromFile()
{
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        typeLine = domElement.attribute(AttrTypeLine, "");
        basePointId = domElement.attribute(AttrBasePoint, "").toUInt();
        p1LineId = domElement.attribute(AttrP1Line, "").toUInt();
        p2LineId = domElement.attribute(AttrP2Line, "").toUInt();
    }
    RefreshGeometry();

}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ShowContextMenu show context menu.
 * @param event context menu event.
 */
void VToolHeight::ShowContextMenu(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogHeight>(this, event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief contextMenuEvent handle context menu events.
 * @param event context menu event.
 */
void VToolHeight::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogHeight>(this, event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddToFile add tag with informations about tool into file.
 */
void VToolHeight::AddToFile()
{
    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    QDomElement domElement = doc->createElement(TagName);

    doc->SetAttribute(domElement, VDomDocument::AttrId, id);
    doc->SetAttribute(domElement, AttrType, ToolType);
    doc->SetAttribute(domElement, AttrName, point->name());
    doc->SetAttribute(domElement, AttrMx, qApp->fromPixel(point->mx()));
    doc->SetAttribute(domElement, AttrMy, qApp->fromPixel(point->my()));

    doc->SetAttribute(domElement, AttrTypeLine, typeLine);
    doc->SetAttribute(domElement, AttrBasePoint, basePointId);
    doc->SetAttribute(domElement, AttrP1Line, p1LineId);
    doc->SetAttribute(domElement, AttrP2Line, p2LineId);

    AddToCalculation(domElement);

}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RefreshDataInFile refresh attributes in file. If attributes don't exist create them.
 */
void VToolHeight::RefreshDataInFile()
{
    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        doc->SetAttribute(domElement, AttrName, point->name());
        doc->SetAttribute(domElement, AttrMx, qApp->fromPixel(point->mx()));
        doc->SetAttribute(domElement, AttrMy, qApp->fromPixel(point->my()));
        doc->SetAttribute(domElement, AttrTypeLine, typeLine);
        doc->SetAttribute(domElement, AttrBasePoint, basePointId);
        doc->SetAttribute(domElement, AttrP1Line, p1LineId);
        doc->SetAttribute(domElement, AttrP2Line, p2LineId);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SaveDialog save options into file after change in dialog.
 */
void VToolHeight::SaveDialog(QDomElement &domElement)
{
    SCASSERT(dialog != nullptr);
    DialogHeight *dialogTool = qobject_cast<DialogHeight*>(dialog);
    SCASSERT(dialogTool != nullptr);
    doc->SetAttribute(domElement, AttrName, dialogTool->getPointName());
    doc->SetAttribute(domElement, AttrTypeLine, dialogTool->getTypeLine());
    doc->SetAttribute(domElement, AttrBasePoint, QString().setNum(dialogTool->getBasePointId()));
    doc->SetAttribute(domElement, AttrP1Line, QString().setNum(dialogTool->getP1LineId()));
    doc->SetAttribute(domElement, AttrP2Line, QString().setNum(dialogTool->getP2LineId()));
}
