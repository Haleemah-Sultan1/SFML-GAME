#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <SFML/System/Clock.hpp>
#include <vector>
#include <string>
#include <ctime>  
#include<cmath>
#include <optional>
#include <algorithm> // for std::clamp 

using namespace std;
using namespace sf;

// global variables
int life = 3;
int score = 0; 

// POWERUP 
std::vector<sf::Sprite> activePowerups;
std::vector<int> activePowerupTypes; 
std::vector<int> activePowerupFrame;     // current frame
std::vector<float> activePowerupTimer;  // animation timer
std::vector<bool>activePowerupPostion_up;
sf::Texture pwrTextures[15]; 
float pwrspawnTimer = 0.0f;

void handlePowerups(sf::RenderWindow& window, float deltaTime, float gameSpeed, sf::Sprite* playerSprite, bool playerup ) {
    // 1. Spawning Logic
    pwrspawnTimer += deltaTime;
    if (pwrspawnTimer >= 4.0f) {
        pwrspawnTimer = 0.0f;

        int type = rand() % 3; // type tells which sprite 
        
        sf::Sprite newPowerup(pwrTextures[type * 5]);

        int c =rand() % 2;
        float randomY = 470.f + 100*(c); // this will keep them on either up or down 
        
        if(c==0)
           activePowerupPostion_up.push_back(true);
        else 
         activePowerupPostion_up.push_back(false);

        if(type==2){
         newPowerup.setScale({0.6f, 0.6f}); 
        newPowerup.setPosition({-100.f, randomY-10});
        }
         else
        {newPowerup.setScale({0.2f, 0.2f}); 
         newPowerup.setPosition({-100.f, randomY});
         }
        
        activePowerups.push_back(newPowerup);
        activePowerupTypes.push_back(type); 
        activePowerupFrame.push_back(0);
        activePowerupTimer.push_back(0.f);
    } 

    // Update, Collision, and Draw Logic
    for (int i = 0; i < (int)activePowerups.size(); ) {

        activePowerups[i].move({gameSpeed * deltaTime, 0.f});

         // Animate sprite
       activePowerupTimer[i] += deltaTime;
       if (activePowerupTimer[i] >= 0.12f) {
        activePowerupTimer[i] = 0.f;
        activePowerupFrame[i] = (activePowerupFrame[i] + 1) % 5;  // 4 frames per powerup

        int type = activePowerupTypes[i];
        int textureIndex = type * 5 + activePowerupFrame[i];

        activePowerups[i].setTexture(pwrTextures[textureIndex]);
    }

        bool collected = false;

        if (playerSprite != nullptr) {
           

        bool sameLane = (playerup == activePowerupPostion_up[i] )? true: false;
        // int diff_x = abs(activePowerups[i].getPosition().x - playerSprite->getPosition().x);
        // bool hitx = diff_x<50.f;
        float playerCenterX =
        playerSprite->getPosition().x +
        playerSprite->getGlobalBounds().size.x * 0.5f;

        float powerupCenterX =
          activePowerups[i].getPosition().x +
          activePowerups[i].getGlobalBounds().size.x * 0.5f;

        float dx = std::abs(powerupCenterX - playerCenterX +360);

        bool hitx = dx < 50.f;

             if ( sameLane && hitx) {
             collected = true;
              int type = activePowerupTypes[i];
              if (type == 0) score += 20;
             else if (type == 1) score += 10;
            else if (type == 2) {life += 1;if (life>3) life = 3;}
            }

        }

        if (collected || activePowerups[i].getPosition().x > 1300.f) {
            activePowerups.erase(activePowerups.begin() + i);
            activePowerupTypes.erase(activePowerupTypes.begin() + i);
            activePowerupFrame.erase(activePowerupFrame.begin() + i);
            activePowerupTimer.erase(activePowerupTimer.begin() + i);
            activePowerupPostion_up.erase(activePowerupPostion_up.begin() + i);

        } else {
            window.draw(activePowerups[i]);
            i++;
        }
    }
}

