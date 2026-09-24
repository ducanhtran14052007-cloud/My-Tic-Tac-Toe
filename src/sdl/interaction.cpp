/**
 * SDL Interaction cpp implementation
 *
 */

#include "interaction.h"

/* ---------- Importing ---------- */

#include <format>
#include <iostream>

/* ---------- Definitions ---------- */

/**
 * Mô tả: Kiểm tra sự kiện quit từ SDL.
 * Đầu vào:
 *   - e: SDL_Event hiện tại.
 * Đầu ra:
 *   - false nếu không phải quit event.
 * Tác dụng phụ:
 *   - Ném QuitException nếu nhận được sự kiện thoát.
 */
bool SDLInteraction::waitForQuit(SDL_Event& e) {
    if (e.type == SDL_QUIT) {
        throw QuitException();
    }
    return false;
}

/**
 * Mô tả: Khởi tạo đối tượng SDLInteraction.
 * Đầu vào: Không có.
 * Đầu ra: Không có.
 * Tác dụng phụ: Không có.
 */
SDLInteraction::SDLInteraction() {
}

/**
 * Mô tả: Destructor của SDLInteraction.
 * Đầu vào: Không có.
 * Đầu ra: Không có.
 * Tác dụng phụ: Không có.
 */
SDLInteraction::~SDLInteraction() {
}

/**
 * Mô tả: Khởi tạo hệ thống interaction cho SDL.
 * Đầu vào:
 *   - config: cấu hình runtime của chương trình.
 * Đầu ra: Không có.
 * Tác dụng phụ:
 *   - Thiết lập trạng thái ban đầu cho input SDL.
 */
void SDLInteraction::init(const RunConfig& config) {
    // TODO:
    // - Khởi tạo các thành phần cần thiết cho input SDL
    // - Có thể reset event queue hoặc trạng thái input
    (void)config;
}

/**
 * Mô tả: Tạm dừng chương trình trong SDL.
 *        - Nếu có timeout > 0: delay trong khoảng thời gian tương ứng.
 *        - Nếu timeout == 0: chờ người dùng tương tác (nhấn phím hoặc click chuột).
 * Đầu vào:
 *   - timeout: thời gian chờ (milliseconds). Nếu = 0 thì chờ event từ người dùng.
 * Đầu ra: Không có.
 * Tác dụng phụ:
 *   - Có thể block thread hiện tại.
 *   - Có thể ném QuitException nếu người dùng đóng cửa sổ.
 */
void SDLInteraction::pause(int timeout) {
    // Nếu có timeout cụ thể -> delay trực tiếp bằng SDL
    if (timeout > 0) {
        SDL_Delay(timeout);
        return;
    }

    bool waiting = true;
    SDL_Event event;

    // Vòng lặp chờ sự kiện từ người dùng
    while (waiting) {
        // SDL_WaitEvent sẽ block cho tới khi có event
        if (SDL_WaitEvent(&event)) {
            // Nếu người dùng đóng cửa sổ -> thoát game
            if (waitForQuit(event)) {
            }

            // tương tự
            // if (event.type == SDL_QUIT) {
            //     throw QuitException();
            // }

            // Nếu có tương tác (nhấn phím hoặc click chuột) -> kết thúc pause
            if (event.type == SDL_KEYDOWN || event.type == SDL_MOUSEBUTTONDOWN) {
                waiting = false;
            }
        }
    }
}

/**
 * Mô tả: Lấy input kích thước bàn cờ từ người dùng qua SDL.
 * Đầu vào:
 *   - size: con trỏ lưu kết quả kích thước.
 * Đầu ra:
 *   - true nếu input hợp lệ, false nếu không hợp lệ.
 * Tác dụng phụ:
 *   - Cập nhật giá trị tại size nếu thành công.
 */
