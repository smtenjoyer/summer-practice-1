#include <SFML/Graphics.hpp> 
#include <SFML/Audio.hpp> 
#include <SFML/Window.hpp> 
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <memory>

// #include "durak.h"
using namespace sf;
// Константы для размеров карт
const int CARD_WIDTH = 100;
const int CARD_HEIGHT = 150;
// Константы для размеров окна 
const int WINDOW_WIDTH = 1920; 
const int WINDOW_HEIGHT = 1080; 

const int NUM_PLAYERS = 2; // Количество игроков 
const int CARDS_PER_PLAYER = 6; // Количество карт на игрока 

struct Card { 
    int value; 
    std::string suit; 
    sf::Texture texture; 
    sf::Sprite sprite;
    Card(int value, const std::string& suit, const sf::Texture& texture) : value(value), suit(suit), texture(texture), sprite(texture, sf::IntRect(0, 0, texture.getSize().x, texture.getSize().y)) 
    {
        // Масштабирование спрайта
        sprite.setScale(static_cast<float>(CARD_WIDTH) / texture.getSize().x, static_cast<float>(CARD_HEIGHT) / texture.getSize().y); 
    }
    // Метод для получения копии спрайта для отрисовки
    sf::Sprite get_drawing_sprite(const sf::Vector2f& position) const {
        sf::Sprite drawingSprite = sprite;
        drawingSprite.setPosition(position);
        return drawingSprite; 
    }
};
 
// Функция для создания колоды карт 
std::vector<Card> create_deck(const sf::Texture& card_texture) { 
    std::vector<Card> deck;
    std::vector<std::string> suits {"HEARTS", "DIAMONDS", "CLUBS", "SPADES"};
    for (int value = 6; value <= 14; ++value)
    { 
        for (const std::string& suit : suits) {
            std::string filename = "resources" + std::to_string(value) + suit + ".png";
            // Используем unique_ptr для управления текстурой
            // auto texture_ptr = std::make_unique<sf::Texture>();
            // if (!texture_ptr->loadFromFile(filename)) {
            //     std::cerr << "Ошибка загрузки текстуры: " << filename << std::endl;
            // }
            // else {
            //     deck.push_back(Card(value, suit, *texture_ptr)); // Передаем ссылку на текстуру
            // }
            // // sf::Texture* texture_ptr = new sf::Texture();
            // // if (!texture_ptr->loadFromFile(filename)) {
            // //     std::cerr << "Error loading texture: " << filename << std::endl;
            // // }
            // // else {
            // //     deck.push_back(Card(value, suit, *texture_ptr)); // Передаем ссылку на текстуру
            // // }
            auto texture_ptr = std::make_unique<sf::Texture>(); 
            try { // Add try-catch block to handle potential memory allocation errors
                if (!texture_ptr->loadFromFile(filename)) { 
                    std::cerr << "Ошибка загрузки текстуры: " << filename << std::endl; 
                } 
                else {
                    deck.push_back(Card(value, suit, *texture_ptr)); // Передаем ссылку на текстуру
                }
            } catch (const std::bad_alloc& e) {
                std::cerr << "Memory allocation error: " << e.what() << std::endl;
                return deck; // Return the partially created deck
            }
        }
    }
    // Добавление карт рубашкой вверх
    for (int i = 0; i < 36; ++i) {
        deck.push_back(Card(0, "back", card_texture));
    }
    return deck; 
}
// Функция для перемешивания колоды карт 
void shuffle_deck(std::vector<Card>& deck) { 
    std::random_device rd; 
    std::mt19937 g(rd()); 
    std::shuffle(deck.begin(), deck.end(), g); 
} 
 
// Функция для получения карты из колоды 
Card draw_card(std::vector<Card>& deck) { 
    Card card = deck.back(); 
    deck.pop_back(); 
    return card; 
} 

// Класс для представления игрока
class Player {
public:
    std::string name;
    std::vector<Card> hand;
    Player() : name("") {}
    Player(const std::string& player_name) : name(player_name) {}
    // Добавление карты в руку
    void add_card(const Card& card) {
        hand.push_back(card);
    }
    // Удаление карты из руки
    void remove_card(int index) {
        if (index >= 0 && index < hand.size()) {
            hand.erase(hand.begin() + index);
        }
    }
};

// Функция для инициализации игры
void initialize_game(std::vector<Player>& players, std::vector<Card>& deck, sf::Texture& card_texture) {
    // Создание колоды карт
    deck = create_deck(card_texture);
    // Перемешивание колоды карт
    shuffle_deck(deck);
    // Раздача карт игрокам
    for (int i = 0; i < NUM_PLAYERS; ++i) {
        for (int j = 0; j < CARDS_PER_PLAYER; ++j) {
            players[i].add_card(draw_card(deck));
        }
    }
}

// Функция для проверки, может ли карта быть атакована
bool can_attack(const Card& attacking_card, const std::vector<Card>& defending_cards) {
    // Проверяем, есть ли в защите карты того же значения
    for (const auto& card : defending_cards) {
        if (card.value == attacking_card.value) {
            return true;
        }
    }
    return false;
}

