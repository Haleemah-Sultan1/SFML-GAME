#include <SFML/Graphics.hpp>
#include <iostream>
#include <SFML/System/Clock.hpp>
#include <vector>
#include <string>

using namespace std;
using namespace sf;

// Function to handle the Menu state
void menu(sf::RenderWindow& window, int& selectedPlayer)
{
    // 1. Load Background and Font
    sf::Texture menuTexture;
    if (!menuTexture.loadFromFile("assets/menu.png")) return; 
   if(! menuTexture.loadFromFile("assets/menu.png"))
   return;

    sf::Sprite menuSprite(menuTexture);
    
    sf::Vector2u winSize = window.getSize();
    sf::Vector2u texSize = menuTexture.getSize();
    menuSprite.setScale({ (float)winSize.x / texSize.x, (float)winSize.y / texSize.y });

    sf::Font font;
    if (!font.openFromFile("assets/font.ttf")) return;

    sf::Texture p1Tex, p2Tex, credTex;
    if (!p1Tex.loadFromFile("assets/p1_avatar.png")) { std::cout << "Error loading p1\n"; }
    if (!p2Tex.loadFromFile("assets/p2_avatar.png")) { std::cout << "Error loading p2\n"; }
    if (!credTex.loadFromFile("assets/pic.png")) { std::cout << "Error loading pic.png\n"; }

    sf::Sprite p1Sprite(p1Tex), p2Sprite(p2Tex), credSprite(credTex);
    
    // Position Player Avatars
    p1Sprite.setPosition({410.f, 310.f});
    if (p1Tex.getSize().x > 0)
        p1Sprite.setScale({180.f / p1Tex.getSize().x, 180.f / p1Tex.getSize().y});
    
    p2Sprite.setPosition({690.f, 310.f});
    if (p2Tex.getSize().x > 0)
        p2Sprite.setScale({180.f / p2Tex.getSize().x, 180.f / p2Tex.getSize().y});

    // Position and Size of Credit Picture
    credSprite.setPosition({750.f, 300.f}); // Original spot
    credSprite.setScale({1.2f, 1.2f});       // Made slightly larger than 1.0f

    // COLORS
    sf::Color yellowishBrown(204, 153, 51, 220); // Mustard yellow
    sf::Color darkBrown(60, 40, 20);            // Dark brown outline/text
    sf::Color instructionGrey(200, 200, 200);   // Darker off-white

    // BUTTONS SETUP
    sf::RectangleShape btnPlay({250.f, 50.f}), btnInst({250.f, 50.f}), btnCred({250.f, 50.f});
    btnPlay.setPosition({515.f, 300.f}); btnInst.setPosition({515.f, 370.f}); btnCred.setPosition({515.f, 440.f});
    
    auto setupBtn = [&](sf::RectangleShape& b) {
        b.setFillColor(yellowishBrown); b.setOutlineThickness(3); b.setOutlineColor(darkBrown);
    };
    setupBtn(btnPlay); setupBtn(btnInst); setupBtn(btnCred);

    sf::Text txtPlay(font, "PLAY", 25); txtPlay.setFillColor(darkBrown); txtPlay.setPosition({610.f, 310.f});
    sf::Text txtInst(font, "INSTRUCTIONS", 25); txtInst.setFillColor(darkBrown); txtInst.setPosition({545.f, 380.f});
    sf::Text txtCred(font, "CREDITS", 25); txtCred.setFillColor(darkBrown); txtCred.setPosition({590.f, 450.f});

    // Selection Boxes for Players
    sf::RectangleShape p1Box({200.f, 200.f}), p2Box({200.f, 200.f});
    p1Box.setPosition({400.f, 300.f}); p2Box.setPosition({680.f, 300.f});
    p1Box.setFillColor(sf::Color(255, 255, 255, 30));
    p2Box.setFillColor(sf::Color(255, 255, 255, 30));
    p1Box.setOutlineThickness(5); p2Box.setOutlineThickness(5);

    int menuState = 0; // 0=Main, 1=Inst, 2=Cred, 3=PlayerSelect
    bool inMenu = true;

    while (inMenu && window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) window.close();

            if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mousePressed->button == sf::Mouse::Button::Left)
                {
                    sf::Vector2f mPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    if (menuState == 0) {
                        if (btnPlay.getGlobalBounds().contains(mPos)) menuState = 3; 
                        if (btnInst.getGlobalBounds().contains(mPos)) menuState = 1;
                        if (btnCred.getGlobalBounds().contains(mPos)) menuState = 2;
                    }
                    else if (menuState == 3) {
                        if (p1Box.getGlobalBounds().contains(mPos)) selectedPlayer = 1;
                        if (p2Box.getGlobalBounds().contains(mPos)) selectedPlayer = 2;
                    }
                }
            }
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::Escape) menuState = 0;
                if (menuState == 3 && keyPressed->code == sf::Keyboard::Key::Enter) inMenu = false; 
            }
        }

        p1Box.setOutlineColor(selectedPlayer == 1 ? sf::Color::Yellow : sf::Color::White);
        p2Box.setOutlineColor(selectedPlayer == 2 ? sf::Color::Yellow : sf::Color::White);

        window.clear();
        window.draw(menuSprite);

        if (menuState == 0) { // MAIN MENU 
            window.draw(btnPlay); window.draw(txtPlay);
            window.draw(btnInst); window.draw(txtInst);
            window.draw(btnCred); window.draw(txtCred);
        }
        else if (menuState == 1) { // INSTRUCTIONS 
            // Dark window background
            sf::RectangleShape overlay({1100.f, 500.f});
            overlay.setPosition({90.f, 150.f});
            overlay.setFillColor(sf::Color(0, 0, 0, 220)); // Very dark overlay
            overlay.setOutlineThickness(3);
            overlay.setOutlineColor(yellowishBrown);
            window.draw(overlay);

            // Larger, yellowish title
            sf::Text instTitle(font, "Karachi Survival Guide:", 55);
            instTitle.setPosition({130.f, 180.f});
            instTitle.setFillColor(yellowishBrown);
            window.draw(instTitle);

            // Darker white/grey body text
            sf::Text instBody(font, "1. Keep your phone hidden like it's a state secret.\n2. Trust the driver... but also pray quietly.\n3. If someone says \"bhai tension nahi leni\" - tension is guaranteed.\n4. If a road is empty, something is wrong.\n5. If it rains, cancel your plans, your hopes, and your shoes.", 28);
            instBody.setPosition({130.f, 290.f});
            instBody.setFillColor(instructionGrey);
            window.draw(instBody);

            sf::Text backMsg(font, "Press ESC to go back", 20);
            backMsg.setPosition({550.f, 600.f});
            window.draw(backMsg);
        }
        else if (menuState == 2) { // CREDITS
            // Header moved to the left
            sf::Text credHeader(font, "CREDITS:\nDeveloped by:", 50);
            credHeader.setPosition({150.f, 350.f}); 
            credHeader.setFillColor(yellowishBrown);
            window.draw(credHeader);

            // Names at original spot
            sf::Text names(font, "Haleema Zuhaib\nSeerat Fatima\nHaiqa Shahid", 50);
            names.setPosition({450.f, 455.f}); 
            names.setFillColor(sf::Color::White);
            window.draw(names);

            window.draw(credSprite); // Picture at original spot
        }
        else if (menuState == 3) { //PLAYER SELECTION
            sf::Text prompt(font, "SELECT PLAYER & PRESS ENTER", 30);
            prompt.setPosition({400.f, 150.f}); prompt.setFillColor(yellowishBrown);
            window.draw(prompt);
            window.draw(p1Box); window.draw(p2Box);
            window.draw(p1Sprite); window.draw(p2Sprite); 
        }
        window.display();
    }
}

