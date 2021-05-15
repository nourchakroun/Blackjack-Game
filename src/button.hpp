#include <SFML/Graphics.hpp>
#include <iostream>

class Button
{
public:
	Button()
	{}
	Button(sf::Sprite notpr, sf::Sprite pr)
	{
		pressed = pr;
		notpressed = notpr;
		active = false;
	}
	void setPosition(float x, float y)
	{
		sf::Vector2f pos(x, y);
		sf::Vector2f pos2(x - 6, y - 6);
		pressed.setPosition(pos);
		notpressed.setPosition(pos2);
	}
	void drawPressedTo(sf::RenderWindow& window)
	{
		window.draw(pressed);
	}
	void drawNotPressedTo(sf::RenderWindow& window)
	{
		window.draw(notpressed);
	}
	bool ispressed()
	{
		return active;
	}
	void press()
	{
		active = true;
	}
	void release()
	{
		active = false;
	}

	bool isMouseOver(sf::RenderWindow& window)
	{
		float mouseX = sf::Mouse::getPosition(window).x;
		float mouseY = sf::Mouse::getPosition(window).y;

		float btnPosX = notpressed.getPosition().x;
		float btnPosY = notpressed.getPosition().y;

		float btnxPoswidth = notpressed.getPosition().x + notpressed.getLocalBounds().width;
		float btnyPosheight = notpressed.getPosition().y + notpressed.getLocalBounds().height;

		if (mouseX < btnxPoswidth && mouseX > btnPosX && mouseY < btnyPosheight && mouseY > btnPosY)
		{
			return true;
		}
		return false;
	}

private:
	sf::Sprite pressed;
	sf::Sprite notpressed;
	bool active;
};