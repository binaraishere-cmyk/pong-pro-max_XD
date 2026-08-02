#include <SFML/Graphics.hpp>
#include <iostream>

// game states

enum class GameState {
    Menu,
    Playing,
    GameOver,
    Pause
};

int main()
{
    // window

    sf::RenderWindow window(sf::VideoMode({ 700, 600 }), "My SFML Pong");
    window.setFramerateLimit(60);

    // game variables

    GameState state = GameState::Menu;

    int menuvalue = 1;
    float menuY = 180.f;

    // player

    float PlayerX = 3.0f;
    float PlayerY = 50.0f;

    // ball

    float x = 100.0f;
    float y = 100.0f;

    float ballSpeedX = 2.5f;
    float ballSpeedY = 2.5f;

    // game objects

    sf::RectangleShape square(sf::Vector2f(10.0f, 10.0f));
    sf::RectangleShape pads(sf::Vector2f(5.0f, 70.0f));

    square.setFillColor(sf::Color::White);
    pads.setFillColor(sf::Color::Cyan);

    // fonts

    sf::Font font;
    sf::Font font2;
    sf::Font font3;

    // load fonts

    if (!font.openFromFile("assets/UNISPACE BD.TTF")) {
        std::cout << "Font error\n";
        return -1;
    }

    if (!font2.openFromFile("assets/pause.ttf")) {
        std::cout << "Font error\n";
        return -1;
    }

    if (!font3.openFromFile("assets/default.ttf")) {
        std::cout << "Font error\n";
        return -1;
    }

    // game over text

    sf::Text gameOverText(font);
    gameOverText.setString("GAME OVER");
    gameOverText.setCharacterSize(60);
    gameOverText.setFillColor(sf::Color(255, 60, 60));
    gameOverText.setPosition({ 170.f, 180.f });

    // title

    sf::Text gameMenu(font2);
    gameMenu.setString("PONG");
    gameMenu.setCharacterSize(70);
    gameMenu.setFillColor(sf::Color::Cyan);
    gameMenu.setPosition({ 245.f, 50.f });

    // play

    sf::Text Play(font3);
    Play.setString("PLAY");
    Play.setCharacterSize(34);
    Play.setFillColor(sf::Color::Green);
    Play.setPosition({ 280.f, 180.f });

    // settings

    sf::Text Settings(font3);
    Settings.setString("SETTINGS");
    Settings.setCharacterSize(34);
    Settings.setFillColor(sf::Color(255, 220, 0));
    Settings.setPosition({ 280.f, 240.f });

    // quit

    sf::Text Close(font3);
    Close.setString("QUIT");
    Close.setCharacterSize(34);
    Close.setFillColor(sf::Color(255, 80, 80));
    Close.setPosition({ 280.f, 300.f });

    // pause

    sf::Text PauseMenu(font2);
    PauseMenu.setString("PAUSED");
    PauseMenu.setCharacterSize(60);
    PauseMenu.setFillColor(sf::Color(255, 0, 255));
    PauseMenu.setPosition({ 205.f, 60.f });

    // menu cursor

    sf::Text picked(font3);
    picked.setString(">");
    picked.setCharacterSize(34);
    picked.setFillColor(sf::Color::White);
    picked.setPosition({ 260.f, menuY });

    // game loop

    while (window.isOpen())
    {
        // events

        while (const std::optional event = window.pollEvent())
        {
            // close window

            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            // keyboard

            if (const auto key = event->getIf<sf::Event::KeyPressed>())
            {
                // pause

                if (key->code == sf::Keyboard::Key::Escape && state == GameState::Playing)
                {
                    state = GameState::Pause;
                }

                // menu

                if (state == GameState::Menu || state == GameState::Pause)
                {
                    if (key->code == sf::Keyboard::Key::W)
                    {
                        menuvalue--;
                    }

                    if (key->code == sf::Keyboard::Key::S)
                    {
                        menuvalue++;
                    }

                    // wrap menu

                    if (menuvalue < 1)
                    {
                        menuvalue = 3;
                    }

                    if (menuvalue > 3)
                    {
                        menuvalue = 1;
                    }

                    // move cursor

                    if (menuvalue == 1)
                        menuY = 180.f;

                    if (menuvalue == 2)
                        menuY = 240.f;

                    if (menuvalue == 3)
                        menuY = 300.f;

                    picked.setPosition({ 260.f, menuY });

                    // select option

                    if (key->code == sf::Keyboard::Key::Space || key->code == sf::Keyboard::Key::Enter)
                    {
                        if (menuvalue == 1)
                        {
                            state = GameState::Playing;
                        }
                        else if (menuvalue == 2)
                        {
                            std::cout << "Settings clicked\n";
                        }
                        else if (menuvalue == 3)
                        {
                            window.close();
                        }
                    }
                }

                // player

                if (state == GameState::Playing)
                {
                    if (key->code == sf::Keyboard::Key::W)
                        PlayerY -= 15.f;

                    if (key->code == sf::Keyboard::Key::S)
                        PlayerY += 15.f;
                }
            }
        }

        // update

        if (state == GameState::Playing)
        {
            // move ball

            x += ballSpeedX;
            y += ballSpeedY;

            // update objects

            square.setPosition({ x, y });
            pads.setPosition({ PlayerX, PlayerY });

            // paddle collision

            if (square.getGlobalBounds().findIntersection(pads.getGlobalBounds()).has_value())
            {
                ballSpeedX = -ballSpeedX;
                x = PlayerX + 5.f;
            }

            // wall collision

            if (y <= 0 || y >= 590)
                ballSpeedY = -ballSpeedY;

            if (x >= 690)
                ballSpeedX = -ballSpeedX;

            // game over

            if (x <= 0)
                state = GameState::GameOver;

            // keep paddle on screen

            if (PlayerY < 0)
                PlayerY = 0;

            if (PlayerY > 530)
                PlayerY = 530;
        }

        // draw

        window.clear(sf::Color::Black);

        if (state == GameState::Menu)
        {
            window.draw(gameMenu);
            window.draw(Play);
            window.draw(Settings);
            window.draw(Close);
            window.draw(picked);
        }
        else if (state == GameState::Playing)
        {
            window.draw(square);
            window.draw(pads);
        }
        else if (state == GameState::Pause)
        {
            window.draw(PauseMenu);
            window.draw(Play);
            window.draw(Settings);
            window.draw(Close);
            window.draw(picked);
        }
        else if (state == GameState::GameOver)
        {
            window.draw(gameOverText);
        }

        window.display();
    }

    return 0;
}