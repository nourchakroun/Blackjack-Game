#include <SFML/Graphics.hpp>
#include <iostream>

class Jeton
{

public:
	Jeton()
	{}

	Jeton(sf::Sprite sp, int val)
	{
		picture = sp;
		value = val;
	}
	void setPosition(float x, float y)
	{
		sf::Vector2f pos(x, y);
		picture.setPosition(pos);
	}
	int getvalue()
	{
		return value;
	}
	void drawTo(sf::RenderWindow& window)
	{
		window.draw(picture);
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

		float btnPosX = picture.getPosition().x;
		float btnPosY = picture.getPosition().y;

		float btnxPoswidth = picture.getPosition().x + picture.getLocalBounds().width;
		float btnyPosheight = picture.getPosition().y + picture.getLocalBounds().height;

		if (mouseX < btnxPoswidth && mouseX > btnPosX && mouseY < btnyPosheight && mouseY > btnPosY)
		{
			return true;
		}
		return false;
	}

private:
	sf::Sprite picture;
	int value;
	bool active;
};