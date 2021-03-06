// A_deck_of_cards.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <vector>
#include <iomanip>
#include <conio.h>
#include <cctype>
#include <algorithm>

using namespace std;
enum Suit{Spades=0,Hearts,Diamonds,Clubs};
class BlackJackHand;
template<typename T>
void remove(std::vector<T>& vector, unsigned int index)//https://qiita.com/akima/items/e50f787c923edf2bc6b7 std::vectorから指定インデックスの要素を削除
{
	vector.erase(vector.begin() + index);
}
bool check_int(std::string str)//https://qiita.com/edo1z/items/da66e28e206d2b01157e C++で文字列が数字かチェックする
{
	if (std::all_of(str.cbegin(), str.cend(), isdigit))
	{
		//std::cout << stoi(str) << std::endl;
		return true;
	}
	std::cout << "not int" << std::endl;
	return false;
}
class card {
	int Number;
	Suit suits;
	bool none=false;
public:
	card(int n, Suit s)
	{
		Number=n;
		suits=s;
	}
	card()
	{
		Number=0;
		suits=Spades;
		none=true;
	}
	Suit getSuit() {
		return suits;
	}
	int getNum() {
		return Number;
	}
	string getInfo() {
		if (none)
		{
			return "[  ]";
		}
		string c;
		string n;
		switch (suits)
		{
			case Spades:
				c='s';
				break;
			case Hearts:
				c='h';
				break;
			case Diamonds:
				c='d';
				break;
			case Clubs:
				c='c';
				break;
			default:
				break;
		}
		switch (Number)
		{
			case 1:
				n="A";
				break;
			case 11:
				n = "J";
				break;
			case 12:
				n = "Q";
				break;
			case 13:
				n = "K";
				break;
			default:
				n=to_string(Number);
				break;
		}
		string rt="["+n+c+"]";
		return rt;
	}
};
class deck_element :public card{
	deck_element *next;

	public:
		deck_element():card() {

	}
		deck_element(int num, Suit s) :card(num, s) {

	}
	void add(deck_element *node)
	{
			node->next=this;
			next=0;
	}
	deck_element *getnext()
	{
			return next;
	}


};

