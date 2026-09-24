/**
 * Ternimal Renderer cpp implementation
 *
 */

#include "renderer.h"

/* ---------- Importing ---------- */

#include <format>
#include <iostream>

#include "../game/setup.h"
#include "../utils/config.h"

/* ---------- Definitions ---------- */

/**
 * Mô tả:
 *   Constructor của TerminalRenderer.
 *
 * TODO:
 *   - Khởi tạo trạng thái ban đầu nếu cần
 */
TerminalRenderer::TerminalRenderer() : I_Renderer() {
    // TODO: init state
}

/**
 * Mô tả:
 *   Destructor của TerminalRenderer.
 *
 * TODO:
 *   - Giải phóng tài nguyên nếu có
 */
TerminalRenderer::~TerminalRenderer() {
    // TODO: cleanup nếu cần
}

/**
 * Mô tả:
 *   Khởi tạo renderer với config.
 *
 * TODO:
 *   - Đọc config
 *   - Thiết lập môi trường hiển thị
 */
void TerminalRenderer::init(const RunConfig& config) {
    // TODO: init renderer theo config
    (void)config;
}

/**
 * Mô tả:
 *   Xóa màn hình terminal.
 *
 * TODO:
 *   - Sử dụng ANSI escape sequence để clear screen
 */
void TerminalRenderer::clearScreen() {
    // TODO: clear screen
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");    
    #endif
}

/**
 * Mô tả:
 *   Hiển thị menu lựa chọn.
 *
 * TODO:
 *   - Bước 1: kiểm tra selectType
 *   - Bước 2: in menu tương ứng
 *   - Trường hợp biên: selectType không hợp lệ
 */
void TerminalRenderer::showSelectMenu(SelectType selectType, int context) {
    // TODO: render menu theo loại
        switch (selectType) {
        case SelectType::TITLE_UI:
            std::cout << std::format(">----- Tic-tac-toe [Console v{}] -----<\n\n", VERSION);
            std::cout << "Press Enter to start . . .\n";
            break;
        case SelectType::SIZE_UI:
            std::cout << std::format("Enter board size ({} - {}): ", BOARD_N_MIN, BOARD_N_MAX);
            break;
        case SelectType::GOAL_UI:
            std::cout << std::format("Enter goal (3 - {}): ", context);
            break;
        case SelectType::GAME_MODE_UI:
            std::cout << "Select game mode:\n";
            std::cout << "  1. PVP (Player vs Player)\n";
            std::cout << "  2. PVE (Player vs Bot)\n";
            std::cout << "  3. EVE (Bot vs Bot)\n";
            std::cout << "Enter your mode: ";
            break;
        case SelectType::BOT_LEVEL_UI:
            std::cout << "Select bot level:\n";
            std::cout << "  1. Easy\n  2. Medium\n  3. Hard\n";
            std::cout << "Your choice: ";
            break;
        case SelectType::MUL_BOT_LEVEL_UI:
            std::cout << std::format("Select level for Bot {} :\n", context + 1);
            std::cout << "  1. Easy\n  2. Medium\n  3. Hard\n";
            std::cout << "Your choice: ";
            break;
        case SelectType::PLAYER_UI:
            std::cout << "Enter your move (row col): ";
            break;
        default:
            break;
    }
}

/**
 * Mô tả:
 *   Hiển thị thông báo lựa chọn không hợp lệ.
 *
 * TODO:
 *   - Bước 1: xác định loại lỗi
 *   - Bước 2: in thông báo tương ứng
 */
void TerminalRenderer::showInvalidSelect(SelectType selectType, int context) {
    // TODO: render invalid message
    switch (selectType) {
        case SelectType::SIZE_UI:
            std::cout << "Invalid size: " << context << ". Must be between " << BOARD_N_MIN 
                      << " and " << BOARD_N_MAX << "\n";
            break;
        case SelectType::GOAL_UI:
            std::cout << "Invalid goal: " << context << ". Must be between 3 and board size\n";
            break;
        case SelectType::GAME_MODE_UI:
            std::cout << "Invalid mode. Enter 1, 2 or 3\n";
            break;
        case SelectType::BOT_LEVEL_UI:
        case SelectType::MUL_BOT_LEVEL_UI:
            std::cout << "Invalid level. Enter 1, 2 or 3\n";
            break;
        default:
            std::cout << "Invalid input. Try again\n";
            break;
    }
}