//collision part 2
void checkCollisions(RenderWindow& window, sf::Sprite* player, bool isOnGround, float deltaTime, sf::Sprite& dog, bool& dogActive, float dogSpeed, sf::Sprite& fruit, bool& fruitActive, float fruitSpeed, sf::Sprite& bus, bool& busActive, float busSpeed, bool& somethingActive, bool& playerInvincible , float& invincibleTimer) {
    if (player == nullptr ) return;  // Only check collisions when player is on ground

     // DOG
        if (dogActive)
        {
             dog.move({dogSpeed * deltaTime, 0.f});
            if ((dog.getPosition().x >= 710.f &&dog.getPosition().x <=800.f )&& (dog.getPosition().y-130)  == player->getPosition().y)
            {
                
                if (!playerInvincible) {
                life -= 1;
                playerInvincible = true;
                invincibleTimer = 0.f;
              }             
                dogActive = false;
                somethingActive = false;
            }
            else if( dog.getPosition().x >= 1280.f)
            {
                dogActive = false;
                somethingActive = false;
            }
            window.draw(dog);
        }

        // FRUIT
        if (fruitActive)
        {
            fruit.move({fruitSpeed * deltaTime, 0.f});
            if ((fruit.getPosition().x >= 545.f && fruit.getPosition().x <= 800.f)&&  player->getPosition().y == 450.f)
            {              
                fruitActive = false;
                somethingActive = false;
                if (!playerInvincible) {
                life -= 1;
                 playerInvincible = true;
                 invincibleTimer = 0.f;
                 }

            }
           else if (fruit.getPosition().x > 1280.f)
            {
                fruitActive = false;
                somethingActive = false;
            }
            window.draw(fruit);
        }

        // BUS
        if (busActive)
        {
            bus.move({busSpeed * deltaTime, 0.f});
            if ((bus.getPosition().x >= 390.f && bus.getPosition().x <= 820.f) && 350.f==player->getPosition().y)
            {
               if (!playerInvincible) {
                life -= 1;
                playerInvincible = true;
              invincibleTimer = 0.f;
              }             
                busActive = false;
                somethingActive = false;
            }
            else if (bus.getPosition().x > 1280.f)
            {
                busActive = false;
                somethingActive = false;
            }
            window.draw(bus);

        }
    

}

