#include "base.hpp"

void SFMLObject::move(const sf::Vector2f& mv, float scale) {
    sprite.move({mv.x*scale, mv.y*scale});
}

float SFMLObject::getSpriteWidth() const {
    return sprite.getGlobalBounds().width;
}

float SFMLObject::getSpriteHeight() const {
    return sprite.getGlobalBounds().height;
}

sf::Vector2f SFMLObject::getSpriteCenter() const {
    float x = sprite.getPosition().x;
    float y = sprite.getPosition().y;
    x += getSpriteWidth()/2.0f;
    y += getSpriteHeight()/2.0f;
    return sf::Vector2f(x, y);
}

void SFMLObject::createSprite() {
    sprite.setTexture(texture);
}

sf::Sprite& SFMLObject::getSprite() {
    return sprite;
}
sf::Texture& SFMLObject::getTexture() {
    return texture;
}

void SFMLObject::setPosition(const sf::Vector2f& pos) {
    float x = pos.x - getSpriteWidth()/2.0f;
    float y = pos.y - getSpriteHeight()/2.0f;
    sprite.setPosition(sf::Vector2f(x, y));
}

sf::FloatRect SFMLObject::getGlobalBounds() const {
    return sprite.getGlobalBounds();
}


void Score::displayWithTitle(sf::RenderWindow& w) {
    w.draw(title.getSprite());
    int x = 0;
    for (std::list<std::shared_ptr<SFMLObject>>::iterator it = number.begin(); it != number.end(); ++it, ++x) {
        it->get()->setPosition(sf::Vector2f(position.x + 150 + distance*x, position.y));
        w.draw(it->get()->getSprite());
    }
}
void Score::displayWithoutTitle(sf::RenderWindow& w) {
    int x = 0;
    for (std::list<std::shared_ptr<SFMLObject>>::iterator it = number.begin(); it != number.end(); ++it, ++x) {
        it->get()->getSprite().setPosition(sf::Vector2f(position.x + distance*x, position.y));
        w.draw(it->get()->getSprite());
    }
}

void Score::setPosition(const sf::Vector2f& pos) {
    position = pos;
}

void Score::increase(int num) {
    current_score += num;
    normalize();
}

void Score::normalize() {
    number.clear();
    int tmp = current_score;
    std::string path = "src/images/digits/";
    while (tmp != 0) {
        char digit = '0' + tmp % 10;
        number.push_front(std::make_shared<SFMLObject>());
        number.front()->getTexture().loadFromFile(path + digit + ".png");
        number.front()->getSprite().setTexture(number.front()->getTexture());
        number.front()->getSprite().setScale(sf::Vector2f(1.0f, 0.8f));
        tmp /= 10;
    }

    if (number.size() == 0) {
        number.push_front(std::make_shared<SFMLObject>());
        number.front()->getTexture().loadFromFile(path + '0' + ".png");
        number.front()->getSprite().setTexture(number.front()->getTexture());
        number.front()->getSprite().setScale(sf::Vector2f(1.0f, 0.8f));
    }
}

void Score::init(const sf::Vector2f& pos) {
    current_score = 0;
    title.getTexture().loadFromFile("src/images/game/score.png");
    title.getSprite().setTexture(title.getTexture());
    normalize();
    title.setPosition(pos);
    position = pos;
}
void Score::multiply(int n) {
    current_score *= n;
    normalize();
}