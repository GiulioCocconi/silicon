/*
 Copyright (c) 2025. Giulio Cocconi

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

#include "graphicalLogicComponent.hpp"

GraphicalLogicComponent::GraphicalLogicComponent(const Component_ptr& component,
                                                 QGraphicsItem*       shape,
                                                 QGraphicsItem* parent, bool scanShape)
  : GraphicalComponent(shape, parent, scanShape)
{
  this->associatedComponent = component;
}
void GraphicalLogicComponent::setPorts(
    const std::vector<std::pair<std::string, QPoint>>& busToPortInputs,
    const std::vector<std::pair<std::string, QPoint>>& busToPortOutputs)
{
  if (associatedComponent) {
    const std::vector<Bus> componentInputs  = associatedComponent->getInputs();
    const std::vector<Bus> componentOutputs = associatedComponent->getOutputs();

    assert(componentInputs.size() == busToPortInputs.size());
    assert(componentOutputs.size() == busToPortOutputs.size());
  }

  GraphicalComponent::setPorts(busToPortInputs, busToPortOutputs);
}
