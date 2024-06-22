#include <C:/Users/Sopha/Downloads/SFML-2.6.1-windows-gcc-13.1.0-mingw-64-bit/SFML-2.6.1/include/SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <memory>

using namespace sf;
// Константы для размеров карты и окна 
const int CARD_WIDTH = 71; 
const int CARD_HEIGHT = 96; 
const int WINDOW_WIDTH = 800; 
const int WINDOW_HEIGHT = 600; 
const int NUM_PLAYERS = 2; // Количество игроков 
const int CARDS_PER_PLAYER = 36/NUM_PLAYERS; // Количество карт на игрока 
// Структура для представления карты 
struct Card { 
    int value; 
    std::string suit; 
    sf::Texture texture; 
    sf::Sprite sprite; 
 
    // Конструктор для инициализации карты 
    Card(int value, const std::string& suit, const sf::Texture& texture) : value(value), suit(suit), texture(texture) { 
        sprite.setTexture(texture); 
        sprite.setScale(static_cast<float>(CARD_WIDTH) / texture.getSize().x, 
                        static_cast<float>(CARD_HEIGHT) / texture.getSize().y); 
    } 

}; 
 
// Функция для создания колоды карт 

std::vector<Card> createDeck(const sf::Texture& cardTexture) { 
    std::vector<Card> deck; 
    std::vector<std::string> suits = {"pik", "cherv", "bub", "cres"}; 
 
    for (int value = 6; value <= 14; ++value) //от 2 но загрузить еще карты
    { 
        for (const std::string& suit : suits) { 
            // Использование 'J', 'Q', 'K', 'A' для значений 11, 12, 13, 14 
            std::string filename = "C:/Users/Sopha/Desktop/proj/cards/" + std::to_string(value) + suit + ".png"; 
            if (value == 11) filename = "C:/Users/Sopha/Desktop/proj/cards/J" + suit + ".png"; 
            if (value == 12) filename = "C:/Users/Sopha/Desktop/proj/cards/Q" + suit + ".png"; 
            if (value == 13) filename = "C:/Users/Sopha/Desktop/proj/cards/K" + suit + ".png"; 
            if (value == 14) filename = "C:/Users/Sopha/Desktop/proj/cards/A" + suit + ".png"; 

            // Используем unique_ptr для управления текстурой
            auto texturePtr = std::make_unique<sf::Texture>(); 
            if (!texturePtr->loadFromFile(filename)) { 
                std::cerr << "Ошибка загрузки текстуры: " << filename << std::endl; 
            } else {
                deck.push_back(Card(value, suit, *texturePtr)); // Передаем ссылку на текстуру
            }
        }
    }
    // Добавление карт рубашкой вверх
    for (int i = 0; i < 36; ++i) {
        deck.push_back(Card(0, "back", cardTexture));
    }
    return deck; 
} 
// Функция для перемешивания колоды карт 
void shuffleDeck(std::vector<Card>& deck) { 
    std::random_device rd; 
    std::mt19937 g(rd()); 
    std::shuffle(deck.begin(), deck.end(), g); 
} 
 
// Функция для получения карты из колоды 
Card drawCard(std::vector<Card>& deck) { 
    Card card = deck.back(); 
    deck.pop_back(); 
    return card; 
} 
// Функция для проверки наличия пары в руке 
bool hasPair(const std::vector<Card>& hand) {
    for (size_t i = 0; i < hand.size(); ++i) {
        for (size_t j = i + 1; j < hand.size(); ++j) {
            if (hand[i].value == hand[j].value && (hand[i].value != 12 && hand[i].suit !="pik")) {
                return true; 
            }
        }
    }
    return false;
}

// Функция для поиска пары в руке
std::pair<size_t, size_t> findPair(const std::vector<Card>& hand) {
    for (size_t i = 0; i < hand.size(); ++i) {
        for (size_t j = i + 1; j < hand.size(); ++j) {
            if (hand[i].value == hand[j].value && (hand[i].value != 12 && hand[i].suit !="pik")) {
                return std::make_pair(i, j); 
            }
        }
    }
    return std::make_pair(-1, -1); 
}

