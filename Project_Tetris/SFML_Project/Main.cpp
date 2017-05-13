#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
#include "TetrisHeader.h"

int main()
{
	srand(static_cast<unsigned int>(time(0)));
	sf::RenderWindow window(sf::VideoMode(420, 580), "Tetris Game!");
	Tetris game;

	float timer = 0;
	sf::Clock clock;
	while (window.isOpen())
	{
		//sf::Time dt = clock.restart();
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;
		sf::Event event;
	
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			game.ButtonControls(event);
			
		}
		
		
		window.clear();
		game.Update(&window, timer);
		if (timer > 0.3)
		{
			timer = 0;
			
		}
		game.Draw(&window);
		window.display();
	}

	return 0;
}