#include "base.cpp"
#include "entities.cpp"
#include "interface.hpp"


bool Page::checkButton(const Button& btn, const sf::Vector2i& pos) {
    if (btn.checkButton()) switched = true;
    if (btn.checkCursor(pos) && switched && !sf::Mouse::isButtonPressed(sf::Mouse::Left)) return true;
    return false;
}


void GamePage::initPlayer(float scale, int lives) {
    player.init(difficulty);
}

void GamePage::initWalls() {
    for (int i = 0; i != WALL_NUMBER; ++i) {
        walls.push_back(std::make_shared<Wall>(sf::Vector2f(WIDTH/WALL_NUMBER/2 + i*WIDTH/WALL_NUMBER, HEIGHT/4*3), difficulty));
    }
}

void GamePage::checkWalls(sf::RenderWindow& w) {
    for (std::list<std::shared_ptr<Wall>>::iterator it = walls.begin(); it != walls.end(); ++it) {
        w.draw(it->get()->getSprite());
        for (std::list<std::shared_ptr<Bullet>>::iterator bullet = bullets.begin(); bullet != bullets.end(); ++bullet) {
            if (bullet->get()->getSprite().getGlobalBounds().intersects(it->get()->getSprite().getGlobalBounds())) {
                it->get()->getDamage();
                if (!it->get()->isAlive()) walls_to_delete.push_back(it);
                bullets_to_delete.push_back(bullet);
            }
        }
    }
}

void GamePage::checkBullets(sf::RenderWindow& w) {
    // check bullets for crossing the end of a window
    for (std::list<std::shared_ptr<Bullet>>::iterator it = bullets.begin(); it != bullets.end(); ++it) {
        it->get()->move();
        sf::Vector2f pos = it->get()->getSprite().getPosition();
        w.draw(it->get()->getSprite());
        if (pos.x + it->get()->getSpriteWidth() < 0 || pos.y + it->get()->getSpriteHeight() < 0 ||
            pos.x - it->get()->getSpriteWidth() > w.getSize().x || pos.y - it->get()->getSpriteHeight() > w.getSize().y) {
            bullets_to_delete.push_back(it);
        }
    }
}

void GamePage::checkEnemies(sf::RenderWindow& w) {
    army.checkCollision(bullets, bullets_to_delete, *score);
}

void GamePage::checkPlayer() {
    // check shoot
    if (player.ready_to_shoot() && auto_shoot && player.isAlive()) {
        bullets.push_back(player.shot());
    }

    // check bullets
    for (std::list<std::shared_ptr<Bullet>>::iterator bullet = bullets.begin(); bullet != bullets.end(); ++bullet) {
        if (bullet->get()->getSender() == Entity_type::Player) continue;
        if (player.getGlobalBounds().intersects(bullet->get()->getGlobalBounds())) {
            bullets_to_delete.push_back(bullet);
            player.getDamage();
        }
    }
}
void GamePage::evaluateFinalScore() {
    score->increase(70 * walls.size());
    score->multiply(player.getLives());
}
void GamePage::deleteSprites() {
    // delete 
    for (auto elem: bullets_to_delete) {
        bullets.erase(elem);
    }
    for (auto elem: walls_to_delete) {
        walls.erase(elem);
    }
    bullets_to_delete.clear();
    walls_to_delete.clear();
}

void GamePage::init(Difficulty dif, Score* sc) {
    score = sc;
    score->init(sf::Vector2f(140, 50));
    difficulty = dif;
    initPlayer();
    initWalls();
    army.init(dif);
    lives.getTexture().loadFromFile("src/images/game/lives.png");
    lives.getSprite().setTexture(lives.getTexture());
    lives.setPosition(sf::Vector2f(WIDTH/4*3 + 50, 40));
    for (int i = 0; i != 3; ++i) {
        hearts.push_back(std::make_shared<SFMLObject>());
        hearts.back().get()->getTexture().loadFromFile("src/images/game/heart.png");
        hearts.back().get()->getSprite().setTexture(hearts.back().get()->getTexture());
        hearts.back().get()->setPosition(sf::Vector2f(WIDTH/4*3 + 220 + 70*i, 40));
    }

}

void GamePage::processKey(sf::Keyboard::Key k) {
    switch (k) {
        case sf::Keyboard::R:
            auto_shoot = !auto_shoot; break;
        case sf::Keyboard::F:
            player.getDamage();
        default:
            break;
    }
}

