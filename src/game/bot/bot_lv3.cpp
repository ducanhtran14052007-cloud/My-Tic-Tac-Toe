/**
 * Bot LV3 cpp implementation
 *
 */

#include "bot_lv3.h"

/* ---------- Importing ---------- */

#include <algorithm>
#include <limits>
#include <sstream>
#include <vector>

#include "../../utils/logger.h"
#include "../logic.h"

namespace {

constexpr int MAX_SCORE = 100000;

int evaluateLine(const char board[][BOARD_N_MAX], const int size, const int row, const int col, const int dr, const int dc,
                 const int goal, const char botSymbol, const char playerSymbol) {
    int botCount = 0;
    int oppCount = 0;
    int emptyCount = 0;

    for (int step = 0; step < goal; ++step) {
        const int r = row + dr * step;
        const int c = col + dc * step;
        if (r < 0 || r >= size || c < 0 || c >= size) {
            break;
        }

        const char cell = board[r][c];
        if (cell == botSymbol) {
            ++botCount;
        } else if (cell == playerSymbol) {
            ++oppCount;
        } else {
            ++emptyCount;
        }
    }

    if (botCount > 0 && oppCount == 0) {
        if (botCount == goal) return MAX_SCORE / 8;
        return static_cast<int>(std::pow(10, botCount)) + emptyCount * 2;
    }

    if (oppCount > 0 && botCount == 0) {
        if (oppCount == goal) return -(MAX_SCORE / 8);
        return -(static_cast<int>(std::pow(10, oppCount)) + emptyCount * 2);
    }

    return 0;
}

int evaluateBoard(const char board[][BOARD_N_MAX], const int size, const int goal, const char botSymbol, const char playerSymbol) {
    int score = 0;
    const int center = size / 2;

    for (int r = 0; r < size; ++r) {
        for (int c = 0; c < size; ++c) {
            if (board[r][c] == botSymbol) {
                score += 3 - std::abs(r - center) - std::abs(c - center);
            } else if (board[r][c] == playerSymbol) {
                score -= 3 - std::abs(r - center) - std::abs(c - center);
            }
        }
    }

    const int directions[4][2] = {{0, 1}, {1, 0}, {1, 1}, {1, -1}};

    for (int r = 0; r < size; ++r) {
        for (int c = 0; c < size; ++c) {
            for (const auto& dir : directions) {
                const int dr = dir[0];
                const int dc = dir[1];
                if (r + (goal - 1) * dr >= 0 && r + (goal - 1) * dr < size && c + (goal - 1) * dc >= 0 && c + (goal - 1) * dc < size) {
                    score += evaluateLine(board, size, r, c, dr, dc, goal, botSymbol, playerSymbol);
                }
            }
        }
    }

    return score;
}

int minimax(char board[][BOARD_N_MAX], const int size, const int goal, const int depth, int alpha, int beta,
            const bool isMaximizing, const char botSymbol, const char playerSymbol) {
    if (Logic::checkWin(board, size, botSymbol, goal, EndRule::OPEN_TWO)) {
        return MAX_SCORE + depth;
    }
    if (Logic::checkWin(board, size, playerSymbol, goal, EndRule::OPEN_TWO)) {
        return -MAX_SCORE - depth;
    }
    if (Logic::checkDraw(board, size) || depth == 0) {
        return evaluateBoard(board, size, goal, botSymbol, playerSymbol);
    }

    std::vector<pII> moves;
    for (int r = 0; r < size; ++r) {
        for (int c = 0; c < size; ++c) {
            if (board[r][c] == '-') {
                moves.push_back({r, c});
            }
        }
    }

    if (moves.empty()) {
        return 0;
    }

    if (isMaximizing) {
        int best = std::numeric_limits<int>::min();
        for (const auto& move : moves) {
            const int r = move.first;
            const int c = move.second;
            board[r][c] = botSymbol;
            const int score = minimax(board, size, goal, depth - 1, alpha, beta, false, botSymbol, playerSymbol);
            board[r][c] = '-';
            best = std::max(best, score);
            alpha = std::max(alpha, best);
            if (beta <= alpha) {
                break;
            }
        }
        return best;
    }

    int best = std::numeric_limits<int>::max();
    for (const auto& move : moves) {
        const int r = move.first;
        const int c = move.second;
        board[r][c] = playerSymbol;
        const int score = minimax(board, size, goal, depth - 1, alpha, beta, true, botSymbol, playerSymbol);
        board[r][c] = '-';
        best = std::min(best, score);
        beta = std::min(beta, best);
        if (beta <= alpha) {
            break;
        }
    }
    return best;
}

}  // namespace

