#include <iostream>
#include <SFML/Graphics.hpp>
#include <chrono>
#include <list>
#include <memory>
#include <vector>
#include <map>

#include <stdio.h>
#include "base.hpp"
#include "entities.hpp"
#include "interface.hpp"

class Game {
private:
    GamePage game_page;
    MainMenuPage main_page;
    ControlsPage controls_page;
    DifficultyPage diff_page;
    VictoryPage victory_page;
    LosePage lose_page;

    sf::RenderWindow window;
    sf::Texture bg_txt;
    sf::Sprite background;
    Pages page = Pages::Main_menu;

    Difficulty current_dif;
    Score cur_score;
public:
    
    Game(int fps=144) {
        bg_txt.loadFromFile("src/images/background.png");
        background.setTexture(bg_txt);
        window.create(sf::VideoMode(WIDTH, HEIGHT), "Space Invaders", sf::Style::None);
        window.setFramerateLimit(fps);
        main_page.init();
    }

    void switchToGame(Difficulty df) {
        game_page.init(df, &cur_score);
    }
    void switchToMain() {
        main_page.init();
    }
    void switchToControls() {
        controls_page.init();
    }

    void switchToDifficulty() {
        diff_page.init();
    }
    
    void switchToVictory() {
        victory_page.init(&cur_score);
    }

    void switchToLose() {
        lose_page.init(&cur_score);
    }

    sf::Vector2i getCursorPosition() {
        int x = sf::Mouse::getPosition().x - window.getPosition().x;
        int y = sf::Mouse::getPosition().y - window.getPosition().y;
        //std::cout << x << ' ' << y << '\n';
        return sf::Vector2i(x, y);
    }
    
    void start() {
        while (window.isOpen()) {
            window.display();
            window.clear();
            window.draw(background);
            Pages last = page;
            switch (page) {
                case Pages::Game:
                    page = game_page.render(window); break;
                case Pages::Main_menu:
                    page = main_page.render(window, getCursorPosition()); break;
                case Pages::Controls:
                    page = controls_page.render(window, getCursorPosition()); break;
                case Pages::Difficulty:
                    page = diff_page.render(window, getCursorPosition(), current_dif); break;
                case Pages::Victory:
                    page = victory_page.render(window, getCursorPosition()); break;
                case Pages::Lose:
                    page = lose_page.render(window, getCursorPosition()); break;
                default:
                    break;
            }
            if (last != page) {
                switch (page) {
                    case Pages::Game:
                        switchToGame(current_dif); break;
                    case Pages::Main_menu:
                        switchToMain(); break;
                    case Pages::Controls:
                        switchToControls(); break;
                    case Pages::Difficulty:
                        switchToDifficulty(); break;
                    case Pages::Victory:
                        switchToVictory(); break;
                    case Pages::Lose:
                        switchToLose(); break;
                    default:
                        break;
                }
            }
        }
    }
};

int main() {
    Game g;
    g.start();
}