bool SDLInteraction::selectSize(int* size) {
    // TODO:
    // - Lắng nghe event từ SDL (keyboard/mouse)
    // - Parse input thành số nguyên
    // - Kiểm tra điều kiện hợp lệ (BOARD_N_MIN <= size <= BOARD_N_MAX)
    // - Trả về true nếu hợp lệ, ngược lại false
    SDL_Event e;
    if (sdlRenderer) sdlRenderer->clearInputBuffer();

    while (true) {
        if (!SDL_WaitEvent(&e)) continue;
        waitForQuit(e);

        if (e.type == SDL_KEYDOWN) {
            SDL_Keycode sym = e.key.keysym.sym;

            if (sym >= SDLK_0 && sym <= SDLK_9) {
                if (sdlRenderer) {
                    sdlRenderer->appendInputBuffer(char('0' + (sym - SDLK_0)));
                    sdlRenderer->showSelectMenu(SelectType::SIZE_UI);
                }

            } else if (sym == SDLK_BACKSPACE) {
                if (sdlRenderer) {
                    sdlRenderer->popInputBuffer();
                    sdlRenderer->showSelectMenu(SelectType::SIZE_UI);
                }

            } else if (sym == SDLK_RETURN || sym == SDLK_KP_ENTER) {
                std::string buf = sdlRenderer ? sdlRenderer->getInputBuffer() : "";
                if (buf.empty()) continue;

                int val = std::stoi(buf);
                if (sdlRenderer) sdlRenderer->clearInputBuffer();

                if (val >= BOARD_N_MIN && val <= BOARD_N_MAX) {
                    *size     = val;
                    boardSize = val;
                    return true;
                }
                return false;
            }
        }
    }
    
}

/**
 * Mô tả: Lấy input goal (số ô liên tiếp để thắng).
 * Đầu vào:
 *   - goal: con trỏ lưu giá trị goal.
 *   - size: kích thước bàn cờ hiện tại.
 * Đầu ra:
 *   - true nếu hợp lệ, false nếu không hợp lệ.
 * Tác dụng phụ:
 *   - Cập nhật goal nếu hợp lệ.
 */
bool SDLInteraction::selectGoal(int* goal, const int size) {
    // TODO:
    // - Lấy input từ SDL
    // - Parse thành số nguyên
    // - Kiểm tra điều kiện (3 <= goal <= size)
    // - Trả về true nếu hợp lệ
    SDL_Event e;
    if (sdlRenderer) sdlRenderer->clearInputBuffer();

    while (true) {
        if (!SDL_WaitEvent(&e)) continue;
        waitForQuit(e);

        if (e.type == SDL_KEYDOWN) {
            SDL_Keycode sym = e.key.keysym.sym;

            if (sym >= SDLK_0 && sym <= SDLK_9) {
                if (sdlRenderer) {
                    sdlRenderer->appendInputBuffer(char('0' + (sym - SDLK_0)));
                    sdlRenderer->showSelectMenu(SelectType::GOAL_UI, size);
                }

            } else if (sym == SDLK_BACKSPACE) {
                if (sdlRenderer) {
                    sdlRenderer->popInputBuffer();
                    sdlRenderer->showSelectMenu(SelectType::GOAL_UI, size);
                }

            } else if (sym == SDLK_RETURN || sym == SDLK_KP_ENTER) {
                std::string buf = sdlRenderer ? sdlRenderer->getInputBuffer() : "";
                if (buf.empty()) continue;

                int val = std::stoi(buf);
                if (sdlRenderer) sdlRenderer->clearInputBuffer();

                int maxGoal = (size < GOAL_MAX) ? size : GOAL_MAX;
                if (val >= 3 && val <= maxGoal) {
                    *goal = val;
                    return true;
                }
                return false;
            }
        }
    }
}

/**
 * Mô tả: Lấy input chế độ chơi (PVP, PVE, EVE).
 * Đầu vào:
 *   - mode: con trỏ lưu giá trị GameMode.
 * Đầu ra:
 *   - true nếu hợp lệ, false nếu không hợp lệ.
 * Tác dụng phụ:
 *   - Cập nhật mode nếu hợp lệ.
 */
bool SDLInteraction::selectGameMode(GameMode* mode) {
    // TODO:
    // - Lắng nghe input từ SDL (phím số hoặc click)
    // - Map input sang GameMode tương ứng
    // - Validate giá trị (1-3)
    static const int cardW  = 200, cardH = 200, gap = 40;
    static const int totalW = 3 * cardW + 2 * gap;
    static const int startX = (1000 - totalW) / 2;
    static const int cardY  = 220;

    SDL_Event e;
    while (true) {
        if (!SDL_WaitEvent(&e)) continue;
        waitForQuit(e);

        if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            int mx = e.button.x;
            int my = e.button.y;

            if (my < cardY || my > cardY + cardH) continue;

            for (int i = 0; i < 3; i++) {
                int cx = startX + i * (cardW + gap);
                if (mx >= cx && mx <= cx + cardW) {
                    switch (i) {
                        case 0: *mode = GameMode::PVP; return true;
                        case 1: *mode = GameMode::PVE; return true;
                        case 2: *mode = GameMode::EVE; return true;
                    }
                }
            }
        }

        // Fallback: phím 1/2/3
        if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_1: *mode = GameMode::PVP; return true;
                case SDLK_2: *mode = GameMode::PVE; return true;
                case SDLK_3: *mode = GameMode::EVE; return true;
                default: *mode = GameMode::INVALID_MODE; return false;
            }
        }
    }
}

