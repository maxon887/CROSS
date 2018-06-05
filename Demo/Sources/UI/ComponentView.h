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

template<class T>
class ComponentView : public View {
public:
	ComponentView(const String& name);

	virtual void Content(T* component) { }
	virtual void EntitySelected(Entity* newEntity) { }

	void Content(float sec) override;

	void OnEntitySelected(Entity* entity);

private:
	Entity* selected_entity = nullptr;

	void OnScreenChanged(Screen* screen);
};

template<class T>
ComponentView<T>::ComponentView(const String& name) :
	View(name)
{
	game->ScreenChanged.Connect(this, &ComponentView::OnScreenChanged);
}

template<class T>
void ComponentView<T>::Content(float sec) {
	if(selected_entity && selected_entity->GetComponent<T>()) {
		Content(selected_entity->GetComponent<T>());
	}
}

template<class T>
void ComponentView<T>::OnEntitySelected(Entity* entity) {
	selected_entity = entity;
	EntitySelected(entity);
}

template<class T>
void ComponentView<T>::OnScreenChanged(Screen* screen) {
	selected_entity = nullptr;
}
