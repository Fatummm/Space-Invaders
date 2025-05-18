#ifndef BASE_HPP
#define BASE_HPP
#include <SFML/Graphics.hpp>
#include <list>
#include <memory>

#define FPS 144

#define WIDTH 1600.0f
#define HEIGHT 1000.0f

#define WALL_STAGES 5
#define WALL_NUMBER 4

enum class Difficulty {
    Easy, Normal, Hard, Extreme
};

enum class Entity_type {
    Player, Enemy_tier1, Enemy_tier2, Enemy_tier3
};

enum class Pages {
    Main_menu, Game, Controls, Victory, Lose, Difficulty
};

class SFMLObject {
protected:
    sf::Sprite sprite;
    sf::Texture texture;
    std::string path;
public:
    void move(const sf::Vector2f& mv, float scale = 1.0f);

    float getSpriteWidth() const;
    float getSpriteHeight() const;
    sf::Vector2f getSpriteCenter() const;
    sf::FloatRect getGlobalBounds() const;
    void createSprite();
    sf::Sprite& getSprite();
    sf::Texture& getTexture();

    void setPosition(const sf::Vector2f& pos);
};

class Score {
private:
    std::list<std::shared_ptr<SFMLObject>> number;
    int current_score = 0;
    SFMLObject title;
    sf::Vector2f position;
    int distance = 50;
    void normalize();
public:
    void displayWithTitle(sf::RenderWindow& w);
    void displayWithoutTitle(sf::RenderWindow& w);
    void setPosition(const sf::Vector2f& pos);
    void multiply(int n);
    void increase(int num);
    void init(const sf::Vector2f& position);
};

#endif