Pages GamePage::render(sf::RenderWindow& w) {
    sf::Event event;
    while (w.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            w.close();
        }
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            clear();
            return Pages::Main_menu;
        } 
        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up) {
            score->increase(1);
        }
        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::X) {
            evaluateFinalScore();
            clear();
            return Pages::Victory;
        }
        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Z) {
            clear();
            return Pages::Lose;
        }
        else if (event.type == sf::Event::KeyPressed) {
            processKey(event.key.code);
        }
    }
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        if (w.getSize().x - player.getSprite().getPosition().x - player.getSprite().getGlobalBounds().width > 20)
        player.move({1.0f, 0.0f}, 4.0f);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        if (player.getSprite().getPosition().x > 20)
        player.move({-1.0f, 0.0f}, 4.0f);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        if (!auto_shoot && player.ready_to_shoot()) {
            bullets.push_back(player.shot());
        }
    }

    

    if (!player.update()) {
        clear();
        return Pages::Lose;
    }
    if (army.isDefeated()) {
        evaluateFinalScore();
        clear();
        return Pages::Victory;
    }
    score->displayWithTitle(w);
    army.draw(w);
    army.shoot(bullets, player.getSpriteCenter());
    w.draw(player.getSprite());
    w.draw(lives.getSprite());
    checkCollision(w);
    for (int i = 0; i != player.getLives(); ++i) {
        w.draw(hearts[i]->getSprite());
    }
    deleteSprites();
    return Pages::Game;
}

void GamePage::checkCollision(sf::RenderWindow& w) {
    if (player.isAlive()) checkPlayer();
    checkWalls(w);
    checkBullets(w);
    checkEnemies(w);
}


void GamePage::clear() {
    hearts.clear();
    army.clear();
    bullets.clear();
    walls.clear();
    bullets_to_delete.clear();
    walls_to_delete.clear();
    player = Player();
    is_over = false;
    auto_shoot = true;

};

void MainMenuPage::init() {
    sf::Texture& txt = title.getTexture();
    sf::Sprite& sp = title.getSprite();
    txt.loadFromFile("src/images/main/title.png");
    sp.setTexture(txt);
    sp.setScale(sf::Vector2f(4.0f, 4.0f));
    title.setPosition(sf::Vector2f(WIDTH/2, HEIGHT/4));
    play_button.init("play", sf::Vector2f(WIDTH/2.0f, HEIGHT/2.0f));
    controls_button.init("controls", sf::Vector2f(WIDTH/2.0f, HEIGHT/2.0f + 100));
    quit_button.init("quit", sf::Vector2f(WIDTH/2.0f, HEIGHT/2.0f + 200));
}

Pages MainMenuPage::render(sf::RenderWindow& w, const sf::Vector2i& position) {
    sf::Event event;
    while (w.pollEvent(event)) {
        if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
            w.close();
        }
    }
    play_button.updateButton(position);
    controls_button.updateButton(position);
    quit_button.updateButton(position);

    w.draw(title.getSprite());
    w.draw(play_button.getSprite());
    w.draw(controls_button.getSprite());
    w.draw(quit_button.getSprite());

    if (checkButton(play_button, position)) {
        switched = false;
        return Pages::Difficulty;
    }
    else if (checkButton(quit_button, position)) {
        w.close();
    }
    else if (checkButton(controls_button, position)) {
        switched = false;
        return Pages::Controls;
    }
    return Pages::Main_menu;
}


void ControlsPage::init() {
    exit_button.init("exit", sf::Vector2f(WIDTH/4*3 - 80, HEIGHT/4 + 30));
    sf::Texture& txt = controls_window.getTexture();
    sf::Sprite& sp = controls_window.getSprite();
    txt.loadFromFile("src/images/controls/controls.png"); // NEEDS TO CHANGE THE PNG
    sp.setTexture(txt);
    controls_window.setPosition(sf::Vector2f(WIDTH/2.0f, HEIGHT/2.0f));
}

Pages ControlsPage::render(sf::RenderWindow& w, const sf::Vector2i& pos) {
    sf::Event event;
    while (w.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            w.close();
        }
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) return Pages::Main_menu;
    }
    exit_button.updateButton(pos);

    w.draw(controls_window.getSprite());
    w.draw(exit_button.getSprite());
    if (checkButton(exit_button, pos)) {
        switched = false;
        return Pages::Main_menu;
    }
    return Pages::Controls;
}

