/*	Copyright © 2018 Maksim Lukyanov

	This file is part of Cross++ Game Engine.

	Cross++ Game Engine is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Cross++ is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Cross++.  If not, see <http://www.gnu.org/licenses/>			*/
#pragma once
#include "UI/Views/View.h"
#include "Property.h"
#include "UI/TransformVisualBox.h"
#include "UI/MeshVisualBox.h"

class SceneView;

class ComponentsView : public View {
public:
	ComponentsView(SceneView* sceneView);

	void Update(float sec) override;

	DockPosition GetDefaultDockPosition() const override { return DockPosition::RIGHT; }

protected:
	TransformVisualBox transform_box;
	MeshVisualBox mesh_box;
	SceneView* scene_view = nullptr;

	void ShowProperty(BaseProperty* baseProperty);

	void ContextMenu(Entity* selectedEntity);
};
