/**
 * Bot LV2 cpp implementation
 *
 */

#include "bot_lv2.h"

/* ---------- Importing ---------- */

#include <sstream>

#include "../../utils/logger.h"
#include "../logic.h"

/* ---------- Definitions ---------- */

/**
 * Mô tả:
 *   Constructor của BotLevel2.
 */
BotLevel2::BotLevel2(const BotLevel& _level, const char& _symbol)
    : BotLevel1(_level, _symbol) {
}

/**
 * Mô tả:
 *   Destructor của BotLevel2.
 */
BotLevel2::~BotLevel2() {
}

/**
 * Mô tả:
 *   Trả về nước đi của bot MEDIUM.
 *
 * TODO:
 *   - Gọi heuristic để chọn nước đi
 */
pII BotLevel2::getMove(char board[][BOARD_N_MAX], const int size, const int goal) {
    // TODO: gọi simple_heuristic với symbol và op_symbol
    return simple_heuristic(board, size, goal, symbol, op_symbol);
}

/**
 * Mô tả:
 *   Heuristic đơn giản:
 *   - Ưu tiên thắng
 *   - Chặn đối thủ
 *   - Fallback random
 *
 * TODO:
 *   - Bước 1: kiểm tra nước thắng
 *   - Bước 2: kiểm tra nước chặn
 *   - Bước 3: chọn random nếu không có
 */
pII BotLevel2::simple_heuristic(char board[][BOARD_N_MAX], const int size, const int goal, const char botSymbol, const char playerSymbol) {
    // TODO: implement heuristic
    // BRILLIANT MOVEEEEE !!
    for (int r = 0; r < size; r++) {
        for (int c = 0; c < size; c++) {
            if (board[r][c] == '-') {
                board[r][c] = botSymbol;
                bool winning_move = Logic::checkWin(board, size, botSymbol, goal, EndRule::OPEN_TWO);
                board[r][c] = '-';
                if (winning_move) return {r, c};
            }
        }
    }

    // BLOCKING OPPONENT'S WINNING MOVE
    for (int r = 0; r < size; r++) {
        for (int c = 0; c < size; c++) {
            if (board[r][c] == '-') {
                board[r][c] = playerSymbol;
                bool loser = Logic::checkWin(board, size, playerSymbol, goal, EndRule::OPEN_TWO);
                board[r][c] = '-';
                if (loser) return {r, c};               
            }
        }
    }

    // FIND BEST MOVE
    int center = size / 2;
    pII best_move = {-1, -1};
    int bestScore = -1;
    for (int r = 0; r < size; r++) {
        for (int c = 0; c < size; c++) {
            if (board[r][c] != '-') continue;
            int score = 0;
            int dist = abs(r - center) + abs(c - center);
            score += (size - dist);
            for (int dr = -1; dr <= 1; dr++) {
                for (int dc = -1; dc <= 1; dc++) {
                    int nr = r + dr, nc = c + dc;
                    if (nr < 0 || nr >= size || nc < 0 || nc >= size) continue;
                    if (board[nr][nc] != '-') score += 2;
                }
            }

            if (score > bestScore) {
                bestScore = score;
                best_move = {r, c};
            }
        }
    }

    return best_move;
}