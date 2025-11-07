//#include <SFML/Graphics.hpp>
//#include <SFML/Audio.hpp>
//#include <sstream>
//#include <ctime>
//#include <iostream>
//using namespace sf;
//
////============================================================
//// ENUMERACIONS I CONSTANTS GLOBALS
////============================================================
//const int NUM_CLOUDS = 3;
//const float timeBarStartWidth = 400;
//const float timeBarHeight = 80;
//const int NUM_BRANCHES = 6;
//enum class Side { LEFT, RIGHT, NONE };
//
////============================================================
//// ESTRUCTURA NPC (ABELLA / NÚVOLS)
////============================================================
//struct NPC {
//    Sprite sprite;
//    bool active;
//    float speed;
//    int maxHeight;
//    int maxSpeed;
//    int sentit; // 1 = dreta, -1 = esquerra
//    float posicioInicialX;
//
//    NPC(const Texture& texture, int maxHeight_, int maxSpeed_, int sentit_, float posicioInicialX_)
//        :sprite(texture), active(false), speed(0),
//        maxHeight(maxHeight_), maxSpeed(maxSpeed_),
//        sentit(sentit_), posicioInicialX(posicioInicialX_) {
//        sprite.setTexture(texture);
//    }
//};
//
////============================================================
//// PROTOTIPS DE FUNCIONS
////============================================================
//void updateNPC(NPC&, float);
//void updateBranches(Side[], int);
//void updateBranchSprites(Side[], Sprite[]);
//
////============================================================
//// FUNCIÓ PRINCIPAL DEL JOC
////============================================================
//int main()
//{
//    //--------------------------------------------------------
//    // CREACIÓ I ELEMENTS VISUALS
//    //--------------------------------------------------------
//    RenderWindow window(VideoMode({ 1920, 1080 }), "Timber!!!");
//
//    Texture textureBackground;
//    if (!textureBackground.loadFromFile("graphics/background.png")) {
//        std::cerr << "Error loading background.png" << std::endl;
//        return -1;
//    }
//    Sprite spriteBackground(textureBackground);
//
//    Texture texturePlayer;
//    if (!texturePlayer.loadFromFile("graphics/player.png")) {
//        std::cerr << "Error loading player.png" << std::endl;
//        return -1;
//    }
//    Sprite spritePlayer(texturePlayer);
//    spritePlayer.setPosition({ 675, 660 });
//
//    Texture textureTree1;
//    if (!textureTree1.loadFromFile("graphics/tree.png")) {
//        std::cerr << "Error loading tree.png" << std::endl;
//        return -1;
//    }
//    Sprite spriteTree1(textureTree1);
//    spriteTree1.setPosition({ 810, 0 });
//
//    //--------------------------------------------------------
//    // ENTITATS MÒBILS (ABELLA + NÚVOLS)
//    //--------------------------------------------------------
//    Texture textureBee;
//    if (!textureBee.loadFromFile("graphics/bee.png")) {
//        std::cerr << "Error loading bee.png" << std::endl;
//        return -1;
//    }
//
//    Texture textureCloud;
//    if (!textureCloud.loadFromFile("graphics/cloud.png")) {
//        std::cerr << "Error loading cloud.png" << std::endl;
//        return -1;
//    }
//
//    NPC bee(textureBee, 500, 400, -1, 2000);
//    NPC clouds[NUM_CLOUDS] = {
//        NPC(textureCloud, 100, 200, 1, -200),
//        NPC(textureCloud, 250, 200, 1, -200),
//        NPC(textureCloud, 500, 200, 1, -200)
//    };
//
//    //--------------------------------------------------------
//    // CONFIGURACIÓ BRANQUES
//    //--------------------------------------------------------
//    Texture textureBranch;
//    if (!textureBranch.loadFromFile("graphics/branch.png")) {
//        std::cerr << "Error loading branch.png" << std::endl;
//        return -1;
//    }
//
//    Side branchPositions[NUM_BRANCHES];
//    Sprite branches[NUM_BRANCHES];
//
//    for (int i = 0; i < NUM_BRANCHES; i++) {
//        branches[i].setTexture(textureBranch);
//        branches[i].setPosition({ -2000, -2000 });
//        branches[i].setOrigin({ 220, 20 });
//        branchPositions[i] = Side::NONE;
//    }
//
//    //--------------------------------------------------------
//    // PREPARAR DESTRAL
//    //--------------------------------------------------------
//    Texture textureAxe;
//    if (!textureAxe.loadFromFile("graphics/axe.png")) {
//        std::cerr << "Error loading axe.png" << std::endl;
//        return -1;
//    }
//    Sprite spriteAxe(textureAxe);
//    spriteAxe.setPosition({ -2000, -2000 });
//    const float AXE_POSITION_LEFT = 700;
//    const float AXE_POSITION_RIGHT = 1075;
//
//    //--------------------------------------------------------
//    // PREPARAR TRONC VOLADOR
//    //--------------------------------------------------------
//    Texture textureLog;
//    if (!textureLog.loadFromFile("graphics/log.png")) {
//        std::cerr << "Error loading log.png" << std::endl;
//        return -1;
//    }
//    Sprite spriteLog(textureLog);
//    spriteLog.setPosition({ -2000, -2000 });
//    bool logActive = false;
//    float logSpeedX = 1000;
//    float logSpeedY = -1500;
//
//    //--------------------------------------------------------
//    // PREPARAR LÀPIDA
//    //--------------------------------------------------------
//    Texture textureRIP;
//    if (!textureRIP.loadFromFile("graphics/rip.png")) {
//        std::cerr << "Error loading rip.png" << std::endl;
//        return -1;
//    }
//    Sprite spriteRIP(textureRIP);
//    spriteRIP.setPosition({ 675, 2000 });
//
//    //--------------------------------------------------------
//    // TEXTOS I MARCADOR
//    //--------------------------------------------------------
//    Font font;
//    if (!font.openFromFile("fonts/KOMIKAP_.ttf")) {
//        std::cerr << "Error loading font" << std::endl;
//        return -1;
//    }
//
//    Text messageText(font);
//    messageText.setString("Press Enter to start!");
//    messageText.setCharacterSize(75);
//    messageText.setFillColor(Color::White);
//    FloatRect textRect = messageText.getLocalBounds();
//    messageText.setOrigin({ textRect.size.x / 2.0f, textRect.size.y / 2.0f });
//    messageText.setPosition({ 1920.0f / 2.0f, 1080.0f / 2.0f });
//
//    Text scoreText(font);
//    scoreText.setCharacterSize(100);
//    scoreText.setString("Score = 0");
//    scoreText.setFillColor(Color::White);
//    scoreText.setPosition({ 20, 20 });
//
//    //--------------------------------------------------------
//    // SONS
//    //--------------------------------------------------------
//    SoundBuffer chopBuffer;
//    if (!chopBuffer.loadFromFile("sound/chop.wav")) {
//        std::cerr << "Error loading chop.wav" << std::endl;
//    }
//    Sound chop(chopBuffer);
//
//    SoundBuffer deathBuffer;
//    if (!deathBuffer.loadFromFile("sound/death.wav")) {
//        std::cerr << "Error loading death.wav" << std::endl;
//    }
//    Sound death(deathBuffer);
//
//    SoundBuffer ootBuffer;
//    if (!ootBuffer.loadFromFile("sound/out_of_time.wav")) {
//        std::cerr << "Error loading out_of_time.wav" << std::endl;
//    }
//    Sound outOfTime(ootBuffer);
//
//    //--------------------------------------------------------
//    // VARIABLES DE JOC
//    //--------------------------------------------------------
//    bool paused = true;
//    Clock clock;
//    int score = 0;
//    float timeRemaining = 6.0f;
//    Side playerSide = Side::LEFT;
//    bool acceptInput = false;
//
//    RectangleShape timeBar({ timeBarStartWidth, timeBarHeight });
//    timeBar.setFillColor(Color::Red);
//    timeBar.setPosition({ 1920.0f / 2.0f - timeBarStartWidth / 2.0f, 980 });
//    float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;
//
//    //--------------------------------------------------------
//    // BUCLE PRINCIPAL DEL JOC
//    //--------------------------------------------------------
//    while (window.isOpen()) {
//
//        //--------------------------------------------------------
//        // GESTIÓ D'ESDEVENIMENTS
//        //--------------------------------------------------------
//        while (const std::optional event = window.pollEvent()) {
//            if (event->is<Event::Closed>()) {
//                window.close();
//            }
//
//            if (const auto* keyEvent = event->getIf<Event::KeyPressed>()) {
//                if (keyEvent->code == Keyboard::Key::Escape) {
//                    window.close();
//                }
//
//                if (keyEvent->code == Keyboard::Key::Enter) {
//                    paused = false;
//                    score = 0;
//                    timeRemaining = 6;
//
//                    for (int i = 0; i < NUM_BRANCHES; i++)
//                        branchPositions[i] = Side::NONE;
//
//                    spriteRIP.setPosition({ 675, 2000 });
//                    spritePlayer.setPosition({ 675, 660 });
//                    acceptInput = true;
//                }
//            }
//        }
//
//        // Detectar tecles per picar
//        if (acceptInput) {
//            if (Keyboard::isKeyPressed(Keyboard::Key::Right)) {
//                playerSide = Side::RIGHT;
//                score++;
//                timeRemaining += (2.0f / score) + 0.15f;
//
//                spriteAxe.setPosition({ AXE_POSITION_RIGHT, spriteAxe.getPosition().y });
//                spritePlayer.setPosition({ 1200, 720 });
//
//                updateBranches(branchPositions, score);
//
//                spriteLog.setPosition({ 810, 720 });
//                logSpeedX = 5000;
//                logActive = true;
//
//                acceptInput = false;
//                chop.play();
//            }
//
//            if (Keyboard::isKeyPressed(Keyboard::Key::Left)) {
//                playerSide = Side::LEFT;
//                score++;
//                timeRemaining += (2.0f / score) + 0.15f;
//
//                spriteAxe.setPosition({ AXE_POSITION_LEFT, spriteAxe.getPosition().y });
//                spritePlayer.setPosition({ 675, 660 });
//
//                updateBranches(branchPositions, score);
//
//                spriteLog.setPosition({ 810, 720 });
//                logSpeedX = -5000;
//                logActive = true;
//
//                acceptInput = false;
//                chop.play();
//            }
//        }
//
//        //--------------------------------------------------------
//        // ACTUALITZACIÓ DEL JOC
//        //--------------------------------------------------------
//        if (!paused) {
//            Time dt = clock.restart();
//
//            // Actualitzar temps
//            timeRemaining -= dt.asSeconds();
//            timeBar.setSize({ timeBarWidthPerSecond * timeRemaining, timeBarHeight });
//
//            if (timeRemaining <= 0) {
//                paused = true;
//                messageText.setString("Out of time!");
//                FloatRect textRect = messageText.getLocalBounds();
//                messageText.setOrigin({ textRect.size.x / 2.0f, textRect.size.y / 2.0f });
//                outOfTime.play();
//            }
//
//            // Entitats mòbils
//            updateNPC(clouds[0], dt.asSeconds());
//            updateNPC(clouds[1], dt.asSeconds());
//            updateNPC(clouds[2], dt.asSeconds());
//            updateNPC(bee, dt.asSeconds());
//
//            // Actualitzar branques
//            updateBranchSprites(branchPositions, branches);
//
//            // Mort del jugador
//            if (branchPositions[5] == playerSide) {
//                paused = true;
//                acceptInput = false;
//                spriteRIP.setPosition({ 525, 760 });
//                spritePlayer.setPosition({ 2000, 660 });
//                messageText.setString("SQUISHED!");
//                FloatRect textRect = messageText.getLocalBounds();
//                messageText.setOrigin({ textRect.size.x / 2.0f, textRect.size.y / 2.0f });
//                death.play();
//            }
//
//            // Actualitzar marcador
//            std::stringstream ss;
//            ss << "Score = " << score;
//            scoreText.setString(ss.str());
//
//            // Gestionar tronc volador
//            if (logActive) {
//                spriteLog.setPosition({
//                    spriteLog.getPosition().x + logSpeedX * dt.asSeconds(),
//                    spriteLog.getPosition().y + logSpeedY * dt.asSeconds()
//                    });
//
//                if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().x > 2000) {
//                    logActive = false;
//                    spriteLog.setPosition({ 810, 720 });
//                }
//            }
//
//            // Reactivar input
//            if (!acceptInput) {
//                if (!Keyboard::isKeyPressed(Keyboard::Key::Right) &&
//                    !Keyboard::isKeyPressed(Keyboard::Key::Left)) {
//                    acceptInput = true;
//                    spriteAxe.setPosition({ -2000, -2000 });
//                }
//            }
//        }
//
//        //--------------------------------------------------------
//        // DIBUIX
//        //--------------------------------------------------------
//        window.clear();
//        window.draw(spriteBackground);
//
//        for (int i = 0; i < NUM_CLOUDS; i++) {
//            window.draw(clouds[i].sprite);
//        }
//
//        for (int i = 0; i < NUM_BRANCHES; i++) {
//            window.draw(branches[i]);
//        }
//
//        window.draw(spriteTree1);
//        window.draw(spritePlayer);
//        window.draw(bee.sprite);
//        window.draw(scoreText);
//        window.draw(timeBar);
//        window.draw(spriteAxe);
//        window.draw(spriteLog);
//        window.draw(spriteRIP);
//
//        if (paused) window.draw(messageText);
//
//        window.display();
//    }
//
//    return 0;
//}
//
////============================================================
//// IMPLEMENTACIÓ DE FUNCIONS
////============================================================
//
//void updateNPC(NPC& npc, float dt) {
//    if (!npc.active) {
//        npc.speed = (rand() % npc.maxSpeed) * npc.sentit;
//        float height = rand() % npc.maxHeight;
//        npc.sprite.setPosition({ npc.posicioInicialX, height });
//        npc.active = true;
//    }
//    else {
//        npc.sprite.setPosition({
//            npc.sprite.getPosition().x + npc.speed * dt,
//            npc.sprite.getPosition().y
//            });
//
//        if (npc.sprite.getPosition().x < -200 || npc.sprite.getPosition().x > 2000)
//            npc.active = false;
//    }
//}
//
//void updateBranches(Side branchPositions[], int seed) {
//    for (int j = NUM_BRANCHES - 1; j > 0; j--)
//        branchPositions[j] = branchPositions[j - 1];
//
//    srand((int)time(0) + seed);
//    int r = rand() % 5;
//
//    switch (r) {
//    case 0:
//        branchPositions[0] = Side::LEFT;
//        break;
//    case 1:
//        branchPositions[0] = Side::RIGHT;
//        break;
//    default:
//        branchPositions[0] = Side::NONE;
//        break;
//    }
//}
//
//void updateBranchSprites(Side branchPositions[], Sprite branches[]) {
//    for (int i = 0; i < NUM_BRANCHES; i++) {
//        float height = i * 150;
//
//        if (branchPositions[i] == Side::LEFT) {
//            branches[i].setPosition({ 610, height });
//            branches[i].setRotation(sf::degrees(180.0f));
//        }
//        else if (branchPositions[i] == Side::RIGHT) {
//            branches[i].setPosition({ 1330, height });
//            branches[i].setRotation(sf::degrees(0.0f));
//        }
//        else {
//            branches[i].setPosition({ -2000, height });
//        }
//    }
//}