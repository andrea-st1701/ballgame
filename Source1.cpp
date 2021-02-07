#include <SFML/Graphics.hpp>
#include <iostream>
class ball: public sf::Drawable
{
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		//states.transform *= getTransform();
		states.texture = NULL;
		target.draw(circle, states);
	}
	sf::CircleShape circle;
	float posyc;
	float xc = 20, yc = 560;
	float radius = 20.f;
	sf::Clock clock;
	sf::Clock colorclk;
	sf::Time elapsed;
	
public:
	ball();
	bool jump(float speed);
	void restartclk() { clock.restart(); };
	void color();
	void draw(sf::RenderWindow& window) { window.draw(circle); };


};
struct obs
{
	sf::RectangleShape rect;
	float posx;
	obs* next;
	obs();
};
obs::obs()
{
	next = NULL;
	posx = 725;
	rect.setSize(sf::Vector2f(25.f, 50.f));
	rect.setPosition(posx, 550);
	rect.setFillColor(sf::Color(255, 255, 255));
}

class obstacles: public sf::Drawable
{
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (!head)
		{
			return;
		}
		else
		{
			obs* p = head;
			while (p != NULL)
			{
				target.draw(p->rect, states);
				p = p->next;
			}
		}
	}
	obs* head;
	sf::Clock obstacleclk;
public:
	obstacles();
	void add();
	void update();
	void Delete();
	void Draw(sf::RenderWindow &window);
	sf::Time getElapsed() { return obstacleclk.getElapsedTime(); };
};
obstacles::obstacles()
{
	head = NULL;
	obstacleclk.restart();
}

void obstacles::Draw(sf::RenderWindow &window) 
{

	if (!head)
	{
		return;
	}
	else
	{
		obs* p = head; 
		while (p != NULL)
		{
			window.draw(p->rect);
			p = p->next;
		}
	}

}
void obstacles::add()
{
	if (!head)
	{
		head = new obs;
	}
	else
	{
		obs* p = head;
		while (p->next != NULL)
		{
			p = p->next;

		}
		p->next = new obs;
		
	}
	obstacleclk.restart();
}
void obstacles::Delete()
{

	obs* p = head;
		head = p->next;
		delete p;

	
}
void obstacles::update()
{
	int i = 0;
	obs* p = head;
	while (p != NULL)
	{

		p->posx--;
		p->rect.setPosition(p->posx, 550);
		if (p->posx <= -50)
		{
			Delete();
			return;
		}
		p = p->next;
		i++;
	}
}
ball::ball()
{
	circle.setRadius(radius);
	circle.setFillColor(sf::Color(255, 255, 255));
	circle.setPosition(xc, yc);
	posyc = 0;
}
bool ball::jump(float speed)
{
	elapsed = clock.getElapsedTime();
	float e = elapsed.asSeconds();
	posyc = 100 * (speed * e - 0.5 * 9.81 * pow(e, 2));
	std::cout << posyc << std::endl;
	if (posyc <= 0) 
	{ 

		posyc = 0; 
		circle.setPosition(xc, yc - posyc);
		return 0; 
	}
	circle.setPosition(xc, yc - posyc);
	return 1;
}
void ball::color()
{
	sf::Time color = colorclk.getElapsedTime();
	int c = color.asMilliseconds();
	circle.setFillColor(sf::Color(c % 256, 256 - (c % 256), (c % 256)));
}
int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(700, 600), "window",sf::Style::Default, settings);
	ball a;
	obstacles b;

	bool z=0;
	
	
	sf::Time obstime=sf::seconds(std::rand() % 3);
	
	std::cout << obstime.asSeconds()<<std::endl;
	bool f = 0, c = 0;
	srand(time(NULL));
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}
		std::cout << b.getElapsed().asSeconds();
		if ((int)b.getElapsed().asSeconds() == obstime.asSeconds())
		{
			b.add();
			obstime = sf::seconds((std::rand() % 5)+1);
			std::cout << "restart" <<obstime.asSeconds()<< std::endl;
		}
		b.update();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)&&!c&&!z)
		{
			c = 1;
			a.restartclk();
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && !z&&!c)
		{
			z = 1;
			a.restartclk();
		}
		if (c == 1)
		{
			c=a.jump(3.5f);
		}
		if (z == 1)
		{
			z = a.jump(10.f);
		}
		a.color();
		window.clear();
		window.draw(a);
		//b.Draw(window);
		window.draw(b);
		window.display();
		
	}
	
}
