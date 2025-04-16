/*
  Copyright (C) 2025 Giulio Cocconi

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <QGraphicsSvgItem>

#include <core/gates.hpp>
#include <ui/logiFlow/components/graphicalLogicComponent.hpp>

class GraphicalGate : public GraphicalLogicComponent {
  Q_OBJECT
protected:
  GraphicalGate(const std::weak_ptr<Gate> gate, QGraphicsItem* shape,
                QGraphicsItem* parent = nullptr);
};

class GraphicalAnd : public GraphicalGate {
  Q_OBJECT
public:
  GraphicalAnd(const std::weak_ptr<AndGate> gate, QGraphicsItem* parent = nullptr)
    : GraphicalGate(gate, new QGraphicsSvgItem(":/gates/AND_ANSI.svg"), parent)
  {
  }
};

class GraphicalOr : public GraphicalGate {
  Q_OBJECT
public:
  GraphicalOr(const std::weak_ptr<OrGate> gate, QGraphicsItem* parent = nullptr)
    : GraphicalGate(gate, new QGraphicsSvgItem(":/gates/OR_ANSI.svg"), parent)
  {
  }
};

class GraphicalNot : public GraphicalLogicComponent {
  Q_OBJECT
public:
  GraphicalNot(const std::weak_ptr<NotGate> gate, QGraphicsItem* parent = nullptr);
};

class GraphicalNand : public GraphicalGate {
  Q_OBJECT
public:
  GraphicalNand(const std::weak_ptr<NandGate> gate, QGraphicsItem* parent = nullptr)
    : GraphicalGate(gate, new QGraphicsSvgItem(":/gates/NAND_ANSI.svg"), parent)
  {
  }
};

class GraphicalNor : public GraphicalGate {
  Q_OBJECT
public:
  GraphicalNor(const std::weak_ptr<NorGate> gate, QGraphicsItem* parent = nullptr)
    : GraphicalGate(gate, new QGraphicsSvgItem(":/gates/NOR_ANSI.svg"), parent)
  {
  }
};

class GraphicalXor : public GraphicalGate {
  Q_OBJECT
public:
  GraphicalXor(const std::weak_ptr<XorGate> gate, QGraphicsItem* parent = nullptr)

    : GraphicalGate(gate, new QGraphicsSvgItem(":/gates/XOR_ANSI.svg"), parent)
  {
  }
};
