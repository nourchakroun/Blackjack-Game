#include <SFML/Graphics.hpp>
#include <algorithm> //std::shuffle
#include <chrono>	 // std::chrono::system_clock
#include <iostream>
#include <map>
#include <random> // std::default_random_engine
#include <string>
#include <vector>
using namespace std;

string suits[4] = { "Hearts", "Diamond", "Spades", "Clubs" };
string ranks[13] = { "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King", "Ace" };
map<string, int> values = { { "Two", 2 }, { "Three", 3 }, { "Four", 4 }, { "Five", 5 }, { "Six", 6 }, { "Seven", 7 }, { "Eight", 8 }, { "Nine", 9 }, { "Ten", 10 }, { "Jack", 10 }, { "Queen", 10 }, { "King", 10 }, { "Ace", 11 } };

/***********************************************************************************************************************************/
class carte
{
private:
	string suit;
	string rank;

public:
	carte(string suit, string rank)
	{
		this->suit = suit;
		this->rank = rank;
	}

	string get_rank()
	{
		return rank;
	}
	string get_suit()
	{
		return suit;
	}
};

class sabot
{

private:
	vector<carte> lis;

public:
	sabot()
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 13; j++)
			{
				carte c(suits[i], ranks[j]);
				lis.push_back(c);
			}
		}
	}

	void melanger()
	{
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		shuffle(lis.begin(), lis.end(), default_random_engine(seed));
	}
	carte tirer()
	{
		auto c = lis.back();
		lis.pop_back();
		return c;
	}
};

class mains
{
private:
	vector<carte> v;
	int value;
	int aces; //keep track of aces

public:
	mains()
	{
		value = 0;
		aces = 0;
	}
	void add_card(carte c)
	{
		v.push_back(c);
		value += values[c.get_rank()];
		if (c.get_rank() == "Ace")
		{
			aces++;
		}
	}
	void adjust_for_ace()
	{
		while (value > 21 and aces != 0)
		{
			value -= 10;
			aces -= 1;
		}
	}
	vector<carte> get_v()
	{
		return v;
	}
	int get_value()
	{
		return value;
	}
};

class mise
{

private:
	int total;
	int mise_enjeu;

public:
	mise(int total = 100)
	{
		this->total = total;
		this->mise_enjeu = 0;
	}

	int get_total()
	{
		return total;
	}

	int get_mise_enjeu()
	{
		return mise_enjeu;
	}

	void gain()
	{
		total += mise_enjeu;
	}
	void perte()
	{
		total -= mise_enjeu;
	}
	void set_ships(int sh)
	{
		mise_enjeu = sh;
	}

	void donner_mise_en_jeu()
	{

		string str;
		int n;
		while (1)
		{
			try
			{
				cout << "How many chips would you like to bet? " << endl;
				cin >> str;
				n = stoi(str);
				if (n > total)
				{
					cout << "Your bet can't exceed " << total << endl;
					continue;
				}
				if (n <= 0)
				{
					cout << "Your bet must be > 0 " << endl;
					continue;
				}
				else
				{
					break;
				}
			}
			catch (...)
			{
				cout << "Sorry! Please can you type in a number: \n";
				continue;
			}
		}
		mise_enjeu = n;
	}
};

class jeu
{
private:
	sabot s;
	mains jr;
	mains crp;
	mise m;
	bool playing = true;

public:
	jeu()
	{
		cout << "Game Started !" << endl;
	}

	void debut_jeu(int ship)
	{
		s.melanger();
		jr.add_card(s.tirer());
		jr.add_card(s.tirer());
		jr.adjust_for_ace();
		crp.add_card(s.tirer());
		crp.add_card(s.tirer());
		crp.adjust_for_ace();
		m.set_ships(ship);
	}

