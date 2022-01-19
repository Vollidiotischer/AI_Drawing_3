#include "Canvas_class.h"
#include <SFML/Graphics.hpp>
#include <array>


std::array<bool, 100> canvas_transmitter; 


void start_canvas() {

	Canvas canvas(1000, 1000); 

	while (canvas.active) {
		canvas.events_canvas(); 
		canvas.draw_canvas();

		canvas_transmitter = canvas.canvas_data;

	}
}

/*

init sfml

canvas[i][i2] = canvas[i * 10 + i2]

29612 byte für 100 array size
29316 byte für 99 array size
= 
296 byte pro array size
*/


