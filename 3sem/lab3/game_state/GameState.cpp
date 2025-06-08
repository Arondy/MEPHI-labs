#include "GameState.h"

using namespace Figures;

namespace State {
    GameState::~GameState(){
        clockFlag = false;
        for (pair<const string, shared_ptr<thread>> &pair: threads){
            shared_ptr<thread> &thread = pair.second;
            if (thread && thread->joinable()){
                thread->join();
            }
        }
    }

    void GameState::examineCheckThread(vector<Square> &threats, int start, int end){
        Square kingPos = kingsPosition.at(whoseTurn);
        for (int i = start; i < end; ++i){
            for (int j = 0; j < 8; ++j){
                shared_ptr<Figure> figure = gameField[i][j];
                if (figure && figure->getName() != "king" && figure->getColor() != whoseTurn){
                    vector<Square> moves = gameField[i][j]->getPossibleMoves(gameField, Square{i, j}, kingsPosition,
                                                                             checkInfo, false, figure->getColor(),
                                                                             history.getLastMove());
                    if (find(moves.begin(), moves.end(), kingPos) != moves.end()){
                        lock_guard<std::mutex> lock(exCheckMutex);
                        threats.emplace_back(i, j);
                    }
                }
            }
        }
    }

    void GameState::examineCheck(){
        vector<thread> checkThreads;
        vector<Square> threats;

        int numThreads = min(4, int(thread::hardware_concurrency()));
        int chunkSize = 8 / numThreads;

        for (int i = 0; i < numThreads; ++i){
            int start = i * chunkSize;
            int end = start + chunkSize;
            checkThreads.emplace_back([this, &threats, start, end]{ examineCheckThread(threats, start, end); });
        }

        for (thread &thread: checkThreads){
            thread.join();
        }
        checkInfo.clear();
        checkInfo = threats;
    }

    bool GameState::isCheck() const{
        return !checkInfo.empty();
    }

    bool GameState::noMoveAvailable() const{
        vector<Square> moves;
        for (int i = 0; i < 8; ++i){
            for (int j = 0; j < 8; ++j){
                if (gameField[i][j] && gameField[i][j]->getColor() == whoseTurn){
                    moves = gameField[i][j]->getPossibleMoves(gameField, Square{i, j}, kingsPosition, checkInfo, false,
                                                              whoseTurn, history.getLastMove());
                    if (!moves.empty()){
                        return false;
                    }
                }
            }
        }
        return true;
    }

    void GameState::startClockThread(){
        threads["clock"] = make_shared<thread>([this]{
            while (clockFlag){
                if (getClockRes()){
                    if (getClockRes() == 1){
                        players.first.changeEloByDelta(eloChange["white"]["defeat"]);
                        players.second.changeEloByDelta(eloChange["black"]["victory"]);
                    } else if (getClockRes() == 2){
                        players.first.changeEloByDelta(eloChange["white"]["victory"]);
                        players.second.changeEloByDelta(eloChange["black"]["defeat"]);
                    }
                    lock_guard<mutex> lock(clockFlagMutex);
                    clockFlag = false;
                } else {
                    std::this_thread::sleep_for(0.1s);
                }
            }
        });
    }

    const int &GameState::getClockRes() const{
        lock_guard<mutex> lock(Components::clockResMutex);
        return clockRes;
    }

    string GameState::checkGameState() const{
        if (isCheck()){
            if (noMoveAvailable()){
                return "mate";
            }
            return "check";
        } else if (noMoveAvailable()){
            return "stalemate";
        } else {
            return "nothing";
        }
    }

    void GameState::fillEloChange(){
        int elo1 = players.first.getElo();
        int elo2 = players.second.getElo();
        int w_d = abs(elo1 - elo2) / elo1;
        int b_d = abs(elo1 - elo2) / elo2;
        unordered_map<string, int> whiteMap = {
                {"victory", w_d + 5},
                {"draw",    w_d},
                {"defeat",  w_d - 5}
        };
        unordered_map<string, int> blackMap = {
                {"victory", b_d + 5},
                {"draw",    b_d},
                {"defeat",  b_d - 5}
        };
        eloChange = {
                {"white", whiteMap},
                {"black", blackMap}
        };
    }

