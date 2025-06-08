#include "gameRender.h"

GameRender::GameRender() : window{VideoMode(windowXStartingSize, windowXStartingSize / aspectRatio), "MyChess"},
                           gameState{make_shared<GameState>(GameState{})}, gui{window}{
    Image icon;
    if (!icon.loadFromFile("textures/icon.png")){
        throw;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    textures["board"] = vector<Texture>{1};
    textures["figures"] = vector<Texture>{};
    textures["shapes"] = vector<Texture>{5};
    sprites["board"] = vector<Sprite>{1};
    sprites["figures"] = vector<Sprite>{};
    sprites["shapes"] = vector<Sprite>{};
    cout << "Starting game render..." << endl;
    squareSize = cast<float>(window.getSize().y) / 8;
    load_textures();
    load_font();
}

GameRender::~GameRender(){
    clockFlag = false;
    for (pair<const string, shared_ptr<thread>> &pair: threads){
        shared_ptr<thread> &thread = pair.second;
        if (thread && thread->joinable()){
            thread->join();
        }
    }
    cout << "Ended game render" << endl;
}

void GameRender::onResize_change_position(const Vector2u &newWinSize, const Vector2u &prevWinSize){
    float dX = cast<float>(newWinSize.x) / prevWinSize.x;
    float dY = cast<float>(newWinSize.y) / prevWinSize.y;
    Sprite &board = sprites["board"][0];

    board.setPosition(board.getPosition().x * dX, board.getPosition().y * dY);

    for (Sprite &sprite: sprites["figures"]){
        sprite.setPosition(sprite.getPosition().x * dX, sprite.getPosition().y * dY);
    }
    for (Sprite &sprite: sprites["shapes"]){
        sprite.setPosition(sprite.getPosition().x * dX, sprite.getPosition().y * dY);
    }
    if (sprites.contains("result")){
        lock_guard<mutex> lockR(resultMutex);
        for (Sprite &sprite: sprites["result"]){
            sprite.setPosition(sprite.getPosition().x * dX, sprite.getPosition().y * dY);
        }
    }
    for (pair<const string, Text> &pair: texts){
        Text &text = pair.second;
        text.setPosition(text.getPosition().x * dX, text.getPosition().y * dY);
    }
    TextArea::Ptr history = std::dynamic_pointer_cast<TextArea>(gui.get("history"));
    history->setPosition(history->getPosition().x * dX, history->getPosition().y * dY);
}

void GameRender::onResize_scale(const Vector2u &newWinSize, const Vector2u &prevWinSize){
    float dX = cast<float>(newWinSize.x) / prevWinSize.x;
    float dY = cast<float>(newWinSize.y) / prevWinSize.y;
    float scaleX = sprites["board"][0].getScale().x * dX;
    float scaleY = sprites["board"][0].getScale().y * dY;
    sprites["board"][0].setScale(scaleX, scaleY);

    if (!sprites["figures"].empty()){
        scaleX = sprites["figures"][0].getScale().x * dX;
        scaleY = sprites["figures"][0].getScale().y * dY;

        for (Sprite &sprite: sprites["figures"]){
            sprite.setScale(scaleX, scaleY);
        }
    }

    if (!sprites["shapes"].empty()){
        scaleX = sprites["shapes"][0].getScale().x * dX;
        scaleY = sprites["shapes"][0].getScale().y * dY;

        for (Sprite &sprite: sprites["shapes"]){
            sprite.setScale(scaleX, scaleY);
        }
    }

    lock_guard<mutex> lockS(resultMutex);
    if (!sprites["result"].empty()){
        scaleX = sprites["result"][0].getScale().x * dX;
        scaleY = sprites["result"][0].getScale().y * dY;

        for (Sprite &sprite: sprites["result"]){
            sprite.setScale(scaleX, scaleY);
        }
    }

    if (!texts.empty()){
        for (pair<const string, Text> &pair: texts){
            Text &text = pair.second;
            text.setScale(text.getScale().x * dX, text.getScale().y * dY);
        }
    }

    TextArea::Ptr history = std::dynamic_pointer_cast<TextArea>(gui.get("history"));
    Vector2f scale{history->getScale().x * dX, history->getScale().y * dY};
    history->setScale(scale);
}

void GameRender::resize_window(Vector2u &prevWinSize){
    Vector2u newWinSize = window.getSize();
    if (newWinSize == prevWinSize){
        return;
    }

    if (newWinSize.x != prevWinSize.x){
        newWinSize.y = newWinSize.x / aspectRatio;
    } else {
        newWinSize.x = newWinSize.y * aspectRatio;
    }

    squareSize = cast<float>(newWinSize.y) / 8;

    onResize_scale(newWinSize, prevWinSize);
    onResize_change_position(newWinSize, prevWinSize);

    window.setSize(newWinSize);
    window.setView(View(FloatRect(0, 0, newWinSize.x, newWinSize.y)));
    prevWinSize = newWinSize;
    cout << "Resized window size_: " << newWinSize.x << "x" << newWinSize.y << endl;
}

void GameRender::load_textures(){
    if (!textures["board"][0].loadFromFile("textures/board.png")){
        throw;
    }
    if (!textures["shapes"][0].loadFromFile("textures/circle.png")){
        throw;
    }
    if (!textures["shapes"][1].loadFromFile("textures/rectangle.png")){
        throw;
    }
    if (!textures["shapes"][2].loadFromFile("textures/border.png")){
        throw;
    }
    if (!textures["shapes"][3].loadFromFile("textures/rounded_rectangle.png")){
        throw;
    }
    if (!textures["shapes"][4].loadFromFile("textures/border_red.png")){
        throw;
    }
    textures["shapes"][0].setSmooth(true);
    textures["shapes"][2].setSmooth(true);
    textures["shapes"][3].setSmooth(true);
    textures["shapes"][4].setSmooth(true);

    Texture texture;
    int sX, sY;
    for (int i = 0; i < figureNum * textureSize; i += textureSize){
        sX = i % (textureSize * (figureNum / 2));
        sY = (i / (textureSize * (figureNum / 2))) * textureSize;
        cout << "Texture " << i / textureSize << " loaded" << endl;
        if (!texture.loadFromFile("textures/figures.png",
                                  IntRect(sX, sY, textureSize, textureSize))){
            throw;
        }
        texture.setSmooth(true);
        textures["figures"].push_back(texture);
    }
    sprites["board"][0].setTexture(textures["board"][0]);
    cout << "Successfully loaded all textures" << endl;
}

void GameRender::load_font(){
    if (!fonts["clock"].loadFromFile("fonts/calibri_bold.ttf")){
        throw;
    }
    cout << "Successfully loaded clock font" << endl;
}

void GameRender::set_position_from_square(Sprite &sprite, const Square &square){
    float posX = window.getSize().x * offsetRatio + square.column * squareSize;
    float posY = square.line * squareSize;
    for (Sprite &figSprite: sprites["figures"]){
        if (figSprite.getGlobalBounds().contains(posX, posY)){
            sprite.setPosition(figSprite.getPosition());
        }
    }
}

void GameRender::create_round_rect(){
    vector<Sprite> vec{};
    vec.emplace_back(textures["shapes"][3]);
    vec[0].setOrigin(vec[0].getLocalBounds().getSize() / 2.f);
    vec[0].setScale(1.3f * sprites["board"][0].getScale());
    vec[0].setPosition(window.getSize().x / 2, window.getSize().y / 2);
    sprites["result"] = vec;
}

void GameRender::create_result_string(const string &gameResult, bool turn){
    string color;
    if (turn){
        color = gameState->getWhoseTurn();
    } else {
        gameState->getWhoseTurn() == "white" ? color = "black" : color = "white";
    }
    float scale = cast<float>(window.getSize().x) / windowXStartingSize;
    string res = format("{} {}", color, gameResult);
    res[0] = toupper(res[0]);
    lock_guard<mutex> lockS(resultMutex);
    create_round_rect();
    texts["result"] = Text();
    Text &result = texts["result"];
    result.setFont(fonts["clock"]);
    result.setCharacterSize(70);
    result.setFillColor(Color::White);
    result.setString(res);
    result.setOrigin(result.getGlobalBounds().getSize() / 2.f +
                     result.getLocalBounds().getPosition());
    result.setScale(result.getScale() * scale);
    result.setPosition(cast<Vector2f>(window.getSize()) / 2.f);
    gameState->stopClock();
    clockFlag = false;
}

void GameRender::start_clock_thread(){
    threads["clock"] = make_shared<thread>([this]{
        while (clockFlag){
            if (gameState->getClockRes()){
                create_result_string("ran out of time", true);
                clockFlag = false;
            } else {
                std::this_thread::sleep_for(0.1s);
            }
        }
    });
}

void GameRender::create_sprite_out_of_figure(const shared_ptr<Figure> &figure){
    string color = figure->getColor();
    string name = figure->getName();
    Sprite sprite;
    if (color == "white"){
        if (name == "pawn"){
            sprite.setTexture(textures["figures"][0]);
        } else if (name == "knight"){
            sprite.setTexture(textures["figures"][1]);
        } else if (name == "bishop"){
            sprite.setTexture(textures["figures"][2]);
        } else if (name == "rook"){
            sprite.setTexture(textures["figures"][3]);
        } else if (name == "queen"){
            sprite.setTexture(textures["figures"][4]);
        } else if (name == "king"){
            sprite.setTexture(textures["figures"][5]);
        }
    } else {
        if (name == "pawn"){
            sprite.setTexture(textures["figures"][6]);
        } else if (name == "knight"){
            sprite.setTexture(textures["figures"][7]);
        } else if (name == "bishop"){
            sprite.setTexture(textures["figures"][8]);
        } else if (name == "rook"){
            sprite.setTexture(textures["figures"][9]);
        } else if (name == "queen"){
            sprite.setTexture(textures["figures"][10]);
        } else if (name == "king"){
            sprite.setTexture(textures["figures"][11]);
        }
    }
    sprites["figures"].push_back(sprite);
}

void GameRender::place_and_scale_board_at_start(){
    Vector2u winSize = window.getSize();
    float basicScale = cast<float>(winSize.y) / sprites["board"][0].getGlobalBounds().height;
    sprites["board"][0].setScale(basicScale, basicScale);
    sprites["board"][0].setPosition(offsetRatio * winSize.x, 0);
}

void GameRender::place_sprite(Sprite &sprite, int i, int j){
    Vector2u winSize = window.getSize();
    Vector2f position;
    position.x = offsetRatio * winSize.x + j * cast<float>(winSize.y) / 8;  // Здесь "y" т.к. поле 1x1 и размер по "y"
    position.y = i * cast<float>(winSize.y) / 8;
    sprite.setPosition(position);
}

void GameRender::place_figures_at_start(){
    vector<vector<shared_ptr<Figure>>> field = gameState->getField();
    for (int i = 0; i < 8; ++i){
        for (int j = 0; j < 8; ++j){
            if (field[i][j]){
                create_sprite_out_of_figure(field[i][j]);
                place_sprite(sprites["figures"].back(), i, j);
            }
        }
    }
}

void GameRender::scale_figures_at_start(){
    float basicScale = squareSize / textureSize;
    for (Sprite &sprite: sprites["figures"]){
        sprite.setScale(basicScale, basicScale);
    }
}

void GameRender::create_hints(const Sprite &figure){
    Sprite rect{textures["shapes"][1]};
    rect.setScale(figure.getScale());
    rect.setPosition(figure.getPosition());
    sprites["shapes"].push_back(rect);
}

void GameRender::place_hints(const Vector2u &currWinSize, const vector<Square> &moves, const Square &frFigure){
    float pX, pY;
    Sprite templateCircle{};
    templateCircle.setTexture(textures["shapes"][0]);
    templateCircle.setColor(Color(255, 255, 255, 180));
    templateCircle.setOrigin(templateCircle.getGlobalBounds().width / 2, templateCircle.getGlobalBounds().height / 2);
    templateCircle.setScale(sprites["figures"][0].getScale().x, sprites["figures"][0].getScale().y);

    for (const Square &move: moves){
        Sprite shape{};
        pX = offsetRatio * currWinSize.x + (move.column + 0.5) * squareSize;
        pY = (move.line + 0.5) * squareSize;
        if (hasEnemyPiece(gameState->getField(), frFigure, move)){
            shape.setTexture(textures["shapes"][2]);
            shape.setScale(sprites["figures"][0].getScale());
            set_position_from_square(shape, move);
        } else {
            shape = templateCircle;
            shape.setPosition(pX, pY);
        }
        sprites["shapes"].push_back(shape);
    }
}

void GameRender::create_check_hint(const Square &king){
    Sprite redBorder{textures["shapes"][4]};
    redBorder.setScale(sprites["figures"][0].getScale());
    set_position_from_square(redBorder, king);
    sprites["shapes"].push_back(redBorder);
}

void GameRender::create_and_place_clock_text(){
    float posX = window.getSize().x * (1 - 0.95 * offsetRatio);

    for (string str: {"white_clock", "black_clock"}){
        texts[str] = Text();
        texts[str].setFont(fonts["clock"]);
        texts[str].setCharacterSize(90);
        texts[str].setFillColor(Color::White);
    }
    texts["white_clock"].setPosition(posX, 4.9 * squareSize);
    texts["black_clock"].setPosition(posX, 1.9 * squareSize);
}

void GameRender::update_clocks_text(){
    static int t1 = 0, t2 = 0;
    pair<Components::Time, Components::Time> timePair = gameState->getClock().getTime();
    Components::Time wC = timePair.first;
    Components::Time bC = timePair.second;
    if (wC != t1){
        string text = format("{}:{}:{}", wC.hours, wC.minutes, wC.seconds);
        texts["white_clock"].setString(text);
        t1 = wC.getTimeInSeconds();
    }
    if (bC != t2){
        string text2 = format("{}:{}:{}", bC.hours, bC.minutes, bC.seconds);
        texts["black_clock"].setString(text2);
        t2 = bC.getTimeInSeconds();
    }

//    texts["white_clock"].setOrigin(texts["white_clock"].getGlobalBounds().getSize() / 2.f + texts["white_clock"].getLocalBounds().getPosition());
//    texts["black_clock"].setOrigin(texts["black_clock"].getGlobalBounds().getSize() / 2.f + texts["black_clock"].getLocalBounds().getPosition());
}

void GameRender::create_and_place_players_names(){
    for (string str: {"white_player", "black_player"}){
        texts[str] = Text();
        texts[str].setFont(fonts["clock"]);
        texts[str].setCharacterSize(40);
        texts[str].setFillColor(Color::White);
    }
    pair<Player, Player> p = gameState->getPlayers();
    unordered_map<string, unordered_map<string, int>> eloMap = gameState->getEloChange();
    unordered_map<string, int> w_elo = eloMap["white"];
    unordered_map<string, int> b_elo = eloMap["black"];
    string white = format("{}, {}\n(+{}, {}, {})", p.first.getName(), p.first.getElo(),
                          w_elo["victory"], w_elo["draw"], w_elo["defeat"]);
    string black = format("{}, {}\n(+{}, {}, {})", p.second.getName(), p.second.getElo(),
                          b_elo["victory"], b_elo["draw"], b_elo["defeat"]);
    Text &wC = texts["white_clock"];
    Text &bC = texts["black_clock"];

    texts["white_player"].setString(white);
    texts["white_player"].setPosition(wC.getPosition().x, wC.getPosition().y + squareSize);
    texts["black_player"].setString(black);
    texts["black_player"].setPosition(bC.getPosition().x, bC.getPosition().y + squareSize);
}

void GameRender::create_and_place_history(){
    TextArea::Ptr historyText = TextArea::create();
    historyText->setPosition(0, 0);
    historyText->setSize(window.getSize().x * offsetRatio, window.getSize().y / 4);
    historyText->setTextSize(24);
    historyText->getRenderer()->setBackgroundColor(Color{65, 65, 65, 255});
    historyText->getRenderer()->setTextColor(Color::White);
    historyText->getRenderer()->setBorders(tgui::Outline{0, 0, 0, 0});
    for (const string &move: gameState->getHistory().getHistory()){
        historyText->addText(move);
    }
    gui.add(historyText, "history");
}

void add_spaces_to_string(string &str, int len){
    if (str.length() >= len - 1){
        return;
    }
    while (str.length() < len - 1){
        str += ' ';
    }
    str += '|';
}

void GameRender::update_history_text(string text){
    static int moveCount = 2;
    TextArea::Ptr historyText = std::dynamic_pointer_cast<TextArea>(gui.get("history"));
    if (moveCount % 2 == 0){
        text = format("{}. {}", moveCount / 2, text);
        add_spaces_to_string(text, 11);
    } else {
        text = format(" {}", text);
        add_spaces_to_string(text, 9);
        text += '\n';
    }
    moveCount++;
    historyText->addText(text);
}

vector<Square>
GameRender::select_friendly_figure(Vector2u &currWinSize, Square &frFigure, int &frIndex, bool &isMakingMove){
    int fX, fY;
    bool forKing;
    vector<Square> moves;
    frIndex = 0;
    Vector2i mousePos = Mouse::getPosition(window);

    for (const Sprite &sprite: sprites["figures"]){
        if (mousePos.x<offsetRatio * currWinSize.x || mousePos.x>(1 - offsetRatio) * currWinSize.x){
            break;
        }
        if (sprite.getGlobalBounds().contains(mousePos.x, mousePos.y)){
            fX = (mousePos.x - offsetRatio * currWinSize.x) / squareSize;
            fY = mousePos.y / squareSize;
            shared_ptr<Figure> figure = gameState->getField()[fY][fX];

            if (figure->getColor() != gameState->getWhoseTurn()){
                break;
            }
            forKing = figure->getName() == "king";

            frFigure = Square{fY, fX};
            moves = figure->getPossibleMoves(gameState->getField(), frFigure,
                                             gameState->getKingsPosition(), gameState->getCheckInfo(),
                                             forKing, gameState->getWhoseTurn(), gameState->getHistory().getLastMove());
            create_hints(sprite);
            isMakingMove = true;
            break;
        }
        frIndex++;
    }

    place_hints(currWinSize, moves, frFigure);

    return moves;
}

void GameRender::enPassant_check(const Vector2u &currWinSize, const Square &frFigure, int &frIndex, int fX, int fY){
    if (gameState->getField()[frFigure.line][frFigure.column]->getName() == "pawn"){
        if (abs(fX - frFigure.column) == 1 && !gameState->getField()[fY][fX]){
            float delX = offsetRatio * currWinSize.x + fX * squareSize;
            float delY = frFigure.line * squareSize;
            int count = 0;
            for (auto it = sprites["figures"].begin(); it != sprites["figures"].end(); it++){
                if ((*it).getGlobalBounds().contains(delX, delY)){
                    sprites["figures"].erase(it);
                    break;
                }
                count++;
            }
            if (frIndex > count){
                frIndex--;
            }
        }
    }
}

void GameRender::onPromote_texture_change(const Square &frFigure, const int &frIndex, int fY){
    if (gameState->getField()[frFigure.line][frFigure.column]->getName() == "pawn"){
        if (fY == 0){
            sprites["figures"][frIndex].setTexture(textures["figures"][4]);
        } else if (fY == 7){
            sprites["figures"][frIndex].setTexture(textures["figures"][10]);
        }
    }
}

void GameRender::castle_check(const Vector2u &currWinSize, const Square &frFigure, int fX, int mY){
    shared_ptr<Figure> figure = gameState->getField()[frFigure.line][frFigure.column];
    float rX, newRX;
    if (figure->getName() == "king"){
        if (fX - frFigure.column == 2){
            rX = (fX + 1) * squareSize;
            newRX = (frFigure.column + 1) * squareSize;
        } else if (fX - frFigure.column == -2){
            rX = (fX - 2) * squareSize;
            newRX = (frFigure.column - 1) * squareSize;
        } else {
            return;
        }
        rX += offsetRatio * currWinSize.x;
        newRX += offsetRatio * currWinSize.x;
        for (Sprite &sprite: sprites["figures"]){
            if (sprite.getGlobalBounds().contains(rX, mY)){
                sprite.setPosition(newRX, sprite.getPosition().y);
                break;
            }
        }
    }
}

void GameRender::make_move(Vector2u &currWinSize, Square &frFigure, int frIndex, vector<Square> &possibleMoves){
    Vector2i mousePos = Mouse::getPosition(window);
    int fX = (mousePos.x - offsetRatio * currWinSize.x) / squareSize;
    int fY = mousePos.y / squareSize;
    Sprite sprite;

    if (find(possibleMoves.begin(), possibleMoves.end(), Square{fY, fX}) != possibleMoves.end()){
        enPassant_check(currWinSize, frFigure, frIndex, fX, fY);
        onPromote_texture_change(frFigure, frIndex, fY);
        castle_check(currWinSize, frFigure, fX, mousePos.y);
        Square resPlace{fY, fX};
        string state;
        bool spriteNeedsToBeDeleted = gameState->gameMove(frFigure, resPlace, state);
        place_sprite(sprites["figures"][frIndex], fY, fX);
        if (spriteNeedsToBeDeleted){
            for (int i = 0; i < sprites["figures"].size(); i++){
                if (i == frIndex){
                    continue;
                }
                sprite = sprites["figures"][i];
                if (sprite.getGlobalBounds().contains(mousePos.x, mousePos.y)){
                    auto it = std::next(sprites["figures"].begin(), i);
                    sprites["figures"].erase(it);
                    break;
                }
            }
        }
        update_history_text(gameState->getHistory().getLastMove());

        if (state == "mate"){
            create_result_string("has won by mate", false);
        } else if (state == "stalemate"){
            create_result_string("has drawn by stalemate", false);
        } else if (state == "check"){
            Square king = gameState->getKingsPosition().at(gameState->getWhoseTurn());
            create_check_hint(king);
        }
    }
    sprites["shapes"].clear();
}

void GameRender::on_game_start(){
    place_and_scale_board_at_start();
    place_figures_at_start();
    scale_figures_at_start();
    create_and_place_clock_text();
    create_and_place_players_names();
    create_and_place_history();
    start_clock_thread();
}

void GameRender::draw(){
    window.clear(Color{65, 65, 65, 255});
    window.draw(sprites["board"][0]);
    for (const Sprite &shape: sprites["shapes"]){
        window.draw(shape);
    }
    for (const Sprite &sprite: sprites["figures"]){
        window.draw(sprite);
    }
    lock_guard<mutex> lockS(resultMutex);
    for (const Sprite &sprite: sprites["result"]){
        window.draw(sprite);
    }
    for (const pair<const string, Text> &pair: texts){
        if (pair.first.find("clock") != string::npos){
            continue;
        }
        window.draw(pair.second);
    }
    for (const pair<const string, Text> &pair: texts){
        if (pair.first.find("clock") != string::npos){
            update_clocks_text();
            window.draw(pair.second);
        }
    }
    gui.draw();
    window.display();
}

void GameRender::render(){
//    std::chrono::high_resolution_clock::time_point start;
//    std::chrono::high_resolution_clock::time_point end;
//    float fps;
    Event event;
    Vector2u currWinSize = window.getSize();
    on_game_start();
    bool isMakingMove = false;
    vector<Square> possibleMoves;
    Square friendlyFigure;
    int frIndex;
    while (window.isOpen()){
//        start = std::chrono::high_resolution_clock::now();
        while (window.pollEvent(event)){
            if (event.type == Event::Closed){
                window.close();
            } else if (event.type == Event::KeyPressed){
                if (event.key.code == sf::Keyboard::Escape){
                    window.close();
                }
            } else if (event.type == Event::Resized){
                resize_window(currWinSize);

            } else if (event.type == Event::MouseButtonPressed){
                if (texts.contains("result")){
                    continue;
                }
                if (isMakingMove){
                    make_move(currWinSize, friendlyFigure, frIndex, possibleMoves);
                    isMakingMove = false;
                } else {
                    possibleMoves = select_friendly_figure(currWinSize, friendlyFigure, frIndex, isMakingMove);
                }
            }
            gui.handleEvent(event);
        }
        draw();

//        end = std::chrono::high_resolution_clock::now();
//        fps = (float)1e9/(float)std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count();
//        cout << fps << endl;
    }
}