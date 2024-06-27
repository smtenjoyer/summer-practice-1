#include <C:/Users/Sopha/Desktop/proj/resources/SFML-2.6.1/include/SFML/Graphics.hpp>
#include <C:/Users/Sopha/Desktop/proj/resources/SFML-2.6.1/include/SFML/Window.hpp>
#include <C:/Users/Sopha/Desktop/proj/resources/SFML-2.6.1/include/SFML/System.hpp>
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <memory>

using namespace sf;
struct Card { 
    int value; 
    std::string suit; 
    sf::Texture texture; 
    sf::Sprite sprite;
    Card(int value, const std::string& suit, const sf::Texture& texture) : value(value), suit(suit), texture(texture), sprite(texture, sf::IntRect(0, 0, texture.getSize().x, texture.getSize().y)){};    // Метод для получения копии спрайта для отрисовки
    sf::Sprite get_drawing_sprite(const sf::Vector2f& position);
};
 
// Функция для создания колоды карт 
std::vector<Card> create_deck(const sf::Texture& card_texture);
// Функция для перемешивания колоды карт 
void shuffle_deck(std::vector<Card>& deck);
 
// Функция для получения карты из колоды 
Card draw_card(std::vector<Card>& deck);

// Класс для представления игрока
class Player {
public:
    std::string name;
    std::vector<Card> hand;
    Player() : name("") {}
    Player(const std::string& player_name) : name(player_name){};

    // Добавление карты в руку
    void add_card(const Card& card);

    // Удаление карты из руки
    void remove_card(int index);
};

// Функция для инициализации игры
void initialize_game(std::vector<Player>& players, std::vector<Card>& deck, sf::Texture& card_texture);

// Функция для проверки, может ли карта быть атакована
bool can_attack(const Card& attacking_card, const std::vector<Card>& defending_cards);

// Функция для проверки, может ли карта быть защищена
bool can_defend(const Card& attacking_card, const std::vector<Card>& defending_cards);

// Функция для атаки
void attack(std::vector<Card>& attacking_cards, std::vector<Card>& defending_cards, Player& attacking_player, Player& defending_player);

// Функция для защиты
void defend(std::vector<Card>& attacking_cards, std::vector<Card>& defending_cards, Player& defending_player);

// Функция для завершения хода
void end_turn(std::vector<Card>& attacking_cards, std::vector<Card>& defending_cards, Player& attacking_player, Player& defending_player);

void display_cards(sf::RenderWindow& window, const std::vector<Card>& cards, float xOffset, float yOffset);

int main_durak(sf::RenderWindow& window);