class cardpool {
	deck_element demo;
	deck_element *pointer,*lastpointer,*deletedemo;
	vector<deck_element> deck;
	vector<deck_element> bak;
	bool test=false;
	public:
	cardpool() 
	{
		lastpointer = &demo;
		deletedemo=&demo;
		if (test)
		{
			for (int s = 0; s<1; ++s)
			{
				for (int i = 1; i < 14; ++i)
				{
					pointer = new deck_element(i, (Suit)s);
					//cout << pointer->getInfo() << endl;

					pointer->add(lastpointer);
					lastpointer = pointer;
				}
			}
		}
		else
		{
			for(int s=0;s<4;++s)
			{
				for (int i = 1; i < 14;++i)
				{
					pointer=new deck_element(i,(Suit)s);
					//cout << pointer->getInfo() << endl;

					pointer->add(lastpointer);
					lastpointer=pointer;
				}
			}
		}
		//cout<<"on";
		pointer=deletedemo->getnext();//pointerは必ず一枚目に置く
		bool flag=true;
		do
		{
			//cout << pointer->getInfo() << endl;
			deck.push_back(*pointer);
			pointer=pointer->getnext();

			
			//cout<<deck.size()<<endl;
		}while (pointer);
		bak=deck;
		
		//cout<<"size:"<<deck.size();
	}
	deck_element pick_cards()
	{
		//メルセンヌ・ツイスタ
		random_device rnd;     // 非決定的な乱数生成器を生成
		mt19937 mt(rnd());
		uniform_int_distribution<> rand52(0,deck.size()-1);
		int id=rand52(mt);
		deck_element card = deck[id];
		remove(deck,id);
		//cout<<"削除後のサイズ:"<<deck.size()<<endl;
		return card;
	}
	void Initialization() {
		deck=bak;
	}
	void DeleteAll() {
		bak=deck;
		deck.erase(deck.begin(),deck.begin()+52);
	}
	void show() 
	{
		int count=0;
		for (int i = 0; i < (int)deck.size(); ++i) {
			cout<<deck[i].getInfo();
			++count;
			if (count == 13)
			{
				cout<<endl;
				count=0;
			}
		}
		cout<<"\n";
	}
	int showSize() {
		return deck.size();
	}
	void insert_cards(deck_element d)
	{
		deck.push_back(d);
	}
	int showSum_BLACKJACK() {
		
		int t=0,ace=0;
		for (int i = 0; i < deck.size(); ++i)
		{
			if(deck[i].getNum()!=1&&deck[i].getNum()<=10)
			{ 
				t+=deck[i].getNum();
			}
			else if (11<=deck[i].getNum()&&deck[i].getNum() <= 13)
			{
				t+=10;
			}
			else if (deck[i].getNum() == 1)
			{
				++ace;
			}
		}
		if (ace == 0)
		{
			return t;
		}
		else if (ace == 1)
		{
			if (t >= 11)
			{
				++t;
			}
			else
			{
				t+=11;
			}
		}
		else if (ace >= 2)
		{
			if (t + 10+ace < 22)
			{
				t+=10+ace;
			}
			else
			{
				t+=ace;
			}
		}
		return t;

	}
};
class BlackJackHand //http ://odds-casino.tokyo/rule/blackjack/ブラックジャック-基本ルール/
{
	cardpool hand;
	bool player;
	//mode 0:ブタ　1:通常手　2:BlackJack(最初の二枚で21を達成)
	int mode, handsum;
public:
	BlackJackHand(bool _IsPlayer)
	{
		player = _IsPlayer;
		hand.DeleteAll();
	}
	void insert_first_hand(deck_element card1, deck_element card2)
	{
		hand.insert_cards(card1);
		hand.insert_cards(card2);
		if (hand.showSize() == 2)
		{
		
			if (hand.showSum_BLACKJACK() == 21)
			{
				mode = 2;
			}
			else
			{
				mode = 1;
			}
		}
		else {
			cout << "手札をもっています" << endl;
		}
	}
	void insert_hand(deck_element card)//hit
	{
		hand.insert_cards(card);
		if (hand.showSum_BLACKJACK() > 21)
		{
			mode = 0;
		}
		else
		{
			mode = 1;
		}
	}
	void show_hands()
	{
		if (player)
		{
			hand.show();
		}
		else
		{
			deck_element temp;
			cout << temp.getInfo();//empty
			temp = hand.pick_cards();
			hand.show();
			hand.insert_cards(temp);
		}
	}
	void show_all()
	{
		hand.show();

	}
	int show_sum()
	{
		if (player)
		{
			return hand.showSum_BLACKJACK();
		}
		else
		{
			int rt;
			deck_element temp;
			//empty
			temp = hand.pick_cards();
			rt = hand.showSum_BLACKJACK();
			hand.insert_cards(temp);
			return rt;
		}
	}
	int show_sum_all()
	{
		return hand.showSum_BLACKJACK();
	}

