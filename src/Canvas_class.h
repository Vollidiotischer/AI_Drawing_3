#include <SFML/Graphics.hpp>
#include <array>
#include <iostream>
#include <fstream>

struct Canvas{
	
	std::array<bool, 100> canvas_data;

	sf::RectangleShape utility_shape; 

	sf::RenderWindow* window; 

	int w, h; 

	char state = 'n'; 

	bool active = true; 

	Canvas(int w, int h) {

		this->w = w; 
		this->h = h; 

		init(); 

	}

	void init() {
		window = new sf::RenderWindow(sf::VideoMode(w, h), "Canvas"); 
		window->setKeyRepeatEnabled(false); 

		utility_shape.setSize({ 100.0, 100.0 }); 

		std::fill_n(canvas_data.begin(), 100, 0); 
	}

	void events_canvas() {
		sf::Event events; 

		while (window->pollEvent(events)) {
			if (events.type == sf::Event::Closed) {
				active = false; 
				window->close();
			}

			if (events.type == sf::Event::KeyPressed) {
				
				// Num0 = 26 | ASCII '0' = 48
				// ...
				// Num9 = 35 | ASCII '9' = 57
				// difference between key code and ascii code: 22 -> add 22 to key code for char

				if (events.key.code >= sf::Keyboard::Num0 && events.key.code <= sf::Keyboard::Num9) {
					state = (char)(events.key.code + 22); 
					std::cout << "Switched to state " << state << std::endl; 
				}

				if (events.key.code == sf::Keyboard::S) {
					if (state == 'n') {
						std::cout << "No state selected" << std::endl; 
					}
					else{
						save_current_map(); 
						std::fill_n(canvas_data.begin(), 100, 0);

					}
				}

				if (events.key.code == sf::Keyboard::C || events.key.code == sf::Keyboard::X) {
					std::fill_n(canvas_data.begin(), 100, 0);

				}


			}

		}



		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
			auto pos = sf::Mouse::getPosition(*window);

			pos.x /= 100;
			pos.y /= 100;

			if (pos.x >= 0 && pos.y >= 0 && pos.x < 10 && pos.y < 10)
				canvas_data[pos.x + pos.y * 10] = sf::Mouse::isButtonPressed(sf::Mouse::Left);

		}

	}

	void save_current_map() {

		/*
		open file, check if file has been opened
		save the state eg 0..9 before the data
		save the data (in one line) eg 010010011010
		append a new line 
		*/

		std::ofstream file; 
		file.open("saved_maps.txt", std::ios::app); 

		if (!file.is_open()) {
			std::cout << "Could not open file" << std::endl; 
			return; 
		}

		file << state; 

		for (int i = 0; i < canvas_data.size(); i++) {
			file << canvas_data[i]; 
		}

		file << "\n"; 

		std::cout << "map saved with state " << state << std::endl; 

		return; 

	}

	void draw_canvas() {
		window->clear();

		for (int i = 0; i < canvas_data.size() / 10; i++) {
			for (int i2 = 0; i2 < canvas_data.size() / 10; i2++) {
				utility_shape.setPosition({ (float)( i * 100.0), (float)(i2 * 100.0) });

				if (canvas_data[i + 10 * i2] == 1) {
					utility_shape.setFillColor(sf::Color::White); 
				}
				else {
					utility_shape.setFillColor(sf::Color::Black);

				}

				window->draw(utility_shape);
			}
		}

		window->display(); 
	}

	void cleanup() {
		delete window; 
	}

};