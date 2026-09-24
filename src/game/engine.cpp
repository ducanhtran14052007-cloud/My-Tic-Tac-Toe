/**
 * Engine cpp implementation
 *
 */

#include "engine.h"

/* ---------- Importing ---------- */

#include <chrono>
#include <optional>
#include <sstream>
#include <thread>
#include <vector>

#include "../utils/helper.h"
#include "../utils/logger.h"
#include "bot/bot_factory.h"
#include "logic.h"

/* ---------- Definitions ---------- */

/**
 * Mô tả:
 *   Constructor của Engine, nhận các dependency từ bên ngoài.
 *
 * Đầu vào:
 *   - _config: cấu hình chương trình
 *   - _iRenderer: renderer cụ thể (terminal/SDL)
 *   - _iInteraction: interaction cụ thể
 *
 * Đầu ra:
 *   - Không có
 *
 * Tác dụng phụ:
 *   - Gán con trỏ để sử dụng trong toàn bộ vòng đời engine
 */
Engine::Engine(const RunConfig* _config, I_Renderer* _iRenderer, I_Interaction* _iInteraction) {
    config = _config;
    iRenderer = _iRenderer;
    iInteraction = _iInteraction;
}

/**
 * Mô tả:
 *   Destructor của Engine.
 *
 * Đầu vào:
 *   - Không có
 *
 * Đầu ra:
 *   - Không có
 *
 * Tác dụng phụ:
 *   - Hiện tại chưa xử lý gì (cleanup ở nơi khác)
 */
Engine::~Engine() {
}

/**
 * Mô tả:
 *   Khởi tạo các thành phần chính của engine (renderer + interaction).
 *
 * Đầu vào:
 *   - Không có
 *
 * Đầu ra:
 *   - Không có
 *
 * Tác dụng phụ:
 *   - Gọi init() của renderer và interaction
 */
void Engine::init() {
    Logger::log("Engine initializing . . .");

    iRenderer->init(*config);     // khởi tạo renderer theo config
    iInteraction->init(*config);  // khởi tạo interaction theo config

    Logger::log("Engine initialized!");
}

/**
 * Mô tả:
 *   Kiểm tra tính hợp lệ của các interface trước khi chạy game.
 *
 * Đầu vào:
 *   - Không có
 *
 * Đầu ra:
 *   - bool: true nếu hợp lệ, false nếu thiếu thành phần
 *
 * Tác dụng phụ:
 *   - Ghi log cảnh báo nếu thiếu renderer/interaction
 */
bool Engine::sanity_check() {
    bool isRendererGood = iRenderer;
    if (!isRendererGood) {
        Logger::log("Interface Renderer is not implemented!", Logger::Level::WARNING);
    }

    bool isInteractionGood = iInteraction;
    if (!isInteractionGood) {
        Logger::log("Interface Interaction is not implemented!", Logger::Level::WARNING);
    }

    return isRendererGood && isInteractionGood;
}

/**
 * Mô tả:
 *   Thiết lập game trước khi bắt đầu:
 *   - Lấy input từ user (size, goal, mode, bot level)
 *   - Khởi tạo board
 *
 * Đầu vào:
 *   - Không có
 *
 * Đầu ra:
 *   - Không có
 *
 * Tác dụng phụ:
 *   - Thay đổi gameSetup
 *   - Gọi render và interaction
 */
