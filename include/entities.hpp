#ifndef ENTITIES_HPP
#define ENTITIES_HPP

#include "base.hpp"
#include <chrono>
#include <SFML/Graphics.hpp>
#include <memory>
#include <map>
#include <vector>
#include <memory>
#include <list>

class Entity: public SFMLObject {
protected:
    bool alive = true;
    Entity_type type;
public:
    bool isAlive();
    Entity_type getType();
    void getDamage();
};

class Wall: public Entity { 
private:
    int durability;
    int damage;
public:
    Wall(const sf::Vector2f& position, Difficulty df);
    void getDamage();
};

class Bullet: public SFMLObject {
protected:
    float speed;
    sf::Vector2f direction;
    Entity_type from;
public:
    Bullet(const std::string& path, float scale, const sf::Vector2f& dir, float sped, const sf::Vector2f& position, Entity_type from_who, float rotation = 0);
    void move();
    Entity_type getSender();
};

class Player: public Entity {
protected:
    int lives;
    bool is_damaging = false;
    std::chrono::high_resolution_clock::time_point last_shot;
    std::chrono::high_resolution_clock::time_point time_from_damaged;
    float current_scale = 2.0f;

    void checkDamage();
public:

    void getDamage();
    bool update();
    void init(Difficulty df);
    bool ready_to_shoot();
    int getLives();
    std::shared_ptr<Bullet> shot();
};

class Button: public SFMLObject {
private:
    std::map<std::string, sf::Texture> textures;
    int state = 0;

    void makeMain();
    void makeHovered();
    void makePressed();
    
public:
    bool checkCursor(const sf::Vector2i& cursor_pos) const;
    void init(const std::string& name, const sf::Vector2f& position, float scale = 2.0f);
    void updateButton(const sf::Vector2i& position);
    bool checkButton() const;
};

class Enemy: public Entity {
private:
    std::chrono::high_resolution_clock::time_point time_to_change_animation;
    std::vector<sf::Texture> animations;
    int current_animation = 0;
    int probabilty_to_shot;
    float current_scale = 2.0f;
public:
    void init(Difficulty df, Entity_type tp, const sf::Vector2f& pos);
    bool readyToShoot();
    void updateAnimation();
    void getDamage();
    bool update();
    std::shared_ptr<Bullet> shot(const sf::Vector2f& where);
    sf::Vector2f getBulletDirection(const sf::Vector2f& where);
};

class Army {
private:
    std::list<std::shared_ptr<Enemy>> enemies;
    std::list<std::list<std::shared_ptr<Enemy>>::iterator> enemies_to_delete;
    float left_border = 500;
    float right_border = WIDTH - 400;
    float position = WIDTH/2;
    sf::Vector2f direction;
    int tick = 0;
    int probability;

    void moveDown();
    int getAlive();

public:
    void init(Difficulty df);
    void move();
    void checkCollision(std::list<std::shared_ptr<Bullet>>& bullets, std::list<std::list<std::shared_ptr<Bullet>>::iterator>& bullets_to_delete, Score& sc);
    void clear();
    void draw(sf::RenderWindow& w);
    void shoot(std::list<std::shared_ptr<Bullet>>& bullets, const sf::Vector2f& where);
    bool isDefeated();
};

#endif