	void afficher_partiel(sf::RenderWindow& window)
	{
		vector<carte> croupie, joueur;
		croupie = crp.get_v();
		joueur = jr.get_v();

		sf::Texture hiden;
		sf::Sprite hiden_spirite;
		hiden.loadFromFile("images\\cardBack.png");
		hiden_spirite.setTexture(hiden);
		hiden_spirite.setPosition(300, 30);
		window.draw(hiden_spirite);
		sf::Texture cp1;
		cp1.loadFromFile("images\\" + croupie[1].get_rank() + "_of_" + croupie[1].get_suit() + ".png");
		sf::Sprite crd1;
		crd1.setTexture(cp1);
		crd1.setPosition(500, 30);
		window.draw(crd1);

		sf::Texture text[50];
		sf::Sprite out[50];
		int distance = 200;
		if (joueur.size() > 5)
		{
			distance = 2 * distance / joueur.size();
		}
		for (unsigned i = 0; i < joueur.size(); i++)
		{
			text[i].loadFromFile("images\\" + joueur[i].get_rank() + "_of_" + joueur[i].get_suit() + ".png");
			out[i].setTexture(text[i]);
			out[i].setPosition(300 + distance * i, 450);
			window.draw(out[i]);
		}
	}
	mains get_jr()
	{
		return jr;
	}
	mains get_crp()
	{
		return crp;
	}
	mise get_m()
	{
		return m;
	}
	void hit_jr()
	{
		jr.add_card(s.tirer());
		jr.adjust_for_ace();
	}
	void hit_crp()
	{
		crp.add_card(s.tirer());
		crp.adjust_for_ace();
	}
	void afficher_tout(sf::RenderWindow& window)
	{
		vector<carte> croupier = crp.get_v();
		vector<carte> joueur = jr.get_v();

		sf::Texture t[50];
		sf::Sprite t1[50];
		int distance = 200;
		if (croupier.size() > 5)
		{
			distance = 2 * distance / croupier.size();
		}
		for (unsigned i = 0; i < croupier.size(); i++)
		{
			t[i].loadFromFile("images\\" + croupier[i].get_rank() + "_of_" + croupier[i].get_suit() + ".png");
			//cout << croupier[i].get_rank() << "_of_" << croupier[i].get_suit() << endl;
			t1[i].setTexture(t[i]);
			t1[i].setPosition(300 + distance * i, 30);
			window.draw(t1[i]);
		}
		distance = 200;
		if (joueur.size() > 5)
		{
			distance = 2 * distance / joueur.size();
		}
		for (unsigned i = 0; i < joueur.size(); i++)
		{
			t[i].loadFromFile("images\\" + joueur[i].get_rank() + "_of_" + joueur[i].get_suit() + ".png");
			t1[i].setTexture(t[i]);
			t1[i].setPosition(300 + distance * i, 450);
			window.draw(t1[i]);
			//cout << joueur[i].get_rank() << "_of_" << joueur[i].get_suit() << endl;
		}
	}
	void playin()
	{
		playing = true;
	}
	void stand()
	{
		playing = false;
	}
	bool getplaying()
	{
		return playing;
	}

	/*void afficher_crp()
	{
		vector<carte> croupier = crp.get_v();
		cout << "\nDealer's Hand: " << endl;
		for (int i = 0; i < croupier.size(); i++)
		{
			cout << croupier[i] << endl;
		}
		cout << "Dealer's Value =" << crp.get_value() << endl;
	}*/
	/*
	void hit_or_stand()
	{
		string ask;
		while (1)
		{
			cout << "\nWould you like to hit or stand? Please enter 'h' or 's': ";
			cin >> ask;
			transform(ask.begin(), ask.end(), ask.begin(), ::tolower); // a function from algorithm module that  convert string to lower ,
			if (ask[0] == 'h')
			{
				this->hit_jr();
				this->afficher_partiel();
				break;
			}
			if (ask[0] == 's')
			{
				cout << "Player stands, Dealer is playing." << endl;
				playing = false;
				break;
			}
			else
			{
				cout << "Sorry! I did not understand that! Please try again!" << endl;
			}
		}
	}*/
	string player_busts()
	{
		m.perte();
		return "PLAYER BUSTS!";
	}

	string player_wins()
	{
		m.gain();
		return "PLAYER WINS!";
	}

	string dealer_busts()
	{
		m.gain();
		return "DEALER BUSTS!";
	}

	string dealer_wins()
	{
		m.perte();
		return "DEALER WINS!";
	}

	string push()
	{
		return "Its a push! Player and Dealer tie!";
	}
};
/*
void jouer()
{
	while (1)
	{
		cout << "Welcome to BlackJack!" << endl;

		//create and shuffle deck
		jeu game;
		game.debut_jeu();

		game.afficher_partiel();


		while (playing)
		{

			game.hit_or_stand();

			if (game.get_jr().get_value() > 21)
			{
				game.player_busts();
				break;
			}
		}

		if (game.get_jr().get_value() <= 21)
		{
			while (game.get_crp().get_value() < 17)
			{
				game.afficher_crp();
				cout << "\nthe dealer's value < 17 :" << endl;
				cout << "the dealer get an other card\n"
					 << endl;
				game.hit_crp();
			}
			game.afficher_tout();
			if (game.get_crp().get_value() > 21)
			{
				game.dealer_busts();
			}
			if (game.get_jr().get_value() > 21)
			{
				game.player_busts();
			}
			else if (game.get_crp().get_value() > game.get_jr().get_value())
			{
				game.dealer_wins();
			}
			else if (game.get_crp().get_value() < game.get_jr().get_value())
			{
				game.player_wins();
			}
			else if (game.get_crp().get_value() == game.get_jr().get_value())
			{
				game.push();
			}
		}
		cout << "\nPlayer's winnings stand at " << game.get_m().get_total() << endl;
		string new_game;
		cout << "\nWould you like to play again? Enter 'y' or 'n': " << endl;
		cin >> new_game;
		transform(new_game.begin(), new_game.end(), new_game.begin(), ::tolower); // a function from algorithm module that  convert string to lowercase
		if (new_game[0] == 'y')
		{
			playing = true;
			continue;
		}
		else
		{
			cout << "\nThanks for playing!" << endl;
			break;
		}
	}
}
*/