void Engine::startGame() {
    Logger::log("[Engine] Starting game . . .");

    if (!sanity_check()) {
        Logger::log("[Engine] Game stopped!", Logger::Level::ERROR);
        return;
    }

    if (config->interactive) {
        iRenderer->clearScreen();
        iRenderer->showSelectMenu(SelectType::TITLE_UI);
        iInteraction->pause();
    }

    bool isSlected;

    // chọn kích thước bàn cờ
    do {
        if (config->interactive)
            iRenderer->showSelectMenu(SelectType::SIZE_UI);
        isSlected = iInteraction->selectSize(&gameSetup.size);

        if (config->interactive && !isSlected)
            iRenderer->showInvalidSelect(SelectType::SIZE_UI, gameSetup.size);
    } while (!isSlected);

    if (config->interactive)
        iRenderer->showValidSelect(SelectType::SIZE_UI, gameSetup.size);

    Logger::log(std::format("user input 'size' = {}", gameSetup.size), Logger::Level::DEBUG);

    // chọn điều kiện thắng (goal)
    do {
        if (config->interactive)
            iRenderer->showSelectMenu(SelectType::GOAL_UI, gameSetup.size);
        isSlected = iInteraction->selectGoal(&gameSetup.goal, gameSetup.size);

        if (config->interactive && !isSlected)
            iRenderer->showInvalidSelect(SelectType::GOAL_UI, gameSetup.goal);
    } while (!isSlected);

    if (config->interactive)
        iRenderer->showValidSelect(SelectType::GOAL_UI, gameSetup.goal);

    Logger::log(std::format("user input 'goal' = {}", gameSetup.goal), Logger::Level::DEBUG);

    // chọn mode chơi
    do {
        if (config->interactive)
            iRenderer->showSelectMenu(SelectType::GAME_MODE_UI);
        isSlected = iInteraction->selectGameMode(&gameSetup.mode);

        if (config->interactive && !isSlected)
            iRenderer->showInvalidSelect(SelectType::GAME_MODE_UI, (int)gameSetup.mode);
    } while (!isSlected);

    if (config->interactive)
        iRenderer->showValidSelect(SelectType::GAME_MODE_UI, (int)gameSetup.mode);

    Logger::log(std::format("user input 'game mode' = {}", modeToString((int)gameSetup.mode)), Logger::Level::DEBUG);

    // mode Player vs Bot
    if (gameSetup.mode == GameMode::PVE) {
        do {
            if (config->interactive)
                iRenderer->showSelectMenu(SelectType::BOT_LEVEL_UI);
            isSlected = iInteraction->selectBotLevel(gameSetup.levels, 1);

            if (config->interactive && !isSlected)
                iRenderer->showInvalidSelect(SelectType::BOT_LEVEL_UI, (int)gameSetup.levels[1]);
        } while (!isSlected);

        if (config->interactive)
            iRenderer->showValidSelect(SelectType::BOT_LEVEL_UI, (int)gameSetup.levels[1]);

        Logger::log(std::format("user input 'bot level' = {}", botToString((int)gameSetup.levels[1])), Logger::Level::DEBUG);
    }

    // mode Bot vs Bot
    if (gameSetup.mode == GameMode::EVE) {
        // bot 0
        do {
            if (config->interactive)
                iRenderer->showSelectMenu(SelectType::MUL_BOT_LEVEL_UI, 0);
            isSlected = iInteraction->selectBotLevel(gameSetup.levels, 0);

            if (config->interactive && !isSlected)
                iRenderer->showInvalidSelect(SelectType::MUL_BOT_LEVEL_UI, (int)gameSetup.levels[0]);

        } while (!isSlected);

        if (config->interactive)
            iRenderer->showValidSelect(SelectType::MUL_BOT_LEVEL_UI, (int)gameSetup.levels[0]);

        Logger::log(std::format("user input 'bot level[0]' = {}", botToString((int)gameSetup.levels[0])), Logger::Level::DEBUG);

        // bot 1
        do {
            if (config->interactive)
                iRenderer->showSelectMenu(SelectType::MUL_BOT_LEVEL_UI, 1);
            isSlected = iInteraction->selectBotLevel(gameSetup.levels, 1);

            if (config->interactive && !isSlected)
                iRenderer->showInvalidSelect(SelectType::MUL_BOT_LEVEL_UI, (int)gameSetup.levels[1]);

        } while (!isSlected);

        if (config->interactive)
            iRenderer->showValidSelect(SelectType::MUL_BOT_LEVEL_UI, (int)gameSetup.levels[1]);

        Logger::log(std::format("user input 'bot level[1]' = {}", botToString((int)gameSetup.levels[1])), Logger::Level::DEBUG);
    }

    // khởi tạo board rỗng
    Logic::initBoard(gameSetup.board, gameSetup.size);
    Logger::log("Board initialized!");

    Logger::log("[Engine] Game started!");
}

/**
 * Mô tả:
 *   Game loop chính:
 *   - Render board
 *   - Lấy input (bot hoặc human)
 *   - Thực hiện move
 *   - Kiểm tra thắng/thua/hòa
 *
 * Đầu vào:
 *   - Không có
 *
 * Đầu ra:
 *   - GameResult: kết quả cuối cùng
 *
 * Tác dụng phụ:
 *   - Thay đổi trạng thái board liên tục
 */