	int show_mode() {
		return mode;
	}
};
class GameMaster
{
	int coins;
	public:
	GameMaster(int n)
	{
		coins=n;
	}
	int COOL104_core()
	{
		cardpool cp;
		deck_element d,h1,h2,h3,h4,h5,m,ghost;
		h1 = cp.pick_cards();
		h2 = cp.pick_cards();
		h3 = cp.pick_cards();
		h4 = cp.pick_cards();
		h5 = cp.pick_cards();
		int maisu=coins;
		int point=0;
		bool con=true;
		bool sign;
		char key;
		int laps=1;
		bool accept;
		while (con)
		{
			cout<<"coin"<<right<<setw(4)<<maisu<<d.getInfo()<<"point:"<<point<<endl;
			cout<<h1.getInfo()<< h2.getInfo() << h3.getInfo() << h4.getInfo() << h5.getInfo() <<endl;
			if (point == 52*laps)
			{
				if (laps == 1)
				{
					cout<<"\nGREAT!! GO TO NEXT STAGE !\n"<<endl;
					++laps;
					cp.Initialization();
					d=ghost;
					h1 = cp.pick_cards();
					h2 = cp.pick_cards();
					h3 = cp.pick_cards();
					h4 = cp.pick_cards();
					h5 = cp.pick_cards();
					cout << "coin" << right << setw(4) << maisu << d.getInfo() << "point:" << point << endl;
					cout << h1.getInfo() << h2.getInfo() << h3.getInfo() << h4.getInfo() << h5.getInfo() << endl;
				}
				else {
					cout<<"\nCONGRATULATION!! PERFECT!!\n"<<endl;
					return 104;
				}
				
			}
			if ((point != 0) && (h1.getNum() != d.getNum() && h1.getSuit() != d.getSuit() && h2.getNum() != d.getNum() && h2.getSuit() != d.getSuit() && h3.getNum() != d.getNum() && h3.getSuit() != d.getSuit() && h4.getNum() != d.getNum() && h4.getSuit() != d.getSuit() && h5.getNum() != d.getNum() && h5.getSuit() != d.getSuit()))
			{
				con=false;
				cout<<"YourPoints:"<<point<<endl;
				return point;
			}

			key=_getch();
			cout<<"\n\n\n\n\n\n\n\n\n";
			if (key == '1' || key == '2' || key == '3' || key == '4' || key == '5')
			{
				switch (key) {
				case '1':
					m = h1;
					break;
				case '2':
					m = h2;
					break;
				case '3':
					m = h3;
					break;
				case '4':
					m = h4;
					break;
				case '5':
					m = h5;
					break;
				}
				if (point == 0)
				{
					accept=true;
				}
				else {

					if (d.getNum() == m.getNum() || (int)d.getSuit() == (int)m.getSuit())
					{
						accept=true;
					}
					else {
						accept=false;
						cout<<"無効な選択です"<<endl;
					}
				}
				if (accept)
				{
					d=m;
					if(cp.showSize()!=0)
					{ 
						switch (key) {
						case '1':
							h1=cp.pick_cards();
							break;
						case '2':
							h2=cp.pick_cards();
							break;
						case '3':
							h3 = cp.pick_cards();
							break;
						case '4':
							h4 = cp.pick_cards();
							break;
						case '5':
							h5 = cp.pick_cards();
							break;
						}
						++point;
					}
					else
					{
						switch (key) {
						case '1':
							h1 = ghost;
							break;
						case '2':
							h2 = ghost;
							break;
						case '3':
							h3 = ghost;
							break;
						case '4':
							h4 = ghost;
							break;
						case '5':
							h5 = ghost;
							break;
						}
						++point;
					}
				}
				

			}
			else if (key == 'l')
			{
				cout<<"残りのカードを表示します。(場に出ているカードは入っていません)"<<endl;
				cp.show();
				cout<<"\n\n";
			}
			else {
				cout<<"入力値が不正です"<<endl;
			}
		}

	
	}
	int  COOL104_ODDS(int cards,bool call)//http://www.adores.jp/machine_manual/cool104.html
	{
		if (cards < 10)
		{
			if(!call)
			{ 
				cout<<"GAME OVER"<<endl;
			}
			return 0;
		}
		else
		{
			if(!call)
			{ 
				cout<<"WINNER!";
			}
			if (10 <= cards <= 13)
			{
				return 1;
			}
			else if (14 <= cards <= 16)
			{
				return 2;
			}
			else if (17 <= cards <= 19)
			{
				return 3;
			}
			else if (20 <= cards <= 22)
			{
				return 4;
			}
			else if (23 <= cards <= 25)
			{
				return 6;
			}
			else if (26 <= cards <= 28)
			{
				return 8;
			}
			else if (29 <= cards <= 30)
			{
				return 10;
			}
			else if (31 <= cards <= 32)
			{
				return 12;
			}
			else if (cards <= 34)
			{
				return 14;
			}
			else if (cards <= 36)
			{
				return 16;
			}
			else if (cards <= 38)
			{
				return 18;
			}
			else if (cards <= 40)
			{
				return 20;
			}
			else if (cards <= 42)
			{
				return 22;
			}
			else if (cards <= 50)
			{
				return (5*cards-190);
			}
			else if (cards <= 52)
			{
				return (20*cards-940);
			}
			else if (cards <= 82)
			{
				return (10*(cards-52)+100);
			}
			else if (cards <= 92)
			{
				return (20*(cards-52)-200);
			}
			else if (cards <= 98)
			{
				return (50*(cards-52)-1400);
			}
			else
			{
				int rt=0;
				switch(cards)
				{ 
					case 99:
					rt=1000;
					break;
					case 100:
					rt=1100;
					break;
					case 101:
					rt=1200;
					break;
					case 102:
					rt=1400;
					break;
					case 103:
					rt=1600;
					break;
					case 104:
					rt=2000;
					break;
				}
				return rt;
			}
		}
	}
	bool all_manager_COOL104()
	{
		bool gs=true;
		bool con = true,con2=true;
		string text;
		int n,p,get;
		char key;
		while(gs)
		{ 
				

				while (con)//BET TURN START
				{
					cout << "\n賭ける枚数は？(MAX5枚)(所持コイン:" << coins << ")" << endl;
					cin>>text;
					if (check_int(text))
					{
						n=stoi(text);
						if (1 <= n <= 5 && n <= coins)
						{
							con=false;
							cout<<"BET:"<<n<<endl;
							coins-=n;
						}
						else
						{
							cout<<"int型だけど値がおかしいです。"<<endl;
						}
					}
					else
					{
						cout<<"正しい値を入力してください"<<endl;
					}
				}
				con=true;
				//BET TURN END
				p=COOL104_core();
				get=COOL104_ODDS(p,false);
				if (get == 0)
				{
					cout << "YOUR COIN(S):" << coins << endl;
				}
				else
				{
					cout<<"You earned "<<(get*n)<<"coins!"<<endl;
					coins+=(n*get);
					cout<<"YOUR COIN(S): "<<coins<<endl;
				}
				if (coins == 0)
				{
					gs=false;
					con=false;
					con2=false;
					//EXITに誘導
				}
				while(con2)
				{
					cout << "\n同ベットで続ける:1,ベットを変えて続ける:2,終了:3,BLACKJACKをプレイする:4"<<endl;
					key = _getch();
					cout<<"\n";
					switch (key)
					{
					case '1':
						con = false;
						con2 = false;
						coins -= n;
						if (coins < 0)
						{
							con = true;
							cout << "コインが足りません" << endl;
						}
						break;
					case '2':
						return false;
						break;
					case '3':
						gs = false;
						con = false;
						con2 = false;
						//EXITに誘導
						break;
					case '4':
						return true;
						break;
					}

				}
				con2=true;

		}
	}
	bool all_manager_BLACKJACK()
	{
		bool gs = true;
		bool con = true, con2 = true;
		string text;
		int n, p;
		char key;
		while (gs)
		{


			while (con)//BET TURN START
			{
				cout << "\n賭ける枚数は？(MAX5枚)(所持コイン:" << coins << ")" << endl;
				cin >> text;
				if (check_int(text))
				{
					n = stoi(text);
					if (1 <= n <= 5 && n <= coins)
					{
						con = false;
						cout << "BET:" << n << endl;
						coins -= n;
					}
					else
					{
						cout << "int型だけど値がおかしいです。" << endl;
					}
				}
				else
				{
					cout << "正しい値を入力してください" << endl;
				}
			}
			con = true;
			//BET TURN END
			p = BLACKJACK_core();
		
			if (p == 0)
			{
				cout << "YOUR COIN(S):" << coins << endl;
			}
			else
			{
				cout << "You earned " << (p*n) << "coins!" << endl;
				coins += (n*p);
				cout << "YOUR COIN(S): " << coins << endl;
			}
			if (coins == 0)
			{
				gs = false;
				con = false;
				con2 = false;
				//EXITに誘導
			}
			while (con2)
			{
				cout << "\n同ベットで続ける:1,ベットを変えて続ける:2,終了:3,COOL104をプレイする:4";
				key = _getch();
				switch (key)
				{
				case '1':
					con = false;
					con2 = false;
					coins -= n;
					if (coins < 0)
					{
						con = true;
						cout << "コインが足りません" << endl;
					}
					break;
				case '2':
					con2=false;
					break;
				case '3':
					gs = false;
					con = false;
					con2 = false;
					//EXITに誘導
					break;
				case '4':
				return true;
				break;
				}

			}
			con2 = true;

		}
	}


