#include "entities.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
// Entity's methods
bool Entity::isAlive() {
    return alive;
}

Entity_type Entity::getType() {
    return type;
}


// Wall's methods
Wall::Wall(const sf::Vector2f& position, Difficulty df) {
    path = "src/images/wall/wall-";
    texture.loadFromFile(path + '0' + ".png");
    durability = 20;
    switch (df) {
        case Difficulty::Easy:
            damage = 2; break;
        case Difficulty::Normal:
            damage = 4; break;
        case Difficulty::Hard:
            damage = 6; break;
        case Difficulty::Extreme:
            damage = 10; break;
    }
    sprite.setTexture(texture);
    sprite.setScale(sf::Vector2f(3.0f, 3.0f));
    setPosition(position);
}

bool Wall::getDamage() { // true if alive, false else
    durability -= damage;
    if (durability > 16) texture.loadFromFile(path + '0' + ".png");
    else if (durability > 12) texture.loadFromFile(path + '1' + ".png");
    else if (durability > 8) texture.loadFromFile(path + '2' + ".png");
    else if (durability > 4) texture.loadFromFile(path + '3' + ".png");
    else if (durability > 0) texture.loadFromFile(path + '4' + ".png");
    else return false;
    return true;
}


// Bullet's methods
Bullet::Bullet(const std::string& path, float scale,
    const sf::Vector2f& dir, float sped, const sf::Vector2f& position, Entity_type from_who, float rotation):
    speed(sped), direction(dir), from(from_who) {
    texture.loadFromFile(path);
    sprite.setTexture(texture);
    sprite.setScale({scale, scale});
    setPosition(position);
    sprite.rotate(rotation);
}
void Bullet::move() {
    sprite.move({direction.x * speed, direction.y * speed});
}

Entity_type Bullet::getSender() {
return from;
}

// Player's methods
void Player::init(Difficulty df) {
    type = Entity_type::Player;
    last_shot = std::chrono::high_resolution_clock::now();
    path = "src/images/game/spaceship";
    // if (df == Difficulty::Extreme) lives = 1;
    // else if (df == Difficulty::Hard) lives = 2;
    // else lives = 3;
    lives = 3;
    texture.loadFromFile(path + ".png");
    sprite.setTexture(texture);
    sprite.setScale({current_scale, current_scale});
    setPosition(sf::Vector2f(WIDTH/2, HEIGHT/8*7));
}

void Player::getDamage() {
    if (is_damaging) return;
    is_damaging = true;
    texture.loadFromFile(path + "-hit" + ".png");
    time_from_damaged = std::chrono::high_resolution_clock::now();
    --lives;
    if (lives <= 0) alive = false;
}

bool Player::update() {
    checkDamage();
    if (alive) return true;
    current_scale -= 0.01f;
    sprite.setScale(sf::Vector2f(current_scale, current_scale));
    return (current_scale >= 0.f);
}

void Player::checkDamage() {
    if (!is_damaging) return;
    std::chrono::high_resolution_clock::time_point nw = std::chrono::high_resolution_clock::now();
    std::chrono::high_resolution_clock::duration d = nw - time_from_damaged;
    std::chrono::milliseconds s = std::chrono::duration_cast<std::chrono::milliseconds>(d);
    if (s >= std::chrono::milliseconds(500)) {
        texture.loadFromFile(path + ".png");
        is_damaging = false;
    }
}

bool Player::ready_to_shoot() {
    std::chrono::high_resolution_clock::time_point nw = std::chrono::high_resolution_clock::now();
    std::chrono::high_resolution_clock::duration d = nw - last_shot;
    std::chrono::seconds s = std::chrono::duration_cast<std::chrono::seconds>(d);
    return (s >= std::chrono::seconds(1));
}

int Player::getLives() {
    return lives;
}

std::shared_ptr<Bullet> Player::shot() {
    last_shot = std::chrono::high_resolution_clock::now();
    return std::make_shared<Bullet>("src/images/bullets/bullet-0.png", 2.0f, sf::Vector2f(0.0f, -1.0f), 3.0f, getSpriteCenter(), Entity_type::Player);
}