    void GameState::stopClock(){
        clock.~Clock();
    }

    void GameState::flipTurn(){
        whoseTurn == "white" ? whoseTurn = "black" : whoseTurn = "white";
    }

    void GameState::specialMoves(const Square &currS, const Square &newS, const string &name, const shared_ptr<Figure> &currFigure, bool &promote){
        string color = currFigure->getColor();
        if (name == "king"){
            kingsPosition[color] = newS;
            if (newS.column - currS.column == 2){
                gameField[currS.line][currS.column + 1] = gameField[newS.line][newS.column + 1];
                gameField[newS.line][newS.column + 1] = nullptr;
            } else if (newS.column - currS.column == -2){
                gameField[currS.line][currS.column - 1] = gameField[newS.line][newS.column - 2];
                gameField[newS.line][newS.column - 2] = nullptr;
            }
            shared_ptr<King> king = std::dynamic_pointer_cast<King>(currFigure);
            king->setMovedTrue();
        } else if (name == "rook"){
            shared_ptr<Rook> rook = std::dynamic_pointer_cast<Rook>(currFigure);
            rook->setMovedTrue();
        } else if (name == "pawn"){
            // En passant
            if (abs(currS.column - newS.column) == 1 && !gameField[newS.line][newS.column]){
                gameField[currS.line][newS.column] = nullptr;
            }
                // Promote
            else if (newS.line == 0){
                gameField[currS.line][currS.column] = make_shared<Queen>(Queen{"white"});
                promote = true;
            } else if (newS.line == 7){
                gameField[currS.line][currS.column] = make_shared<Queen>(Queen{"black"});
                promote = true;
            }
        }
    }

    void GameState::basicMove(const Square &currS, const Square &newS){
        gameField[newS.line][newS.column] = gameField[currS.line][currS.column];
        gameField[currS.line][currS.column] = nullptr;
        flipTurn();
        examineCheck();
    }

    void GameState::updateClock(){
        clock.clockFlagFlip();
        if (whoseTurn == "white"){
            clock.increaseTime2();
        } else {
            clock.increaseTime1();
        }
    }

    void GameState::updateElo(const string &state){
        if (state == "mate"){
            if (whoseTurn == "white"){
                players.first.changeEloByDelta(eloChange["white"]["defeat"]);
                players.second.changeEloByDelta(eloChange["black"]["victory"]);
            } else {
                players.first.changeEloByDelta(eloChange["white"]["victory"]);
                players.second.changeEloByDelta(eloChange["black"]["defeat"]);
            }
        } else if (state == "stalemate"){
            players.first.changeEloByDelta(eloChange["white"]["draw"]);
            players.second.changeEloByDelta(eloChange["black"]["draw"]);
        }
    }

    bool GameState::gameMove(const Square &currS, const Square &newS, string &state){
        shared_ptr<Figure> currFigure = gameField[currS.line][currS.column];
        string name = currFigure->getName();
        bool attackedSquareFigureIsDeleted = gameField[newS.line][newS.column] != nullptr;
        bool takes = (gameField[newS.line][newS.column] != nullptr || (name == "pawn" && abs(currS.column - newS.column) == 1));
        bool promote = false;

        specialMoves(currS, newS, name, currFigure, promote);
        basicMove(currS, newS);

        state = checkGameState();

        updateElo(state);
        updateClock();

        history.addMove(currS, name, newS, takes, state, promote);

        return attackedSquareFigureIsDeleted;
    }

