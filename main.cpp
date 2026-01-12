#include <SFML/Graphics.hpp>

int main()
{
    // Create window (SFML 3 syntax)
    sf::RenderWindow window(
        sf::VideoMode({1280, 720}),
        "SFML 3 Endless Background Forward"
    );
    window.setFramerateLimit(60);

    // Load texture
    sf::Texture bgTexture;
    if (!bgTexture.loadFromFile("assets/gpt5.png"))
        return -1;

    // Create sprites
    sf::Sprite bg1(bgTexture);
    sf::Sprite bg2(bgTexture);

    // Get background width
    float bgWidth = static_cast<float>(bgTexture.getSize().x);

    // Position side by side
    bg1.setPosition({0.f, 0.f});
    bg2.setPosition({-bgWidth, 0.f}); // bg2 starts to the left

    float speed = 2.5f;

    // Game loop
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // Move backgrounds to the right
        bg1.move({speed, 0.f});
        bg2.move({speed, 0.f});

        // Reset when off screen
        if (bg1.getPosition().x >= bgWidth)
            bg1.setPosition({bg2.getPosition().x - bgWidth, 0.f});

        if (bg2.getPosition().x >= bgWidth)
            bg2.setPosition({bg1.getPosition().x - bgWidth, 0.f});

        window.clear();
        window.draw(bg1);
        window.draw(bg2);
        window.display();
    }

    return 0;
}
