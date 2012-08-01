#include <SFML/Graphics.hpp>

int main()
{
	sf::VideoMode viewMode(640, 480, 32);
	sf::RenderWindow window(viewMode, "SFML works!");
	sf::Text text("Hello world!");

	while (window.isOpen())
	{
		sf::Event evt;
		while (window.pollEvent(evt))
		{
			switch(evt.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			default:
				break;
			}
		}

		window.clear();
		window.draw(text);
		window.display();
	}

	return EXIT_SUCCESS;
}