// Button's methods
void Button::init(const std::string& name, const sf::Vector2f& position, float scale) {
    path = "src/images/buttons/" + name;
    textures["main"].loadFromFile(path + ".png");
    textures["hovered"].loadFromFile(path + "-hovered.png");
    textures["pressed"].loadFromFile(path + "-pressed.png");
    sprite.setTexture(textures["main"]);
    sprite.setScale(sf::Vector2f(scale, scale));
    setPosition(position);
}

bool Button::checkCursor(const sf::Vector2i& cursor_pos) const {
    return sprite.getGlobalBounds().intersects(sf::FloatRect(sf::Vector2f(cursor_pos), sf::Vector2f(1.0f, 1.0f)));
}

void Button::updateButton(const sf::Vector2i& position) {
    if (!checkCursor(position)) {
        if (state != 0) makeMain();
        state = 0;
    }
    else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if (state != 1) makeHovered();
        state = 1;
    }
    else {
        if (state != 2) makePressed();
        state = 2;
    }
}

bool Button::checkButton() const {
    return (state == 2) ? true : false;
}

void Button::makeMain() {
    sprite.setTexture(textures["main"]);
}

void Button::makeHovered() {
    sprite.setTexture(textures["hovered"]);
}

void Button::makePressed() {
    sprite.setTexture(textures["pressed"]);
}

void Enemy::init(Difficulty df, Entity_type tp, const sf::Vector2f& pos) {
    time_to_change_animation = std::chrono::high_resolution_clock::now();
    path = "src/images/enemies/";
    switch (tp) {
        case Entity_type::Enemy_tier1:
            path += "tier1"; break;
        case Entity_type::Enemy_tier2:
            path += "tier2"; break;
        case Entity_type::Enemy_tier3:
            path += "tier3"; break;
        default:
            break;
    }
    for (int i = 1; i != 4; ++i) {
        animations.emplace_back();
        animations.back().loadFromFile(path + '-' + static_cast<char>('0' + i) + ".png");
    }
    sprite.setTexture(animations.front());
    sprite.setScale(sf::Vector2f(current_scale, current_scale));
    setPosition(pos);
    type = tp;
    switch (df) {
        case Difficulty::Easy:
            probabilty_to_shot = 10000; break;
        case Difficulty::Normal:
            probabilty_to_shot = 5000; break;
        case Difficulty::Hard:
            probabilty_to_shot = 2500; break;
        case Difficulty::Extreme:
            probabilty_to_shot = 1000; break;
        default:
            break;
    }
}

void Enemy::getDamage() {
    alive = false;
    texture.loadFromFile(path + "-hit.png");
    sprite.setTexture(texture);
}

void Enemy::updateAnimation() {
    current_animation = (current_animation + 1) % 3;
    sprite.setTexture(animations[current_animation]);
}

bool Enemy::update() {
    std::chrono::high_resolution_clock::time_point nw = std::chrono::high_resolution_clock::now();
    std::chrono::high_resolution_clock::duration d = nw - time_to_change_animation;
    std::chrono::milliseconds s = std::chrono::duration_cast<std::chrono::milliseconds>(d);
    if (s >= std::chrono::milliseconds(500) && alive) {
        updateAnimation();
        time_to_change_animation = nw;
    }
    if (alive) return true;
    current_scale -= 0.02f;
    sprite.setScale(sf::Vector2f(current_scale, current_scale));
    return current_scale >= 0.f;
}

bool Enemy::readyToShoot() {
    return rand() % probabilty_to_shot == 0;
}

sf::Vector2f Enemy::getBulletDirection(const sf::Vector2f& where) {
    float x = where.x - getSpriteCenter().x;
    float y = where.y - getSpriteCenter().y;
    float r = sqrt(x*x + y*y);
    return sf::Vector2f(x/r, y/r);
}

std::shared_ptr<Bullet> Enemy::shot(const sf::Vector2f& where) {
    sf::Vector2f dir = getBulletDirection(where);
    double pi = 3.14;
    double angle = atan(dir.y / dir.x) * 180 / pi;
    if (angle > 0) angle -= 90;
    else angle += 90;
    std::string bullet_path = "src/images/bullets/bullet-";
    bullet_path += static_cast<char>('0' + static_cast<int>(type));
    bullet_path += ".png";
    return std::make_shared<Bullet>(bullet_path, 2.0f, dir, 3.0f, getSpriteCenter(), type, angle);
}

