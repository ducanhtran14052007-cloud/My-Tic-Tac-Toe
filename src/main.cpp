/**
 * Tic-tac-toe Game in C++
 * Using Procedural Programming paradigm.
 * Approaching the game with four functional components (Logic, Interaction, UI and Helper)
 *
 * by baluong.87
 *
 * level 2.0 (OOP paradigm + SDL graphic - module)
 *
 * base from v0.5 23/02/2026
 */

/* ---------- Importing ---------- */

#include <iostream>

#include "game/engine.h"
#include "utils/config.h"
#include "utils/logger.h"

// interface
#include "game/interface/i_interaction.h"
#include "game/interface/i_renderer.h"

// terminal
#include "terminal/interaction.h"
#include "terminal/renderer.h"

// sdl - COMMENTED OUT FOR TERMINAL-ONLY BUILD
#include "sdl/interaction.h"
#include "sdl/renderer.h"

/**
 * Mô tả:
 *   Hàm main là entry point của chương trình.
 *   Thực hiện các bước:
 *   - Parse cấu hình từ command line
 *   - Khởi tạo logger
 *   - Khởi tạo hạ tầng (renderer + interaction) theo mode (terminal/SDL)
 *   - Khởi tạo và chạy game engine
 *   - Dọn dẹp tài nguyên trước khi thoát
 *
 * Đầu vào:
 *   - argc: số lượng tham số dòng lệnh
 *   - argv: mảng chứa các tham số dòng lệnh
 *
 * Đầu ra:
 *   - int: mã thoát của chương trình (0 = thành công)
 *
 * Tác dụng phụ:
 *   - Ghi log ra console/file
 *   - Cấp phát và giải phóng bộ nhớ động (new/delete)
 *   - Tương tác với hệ thống (terminal hoặc SDL)
 */
int main(int argc, char* argv[]) {
    // parse config từ command line
    RunConfig config = parseArgs(argc, argv);

    // nếu có flag help thì in hướng dẫn và thoát
    if (config.is_help) {
        std::cout << configHelpStr();
        return 0;
    }

    // khởi tạo logger với các cấu hình tương ứng
    Logger::init(config.judge_mode, config.to_file, config.log_file, config.verbose_flag);
    Logger::log("Logger initialized!");

    // log chi tiết config (DEBUG level)
    Logger::log(config.toString(), Logger::Level::DEBUG);

    // khởi tạo các interface trừu tượng
    I_Renderer* iRenderer = nullptr;
    I_Interaction* iInteraction = nullptr;

    // chọn implementation dựa trên config
    if (!config.gui_flag || config.judge_mode) {
        // sử dụng terminal (CLI)
        iRenderer = new TerminalRenderer();
        Logger::log("Terminal Renderer initialized!");

        iInteraction = new TerminalInteraction();
        Logger::log("Terminal Interaction initialized!");
    } else {
        // sử dụng SDL (GUI)
        // NOTE: SDL support disabled in this build - using terminal instead
        iRenderer = new SDLRenderer();
        Logger::log("SDL Renderer initialized!");

        iInteraction = new SDLInteraction();
        Logger::log("SDL Interaction initialized!");

        static_cast<SDLInteraction*>(iInteraction)
    ->setRenderer(static_cast<SDLRenderer*>(iRenderer));
        
        // Fallback to terminal when SDL is not available
        //iRenderer = new TerminalRenderer();
        //Logger::log("Terminal Renderer initialized (SDL not available)!");

        //iInteraction = new TerminalInteraction();
        //Logger::log("Terminal Interaction initialized (SDL not available)!");
    }
    Logger::log("Infra initialized!");

    // tạo game engine và inject dependency (config, renderer, interaction)
    Engine* engine = new Engine(
        &config,
        iRenderer,
        iInteraction);

    try {
        if (engine) {
            // khởi tạo engine (setup ban đầu)
            engine->init();

            // vòng đời game
            engine->startGame();                         // chuẩn bị bắt đầu game
            GameResult gameResult = engine->playGame();  // chạy game loop chính
            engine->endGame(gameResult);                 // xử lý sau khi game kết thúc
        }
    } catch (const QuitException& e) {
        // NOTE: bắt tín hiệu quit (ví dụ user thoát game giữa chừng)
        Logger::log("Quit signal received. Cleaning up...", Logger::Level::WARNING);
    } catch (const NotImplementedException& e) {
        // NOTE: bắt tín hiệu chưa cài đặt (ví dụ chưa hoàn thành toàn bộ mã nguồn)
        Logger::log("Some functions is not implemented. Cleaning up...", Logger::Level::WARNING);
    }

    // đóng tài nguyên nội bộ của engine
    engine->close();

    // giải phóng bộ nhớ engine
    delete engine;
    Logger::log("Engine destroyed!");

    // giải phóng renderer nếu đã được cấp phát
    if (iRenderer) {
        delete iRenderer;
    }

    // giải phóng interaction nếu đã được cấp phát
    if (iInteraction) {
        delete iInteraction;
    }
    Logger::log("Infra destroyed!");

    // đóng logger (flush và cleanup)
    Logger::log("Logger closing . . .");
    Logger::close();

    return 0;
}

/* ---------- Compile ---------- */
/**
 * Open terminal at folder 'level-2':
 *
 * 1 - Prepare build folder:
 *  mkdir build
 *  cd build
 *
 * 2 - Config project:
 *  cmake ..
 *
 * 3 - Compile:
 *  cmake --build .
 *
 * After setup, next compile only need to run 2 + 3
 */

/*
level-2/
│
├── CMakeLists.txt
│
├── build/
│
├── assets/
│
└── src/
    ├── main.cpp
    │
    ├── game/
    │   ├── engine.cpp
    │   ├── engine.h
    │   ├── logic.cpp
    │   ├── logic.h
    │   ├── setup.h
    │   │
    │   ├── bot/
    │   │   ├── bot_factory.cpp
    │   │   ├── bot_factory.h
    │   │   ├── bot_lv1.cpp
    │   │   ├── bot_lv1.h
    │   │   ├── ...
    │   │   ├── bot.cpp
    │   │   └── bot.h
    │   │
    │   └── interface/
    │       ├── i_interaction.cpp
    │       ├── i_interaction.h
    │       ├── i_renderer.cpp
    │       └── i_renderer.h
    │
    ├── sdl/
    │   ├── interaction.cpp
    │   ├── interaction.h
    │   ├── renderer.cpp
    │   └── renderer.h
    │
    ├── terminal/
    │   ├── interaction.cpp
    │   ├── interaction.h
    │   ├── renderer.cpp
    │   └── renderer.h
    │
    └── utils/
        ├── config.cpp
        ├── config.h
        ├── helper.h
        ├── helper.tpp
        ├── logger.cpp
        └── logger.h
*/