	int BLACKJACK_core ()
	{
		BlackJackHand player1(true);
		BlackJackHand dealer(false);
		cardpool cp;
		deck_element c1,c2,c3,c4;
		c1=cp.pick_cards();
		c2=cp.pick_cards();
		c3=cp.pick_cards();
		c4=cp.pick_cards();
		player1.insert_first_hand(c1,c2);
		dealer.insert_first_hand(c3,c4);
		cout<<"\n\n\n\n\n\n\n\nDEALER:";
		dealer.show_hands();
		dealer.show_sum();
		cout<<"coin(s):"<<coins<<endl;
		cout<<"You:";
		player1.show_all();
		player1.show_sum();

		if (player1.show_sum_all() == 21)
		{
			cout<<"\nBLACKJACK!"<<endl;
			cout << "DEALER:";
			dealer.show_all();
			dealer.show_sum_all();
			if (dealer.show_sum_all() == 21)
			{
				cout<<"DEALER:BLACKJACK"<<endl;
				cout<<"引き分け！"<<endl;
				return 1;
			}
			else
			{
				cout<<"DEALER:"<<dealer.show_sum_all()<<endl;
				cout<<"WINNER!"<<endl;
				return 3;
			}
		}
		else
		{
			bool con3=true;
			char key;
			while(con3)
			{ 
				cout<<"HIT:1,STAND:2"<<endl;
				key=_getch();
				switch (key)
				{
					case '1':
					player1.insert_hand(cp.pick_cards());
					break;
					case '2':
					con3=false;
					break;
				}
				cout << "\n\n\n\n\n\nDEALER:";
				dealer.show_hands();
				cout << "coin(s):" << coins << endl;
				cout << "You:";
				player1.show_all();
				player1.show_sum();
				if (player1.show_mode()==0)
				{
					cout << "You:";
					player1.show_all();
					player1.show_sum();
					cout<<"BUST";
					con3=false;
				}
			}
			while (dealer.show_sum_all() < 16)
			{
				
				cout<<"DEAULER:HIT"<<endl;
				dealer.insert_hand(cp.pick_cards());
			}
			cout<<"SHOW OUR HANDS!"<<endl;
			cout<<"YOU:"<<player1.show_sum_all()<<endl;
			player1.show_all();
			cout << "DEALER:" <<dealer.show_sum_all() << endl;
			dealer.show_all();
			if (dealer.show_mode() == player1.show_mode())
			{
				if (dealer.show_mode() == 0)
				{
					cout<<"EVEN!"<<endl;
					return 1; 
				}
				else
				{
					if (dealer.show_sum_all() > player1.show_sum_all())
					{
						cout<<"LOSE!"<<endl;
						return 0;
					}
					else if (dealer.show_sum_all() < player1.show_sum_all())
					{
						cout<<"WINNER!"<<endl;
						return 2;
					}
					else
					{
						cout << "EVEN!" << endl;
						return 1;
					}
				}
			}
			else
			{
				if (player1.show_mode() == 0||dealer.show_mode()==2)
				{
					cout << "LOSE!" << endl;
					return 0;
				}
				else 
				{
					cout << "WINNER!" << endl;
					return 2;
				}
				
			}
		}
	}
};


int main()
{
	GameMaster gm(30);
	//gm.all_manager_COOL104();
	while (gm.all_manager_BLACKJACK()&&gm.all_manager_COOL104())
	{

	}
	cout<<"THANK YOU FOR YOUR PLAYING! "<<endl;
    return 0;
}