void menu(sf::RenderWindow& window, int& selectedPlayer)
{
    sf::Texture menuTexture;
    if (!menuTexture.loadFromFile("assets/menu.png")) return;

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
    
    p1Sprite.setPosition({410.f, 310.f});
    if (p1Tex.getSize().x > 0)
        p1Sprite.setScale({180.f / (float)p1Tex.getSize().x, 180.f / (float)p1Tex.getSize().y});
    
    p2Sprite.setPosition({690.f, 310.f});
    if (p2Tex.getSize().x > 0)
        p2Sprite.setScale({180.f / (float)p2Tex.getSize().x, 180.f / (float)p2Tex.getSize().y});

    credSprite.setPosition({750.f, 300.f}); 
    credSprite.setScale({1.2f, 1.2f});       

    sf::Color yellowishBrown(204, 153, 51, 220); 
    sf::Color darkBrown(60, 40, 20);            
    sf::Color instructionGrey(200, 200, 200);   

    sf::RectangleShape btnPlay({250.f, 50.f}), btnInst({250.f, 50.f}), btnCred({250.f, 50.f});
    btnPlay.setPosition({515.f, 300.f}); btnInst.setPosition({515.f, 370.f}); btnCred.setPosition({515.f, 440.f});
    
    auto setupBtn = [&](sf::RectangleShape& b) {
        b.setFillColor(yellowishBrown); b.setOutlineThickness(3); b.setOutlineColor(darkBrown);
    };
    setupBtn(btnPlay); setupBtn(btnInst); setupBtn(btnCred);

    sf::Text txtPlay(font, "PLAY", 25); txtPlay.setFillColor(darkBrown); txtPlay.setPosition({610.f, 310.f});
    sf::Text txtInst(font, "INSTRUCTIONS", 25); txtInst.setFillColor(darkBrown); txtInst.setPosition({545.f, 380.f});
    sf::Text txtCred(font, "CREDITS", 25); txtCred.setFillColor(darkBrown); txtCred.setPosition({590.f, 450.f});

    sf::RectangleShape p1Box({200.f, 200.f}), p2Box({200.f, 200.f});
    p1Box.setPosition({400.f, 300.f}); p2Box.setPosition({680.f, 300.f});
    p1Box.setFillColor(sf::Color(255, 255, 255, 30));
    p2Box.setFillColor(sf::Color(255, 255, 255, 30));
    p1Box.setOutlineThickness(5); p2Box.setOutlineThickness(5);

    int menuState = 0; 
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

        if (menuState == 0) { 
            window.draw(btnPlay); window.draw(txtPlay);
            window.draw(btnInst); window.draw(txtInst);
            window.draw(btnCred); window.draw(txtCred);
        }
        else if (menuState == 1) { 
            sf::RectangleShape overlay({1100.f, 500.f});
            overlay.setPosition({90.f, 150.f});
            overlay.setFillColor(sf::Color(0, 0, 0, 220)); 
            overlay.setOutlineThickness(3);
            overlay.setOutlineColor(yellowishBrown);
            window.draw(overlay);

            sf::Text instTitle(font, "Karachi Survival Guide:", 55);
            instTitle.setPosition({130.f, 180.f});
            instTitle.setFillColor(yellowishBrown);
            window.draw(instTitle);

            sf::Text instBody(font, "1. Keep your phone hidden like it's a state secret.\n2. Trust the driver... but also pray quietly.\n3. If someone says \"bhai tension nahi leni\" - tension is guaranteed.\n4. If a road is empty, something is wrong.\n5. If it rains, cancel your plans, your hopes, and your shoes.", 28);
            instBody.setPosition({130.f, 250.f});
            instBody.setFillColor(instructionGrey);
            window.draw(instBody);
           
            Text newBody(font, "1. Esc will end the game.\n2. Samosa and chai, are good for health, increases your score.\n3.Flag will reward you for your Patriotism as your new life.\n4. Press D to start again.", 28);
            newBody.setPosition({130.f, 430.f});
            newBody.setFillColor(instructionGrey);
            window.draw(newBody);
            

            sf::Text backMsg(font, "Press ESC to go back", 20);
            backMsg.setPosition({550.f, 600.f});
            window.draw(backMsg);
        }
        else if (menuState == 2) { 
            sf::Text credHeader(font, "CREDITS:\nDeveloped by:", 50);
            credHeader.setPosition({150.f, 350.f}); 
            credHeader.setFillColor(yellowishBrown);
            window.draw(credHeader);

            sf::Text names(font, "Haleema Zuhaib\nSeerat Fatima\nHaiqa Shahid", 50);
            names.setPosition({450.f, 455.f}); 
            names.setFillColor(sf::Color::White);
            window.draw(names);

            window.draw(credSprite); 
        }
        else if (menuState == 3) { 
            sf::Text prompt(font, "SELECT PLAYER & PRESS ENTER", 34);
            Text name_boy(font, "Ali",30);   
            Text name_girl(font, "Ayesha",30);
            Color darkBrown(60, 40, 20, 180);
                      
            
            prompt.setFillColor(Color(60, 40, 2, 180));
            prompt.setPosition({450.f, 150.f+2});
            window.draw(prompt);
            prompt.setPosition({450.f, 150.f}); prompt.setFillColor(yellowishBrown);
            window.draw(prompt);

            window.draw(p1Box); window.draw(p2Box);
            window.draw(p1Sprite); window.draw(p2Sprite); 
            // ---- NAME SHADOWS ----
            name_boy.setFillColor(darkBrown);
            name_boy.setPosition({472.f, 502.f});
            window.draw(name_boy);

            name_girl.setFillColor(darkBrown);
            name_girl.setPosition({752.f, 502.f});
            window.draw(name_girl);

            // ---- MAIN NAMES ----
            name_boy.setFillColor(Color::White);
            name_boy.setPosition({470.f, 500.f});
            window.draw(name_boy);

            name_girl.setFillColor(Color::White);
            name_girl.setPosition({750.f, 500.f});
            window.draw(name_girl);
        }
        window.display();
    }
}

