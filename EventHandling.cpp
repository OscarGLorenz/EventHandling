/*
 * EventHandling.cpp
 *
 * Copyright 2016 Oscar Garcia Lorenz <oscar@OGLPC>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 *
 */

#include "EventHandling.hpp"

bool Event::isCancelled() {
	return _isCancelled;
}

void Event::setCancelled() {
	_isCancelled = true;
}

bool Handler::ignoresCancelled() {
	return ignoreCancelled;
}

EventPriority Handler::getPriority() {
	return priority;
}

Handler::Handler(bool IgnoreCancelled, EventPriority Priority) {
	ignoreCancelled = IgnoreCancelled;
	priority = Priority;
}

Handler::Handler() :
		Handler(false, EventPriority::LOWEST) {
}

Handler::Handler(bool IgnoreCancelled) :
		Handler(IgnoreCancelled, EventPriority::LOWEST) {
}

Handler::Handler(EventPriority Priority) :
		Handler(false, Priority) {
}

void EventHandler::update() {
	dyn::Queue_Iterator<Event *> itrEvents = checkEvents().getIterator();
	dyn::Queue_Iterator<Handler *> itrHandlers = handlers.getIterator();

	while (itrEvents.hasNext()) {
		Event * event = itrEvents.next();
		for (EventPriority priority : EventPriorities) {
			while (itrHandlers.hasNext()) {
				Handler * handler = itrHandlers.next();
				if (!(event->isCancelled() && !handler->ignoresCancelled())) {
					if (handler->getPriority() == priority) {
						handler->triggerEvent(event);
					}
				}
			}
		}
	}
}

void EventHandler::registerHandler(Handler * handler) {
	handlers.pushBack(handler);
}

void EventHandler::registerListener(Listener * listener) {
	listeners.pushBack(listener);
	listeners.pushBack(listener);
}

dyn::Queue<Event *> EventHandler::checkEvents() {
	dyn::Queue<Event *> events;
	dyn::Queue_Iterator<Listener *> itrListeners = listeners.getIterator();

	while (itrListeners.hasNext()) {
		Event * event = itrListeners.next()->check();
		if (event != nullptr) {
			events.pushBack(event);
		}
	}
	return events;
}
