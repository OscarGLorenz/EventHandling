/*
 * EventHandling.hpp
 *
 *  Created on: 17 de ago. de 2016
 *      Author: root
 */

#ifndef EVENTHANDLING_HPP_
#define EVENTHANDLING_HPP_

#include "Dinamicas/Estructuras/Queue.h"

enum class EventPriority {
	LOWEST, LOW, NORMAL, HIGH, HIGHEST, MONITOR
};
/** Prioridades diferentes para Handlers, de menor a mayor */

const EventPriority EventPriorities[] = { EventPriority::MONITOR,
		EventPriority::HIGHEST, EventPriority::HIGH, EventPriority::NORMAL,
		EventPriority::LOW, EventPriority::LOWEST };
/** Array con las prioridades en orden descendiente para iterar a través de él */

class Event {

	/** Clase que deben heredar los eventos definidos por el usuario */

public:

	bool isCancelled();
	/** Devuelve si el evento esta cancelado o no */

	void setCancelled();
	/** Cambia el estado del evento a cancelado */

protected:

	bool _isCancelled = false;
	/** Estado del evento */

};

class Handler {

	/** Clase que deben heredar los actuadores que defina el usuario */

public:

	bool ignoresCancelled();
	/** Devuelve si ignora eventos cancelados o no */

	EventPriority getPriority();
	/** Devuelve la prioridad del Handler */

	virtual void triggerEvent(Event * event) = 0;
	/** Esta función debe ser sobreescrita al heredar esta clase, como parametro se
	 *  pasa un puntero a Evento. Es el punto de entrada de la clase */

	Handler(bool IgnoreCancelled, EventPriority Priority);
	/** Constructor de la función dados la prioridad y si ignora eventos cancelados */

	Handler();
	/** Constructor sin parametros, no admite eventos cancelados y tiene la menor prioridad */

	Handler(bool IgnoreCancelled);
	/** Constructor con el parametro de si ignora eventos cancelados, la prioridad es la menor */

	Handler(EventPriority Priority);
	/** Constructor con la prioridad como parametro, no admite eventos cancelados */

	virtual ~Handler() {
	}

protected:

	bool ignoreCancelled;
	/** Ignora eventos cancelados */

	EventPriority priority;
	/** Prioridad del Handler */

};

class Listener {

	/** Clase que debe ser heredada para crear detectores de cambio, si
	 * debe estar pendiente al cambio de cierto valor es recomendble pasar
	 * la referencia de ese valor en el constructor */

public:

	virtual Event * check() = 0;
	/** Esta función debe ser sobreescrita, comprueba si debe devolverse un evento o no
	 * 	Si no hay cambios debe devolverse nullptr */

	virtual ~Listener() {

	}
};

class EventHandler {

	/** Se debe tener una instancia de esta clase, almacena Listeners y Handlers */

public:

	void update();
	/** Esta función debe ser llamada para que se ejecute en todos los Listeners su función check().
	 * Posteriormente pase a los Handlers los Eventos resultantes. Por prioridad descendiente y teniendo en cuenta
	 * si han sido cancelados */

	void registerHandler(Handler * handler);
	/** Añade un Handler */

	void registerListener(Listener * listener);
	/** Añade un Listener */

private:

	dyn::Queue<Event *> checkEvents();
	/** Comprueba los Listeners y retorna un vector con los eventos resultantes */

	dyn::Queue<Handler *> handlers;
	/** Almacena los Handlers */

	dyn::Queue<Listener *> listeners;
	/** Almacena los Listeners */

};

#endif