// Функция для проверки выигрыша 
bool checkWin(const std::vector<Card>& hand) {  
    // Проверка, есть ли у игрока карты в руке 
    return hand.empty();
}
// Функция для отрисовки карт 
void drawCards(RenderWindow& window, std::vector<Card>& hand, int playerIndex) { 
    for (size_t i = 0; i < hand.size(); ++i) { 
        // Определение позиции карты на экране 
        float x = i * CARD_WIDTH + 100 + playerIndex * (800 / 2);  
        float y = (playerIndex == 0) ? 600 - 96 - 50 : 50;  

        // Создайте копию sf::Sprite
        sf::Sprite spriteCopy = hand[i].sprite; 
        spriteCopy.setPosition(x, y);  
        window.draw(spriteCopy); 
    } 
}

int main() {
    setlocale(LC_ALL, "Russian");
    // Инициализация окна SFML
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Pik Dam");

    // Загрузка текстуры карт
    sf::Texture cardTexture;
    if (!cardTexture.loadFromFile("C:/Users/Sopha/Desktop/proj/cards/back.png")) {
        std::cerr << "Ошибка загрузки текстуры карты." << std::endl;
        return 1;
    }

    // Создание колоды карт
    std::vector<Card> deck = createDeck(cardTexture);
    shuffleDeck(deck);

    // Загрузка текстуры стола
    sf::Texture tableTexture;
    if (!tableTexture.loadFromFile("C:/Users/Sopha/Desktop/proj/table.png")) {
        std::cerr << "Ошибка загрузки текстуры стола." << std::endl;
        return 1;
    }
    sf::Sprite tableSprite(tableTexture);
    tableSprite.setScale(static_cast<float>(WINDOW_WIDTH) / tableTexture.getSize().x,
                        static_cast<float>(WINDOW_HEIGHT) / tableTexture.getSize().y);

    // Руки игрока и противника
    // std::vector<Card> playerHand;
    // std::vector<Card> opponentHand;

    // Руки игроков
    std::vector<std::vector<Card>> playerHands(NUM_PLAYERS);
    
    // Раздача карт 
    // for (int i = 0; i < 18; ++i) { // Раздаем по 18 карт если играют 2
    //     playerHand.push_back(drawCard(deck));
    //     opponentHand.push_back(drawCard(deck));
    // }
    // Раздача карт 
    for (int i = 0; i < NUM_PLAYERS; ++i) { 
        for (int j = 0; j < CARDS_PER_PLAYER; ++j) {
            playerHands[i].push_back(drawCard(deck));
        }
    }
    // Флаг хода (true - ход игрока, false - ход противника)
    bool playerTurn = true; 
    // Индекс текущего игрока (0 - первый игрок, 1 - второй)
    int currentPlayer = 0;

     // Игровой цикл 
    while (window.isOpen()) { 
        // Обработка событий 
        Event event; 
        while (window.pollEvent(event)) { 
            if (event.type == Event::Closed) { 
                window.close(); 
            }
            // Обработка нажатия мыши (только если ход игрока)
            if (event.type == Event::MouseButtonPressed && playerTurn) {
                // Получение позиции мыши
                Vector2i mousePos = Mouse::getPosition(window);

                // Проверка, попала ли мышь на карту противника
                for (size_t i = 0; i < playerHands[(currentPlayer + 1) % NUM_PLAYERS].size(); ++i) {
                    // Рассчитываем прямоугольник для карты
                    FloatRect cardRect = playerHands[(currentPlayer + 1) % NUM_PLAYERS][i].sprite.getGlobalBounds();
                    if (cardRect.contains(mousePos.x, mousePos.y)) {
                        // Взять карту противника
                        playerHands[currentPlayer].push_back(playerHands[(currentPlayer + 1) % NUM_PLAYERS][i]);
                        // Удалить карту из руки противника
                        playerHands[(currentPlayer + 1) % NUM_PLAYERS].erase(playerHands[(currentPlayer + 1) % NUM_PLAYERS].begin() + i);
                        // Проверка наличия пары в руке текущего игрока
                        if (hasPair(playerHands[currentPlayer])) {
                            // Найти пару
                            std::pair<size_t, size_t> pairIndices = findPair(playerHands[currentPlayer]);
                            // Удалить пару из руки
                            playerHands[currentPlayer].erase(playerHands[currentPlayer].begin() + pairIndices.second);
                            playerHands[currentPlayer].erase(playerHands[currentPlayer].begin() + pairIndices.first);
                        }
                        // Передать ход следующему игроку
                        playerTurn = false;
                        currentPlayer = (currentPlayer + 1) % NUM_PLAYERS;
                        break;
                    }
                }
            }
        } 

        // Очистка экрана
        window.clear();

        // Рисование стола
        window.draw(tableSprite);

        // Рисование карт
        // for (size_t i = 0; i < playerHand.size(); ++i) {
        //     playerHand[i].sprite.setPosition(i * CARD_WIDTH + 200, WINDOW_HEIGHT - CARD_HEIGHT - 50);
        //     window.draw(playerHand[i].sprite);
        // }
        // Отрисовка карт игроков
        for (int i = 0; i < NUM_PLAYERS; ++i) {
            drawCards(window, playerHands[i], i); 
        } 
        
        // Рисование карт противника (лицевой стороной вниз)
        // for (size_t i = 0; i < opponentHand.size(); ++i) {
        //     opponentHand[i].sprite.setPosition(i * CARD_WIDTH + 200, 50); 
        //     window.draw(opponentHand[i].sprite);
        // }

        // Проверка выигрыша
        // if (checkWin(playerHand)) {
        //     std::cout << "Вы выиграли!" << std::endl;
        //     //  добавить визуальное отображение победы
        //     break;
        // } else if (checkWin(opponentHand)) {
        //     std::cout << "Противник выиграл!" << std::endl;
        //     //  добавить визуальное отображение поражения
        //     break;
        // }
        // // Логика хода
        // if (playerTurn) {
        //     // Ход игрока
        //     if (hasPair(playerHand)) {
        //         // Найти пару в руке игрока
        //         std::pair<size_t, size_t> pairIndices = findPair(playerHand); 
        //         // Удалить пару из руки игрока
        //         playerHand.erase(playerHand.begin() + pairIndices.second);
        //         playerHand.erase(playerHand.begin() + pairIndices.first); 
        //         // Передать ход противнику
        //         playerTurn = false;
        //     } else if (!deck.empty()) {
        //         // Взять карту из колоды
        //         playerHand.push_back(drawCard(deck));
        //         // Передать ход противнику
        //         playerTurn = false;
        //     }
        // } else {
        //     // Ход противника (имитация)
        //     if (hasPair(opponentHand)) {
        //         // Найти пару в руке противника
        //         std::pair<size_t, size_t> pairIndices = findPair(opponentHand); 
        //         // Удалить пару из руки противника
        //         opponentHand.erase(opponentHand.begin() + pairIndices.second);
        //         opponentHand.erase(opponentHand.begin() + pairIndices.first); 
        //         // Передать ход игроку
        //         playerTurn = true;
        //     } else if (!deck.empty()) {
        //         // Взять карту из колоды
        //         opponentHand.push_back(drawCard(deck));
        //         // Передать ход игроку
        //         playerTurn = true;
        //     }
        // }
        // Проверка выигрыша
        for (int i = 0; i < NUM_PLAYERS; ++i) {
            if (checkWin(playerHands[i])) {
                std::cout << "Игрок " << i + 1 << " выиграл!" << std::endl; 
                // Можно добавить визуальное отображение победы
                break;
            }
        }
        // Обновление окна
        window.display();
    }

    return 0;
}

// #include <windows.h>
// #include <tchar.h>
// int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
// {
//   MessageBox(NULL, _T("Welcome!"),
//   _T("prog"), MB_OK);
//   return 0;
// }