/**
 * Mô tả: Lấy input cấp độ bot.
 * Đầu vào:
 *   - levels: mảng lưu cấp độ bot.
 *   - index: vị trí bot cần gán.
 * Đầu ra:
 *   - true nếu hợp lệ, false nếu không hợp lệ.
 * Tác dụng phụ:
 *   - Cập nhật levels[index] nếu hợp lệ.
 */
bool SDLInteraction::selectBotLevel(BotLevel* levels, const int index) {
    // TODO:
    // - Kiểm tra index hợp lệ (0 hoặc 1)
    // - Lấy input từ SDL
    // - Map sang BotLevel (EASY, MEDIUM, HARD)
    // - Trả về true nếu hợp lệ
    if (index < 0 || index > 1) return false;

    static const int btnW   = 120, btnH = 60, gap = 30;
    static const int totalW = 3 * btnW + 2 * gap;
    static const int startX = (1000 - totalW) / 2;
    static const int btnY   = 160;

    SDL_Event e;
    while (true) {
        if (!SDL_WaitEvent(&e)) continue;
        waitForQuit(e);

        if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            int mx = e.button.x;
            int my = e.button.y;

            if (my < btnY || my > btnY + btnH) continue;

            for (int i = 0; i < 3; i++) {
                int bx = startX + i * (btnW + gap);
                if (mx >= bx && mx <= bx + btnW) {
                    switch (i) {
                        case 0: levels[index] = BotLevel::EASY;   return true;
                        case 1: levels[index] = BotLevel::MEDIUM; return true;
                        case 2: levels[index] = BotLevel::HARD;   return true;
                    }
                }
            }
        }

        // Fallback phím số
        if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_1: levels[index] = BotLevel::EASY;   return true;
                case SDLK_2: levels[index] = BotLevel::MEDIUM; return true;
                case SDLK_3: levels[index] = BotLevel::HARD;   return true;
                default:     levels[index] = BotLevel::INVALID_LV; return false;
            }
        }
    }
}

/**
 * Mô tả: Lấy nước đi từ người chơi (row, col) qua SDL.
 * Đầu vào:
 *   - row: con trỏ lưu hàng.
 *   - col: con trỏ lưu cột.
 * Đầu ra:
 *   - true nếu lấy được input hợp lệ, false nếu không.
 * Tác dụng phụ:
 *   - Cập nhật row, col nếu hợp lệ.
 */
bool SDLInteraction::getPlayerMove(int* row, int* col) {
    // TODO:
    // - Lắng nghe mouse click hoặc keyboard input
    // - Chuyển đổi tọa độ click thành (row, col)
    // - Validate phạm vi hợp lệ
    SDL_Event e;

    while (true) {
        if (!SDL_WaitEvent(&e)) continue;
        waitForQuit(e);

        if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            int mx = e.button.x;
            int my = e.button.y;

            int boardW  = boardSize * 50;          // CELL_SIZE = 50
            int offsetX = (1000 - boardW) / 2;     // SCREEN_W  = 1000
            int offsetY = (680  - boardW) / 2;     // SCREEN_H  = 680

            if (mx < offsetX || mx >= offsetX + boardW ||
                my < offsetY || my >= offsetY + boardW) continue;

            *col = (mx - offsetX) / 50;
            *row = (my - offsetY) / 50;
            return true;
        }
    }
}

/**
 * Mô tả: Dọn dẹp tài nguyên liên quan đến SDL interaction.
 * Đầu vào: Không có.
 * Đầu ra: Không có.
 * Tác dụng phụ:
 *   - Giải phóng hoặc reset trạng thái input nếu cần.
 */
void SDLInteraction::close() {
    // TODO:
    // - Dọn dẹp trạng thái, nếu có resource thì giải phóng
}