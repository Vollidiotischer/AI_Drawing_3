#pragma once

#define AI_COUNT 250
#define AI_LAYOUT 100, 50, 10, 10
#define AI_RNG 10
#define AI_RNG_FACTOR 1



/*
AI_COUNT number of ai instances per generation
AI_LAYOUT the layout of the ai:
	inp layer
	hidden layer(s)
	output layer
AI_RNG the number of totally random AIs per generation
AI_RNG_FACTOR number the ai training random generator is multiplies with (smaller -> more precise) 
*/