// Функция для проверки, может ли карта быть защищена
bool can_defend(const Card& attacking_card, const std::vector<Card>& defending_cards) {
    // Проверяем, есть ли в защите карта того же значения
    for (const auto& card : defending_cards) {
        if (card.value == attacking_card.value) {
            return true;
        }
    }
    return false;
}

// Функция для атаки
void attack(std::vector<Card>& attacking_cards, std::vector<Card>& defending_cards, Player& attacking_player, Player& defending_player) {
    // Выбор карты для атаки
    int card_index;
    std::cout << attacking_player.name << ", выберите карту для атаки (введите номер карты): ";
    for (int i = 0; i < attacking_player.hand.size(); ++i) {
        std::cout << i << ": " << attacking_player.hand[i].value << " " << attacking_player.hand[i].suit << std::endl;
    }
    std::cin >> card_index;
    // Проверка корректности ввода
    if (card_index < 0 || card_index >= attacking_player.hand.size()) {
        std::cerr << "Некорректный номер карты!" << std::endl;
        return;
    }
    // Проверка возможности атаки
    if (!can_attack(attacking_player.hand[card_index], defending_cards)) {
        std::cerr << "Невозможно атаковать этой картой!" << std::endl;
        return;
    }
    // Добавление карты в атаку
    attacking_cards.push_back(attacking_player.hand[card_index]);
    // Удаление карты из руки
    attacking_player.remove_card(card_index);
}

// Функция для защиты
void defend(std::vector<Card>& attacking_cards, std::vector<Card>& defending_cards, Player& defending_player) {
    // Выбор карты для защиты
    int card_index;
    std::cout << defending_player.name << ", выберите карту для защиты (введите номер карты): ";
    for (int i = 0; i < defending_player.hand.size(); ++i) {
        std::cout << i << ": " << defending_player.hand[i].value << " " << defending_player.hand[i].suit << std::endl;
    }
    std::cin >> card_index;
    // Проверка корректности ввода
    if (card_index < 0 || card_index >= defending_player.hand.size()) {
        std::cerr << "Некорректный номер карты!" << std::endl;
        return;
    }
    // Проверка возможности защиты
    if (!can_defend(attacking_cards.back(), defending_player.hand)) {
        std::cerr << "Невозможно защититься от этой карты!" << std::endl;
        return;
    }
    // Добавление карты в защиту
    defending_cards.push_back(defending_player.hand[card_index]);
    // Удаление карты из руки
    defending_player.remove_card(card_index);
    // Проверка, есть ли еще карты в атаке
    if (attacking_cards.size() > 1) {
        defend(attacking_cards, defending_cards, defending_player);
    }
}

// Функция для завершения хода
void end_turn(std::vector<Card>& attacking_cards, std::vector<Card>& defending_cards, Player& attacking_player, Player& defending_player) {
    // Передача карт в руку защитившегося игрока
    for (auto& card : attacking_cards) {
        defending_player.add_card(card);
    }
    for (auto& card : defending_cards) {
        defending_player.add_card(card);
    }
    attacking_cards.clear();
    defending_cards.clear();
    // Проверка, есть ли у игроков карты
    if (attacking_player.hand.empty()) {
        std::cout << attacking_player.name << " - победил!" << std::endl;
        return;
    }
    if (defending_player.hand.empty()) {
        std::cout << defending_player.name << " - победил!" << std::endl;
        return;
    }
}

void display_cards(sf::RenderWindow& window, const std::vector<Card>& cards, float xOffset, float yOffset) {
    for (size_t i = 0; i < cards.size(); ++i) {
        // Получаем копию спрайта с заданной позицией
        sf::Sprite drawingSprite = cards[i].get_drawing_sprite(sf::Vector2f(xOffset + i * (CARD_WIDTH + 10), yOffset));
        
        // Отрисовываем копию спрайта 
        window.draw(drawingSprite);
    }
}

