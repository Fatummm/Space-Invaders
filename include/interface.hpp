#ifndef INTERFACE_HPP
#define INTERFACE_HPP
#include <list>
#include <memory>
#include <deque>

class Page {
protected:
    bool switched;
    bool checkButton(const Button&, const sf::Vector2i&);
};

class GamePage: public Page {
private:
    Player player;
    Difficulty difficulty;
    std::list<std::shared_ptr<Bullet>> bullets;
    std::list<std::shared_ptr<Wall>> walls;
    std::list<std::list<std::shared_ptr<Bullet>>::iterator> bullets_to_delete;
    std::list<std::list<std::shared_ptr<Wall>>::iterator> walls_to_delete;
    bool is_over = false;
    bool auto_shoot = true;
    Score* score;
    Army army;

    SFMLObject lives;
    std::vector<std::shared_ptr<SFMLObject>> hearts;

    void initPlayer(float scale = 2.0f, int lives = 3);
    void initWalls();

    void checkWalls(sf::RenderWindow& w);
    void checkBullets(sf::RenderWindow& w);
    void checkEnemies(sf::RenderWindow& w);
    void checkPlayer();
public:
    void deleteSprites();
    void evaluateFinalScore();
    void init(Difficulty dif, Score* sc);
    void processKey(sf::Keyboard::Key k);
    Pages render(sf::RenderWindow& w);
    void checkCollision(sf::RenderWindow& w);
    void clear();
};

class MainMenuPage: public Page {
private:
    SFMLObject title;
    Button play_button;
    Button controls_button;
    Button quit_button;
public:
    void init();
    Pages render(sf::RenderWindow& w, const sf::Vector2i& position);
};

class DifficultyPage: public Page {
private:
    Button easy;
    Button normal;
    Button hard;
    Button extreme;
    SFMLObject difficulty_window;
public:
    void init();
    Pages render(sf::RenderWindow& w, const sf::Vector2i& pos, Difficulty& df);
};

class ControlsPage: public Page {
private:
    Button exit_button;
    SFMLObject controls_window;
public:
    void init();
    Pages render(sf::RenderWindow& w, const sf::Vector2i& pos);
};

class VictoryPage: Page {
private:
    Button retry;
    Button main_menu;
    SFMLObject victory_window;
    Score* score;
public:
    void init(Score* sc);
    Pages render(sf::RenderWindow& w, const sf::Vector2i& pos);
};

class LosePage: public Page {
private:
    Button retry;
    Button main_menu;
    SFMLObject lose_window;
    Score* score;
public:
    void init(Score* sc);
    Pages render(sf::RenderWindow& w, const sf::Vector2i& pos);
};

#endif