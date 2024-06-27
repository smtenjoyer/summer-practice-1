#include <stdio.h>
#include <C:/Program Files (x86)/SFML-2.6.1/include/SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>
#include <set>

using namespace sf;

class Card {
public:
    int rank; // 6-14 (6 to Ace)
    std::string suit; //  (Spades, Clubs, Diamonds, Hearts)
    sf::Texture texture; 
    sf::Sprite sprite; 
    bool isTaking = false; //для реализации взятия карты
};

class Player {
public:
    std::vector<Card> hand;
    bool isAttacker;
    bool isDefender;
};


void share_cards(std::vector<Player> players, int num_of_player, std::vector<int> selected_cards)
{
    if(num_of_player == 0)
    {
        players[num_of_player-1].hand.push_back(players[num_of_player].hand[selected_cards[0]]);
        players[num_of_player-1].hand.push_back(players[num_of_player].hand[selected_cards[1]]);
        players[num_of_player-1].hand.push_back(players[num_of_player].hand[selected_cards[3]]);
    }
    else
    {
        players[3].hand.push_back(players[0].hand[selected_cards[0]]);
        players[3].hand.push_back(players[0].hand[selected_cards[1]]);
        players[3].hand.push_back(players[0].hand[selected_cards[3]]);
    }
}


int main() {

    srand(time(0));
    
     RenderWindow window(VideoMode(1920, 1080), L"Червы");
  //RenderWindow window(VideoMode::getDesktopMode(),  L"Червы", Style::Fullscreen);

    Texture tableTexture;
    if (!tableTexture.loadFromFile("C:/Users/user/Desktop/hearts/cards/table.png")) { return 1; }
    RectangleShape background(Vector2f(1920,1080));
    background.setTexture(&tableTexture);
    background.setPosition(Vector2f(0,0));


    std::vector<std::string> suits = {"pik", "cherv", "bub", "cres"}; 
        //основная колода 
    std::vector<Card> deck;
    for (int rank = 2; rank <= 14; ++rank) {
        for (const std::string& suit : suits) {
            Card card;
            card.rank = rank;
            card.suit = suit;
            std::string filename = "C:/Users/user/Desktop/hearts/cards/" + std::to_string(rank) + suit + ".png"; 
            if (rank == 11) filename = "C:/Users/user/Desktop/hearts/cards/J" + suit + ".png"; 
            if (rank == 12) filename = "C:/Users/user/Desktop/hearts/cards/Q" + suit + ".png"; 
            if (rank == 13) filename = "C:/Users/user/Desktop/hearts/cards/K" + suit + ".png"; 
            if (rank == 14) filename = "C:/Users/user/Desktop/hearts/cards/A" + suit + ".png"; 

            Texture CardTexture;
            if (!CardTexture.loadFromFile(filename)) { 
                std::cerr << "Ошибка загрузки текстуры: " << filename << std::endl; 
            } else {
                card.texture = CardTexture; 
                // RectangleShape CardShape(Vector2f(77,108));
                // CardShape.setTexture(&CardTexture);
            }
            deck.push_back(card);
        }
    }

    std::random_shuffle(deck.begin(), deck.end());

    std::vector<Player> players(4);
    players[0].isAttacker = true;
    players[0].isDefender = false;
    players[1].isAttacker = false;
    players[1].isDefender = true;
    players[2].isAttacker = false;
    players[1].isDefender = true;
    players[3].isAttacker = false;
    players[1].isDefender = true;

    //раздача карт на руки
    for (int i = 0; i < 13; ++i) {
        players[0].hand.push_back(deck.back());
        deck.pop_back();
        players[1].hand.push_back(deck.back());
        deck.pop_back();
        players[2].hand.push_back(deck.back());
        deck.pop_back();
        players[3].hand.push_back(deck.back());
        deck.pop_back();
    }

int selectedCardIndex = 0;
std::vector<int> selected_cards(0);
 //   int trumpSuit = deck.back().suit; // Suit of the last card in deck is the trump suit

// Game Loop
while (window.isOpen()) {
    Event event;
    while (window.pollEvent(event)) 
    {
    // Game logic goes here
    switch(event.type)
    {
    // закрытие окна
        case sf::Event::Closed:
            window.close();
            break;
        case Event::KeyPressed:
            if (event.key.code == sf::Keyboard::Left) 
            {
                // Выбираем предыдущую карту (если возможно)
                if (selectedCardIndex > 0) 
                {
                    --selectedCardIndex;
                }
            }
                
            else if (event.key.code == sf::Keyboard::Right) 
            {
                // Выбираем следующую карту (если возможно)
                if (selectedCardIndex < players[0].hand.size() - 1) 
                {
                    ++selectedCardIndex;
                }
            } 
            else if (event.key.code == Keyboard::Enter) {
                        // Добавляем выбранную карту в вектор selected_cards
                        selected_cards.push_back(selectedCardIndex);
                    }
            break;
        case Event::KeyReleased:
            if (event.key.code == sf::Keyboard::Left) {}
            else if (event.key.code == sf::Keyboard::Right){}
            else if (event.key.code == sf::Keyboard::Enter){}
        default:
        break;
    }

    window.clear(Color(50,200,150));
     window.draw(background);
    // int id_P1 = 1;
    // for (Card& card : players[0].hand) {
        
    //     card.sprite.setTexture(card.texture);
    //     card.sprite.setPosition(Vector2f(200+(1300/players[0].hand.size())*id_P1 ,800));
    //     window.draw(card.sprite);
    //     id_P1++;
    // }
    
    
        // Отрисовка карты игрока
        int id_P1 = 1; 
        for (size_t i = 0; i < players[0].hand.size(); ++i)
        { 
            Card& card = players[0].hand[i]; card.sprite.setTexture(card.texture);
            // Определение позиции карты в зависимости от её индекса 
            card.sprite.setPosition(Vector2f(200 + (1300 / players[0].hand.size()) * id_P1, 800)); 
            // Если карта выбрана, она рисуется в желтый цвет 
            if (i == selectedCardIndex) 
            { 
                if(i!=0){card.sprite.setColor(Color::Magenta); }
                else if((i == 0) && (std::find(selected_cards.begin(), selected_cards.end(), i) == selected_cards.end()))
                {
                    card.sprite.setColor(Color::Magenta); 
                }
            } 
            else if (std::find(selected_cards.begin(), selected_cards.end(), i) == selected_cards.end()) 
            { 
            card.sprite.setColor(Color::White);
             // сбрасываем цвет для не выбранных карт
            } 
            window.draw(card.sprite);
            ++id_P1; 
        }
    int id_P2 = 1;
    Texture back_card_texture;
    if (!back_card_texture.loadFromFile("C:/Users/user/Desktop/hearts/cards/back.png")) { return 1; }
    RectangleShape back_card_sprite(Vector2f(77,110));
    back_card_sprite.setTexture(&back_card_texture);

    for (Card& card : players[1].hand) {
        back_card_sprite.setPosition(Vector2f(200+(1300/players[0].hand.size())*id_P2 ,150));
        window.draw(back_card_sprite);
        id_P2++;
    }
    // Drawing code for graphics can be added here
    window.display();
    }
}
return 0;
}