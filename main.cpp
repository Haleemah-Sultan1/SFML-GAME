#include <SFML/Graphics.hpp>

// Function to handle the Menu state
void menu(sf::RenderWindow& window)
{
    // Load menu texture
    sf::Texture menuTexture;
    if (!menuTexture.loadFromFile("assets/menu.png"))
        return; // Exit function if file not found

    sf::Sprite menuSprite(menuTexture);

    // Optional: Scale menu background to fit window size
    sf::Vector2u windowSize = window.getSize();
    sf::Vector2u textureSize = menuTexture.getSize();
    menuSprite.setScale({
        static_cast<float>(windowSize.x) / textureSize.x,
        static_cast<float>(windowSize.y) / textureSize.y
    });

    bool inMenu = true;
    while (inMenu && window.isOpen())
    {
        // Event handling for the menu
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            // Check for "Enter" key to start the game
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->code == sf::Keyboard::Key::Enter)
                {
                    inMenu = false; // Break the menu loop to start the game
                }
            }
        }

        window.clear();
        window.draw(menuSprite);
        window.display();
    }
}

int main()
{
    // Create window (SFML 3 syntax)
    sf::RenderWindow window(
        sf::VideoMode({1280, 720}),
        "SFML 3 Endless Background Forward"
    );
    window.setFramerateLimit(60);

    // --- START MENU ---
    // This will block execution until the user presses Enter
    menu(window);

    // If the user closed the window during the menu, don't run the game
    if (!window.isOpen()) return 0;

    // --- GAME SETUP ---
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
    bg2.setPosition({-bgWidth, 0.f}); 

    float speed = 2.5f;

    // --- GAME LOOP ---
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // Move backgrounds
        bg1.move({speed, 0.f});
        bg2.move({speed, 0.f});

        // Reset positions for endless effect
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