void spritesfix(Texture* chtexture[], Sprite* chsprite[], RenderWindow& window, string character)
{
    for (int i = 0; i < 86; i++)
    {
        string filename = "assets/"+ character +"/";

        if (i < 10) filename += "0";
        filename += to_string(i) + "_"+character +"sheet2.png";

        if (!chtexture[i]->loadFromFile(filename))
            continue;

        chsprite[i] = new Sprite(*chtexture[i]);
        chsprite[i]->setScale({-0.5f, 0.5f});
        chsprite[i]->setPosition({1000.f, 350.f});
    }
    int c= 5; int k=0;int b =3;
    for (int i = 30; i <= 42; i++)
    {
        if(chsprite[i]) chsprite[i]->setPosition({1000.f + (float)b * k, 350.f - (float)c * k});
        k++;
    }
    for (int i = 43; i <= 55; i++)
    {
        k--;
        if(chsprite[i]) chsprite[i]->setPosition({1000.f + (float)b * k, 350.f - (float)c * k});
    }
}

//scoreboard
void scoreboard(sf::RenderWindow& window, int finalDistance,int score) {
    sf::Font font;
    if (!font.openFromFile("assets/font.ttf")) return;

    sf::Text gameOverText(font, "GAME OVER", 80);
    gameOverText.setFillColor(sf::Color::Red);
    
    sf::Text finalScoreText(font, "Distance Covered: " + std::to_string(finalDistance) + "m", 40);
    sf::Text ScoreText(font, "SCORE: " + std::to_string(score), 40);
    ScoreText.setFillColor(sf::Color::White);

    finalScoreText.setFillColor(sf::Color::White);

  sf::Text restartText(font, "Press Enter to restart", 25);
    restartText.setFillColor(sf::Color(200, 200, 200));

    sf::Text closeText(font, "Press ESC to Exit", 20);
    closeText.setFillColor(sf::Color(200, 200, 200));

  

    // Center the text
    auto centerText = [&](sf::Text& text, float yOffset) {
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin({textRect.size.x / 2.0f, textRect.size.y / 2.0f});
        text.setPosition({window.getSize().x / 2.0f, (window.getSize().y / 2.0f) + yOffset});
    };

    centerText(gameOverText, -100.f);
    centerText(finalScoreText, 20.f);
    centerText(closeText, 150.f);

    centerText(gameOverText, -100.f);
    centerText(finalScoreText, 15.f);
    centerText(ScoreText, 70.f);  
    centerText(closeText, 150.f);
    centerText(restartText, 130.f);

    sf::RectangleShape box(sf::Vector2f({450.f, 300.f})); // width=200, height=100
    box.setPosition({430.f, 230.f});

    // Set fill color with transparency (alpha)
    box.setFillColor(sf::Color(139, 69, 19, 180)); // Red, 50% transparent
    // box.setOutlineColor(sf::Color(255, 255, 255, 200)); // white outline, mostly opaque 
    // restartText.setPosition({450.f,400.f});


        window.draw(box);
        window.draw(gameOverText);
        window.draw(finalScoreText);
        window.draw(restartText);
        window.draw(closeText);
        window.draw(ScoreText);
    
}
int main()
{
      Music music;
     if (!music.openFromFile("assets/music.ogg"))
      return -1;
     music.setLooping(true); 
     music.setVolume(50.f);
      music.play();
    srand(static_cast<unsigned>(time(0))); 
    sf::RenderWindow window(sf::VideoMode({1280, 720}), "Karachi Survival - SFML 3");
    window.setFramerateLimit(60);

    int playerChoice = 1; 
    menu(window, playerChoice);

    if (!window.isOpen()) return 0;

    // UI Setup
    sf::Font gameFont;
    if (!gameFont.openFromFile("assets/font.ttf")) return -1;

    sf::Text scoreText(gameFont, "", 30);
    scoreText.setFillColor(sf::Color(204, 153, 51)); // Yellowish Brown
    scoreText.setOutlineThickness(2);
    scoreText.setOutlineColor(sf::Color::Black);
    scoreText.setPosition({1000.f, 20.f});

    sf::Text lifeText(gameFont, "", 30);
    lifeText.setFillColor(sf::Color::Red);
    lifeText.setOutlineThickness(2);
    lifeText.setOutlineColor(sf::Color::Black);
    lifeText.setPosition({1000.f, 60.f});

    //Asset Loading
    for(int i=0;i<5;i++)
    if (!pwrTextures[i].loadFromFile("assets/samosa/0"+ to_string(i) +"_samosa.png")) std::cout << "Samosa missing\n";
    for(int i = 5;i<10;i++)
    if (!pwrTextures[i].loadFromFile("assets/tea/0"+ to_string(i-5) +"_tea.png"))    std::cout << "Tea missing\n";
    for(int i=10;i<15;i++)
    if (!pwrTextures[i].loadFromFile("assets/flag/0"+ to_string(i-10) +"_flag.png"))   std::cout << "Flag missing\n";

    sf::Texture bgTexture;
    if (!bgTexture.loadFromFile("assets/gpt5.png")) return -1;
    sf::Sprite bg1(bgTexture), bg2(bgTexture);
    float bgWidth = (float)bgTexture.getSize().x;
    bg2.setPosition({-bgWidth, 0.f}); 

    float speed = 180.0f;
    Clock clock;  

    std::vector<sf::Texture> dogFrames;
    int totalFrames = 24;
    float dogSpeed = 420.0f;

    for (int i = 1; i <= totalFrames; i++)
    {
        sf::Texture texture;
        if (!texture.loadFromFile("assets/dog/dog" + std::to_string(i) + ".png"))
            return -1;
        dogFrames.push_back(texture);
    }
    sf::Sprite dog(dogFrames[0]);
    dog.setScale({0.9f, 0.9f});
    dog.setPosition({90.f, 480.f});

    int frame = 0;
    float animationSpeed = 0.05f;    
    float timer = 0.f;

    Texture* chtexture[86];
    Sprite* chsprite[86];
    for (int i = 0; i < 86; i++)
    {
        chtexture[i] = new Texture; 
        chsprite[i] = nullptr;
    }

    string character = (playerChoice == 1) ? "boy" : "girl";
    spritesfix(chtexture, chsprite, window, character);
 
    float distanceMeters = 0.f;// for player covered distance

    // player distance text
    Font font;
    if (!font.openFromFile("assets/distancetext.ttf"))
        return -1;
    Text distancetext(font, "Distance: 0m", 36);
    distancetext.setFillColor(Color(255, 204, 0));
    distancetext.setPosition({30.f,10.f});

    bool jump = false;
    bool isOnGround = true;
    int i_run = 0;
    int j = 0;
    Clock playeranimeclock;
    float frameTimeplayer = 0.0175f;
    bool dead = false;
    int d = 0;
    bool playerup = true;

    bool somethingActive = false;

    bool dogActive = false;
    bool fruitActive = false;
    bool busActive = false;

    float spawnTimer = 0.f;
    float spawnDelay = 2.2f; // wait before trying to spawn again
    bool exit=false;

    //fruit truck 
    sf::Texture fruitTexture; 
    if (!fruitTexture.loadFromFile("assets/fruit/fruit.png")) return -1; 
    sf::Sprite fruit(fruitTexture);
    float fruitSpeed = 250.0f;
    frame = 0; 
    float frameWidth = 119.f; // width of a single frame
    animationSpeed = 0.01f; // how fast it animates 
    fruit.setScale({0.25f, 0.25f});
    fruit.setPosition({90.0f, 470.f});
    
    //truck
    sf::Texture busTexture;
    if (!busTexture.loadFromFile("assets/bus.png")) return -1;
    sf::Sprite bus(busTexture);
    float busSpeed = 250.0f;
    bus.setScale({0.4f, 0.4f});
    bus.setOrigin({0.f, bus.getGlobalBounds().size.y});
    bus.setPosition({-bus.getGlobalBounds().size.x, 520.f});

    Texture* hearttexture[4] = {nullptr, nullptr, nullptr, nullptr};
     Sprite*  heart[4]        = {nullptr, nullptr, nullptr, nullptr};
    for(int i=0;i<4;i++){
         hearttexture[i] = new Texture();  
    if(!hearttexture[i]->loadFromFile("assets/heart/"+ to_string(i)+"_heart.png")) return -1;
    heart[i] = new Sprite(*hearttexture[i]);
    heart[i]->setScale({0.5f,0.5f});
    heart[i]->setPosition({960.f,50.f});}

    //HALEEEMA LAST CDEEEE

    float dogSpeedBase   = 420.0f;
    float fruitSpeedBase = 250.0f;
    float busSpeedBase   = 250.0f;

    // actual speeds used
     dogSpeed   = dogSpeedBase;
     fruitSpeed = fruitSpeedBase;
     busSpeed   = busSpeedBase;
     int lives=3;

     bool playerInvincible = false;
     float invincibleTimer = 0.f;
     float invincibleDuration = 1.5f; // seconds

     bool blinkVisible = true;
     float blinkTimer = 0.f;
     float blinkInterval = 0.1f; // how fast it blinks

 

    while (window.isOpen())
    {
        
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
                window.close();

            if (const auto* key = event->getIf<sf::Event::KeyPressed>())
            {
                if (key->code == sf::Keyboard::Key::Escape) window.close(); 
                 if (key->code == sf::Keyboard::Key::Enter && dead) {
                     dead = false; life =3;
                     distanceMeters =0; score = 0;
                 }
                     

                if (key->code == sf::Keyboard::Key::Space && isOnGround && !dead) {
                    isOnGround = false; jump = true; j = 0;
                }
                if (key->code == sf::Keyboard::Key::D && isOnGround) dead = true;

                if (key->code == sf::Keyboard::Key::Down && !dead)
                {
                    playerup = false;
                    for (int i = 0; i < 86; i++)
                        if(chsprite[i] && chsprite[i]->getPosition().y <= 350.f && chsprite[i]->getPosition().y >= 290.f)
                            chsprite[i]->setPosition({chsprite[i]->getPosition().x, chsprite[i]->getPosition().y+100.f});
                }
                if (key->code == sf::Keyboard::Key::Up && !dead)
                {
                    playerup = true;
                    for (int i = 0; i < 86; i++)
                        if(chsprite[i] && chsprite[i]->getPosition().y <= 450.f && chsprite[i]->getPosition().y >= 390.f)
                            chsprite[i]->setPosition({chsprite[i]->getPosition().x,chsprite[i]->getPosition().y-100.f});
                }
            }
        }

        float deltaTime = clock.restart().asSeconds();
 
         if (playerInvincible) {
         invincibleTimer += deltaTime;
           blinkTimer += deltaTime;

           if (blinkTimer >= blinkInterval) {
           blinkTimer = 0.f;
           blinkVisible = !blinkVisible; // toggle visibility
         }

          if (invincibleTimer >= invincibleDuration) {
           playerInvincible = false;
            blinkVisible = true; // ensure visible at end
           }
            }

       if(!dead){
        bg1.move({speed * deltaTime, 0.f});
        bg2.move({speed * deltaTime, 0.f});
     // distance counter 
        distanceMeters += speed * deltaTime / 10.f; // adjust scale
    }

        if (bg1.getPosition().x >= bgWidth)
            bg1.setPosition({bg2.getPosition().x - bgWidth, 0.f});
        if (bg2.getPosition().x >= bgWidth)
            bg2.setPosition({bg1.getPosition().x - bgWidth, 0.f});
        
        timer += deltaTime;
        if (timer >= animationSpeed)
        {
            timer = 0.f;
            frame = (frame + 1) % totalFrames;
            dog.setTexture(dogFrames[frame]); 
        }
        
        if(j > 28) {
            jump = false; j = 0; isOnGround = true;
        }

        window.clear();
        window.draw(bg1);
        window.draw(bg2);


        // ---- Difficulty scaling based on distance ----
float difficultyMultiplier = 1.0f;

// every 200 meters → +10% speed
difficultyMultiplier += (distanceMeters / 200.f) * 0.10f;

// cap difficulty so it doesn’t become insane
difficultyMultiplier = std::clamp(difficultyMultiplier, 1.0f, 2.2f);

// apply scaled speeds
dogSpeed   = dogSpeedBase   * difficultyMultiplier;
fruitSpeed = fruitSpeedBase * difficultyMultiplier;
busSpeed   = busSpeedBase   * difficultyMultiplier;


        distancetext.setString("Distance: " + std::to_string((int)distanceMeters) + "m");
        // Shadow
        distancetext.setFillColor(Color(60, 40, 2, 180));
        distancetext.setPosition({30.f + 2, 10.f + 2});
        distancetext.setOutlineThickness(0.5);
        window.draw(distancetext);

        // Main text
        distancetext.setFillColor(Color(255, 204, 0));
        distancetext.setPosition({30.f, 10.f});
        window.draw(distancetext);

        // Update UI Text Strings
        scoreText.setString("Score: " + std::to_string(score));
        // lifeText.setString("Lives: " + std::to_string(life));

        // Determine Active Player Sprite
        sf::Sprite* currentActiveSprite = nullptr;
        
        if(dead) {
            if (playeranimeclock.getElapsedTime().asSeconds() >= frameTimeplayer && d < 29) {
                d++; playeranimeclock.restart();
            }
            if(d < 29 && chsprite[56+d]) currentActiveSprite = chsprite[56+d]; 
            else if (chsprite[85]) currentActiveSprite = chsprite[85];
        }
        else if(jump) { 
            if (playeranimeclock.getElapsedTime().asSeconds() >= frameTimeplayer) {
                j++; playeranimeclock.restart();
            }
            if (j < 29 && chsprite[27+j]) currentActiveSprite = chsprite[27+j];
        }
        else { 
            // Running animation
            i_run = (i_run + 1) % 28;
            if (chsprite[i_run]) currentActiveSprite = chsprite[i_run];
        }

        if(life>=3) window.draw(*heart[0]);
       else if(life==2) window.draw(*heart[1]);
        else if(life==1) window.draw(*heart[2]);
        else window.draw(*heart[3]);


        spawnTimer += deltaTime;

if (spawnTimer >= spawnDelay)
{
    spawnTimer = 0.f;

    int r = rand() % 4;

    if (r == 0 && !dogActive)
    {
        dogActive = true;
       dog.setPosition({-dog.getGlobalBounds().size.x, 480.f + 100*(rand()%2)});
    }
    else if (r == 1 && !fruitActive)
    {
        fruitActive = true;
        fruit.setPosition({-fruit.getGlobalBounds().size.x, 400.f});
    }
    else if (r == 2 && !busActive && distanceMeters>=100.f)
    {
        busActive = true;
        bus.setPosition({-bus.getGlobalBounds().size.x, 350.f});
    }
    else 
     {
        dogActive = true;
        dog.setPosition({-dog.getGlobalBounds().size.x, 480.f});
    }
}
if (spawnTimer >= spawnDelay &&(distanceMeters>=200) )
{
    spawnTimer = 0.f;

    int r = rand() % 4;

    if (r == 0 && !dogActive)
    {
        dogActive = true;
       dog.setPosition({-dog.getGlobalBounds().size.x, 480.f + 100*(rand()%2)});
    }
    else if (r == 1 && !fruitActive)
    {
        fruitActive = true;
        fruit.setPosition({-fruit.getGlobalBounds().size.x, 400.f});
    }
    else if (r == 2 && !busActive && distanceMeters>=100.f)
    {
        busActive = true;
        bus.setPosition({-bus.getGlobalBounds().size.x, 350.f});
    }
    else 
     {
        dogActive = true;
        dog.setPosition({-dog.getGlobalBounds().size.x, 480.f});
    }
}

        // Draw Player and Handle Powerups
        if (currentActiveSprite && !dead ) {
            if (!playerInvincible || blinkVisible) {
             window.draw(*currentActiveSprite);}

            
            handlePowerups(window, deltaTime, speed, currentActiveSprite, playerup);
            checkCollisions(window,currentActiveSprite, isOnGround,deltaTime, dog, dogActive, dogSpeed, fruit, fruitActive, fruitSpeed, bus, busActive, busSpeed, somethingActive, playerInvincible, invincibleTimer);  
        }
        else if(currentActiveSprite) window.draw(*currentActiveSprite);

        window.draw(scoreText);
        window.draw(lifeText);

       if (life==0 || dead ){ 
        dead = true;
        scoreboard(window, distanceMeters, score );}
        window.display();
    } 

    for (int k = 0; k < 86; ++k)
    {
        delete chsprite[k];
        delete chtexture[k];
    }
    for (int i = 0; i < 4; i++)
{
    delete heart[i];
    delete hearttexture[i];
}

    return 0;
}
//~/Desktop/SFMLPROJECT/SFML-GAME 