/**
 * Mô tả:
 *   Hiển thị thông báo lựa chọn hợp lệ.
 *
 * TODO:
 *   - Bước 1: xác định loại select
 *   - Bước 2: in thông báo xác nhận
 */
void TerminalRenderer::showValidSelect(SelectType selectType, int context) {
    // TODO: render success message
     switch (selectType) {
        case SelectType::SIZE_UI:
            std::cout << std::format("Board size set to {}\n", context);
            break;
        case SelectType::GOAL_UI:
            std::cout << std::format("Goal set to {}\n", context);
            break;
        case SelectType::GAME_MODE_UI:
            std::cout << std::format("Mode set to: {}\n", modeToString(context));
            break;
        case SelectType::BOT_LEVEL_UI:
        case SelectType::MUL_BOT_LEVEL_UI:
            std::cout << std::format("Bot level set to: {}\n", botToString(context));
            break;
        default:
            break;
    }
}

/**
 * Mô tả:
 *   Hiển thị bàn cờ.
 *
 * TODO:
 *   - Bước 1: in header
 *   - Bước 2: in trục tọa độ
 *   - Bước 3: duyệt board và in từng ô
 */
void TerminalRenderer::displayBoard(const char board[][BOARD_N_MAX], const int size) {
    // TODO: render board
    std::cout << "\n";
    std::cout << "    ";
    for (int i=0; i < size; i++) {
        std::cout << i << " ";
    }
    std::cout << "\n";
    std::cout << "    ";
    for (int i=0;i<size*2;i++) {
        std::cout << "-";
    }
    std::cout << "\n";
    for (int i = 0; i < size; ++i) {
        if (i < 10) {
            std::cout << i << "  |";
        }
        else {
            std::cout << i << " |";
        }
        for (int j=0; j < size; j++) {
            if (board[i][j] == 'X') {
                std::cout << "\033[31m" << board[i][j] << "\033[0m ";
            } else if (board[i][j] == 'O') {
                std::cout << "\033[34m" << board[i][j] << "\033[0m ";
            } else {
                std::cout << board[i][j] << " ";
            }
        }
        std::cout << "\n";
    }
}

/**
 * Mô tả:
 *   Hiển thị thông tin player hiện tại.
 *
 * TODO:
 *   - In player index
 *   - Nếu là bot thì thêm label "(Bot)"
 */
void TerminalRenderer::showPlayer(int player, bool is_bot) {
    // TODO: render player info
    if (is_bot) {
        std::cout << std::format("Player {} (Bot) is thinking...\n", player + 1);
    } else {
        std::cout << std::format("Player {}'s turn\n", player + 1);
    }
}

/**
 * Mô tả:
 *   Hiển thị nước đi vừa thực hiện.
 *
 * TODO:
 *   - In ra tọa độ (row, col)
 */
void TerminalRenderer::showMove(const int row, const int col) {
    // TODO: render move
    std::cout << "Made move at (" << row << ", " << col << ")\n";
}

/**
 * Mô tả:
 *   Hiển thị thông báo nước đi không hợp lệ.
 *
 * TODO:
 *   - In message lỗi
 */
void TerminalRenderer::showInvalidMove() {
    // TODO: render invalid move message
    std::cout << "Invalid move! Please make the right move.\n";
}

/**
 * Mô tả:
 *   Hiển thị kết quả game.
 *
 * TODO:
 *   - Bước 1: kiểm tra draw hoặc winner
 *   - Bước 2: in kết quả
 *   - Trường hợp biên: winner = -1
 */
void TerminalRenderer::showResult(const int winner, const bool is_bot, const WinLine* winLine) {
    // TODO: render result
    if (winner == DRAW_RESULT) {
        std::cout << "It's a draw!\n";
    } else {
        if (is_bot) {
            std::cout << std::format("Player {} (Bot) wins!\n", winner + 1);
        } else {
            std::cout << std::format("Player {} wins!\n", winner + 1);
        }
    }
}

/**
 * Mô tả:
 *   In kết quả đơn giản (judge mode).
 *
 * TODO:
 *   - In winner và số lượt
 */
void TerminalRenderer::printResult(const GameResult& gameResult) {
    // TODO: print result đơn giản
    std::cout << gameResult.winner << " " << gameResult.turns << "\n";

}

/**
 * Mô tả:
 *   Đóng renderer.
 *
 * TODO:
 *   - Giải phóng tài nguyên nếu có
 */
void TerminalRenderer::close() {
    // TODO: cleanup renderer
}