GameResult Engine::playGame() {
    Logger::log("[Engine] Playing game . . .");

    GameResult gameResult = GameResult(-1, false, 0);

    if (!sanity_check()) {
        Logger::log("[Engine] Game stopped!", Logger::Level::ERROR);
        return gameResult;
    }

    int row, col;
    bool is_running = true;
    const char symbols[2] = {'X', 'O'};

    // xác định player nào là bot
    bool is_bot[2] = {
        gameSetup.mode == GameMode::EVE,
        gameSetup.mode != GameMode::PVP,
    };

    std::vector<std::unique_ptr<Bot>> bots;
    bots.clear();

    // khởi tạo bot theo mode
    if (gameSetup.mode == GameMode::PVE) {
        bots.emplace_back(std::unique_ptr<Bot>(nullptr));  // player 0 là human
        bots.emplace_back(std::unique_ptr<Bot>(
            BotFactory::createBot(gameSetup.levels[1], symbols[1])));
    } else if (gameSetup.mode == GameMode::EVE) {
        bots.emplace_back(std::unique_ptr<Bot>(
            BotFactory::createBot(gameSetup.levels[0], symbols[0])));
        bots.emplace_back(std::unique_ptr<Bot>(
            BotFactory::createBot(gameSetup.levels[1], symbols[1])));
    }

    int player = 0;  // X đi trước

    // Game Loop
    while (is_running) {
        Logger::log(
            std::format("[Engine] starting turn #{}", gameResult.turns), Logger::Level::DEBUG);

        // render board
        if (config->interactive) iRenderer->clearScreen();
        if (config->interactive) iRenderer->displayBoard(gameSetup.board, gameSetup.size);

        // hiển thị player hiện tại
        if (config->interactive) iRenderer->showPlayer(player, is_bot[player]);

        // 1. Get Move
        if (is_bot[player]) {
            // NOTE: đo thời gian thực thi bot
            pII point = measureExecutionTime(
                std::format("bot#{}->getMove()", player),
                [&]() {
                    return bots[player]->getMove(
                        gameSetup.board,
                        gameSetup.size,
                        gameSetup.goal);
                },
                TIME_ENABLED);

            row = point.first;
            col = point.second;
        } else {
            // human input
            bool is_valid = true;
            do {
                if (config->interactive) iRenderer->showSelectMenu(SelectType::PLAYER_UI);
                if (config->interactive) iRenderer->displayBoard(gameSetup.board, gameSetup.size);
                iInteraction->getPlayerMove(&row, &col);
                is_valid = Logic::isValidMove(gameSetup.board, gameSetup.size, row, col);

                if (!is_valid) {
                    if (config->interactive) iRenderer->showInvalidMove();
                }
            } while (!is_valid);
        }

        // 2. Make move
        Logic::makeMove(gameSetup.board, row, col, symbols[player]);
        if (config->interactive) {
            iRenderer->showMove(row, col);
        }
        gameResult.turns += 1;

        std::stringstream ss;
        ss << "player " << player + 1 << " make move to (" << row << ", " << col << ")";
        Logger::log(ss.str(), Logger::Level::DEBUG);

        // delay khi bot chơi để dễ quan sát
        if (is_bot[player] and config->interactive) {
            std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));
        }

        // 3. Check result
        if (Logic::checkWin(gameSetup.board, gameSetup.size, symbols[player], gameSetup.goal)) {
            gameResult.winner = player;
            gameResult.isBot = is_bot[player];
            is_running = false;
            Logger::log(std::format("Player {} ({}) wins at turn {}", player + 1, symbols[player], gameResult.turns));
        } else if (Logic::checkDraw(gameSetup.board, gameSetup.size)) {
            gameResult.winner = -1;
            gameResult.isBot = false;
            is_running = false;
            Logger::log(std::format("Game Draw (total turns: {})", gameResult.turns));
        }

        // 4. Switch player
        player = 1 - player;

        Logger::log("[Engine] turn done!", Logger::Level::DEBUG);
    }

    Logger::log("[Engine] Game done!");

    return gameResult;
}

/**
 * Mô tả:
 *   Xử lý sau khi game kết thúc:
 *   - Hiển thị kết quả
 *   - Log thông tin game
 *
 * Đầu vào:
 *   - gameResult: kết quả game
 *
 * Đầu ra:
 *   - Không có
 *
 * Tác dụng phụ:
 *   - Render UI và log
 */
void Engine::endGame(const GameResult& gameResult) {
    Logger::log("[Engine] Ending game . . .");

    if (!sanity_check()) {
        Logger::log("[Engine] Game stopped!", Logger::Level::ERROR);
        return;
    }

    if (config->interactive) {
        iRenderer->clearScreen();
        iRenderer->displayBoard(gameSetup.board, gameSetup.size);

        char symbol = (gameResult.winner == 0) ? 'X' : 'O';

        // NOTE: lấy danh sách ô tạo thành đường thắng
        auto winLine = Logic::getWinLine(gameSetup.board, gameSetup.size, symbol, gameSetup.goal);

        if (winLine) {
            std::string s = "[WinLine] cells: ";
            for (auto [r, c] : winLine->cells)
                s += std::format("({}, {}) ", r, c);

            Logger::log(s, Logger::Level::DEBUG);
        } else {
            Logger::log("[WinLine] none", Logger::Level::DEBUG);
        }

        // hiển thị kết quả cuối cùng
        iRenderer->showResult(gameResult.winner, gameResult.isBot, winLine ? &(*winLine) : nullptr);
        iInteraction->pause();
    } else if (config->judge_mode) {
        // mode judge (không interactive)
        iRenderer->printResult(gameResult);
    }

    // log thống kê
    std::stringstream ss;
    ss << "after " << gameResult.turns << " turns";
    Logger::log(ss.str(), Logger::Level::DEBUG);

    ss.str(std::string());
    ss.clear();

    ss << "game end with result: ";
    if (gameResult.winner == -1)
        ss << "draw";
    else
        ss << "player " << gameResult.winner + 1
           << " (" << (gameResult.isBot ? "bot" : "human") << ") "
           << "win!";
    Logger::log(ss.str(), Logger::Level::DEBUG);

    Logger::log("[Engine] Game ended!");
}

/**
 * Mô tả:
 *   Dọn dẹp tài nguyên của engine.
 *
 * Đầu vào:
 *   - Không có
 *
 * Đầu ra:
 *   - Không có
 *
 * Tác dụng phụ:
 *   - Gọi close() của renderer và interaction
 */
void Engine::close() {
    Logger::log("Engine closing . . .");

    iRenderer->close();
    iInteraction->close();

    Logger::log("Engine closed!");
}