void Army::init(Difficulty df) {
    position = WIDTH/2.0f;
    direction = sf::Vector2f(1, 0);
    for (int i = 0; i != 8; ++i) {
        for (int j = 0; j != 5; ++j) {
            enemies.push_back(std::make_shared<Enemy>());
            std::shared_ptr p = enemies.back();
            if (j == 0) p->init(df, Entity_type::Enemy_tier3, sf::Vector2f(WIDTH/2.0f+ 100*(i-4), 120 + 80*j));
            else if (j <= 2) p->init(df, Entity_type::Enemy_tier2, sf::Vector2f(WIDTH/2.0f+ 100*(i-4), 120 + 80*j));
            else p->init(df, Entity_type::Enemy_tier1, sf::Vector2f(WIDTH/2.0f+ 100*(i-4), 120 + 80*j));
        }
    }
    switch (df) {
        case Difficulty::Easy:
            probability = 10; break;
        case Difficulty::Normal:
            probability = 7; break;
        case Difficulty::Hard:
            probability = 5; break;
        case Difficulty::Extreme:
            probability = 2; break;
    }
}

void Army::move() {
    ++tick;
    if (tick % probability != 0) return;
    if (position + direction.x <= left_border) {
        direction.x = 1;
        moveDown();
    }
    else if (position + direction.x >= right_border) {
        direction.x = -1;
        moveDown();
    }
    position += direction.x;
    for (auto& elem: enemies) {
        elem->move(direction);
    }
    
}

void Army::moveDown() {
    for (auto& elem: enemies) {
        elem->move(sf::Vector2f(0.0f, 40.0f));
    }
}

void Army::checkCollision(std::list<std::shared_ptr<Bullet>>& bullets, std::list<std::list<std::shared_ptr<Bullet>>::iterator>& bullets_to_delete, Score& sc) {
    for (std::list<std::shared_ptr<Enemy>>::iterator enemy = enemies.begin(); enemy != enemies.end(); ++enemy) {
        if (!enemy->get()->isAlive()) continue;
        for (std::list<std::shared_ptr<Bullet>>::iterator bullet = bullets.begin(); bullet != bullets.end(); ++bullet) {
            if (bullet->get()->getSender() != Entity_type::Player) continue;
            if (enemy->get()->getGlobalBounds().intersects(bullet->get()->getGlobalBounds())) {
                bullets_to_delete.push_back(bullet);
                enemy->get()->getDamage();
                switch (enemy->get()->getType()) {
                    case Entity_type::Enemy_tier1:
                        sc.increase(10); break;
                    case Entity_type::Enemy_tier2:
                        sc.increase(20); break;
                    case Entity_type::Enemy_tier3:
                        sc.increase(30); break;
                    default:
                        break;
                }
            }
        }
        for (auto& elem: bullets_to_delete) {
            bullets.erase(elem);
        }
        bullets_to_delete.clear();
    }
    for (auto& elem: enemies_to_delete) {
        enemies.erase(elem);
    }
    enemies_to_delete.clear();
}
void Army::clear() {
    enemies.clear();
    enemies_to_delete.clear();
}

void Army::draw(sf::RenderWindow& w) {
    move();
    for (std::list<std::shared_ptr<Enemy>>::iterator enemy = enemies.begin(); enemy != enemies.end(); ++enemy) {
        if (!enemy->get()->update()) enemies_to_delete.push_back(enemy); // ne tuda
        else w.draw(enemy->get()->getSprite());
    }
}

void Army::shoot(std::list<std::shared_ptr<Bullet>>& bullets, const sf::Vector2f& where) {
    for (std::list<std::shared_ptr<Enemy>>::iterator it = enemies.begin(); it != enemies.end(); ++it) {
        if (it->get()->readyToShoot() && it->get()->isAlive()) {
            bullets.push_back(it->get()->shot(where));
        }
    }
}

bool Army::isDefeated() {
    return enemies.size() == 0;
}

int Army::getAlive() {
    return enemies.size();
}
