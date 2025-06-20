#include "InputSystem.h"

void InputSystem::ProcessInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			// Handle quit event
		}
	}
}