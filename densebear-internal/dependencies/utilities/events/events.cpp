#include "events.hpp"
#include "../../../core/features/misc/hitmarker.hpp"
#pragma comment(lib, "Winmm.lib")

c_hooked_events events;

void c_hooked_events::fire_game_event(i_game_event* event) {
	auto event_name = event->get_name();

	if (!strcmp(event_name, "player_hurt")) {
		misc::hitmarker::event(event);
	}

	else if (strstr(event->get_name(), "item_purchase")) {
	}

	else if (strstr(event->get_name(), "player_footstep")) {
	}

	else if (!strcmp(event_name, "player_death")) {
	}

}

int c_hooked_events::get_event_debug_id(void) {
	return EVENT_DEBUG_ID_INIT;
}

void c_hooked_events::initialize() {
	debug_id = EVENT_DEBUG_ID_INIT;
	interfaces::event_manager->add_listener(this, "player_hurt", false);
	interfaces::event_manager->add_listener(this, "item_purchase", false);
	interfaces::event_manager->add_listener(this, "player_footstep", false);
	interfaces::event_manager->add_listener(this, "player_death", false);

	printf("[+] Events system initialized!\n");
}

void c_hooked_events::release() {
	interfaces::event_manager->remove_listener(this);
}