void DifficultyPage::init() {
    easy.init("easy", sf::Vector2f(WIDTH/2.0f, HEIGHT/2 - 100));
    normal.init("normal", sf::Vector2f(WIDTH/2.0f, HEIGHT/2));
    hard.init("hard", sf::Vector2f(WIDTH/2.0f, HEIGHT/2 + 100));
    extreme.init("extreme", sf::Vector2f(WIDTH/2.0f, HEIGHT/2 + 200));
    sf::Texture& txt = difficulty_window.getTexture();
    sf::Sprite& sp = difficulty_window.getSprite();
    txt.loadFromFile("src/images/difficulty/difficulty.png");
    sp.setTexture(txt);
    difficulty_window.setPosition(sf::Vector2f(WIDTH/2.0f, HEIGHT/2.0f));
}

Pages DifficultyPage::render(sf::RenderWindow& w, const sf::Vector2i& pos, Difficulty& df) {
    sf::Event event;
    while (w.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            w.close();
        }
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) return Pages::Main_menu;
    }
    easy.updateButton(pos);
    normal.updateButton(pos);
    hard.updateButton(pos);
    extreme.updateButton(pos);

    w.draw(difficulty_window.getSprite());
    w.draw(easy.getSprite());
    w.draw(normal.getSprite());
    w.draw(hard.getSprite());
    w.draw(extreme.getSprite());

    if (checkButton(easy, pos)) {
        switched = false;
        df = Difficulty::Easy;
        return Pages::Game;
    }
    else if (checkButton(normal, pos)) {
        switched = false;
        df = Difficulty::Normal;
        return Pages::Game;
    }
    else if (checkButton(hard, pos)) {
        switched = false;
        df = Difficulty::Hard;
        return Pages::Game;
    }
    else if (checkButton(extreme, pos)) {
        switched = false;
        df = Difficulty::Extreme;
        return Pages::Game;
    }
    return Pages::Difficulty;
}

void VictoryPage::init(Score* sc) {
    score = sc;
    retry.init("retry", sf::Vector2f(WIDTH/2.0f, HEIGHT/2 + 100));
    main_menu.init("menu", sf::Vector2f(WIDTH/2.0f, HEIGHT/2 + 200));
    sf::Texture& txt = victory_window.getTexture();
    sf::Sprite& sp = victory_window.getSprite();
    txt.loadFromFile("src/images/game_over/victory.png");
    sp.setTexture(txt);
    victory_window.setPosition(sf::Vector2f(WIDTH/2.0f, HEIGHT/2.0f));
    score->setPosition(sf::Vector2f(WIDTH/2.0f + 190, HEIGHT/2.0f - 145));
}

Pages VictoryPage::render(sf::RenderWindow& w, const sf::Vector2i& pos) {
    sf::Event event;
    while (w.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            w.close();
        }
    }
    retry.updateButton(pos);
    main_menu.updateButton(pos);

    w.draw(victory_window.getSprite());
    w.draw(retry.getSprite());
    w.draw(main_menu.getSprite());
    score->displayWithoutTitle(w);

    if (checkButton(retry, pos)) {
        switched = false;
        return Pages::Difficulty;
    }
    else if (checkButton(main_menu, pos)) {
        switched = false;
        return Pages::Main_menu;
    }
    return Pages::Victory;
}

void LosePage::init(Score* sc) {
    score = sc;
    retry.init("retry", sf::Vector2f(WIDTH/2.0f, HEIGHT/2 + 100));
    main_menu.init("menu", sf::Vector2f(WIDTH/2.0f, HEIGHT/2 + 200));
    sf::Texture& txt = lose_window.getTexture();
    sf::Sprite& sp = lose_window.getSprite();
    txt.loadFromFile("src/images/game_over/lose.png");
    sp.setTexture(txt);
    lose_window.setPosition(sf::Vector2f(WIDTH/2.0f, HEIGHT/2.0f));
    score->setPosition(sf::Vector2f(WIDTH/2.0f + 190, HEIGHT/2.0f - 145));
}

Pages LosePage::render(sf::RenderWindow& w, const sf::Vector2i& pos) {
    sf::Event event;
    while (w.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            w.close();
        }
    }
    retry.updateButton(pos);
    main_menu.updateButton(pos);

    w.draw(lose_window.getSprite());
    w.draw(retry.getSprite());
    w.draw(main_menu.getSprite());
    score->displayWithoutTitle(w);

    if (checkButton(retry, pos)) {
        switched = false;
        return Pages::Difficulty;
    }
    else if (checkButton(main_menu, pos)) {
        switched = false;
        return Pages::Main_menu;
    }
    return Pages::Lose;
}
