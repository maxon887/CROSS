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
#include "Function.h"

namespace cross{

/*	Event provides functional for object to notify other instances when something of interest happened.
	Class that want to notify of something must call Emit() function of Event the rest object which was
	interested in particular Event must subscribed for this event by calling Connect() function */
template<class... Args>
class Event {
public:
	/* Notifies all subscribers than something of interest has happened by calling their functions provided in Connect() function */
	void Emit(Args... p);

	/* Subscribes current object on this Event. Object must provide its class function which will be called when Event Occurs */
	template<class Clazz> void Connect(Clazz* obj, void(Clazz::*func)(Args...));
	/* Subscribes current object on this Event. Object must provide its const class function which will be called when Event Occurs */
	template<class Clazz> void Connect(Clazz* obj, void(Clazz::*func)(Args...) const);
	/* Unsubscribes current object from this Event. IMPORTANT! All objects must be disconnected from Events before they die */
	template<class Clazz> void Disconnect(Clazz* obj, void(Clazz::*func)(Args...));
	/* Disconnect all subscribers from this Event */
	void DisconnectAll();

private:
	std::map<U64, Function<void(Args...)>> connections;

	template<class Clazz> U64 Hash(Clazz* obj, void(Clazz::*func)(Args...));
	void Connect(U64 hash, Function<void(Args...)> const& func);
};

//implementation
template<class... Args>
void Event<Args...>::Emit(Args... p) {
	for (auto it : connections) {
		it.second(p...);
	}
}

template<class... Args>
template<class Clazz>
void Event<Args...>::Connect(Clazz* obj, void(Clazz::*func)(Args...)) {
	Connect(Hash(obj, func), [=](Args... args) { (obj->*func)(args...); });
}

template<class... Args>
template<class Clazz>
void Event<Args...>::Connect(Clazz* obj, void(Clazz::*func)(Args...) const) {
	Connect(Hash(obj, func), [=](Args... args) { (obj->*func)(args...); });
}

template<class... Args>
template<class Clazz>
void Event<Args...>::Disconnect(Clazz* obj, void(Clazz::*func)(Args...)) {
	U64 hash = Hash(obj, func);
	//Function does not connected
	if(connections.find(hash) == connections.end()) {
		assert(false);
	}
	connections.erase(hash);
}

template<class... Args>
void Event<Args...>::DisconnectAll() {
	connections.clear();
}

template<class... Args>
template<class Clazz>
U64 Event<Args...>::Hash(Clazz* obj, void(Clazz::*func)(Args...)) {
	void* fPointer = (void*&)func;
	return (U64)obj + (U64)fPointer;
}

template<class... Args>
void Event<Args...>::Connect(U64 hash, Function<void(Args...)> const& func) {
	connections.insert(std::make_pair(hash, func));
}

};