    void GameState::saveGame(const string &filename) const{
        if (filename.find(basic_game_file) != string::npos){
            throw std::runtime_error("This name is taken by the game. Consider choosing another one.");
        }

        // Если нет фигуры, будет /
        unordered_map<string, char> figNameToLower = {
                {"pawn",   'p'},
                {"knight", 'n'},
                {"bishop", 'b'},
                {"rook",   'r'},
                {"queen",  'q'},
                {"king",   'k'}
        };
        string savedGameStr;

        for (int i = 0; i < 8; i++){
            for (int j = 0; j < 8; j++){
                shared_ptr<Figure> figure = gameField[i][j];
                if (figure){
                    if (figure->getColor() == "white"){
                        savedGameStr.push_back(toupper(figNameToLower[gameField[i][j]->getName()]));
                    } else {
                        savedGameStr.push_back(figNameToLower[gameField[i][j]->getName()]);
                    }
                } else {
                    savedGameStr.push_back('/');
                }
            }
        }
        std::ofstream file(filename);
        json jsonSave;
        jsonSave["field"] = savedGameStr;
        jsonSave["names"] = {players.first.getName(), players.second.getName()};
        jsonSave["elo"] = {players.first.getElo(), players.second.getElo()};
        jsonSave["clock"] = {clock.getTime().first.getTimeInSeconds(), clock.getTime().second.getTimeInSeconds(),
                             clock.getInc()};
        jsonSave["history"] = history.getHistory();
        jsonSave["turn"] = getWhoseTurn();

        if (file.is_open()){
            file << std::setw(4) << jsonSave << endl;
            file.close();
            cout << "Successfully saved game" << endl;
        } else {
            std::cerr << "Не удалось открыть файл для записи." << endl;
        }
    }

    void GameState::loadGame(const string &filename){
        unordered_map<char, std::pair<string, string>> letterToFigure = {
                {'P', {"pawn",   "white"}},
                {'N', {"knight", "white"}},
                {'B', {"bishop", "white"}},
                {'R', {"rook",   "white"}},
                {'Q', {"queen",  "white"}},
                {'K', {"king",   "white"}},
                {'p', {"pawn",   "black"}},
                {'n', {"knight", "black"}},
                {'b', {"bishop", "black"}},
                {'r', {"rook",   "black"}},
                {'q', {"queen",  "black"}},
                {'k', {"king",   "black"}},
        };

        char c;
        std::ifstream file(filename);
        json jsonSave;

        if (file.is_open()){
            file >> jsonSave;
            file.close();
        } else {
            std::cerr << "Не удалось открыть файл для чтения." << std::endl;
        }

        string savedGameStr = jsonSave["field"];
        int d_count = 1;

        for (int i = 0; i < 8; i++){
            for (int j = 0; j < 8; j++){
                c = savedGameStr[i * 8 + j];
                if (c != '/'){
                    cout << "Figure " << d_count++ << " loaded" << endl;
                    if (c == 'k' or c == 'K'){
                        kingsPosition[letterToFigure[c].second] = Square{i, j};
                    }
                    gameField[i][j] = createFigureByName(letterToFigure[c].first, letterToFigure[c].second);
                } else {
                    gameField[i][j] = nullptr;
                }
            }
        }

        whoseTurn = jsonSave["turn"];
        clock = Clock{jsonSave["clock"][0], jsonSave["clock"][1], jsonSave["clock"][2], (whoseTurn == "white")};
        history = GameHistory{jsonSave["history"]};
        players = {Player{jsonSave["names"][0], jsonSave["elo"][0]}, Player{jsonSave["names"][1], jsonSave["elo"][1]}};
        fillEloChange();

        Square kingS;
        for (const string &move: history.getHistory()){
            if (move[0] == 'K'){
                kingS = kingsPosition["white"];
            } else if (move[0] == 'k'){
                kingS = kingsPosition["black"];
            } else {
                continue;
            }
            shared_ptr<King> king = std::dynamic_pointer_cast<King>(gameField[kingS.line][kingS.column]);
            king->setMovedTrue();
        }
        examineCheck();
        clock.startThread(clockRes);
        startClockThread();
        cout << "Successfully loaded game" << endl;
    }

    shared_ptr<Figure> createFigureByName(const string &name, const string &color){
        if (name == "pawn"){
            return make_shared<Pawn>(Pawn(color));
        } else if (name == "knight"){
            return make_shared<Knight>(Knight(color));
        } else if (name == "bishop"){
            return make_shared<Bishop>(Bishop(color));
        } else if (name == "rook"){
            return make_shared<Rook>(Rook(color));
        } else if (name == "queen"){
            return make_shared<Queen>(Queen(color));
        } else if (name == "king"){
            return make_shared<King>(King(color));
        } else {
            return nullptr;
        }
    }
}