// animation array of players 
void spritesfix(Texture* chtexture[], Sprite* chsprite[], RenderWindow& window, string character)
{
    for (int i = 0; i < 57; i++)
    {
        string filename = "assets/"+ character +"/";

        if (i < 10) filename += "0";
        filename += to_string(i) + "_"+character +"sheet2.png";

        if (!chtexture[i]->loadFromFile(filename))
            return;

        chsprite[i] = new Sprite(*chtexture[i]);
        chsprite[i]->setScale({-0.5f, 0.5f});
        // if(i>30&&i<45)
        // chsprite[i]->setPosition({1000.f, 345.f});
        chsprite[i]->setPosition({1000.f, 350.f});
    }
    int c= 5; int k=0;int b =3;
    for (int i = 30; i <= 42; i++)
    {
    chsprite[i]->setPosition({1000.f + b * k, 350.f - c * k});
    k++;
    }
    for (int i = 43; i <= 55; i++)
    {
    k--;
    chsprite[i]->setPosition({1000.f + b * k, 350.f - c * k});
    }
}



int main()
{
    sf::RenderWindow window(sf::VideoMode({1280, 720}), "Karachi Survival - SFML 3");
    window.setFramerateLimit(60);

    int playerChoice = 1; 
    menu(window, playerChoice);

    if (!window.isOpen()) return 0;

    // --- MAIN GAME LOOP ---
    sf::Texture bgTexture;
    if (!bgTexture.loadFromFile("assets/gpt5.png")) return -1;
    sf::Sprite bg1(bgTexture), bg2(bgTexture);
    float bgWidth = (float)bgTexture.getSize().x;
    bg2.setPosition({-bgWidth, 0.f}); 

    float speed = 180.0f;
    Clock clock;  // SFML clock to get real deltaTime

    std::vector<sf::Texture> dogFrames;
    int totalFrames = 25;
    int dogSpeed=420.0f;

    for (int i = 1; i <= totalFrames; i++)
   {
    sf::Texture texture;
    if (!texture.loadFromFile("assets/dog" + std::to_string(i) + ".png"))
        return -1;

    dogFrames.push_back(texture);
   }
   sf::Sprite dog(dogFrames[0]);
   dog.setScale({0.6f, 0.6f});
   dog.setPosition({90.f, 480.f});


    int frame = 0;
    float frameWidth = 119.f;       // width of a single frame
    float animationSpeed = 0.01f;    // how fast it animates
    float timer = 0.f;

    ///////////// player sprite /////////////////////
    Texture* chtexture[57];
    Sprite* chsprite[57];
    for (int i = 0; i < 57; i++)
    {
    chtexture[i] = new Texture; 
    chsprite[i] = nullptr;
   }
   string character = "boy";
    spritesfix(chtexture, chsprite, window,character);
 
    bool jump =false;
    bool isOnGround =true;
    int i=0;
    int j=0;
    Clock playeranimeclock;
    float frameTimeplayer = 0.0175f;


    // --- GAME LOOP ---
   while (window.isOpen())
   {
    // --- Event handling ---
    while (const std::optional event = window.pollEvent())
    {
        if (event->is<Event::Closed>())
            window.close();

                     // ESC key
        if (const auto* key = event->getIf<sf::Event::KeyPressed>())
        {
            if (key->code == sf::Keyboard::Key::Escape)
            {
                window.close();
            }
            //jumping of player 
            if (key->code == sf::Keyboard::Key::Space && isOnGround)
             {
              isOnGround = false; jump = true;
             }
        }
    }

    // --- Delta time ---
    float deltaTime = clock.restart().asSeconds();

    // --- Update background ---
    bg1.move({speed * deltaTime, 0.f});
    bg2.move({speed * deltaTime, 0.f});
    if (bg1.getPosition().x >= bgWidth)
        bg1.setPosition({bg2.getPosition().x - bgWidth, 0.f});
    if (bg2.getPosition().x >= bgWidth)
        bg2.setPosition({bg1.getPosition().x - bgWidth, 0.f});

    // --- Update dog movement ---
        if (dog.getPosition().x > 1280.f)
        dog.setPosition({-dog.getGlobalBounds().size.x, dog.getPosition().y});

    // --- Update dog animation ---
    timer += deltaTime;
    if (timer >= animationSpeed)
    {
         timer = 0.f;
        frame = (frame + 1) % totalFrames;
        dog.setTexture(dogFrames[frame]); // 
    }
    dog.setScale({1.0f, 1.0f}); // smaller dog
    dog.move({dogSpeed * deltaTime, 0.f});

     
    // --- Draw everything ---
    window.clear();
    window.draw(bg1);
    window.draw(bg2);
    window.draw(dog);

     //player sprite draw
       if(j>28) {
            jump =false;
             j=0; 
            isOnGround = true;
         }   
        if(jump)
       { 
       // for frame count  
          if (playeranimeclock.getElapsedTime().asSeconds() >= frameTimeplayer)
          {
            j++;
            playeranimeclock.restart();
          }
         window.draw(*chsprite[27+j]);
        }
        else 
       { window.draw(*chsprite[i%28]);
        i++;}

    window.display();
    } 
         // Clean up dynamic memory
    for (int i = 0; i < 28; ++i)
{
    delete chsprite[i];
    delete chtexture[i];
}
    return 0;
}