/****************************************************************************
 **
 **  Copyright (C) 2013 Valentina project All Rights Reserved.
 **
 **  This file is part of Valentina.
 **
 **  Tox is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Tox is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 ****************************************************************************/

#include "vmodelingalongline.h"
#include "container/calculator.h"

VModelingAlongLine::VModelingAlongLine(VDomDocument *doc, VContainer *data, qint64 id,
                                       const QString &formula, const qint64 &firstPointId,
                                       const qint64 &secondPointId, const QString &typeLine,
                                       Tool::Sources typeCreation, QGraphicsItem *parent):
    VModelingLinePoint(doc, data, id, typeLine, formula, firstPointId, 0, parent),
    secondPointId(secondPointId), dialogAlongLine(QSharedPointer<DialogAlongLine>()){

    if(typeCreation == Tool::FromGui){
        AddToFile();
    }
}

void VModelingAlongLine::FullUpdateFromFile(){
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if(domElement.isElement()){
        typeLine = domElement.attribute("typeLine", "");
        formula = domElement.attribute("length", "");
        basePointId = domElement.attribute("firstPoint", "").toLongLong();
        secondPointId = domElement.attribute("secondPoint", "").toLongLong();
    }
    RefreshGeometry();
}

void VModelingAlongLine::FullUpdateFromGui(int result){
    if(result == QDialog::Accepted){
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if(domElement.isElement()){
            domElement.setAttribute("name", dialogAlongLine->getPointName());
            domElement.setAttribute("typeLine", dialogAlongLine->getTypeLine());
            domElement.setAttribute("length", dialogAlongLine->getFormula());
            domElement.setAttribute("firstPoint", QString().setNum(dialogAlongLine->getFirstPointId()));
            domElement.setAttribute("secondPoint", QString().setNum(dialogAlongLine->getSecondPointId()));
            emit FullUpdateTree();
        }

    }
    dialogAlongLine.clear();
}

void VModelingAlongLine::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    ContextMenu(dialogAlongLine, this, event);
}

void VModelingAlongLine::AddToFile(){
    VPointF point = VAbstractTool::data.GetModelingPoint(id);
    QDomElement domElement = doc->createElement("point");

    AddAttribute(domElement, "id", id);
    AddAttribute(domElement, "type", "alongLine");
    AddAttribute(domElement, "name", point.name());
    AddAttribute(domElement, "mx", toMM(point.mx()));
    AddAttribute(domElement, "my", toMM(point.my()));

    AddAttribute(domElement, "typeLine", typeLine);
    AddAttribute(domElement, "length", formula);
    AddAttribute(domElement, "firstPoint", basePointId);
    AddAttribute(domElement, "secondPoint", secondPointId);

    AddToModeling(domElement);
}

void VModelingAlongLine::RemoveReferens(){
    doc->DecrementReferens(secondPointId);
    VModelingLinePoint::RemoveReferens();
}

void VModelingAlongLine::setDialog(){
    Q_ASSERT(!dialogAlongLine.isNull());
    if(!dialogAlongLine.isNull()){
        VPointF p = VAbstractTool::data.GetModelingPoint(id);
        dialogAlongLine->setTypeLine(typeLine);
        dialogAlongLine->setFormula(formula);
        dialogAlongLine->setFirstPointId(basePointId, id);
        dialogAlongLine->setSecondPointId(secondPointId, id);
        dialogAlongLine->setPointName(p.name());
    }
}

VModelingAlongLine *VModelingAlongLine::Create(QSharedPointer<DialogAlongLine> &dialog,
                                               VDomDocument *doc, VContainer *data){
    QString formula = dialog->getFormula();
    qint64 firstPointId = dialog->getFirstPointId();
    qint64 secondPointId = dialog->getSecondPointId();
    QString typeLine = dialog->getTypeLine();
    QString pointName = dialog->getPointName();
    return Create(0, pointName, typeLine, formula, firstPointId, secondPointId, 5, 10, doc, data,
           Document::FullParse, Tool::FromGui);
}

VModelingAlongLine *VModelingAlongLine::Create(const qint64 _id, const QString &pointName,
                                               const QString &typeLine, const QString &formula,
                                               const qint64 &firstPointId, const qint64 &secondPointId,
                                               const qreal &mx, const qreal &my, VDomDocument *doc,
                                               VContainer *data, const Document::Documents &parse,
                                               Tool::Sources typeCreation){
    VModelingAlongLine *point = 0;
    VPointF firstPoint = data->GetModelingPoint(firstPointId);
    VPointF secondPoint = data->GetModelingPoint(secondPointId);
    QLineF line = QLineF(firstPoint.toQPointF(), secondPoint.toQPointF());
    Calculator cal(data);
    QString errorMsg;
    qreal result = cal.eval(formula, &errorMsg);
    if(errorMsg.isEmpty()){
        line.setLength(toPixel(result));
        qint64 id = _id;
        if(typeCreation == Tool::FromGui){
            id = data->AddModelingPoint(VPointF(line.p2().x(), line.p2().y(), pointName, mx, my));
        } else {
            data->UpdateModelingPoint(id, VPointF(line.p2().x(), line.p2().y(), pointName, mx, my));
            if(parse != Document::FullParse){
                doc->UpdateToolData(id, data);
            }
        }
        data->AddLine(firstPointId, id);
        data->AddLine(id, secondPointId);

        if(parse == Document::FullParse){
            point = new VModelingAlongLine(doc, data, id, formula, firstPointId, secondPointId, typeLine,
                                           typeCreation);
            doc->AddTool(id, point);
            doc->IncrementReferens(firstPointId);
            doc->IncrementReferens(secondPointId);
        }
    }
    return point;
}
