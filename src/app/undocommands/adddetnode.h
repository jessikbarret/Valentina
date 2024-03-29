/************************************************************************
 **
 **  @file   adddetnode.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 6, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2014 Valentina project
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

#ifndef ADDDETNODE_H
#define ADDDETNODE_H

#include <QDomElement>
#include <QUndoCommand>

class VPattern;

class AddDetNode : public QUndoCommand
{
public:
    AddDetNode(const QDomElement &xml, VPattern *doc, QUndoCommand *parent = 0);
    virtual ~AddDetNode();
    virtual void undo();
    virtual void redo();
private:
    Q_DISABLE_COPY(AddDetNode)
    const QDomElement xml;
    VPattern *doc;
};

#endif // ADDDETNODE_H