int main_durak(sf::RenderWindow& window) {
    // Создание окна
    // sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Дурак");
    // Загрузка текстуры рубашки карты
    sf::Texture card_texture;
    if (!card_texture.loadFromFile("resources/back.png")) {
        std::cerr << "Ошибка загрузки текстуры карты" << std::endl;
        return 1;
    }
    // Загрузка текстуры стола
    sf::Texture tableTexture;
    if (!tableTexture.loadFromFile("resources/table.png")) {
        std::cerr << "Ошибка загрузки текстуры стола." << std::endl;
        return 1;
    }
    sf::Sprite tableSprite(tableTexture);
    tableSprite.setScale(static_cast<float>(WINDOW_WIDTH) / tableTexture.getSize().x, static_cast<float>(WINDOW_HEIGHT) / tableTexture.getSize().y);
    // Создание игроков
    std::vector<Player> players(NUM_PLAYERS);
    players[0].name = "Игрок 1";
    players[1].name = "Игрок 2";
    // Создание колоды карт
    std::vector<Card> deck;
    // Шрифт для текста
    sf::Font font;
    if (!font.loadFromFile("resorces/troika.otf")) { 
        std::cerr << "Ошибка загрузки шрифта!" << std::endl;
        return 1;
    }
    // Текст для отображения имени игрока
    sf::Text player_name_text;
    player_name_text.setFont(font);
    player_name_text.setCharacterSize(24);
    player_name_text.setFillColor(sf::Color::Black);
    // Инициализация игры
    initialize_game(players, deck, card_texture);
// Основной цикл игры
    int current_player = 0; // Текущий игрок
    std::vector<Card> attacking_cards; // Карты в атаке
    std::vector<Card> defending_cards; // Карты в защите
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::MouseButtonPressed) {
                // Обработка клика мыши
                if (event.mouseButton.button == sf::Mouse::Left) {
                    // Получить координаты клика
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    // Проверка, какая карта была кликнута
                    // 1. Проверка клика по руке текущего игрока
                    for (size_t i = 0; i < players[current_player].hand.size(); ++i) {
                        sf::Sprite cardSprite = players[current_player].hand[i].get_drawing_sprite(
                            sf::Vector2f(10 + i * (CARD_WIDTH + 10), 400) // Позиция для игрока 1
                        );
                        if (cardSprite.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                            // Выбрана карта игрока
                            if (attacking_cards.empty()) { // Атака
                                attack(attacking_cards, defending_cards, players[current_player], players[(current_player + 1) % NUM_PLAYERS]);
                            } else { // Защита
                                defend(attacking_cards, defending_cards, players[(current_player + 1) % NUM_PLAYERS]);
                            }
                            break;
                        }
                    }
                    // 2. Проверка клика по картам в атаке
                    for (size_t i = 0; i < attacking_cards.size(); ++i) {
                        sf::Sprite cardSprite = attacking_cards[i].get_drawing_sprite(
                            sf::Vector2f(100 + i * (CARD_WIDTH + 10), 250) // Позиция для карт в атаке
                        );
                        if (cardSprite.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                            // Выбрана карта в атаке
                            // Завершение хода
                            end_turn(attacking_cards, defending_cards, players[current_player], players[(current_player + 1) % NUM_PLAYERS]);
                            current_player = (current_player + 1) % NUM_PLAYERS; // Переход хода
                            break;
                        }
                    }
                    // 3. Проверка клика по картам в защите
                    for (size_t i = 0; i < defending_cards.size(); ++i) {
                        sf::Sprite cardSprite = defending_cards[i].get_drawing_sprite(
                            sf::Vector2f(100 + i * (CARD_WIDTH + 10), 250) // Позиция для карт в атаке
                        );
                        if (cardSprite.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                            // Завершение хода
                            end_turn(attacking_cards, defending_cards, players[current_player], players[(current_player + 1) % NUM_PLAYERS]);
                            current_player = (current_player + 1) % NUM_PLAYERS; // Переход хода
                            break;
                        }
                    }
                    // Проверка, можно ли переходить к следующему ходу
                    if (attacking_cards.empty() || defending_cards.size() == attacking_cards.size()) {
                        // Переход хода
                        current_player = (current_player + 1) % NUM_PLAYERS; 
                        // Обновление состояния игры 
                        attacking_cards.clear();
                        defending_cards.clear(); 
                        // Проверка, есть ли у игроков карты
                        if (players[current_player].hand.empty()) {
                            std::cout << players[current_player].name << " - победил!" << std::endl;
                            window.close(); // Завершение игры
                            return 0;
                        }
                        if (players[(current_player + 1) % NUM_PLAYERS].hand.empty()) {
                            std::cout << players[(current_player + 1) % NUM_PLAYERS].name << " - победил!" << std::endl;
                            window.close(); // Завершение игры
                            return 0;
                        }
                        // Вывод сообщения о новом ходе
                        std::cout << "Ход " << players[current_player].name << std::endl; 
                    }
                }
            }
        }
        // Очистка окна
        window.clear(sf::Color::White);
        // Отображение карт игроков
        display_cards(window, players[0].hand, 10, 400); // Игрок 1
        display_cards(window, players[1].hand, 10, 100); // Игрок 2
        // Отображение карт в атаке
        display_cards(window, attacking_cards, 100, 250); // Карты в атаке
        // Отображение карт в защите
        display_cards(window, defending_cards, 300, 250); // Карты в защите
        // Подсветка имени текущего игрока
        player_name_text.setString(players[current_player].name);
        player_name_text.setPosition(10, 10); // Позиция текста
        // Изменение цвета текста для текущего игрока
        if (current_player == 0) {
            player_name_text.setFillColor(sf::Color::Green); // Зеленый цвет для игрока 1
        } else {
            player_name_text.setFillColor(sf::Color::Red); // Красный цвет для игрока 2
        }
        window.draw(player_name_text);
        // Обновление экрана
        window.display();
        // Завершение хода
        if (!attacking_cards.empty() && defending_cards.size() == attacking_cards.size()) {
            end_turn(attacking_cards, defending_cards, players[current_player], players[(current_player + 1) % NUM_PLAYERS]);
            current_player = (current_player + 1) % NUM_PLAYERS; // Переход хода
        }
    }
    return 0;
}