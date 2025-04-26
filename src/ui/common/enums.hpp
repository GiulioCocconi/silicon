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
#include <QGraphicsItem>

enum SiliconTypes {
  UNKNOWN = QGraphicsItem::UserType,
  PORT,
  WIRE,
  WIRE_SEGMENT,
  WIRE_JUNCTION,

  /* LogiFlow */
  SINGLE_INPUT = QGraphicsItem::UserType + 10,
  SINGLE_OUTPUT,
  AND_GATE = QGraphicsItem::UserType + 10,
  NAND_GATE,
  OR_GATE,
  NOR_GATE,
  NOT_GATE,
  XOR_GATE,
  HALF_ADDER,
  FULL_ADDER,
};