/* ---------- Definitions ---------- */

/**
 * Mô tả:
 *   Constructor của BotLevel3.
 */
BotLevel3::BotLevel3(const BotLevel& _level, const char& _symbol)
    : BotLevel2(_level, _symbol) {
}

/**
 * Mô tả:
 *   Destructor của BotLevel3.
 */
BotLevel3::~BotLevel3() {
}

/**
 * Mô tả:
 *   Trả về nước đi của bot HARD.
 *
 * TODO:
 *   - Bước 1: Gọi hard_level để tìm nước đi tối ưu
 *   - Bước 2: Nếu không tìm được → fallback về logic của BotLevel2
 */
pII BotLevel3::getMove(char board[][BOARD_N_MAX], const int size, const int goal) {
    return hard_level(board, size, goal);
}

/**
 * Mô tả:
 *   Logic nâng cao cho bot HARD.
 *   Có thể sử dụng:
 *   - minimax
 *   - alpha-beta pruning
 *   - heuristic scoring
 *
 * TODO:
 *   - Bước 1: Duyệt các nước đi hợp lệ
 *   - Bước 2: Đánh giá từng nước (score)
 *   - Bước 3: Chọn nước có score tốt nhất
 *   - Trường hợp biên: không còn nước đi
 */
pII BotLevel3::hard_level(char board[][BOARD_N_MAX], const int size, const int goal) {
    std::vector<pII> legalMoves;
    for (int r = 0; r < size; ++r) {
        for (int c = 0; c < size; ++c) {
            if (board[r][c] == '-') {
                legalMoves.push_back({r, c});
            }
        }
    }

    if (legalMoves.empty()) {
        return {-1, -1};
    }

    for (const auto& move : legalMoves) {
        const int r = move.first;
        const int c = move.second;
        board[r][c] = symbol;
        if (Logic::checkWin(board, size, symbol, goal, EndRule::OPEN_TWO)) {
            board[r][c] = '-';
            return {r, c};
        }
        board[r][c] = '-';
    }

    for (const auto& move : legalMoves) {
        const int r = move.first;
        const int c = move.second;
        board[r][c] = op_symbol;
        if (Logic::checkWin(board, size, op_symbol, goal, EndRule::OPEN_TWO)) {
            board[r][c] = '-';
            return {r, c};
        }
        board[r][c] = '-';
    }

    int bestScore = std::numeric_limits<int>::lowest();
    pII bestMove = legalMoves.front();
    int depth = 3;
    if (size <= 5) {
        depth = 4;
    } else if (size <= 8) {
        depth = 3;
    } else {
        depth = 2;
    }

    int alpha = std::numeric_limits<int>::min();
    int beta = std::numeric_limits<int>::max();

    for (const auto& move : legalMoves) {
        const int r = move.first;
        const int c = move.second;
        board[r][c] = symbol;
        const int score = minimax(board, size, goal, depth - 1, alpha, beta, false, symbol, op_symbol);
        board[r][c] = '-';

        if (score > bestScore) {
            bestScore = score;
            bestMove = {r, c};
        }
        alpha = std::max(alpha, bestScore);
    }

    return bestMove;
}