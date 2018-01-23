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
#include "Cross.h"
#include "UI/View.h"
#include "Game.h"
#include "Entity.h"

using namespace cross;

template<class ComponentType>
class ComponentView : public View {
public:
	ComponentView(const String& name);

	virtual void Content(ComponentType* component) { }

	void Content(float sec) override;

	void OnEntitySelected(Entity* entity);

private:
	Entity* selected_entity = null;

	void OnScreenChanged(Screen* screen);
};

template<class ComponentType>
ComponentView<ComponentType>::ComponentView(const String& name) :
	View(name)
{
	game->ScreenChanged.Connect(this, &ComponentView::OnScreenChanged);
}

template<class ComponentType>
void ComponentView<ComponentType>::Content(float sec) {
	if(selected_entity && selected_entity->GetComponent<ComponentType>()) {
		Content(selected_entity->GetComponent<ComponentType>());
	}
}

template<class ComponentType>
void ComponentView<ComponentType>::OnEntitySelected(Entity* entity) {
	selected_entity = entity;
}

template<class ComponentType>
void ComponentView<ComponentType>::OnScreenChanged(Screen* screen) {
	selected_entity = null;
}
