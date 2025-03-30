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

#include "graphicalComponent.hpp"

#include <QGraphicsSvgItem>


#include <core/gates.hpp>

class GraphicalGate : public GraphicalComponent {
protected:
  GraphicalGate(const std::weak_ptr<Gate> gate,
		QGraphicsItem*            shape,
		QGraphicsItem*            parent = nullptr);
};

class GraphicalAnd : public GraphicalGate {
public:
  GraphicalAnd(const std::weak_ptr<AndGate> gate,
	       QGraphicsItem* parent = nullptr)
    : GraphicalGate(gate,
		  new QGraphicsSvgItem(":/gates/AND_ANSI.svg"),
		  parent) { }

};

class GraphicalOr : public GraphicalGate {
public:
  GraphicalOr(const std::weak_ptr<OrGate> gate,
	       QGraphicsItem* parent = nullptr)
    : GraphicalGate(gate,
		    new QGraphicsSvgItem(":/gates/OR_ANSI.svg"),
		    parent) { }
};


class GraphicalNot : public GraphicalComponent {
public:
  GraphicalNot(const std::weak_ptr<NotGate> gate,
	       QGraphicsItem* parent = nullptr);
};

class GraphicalNand : public GraphicalGate {
public:
  GraphicalNand(const std::weak_ptr<NandGate> gate,
	       QGraphicsItem* parent = nullptr)
    : GraphicalGate(gate,
		    new QGraphicsSvgItem(":/gates/NAND_ANSI.svg"),
		    parent) {}
};


class GraphicalNor : public GraphicalGate {
public:
  GraphicalNor(const std::weak_ptr<NorGate> gate,
	       QGraphicsItem* parent = nullptr)
    : GraphicalGate(gate,
		    new QGraphicsSvgItem(":/gates/NOR_ANSI.svg"),
		    parent) {}
};

class GraphicalXor : public GraphicalGate {
public:
  GraphicalXor(const std::weak_ptr<XorGate> gate,
	       QGraphicsItem* parent = nullptr)

    : GraphicalGate(gate,
		    new QGraphicsSvgItem(":/gates/XOR_ANSI.svg"),
		    parent) {}
};
