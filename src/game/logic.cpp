/**
 * Logic cpp implementation
 *
 */

#include "logic.h"

/* ---------- Importing ---------- */

#include <format>
#include <limits>
#include <sstream>

#include "../utils/logger.h"

/* ---------- Definitions ---------- */

/**
 * Mô tả: Khởi tạo bàn cờ với ký tự mặc định.
 * Đầu vào: board, size
 * Đầu ra: Không có
 * Tác dụng phụ: Ghi dữ liệu vào board
 * TODO:
 */
void Logic::initBoard(char board[][BOARD_N_MAX], const int size) {
    // TODO: implement
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            board[i][j] = '-';
        }
    }
}

/**
 * Mô tả: Kiểm tra nước đi có hợp lệ không.
 * Đầu vào: board, size, row, col
 * Đầu ra: true nếu hợp lệ
 * Tác dụng phụ: Không có
 * TODO:
 */
bool Logic::isValidMove(const char board[][BOARD_N_MAX], const int size, const int row, const int col) {
    // TODO: implement
    if (row < 0 || row >= size || col < 0 || col >= size) {
        return false;
    }
    if (board[row][col] != '-') {
        return false;
    }
    return true;
}

/**
 * Mô tả: Thực hiện một nước đi.
 * Đầu vào: board, row, col, symbol
 * Đầu ra: Không có
 * Tác dụng phụ: Ghi vào board
 * TODO:
 */
void Logic::makeMove(char board[][BOARD_N_MAX], const int row, const int col, const char symbol) {
    // TODO: implement
    board[row][col] = symbol;
}

/**
 * Mô tả: Kiểm tra một ô có phải là đầu mở hay không.
 * Đầu vào: board, size, x, y, symbol
 * Đầu ra: true nếu là đầu mở
 * Tác dụng phụ: Không có
 * TODO:
 */
bool Logic::isEmptyHead(const char board[][BOARD_N_MAX], const int size, int x, int y, const char symbol) {
    // TODO: implement
    if (x < 0 || x >= size || y < 0 || y >= size) {
        return true;
    }
    if (board[x][y] == '-') {
        return true;
    }
    if (board[x][y] == symbol) {
            return true;
    }
    return false;
}

/**
 * Mô tả: Kiểm tra điều kiện thắng.
 * Đầu vào: board, size, symbol, goal, rule
 * Đầu ra: true nếu thắng
 * Tác dụng phụ: Có thể log debug
 * TODO:
 */
bool Logic::checkWin(char board[][BOARD_N_MAX], const int size, const char symbol, const int goal, EndRule rule) {
    // TODO: implement
        return getWinLine(board, size, symbol, goal, rule).has_value();

}

/**
 * Mô tả: Kiểm tra trạng thái hòa.
 * Đầu vào: board, size
 * Đầu ra: true nếu hòa
 * Tác dụng phụ: Không có
 * TODO:
 */
bool Logic::checkDraw(char board[][BOARD_N_MAX], const int size) {
    // TODO: implement
    for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                if (board[i][j] == '-') {
                    return false;
                }
            }
        }
    return true;
}

/**
 * Mô tả: Lấy đường thắng nếu tồn tại.
 * Đầu vào: board, size, symbol, goal, rule
 * Đầu ra: optional WinLine
 * Tác dụng phụ: Không có
 * TODO:
 */
std::optional<WinLine> Logic::getWinLine(
    const char board[][BOARD_N_MAX],
    const int size,
    const char symbol,
    const int goal,
    EndRule rule) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (board[i][j] == symbol) {
                int count = 1;

                // Check ngang
                if (size-j>=goal) {
                    count=1;
                    for (int h = 1; j+h < size; h++) {  
                        if (board[i][j+h] == symbol) {
                            count++;
                        } else {
                            break;
                        }
                        if (count == goal) {
                            bool h1 = isEmptyHead(board, size, i, j-1, symbol);
                            bool h2 = isEmptyHead(board, size, i, j+count, symbol);
                            bool win = false;
                            switch (rule) {
                                case EndRule::NONE: win = true; break;
                                case EndRule::OPEN_ONE: if (h1 || h2) win = true; break;
                                case EndRule::OPEN_TWO: if (h1 && h2) win = true; break;
                            }
                            if (win) {
                                WinLine wl;
                                for (int k = 0; k < goal; k++)
                                    wl.cells.push_back({i, j+k});
                                return wl;
                            }
                        }
                    }
                }

                // Check doc
                if (size-i>=goal) {
                    count = 1;
                    for (int v = 1; i+v < size; v++) {
                        if (board[i+v][j] == symbol) {
                            count++;
                        } else {
                            break;
                        }
                        if (count == goal) {
                            bool h1 = isEmptyHead(board, size, i-1, j, symbol);
                            bool h2 = isEmptyHead(board, size, i+count, j, symbol);
                            bool win = false;
                            switch (rule) {
                                case EndRule::NONE: win = true; break;
                                case EndRule::OPEN_ONE: if (h1 || h2) win = true; break;
                                case EndRule::OPEN_TWO: if (h1 && h2) win = true; break;
                            }
                            if (win) {
                                WinLine wl;
                                for (int k = 0; k < goal; k++)
                                    wl.cells.push_back({i+k, j});
                                return wl;
                            }
                        }
                    }
                }
                // Check cheo chinh
                if (size - i >= goal && size - j >= goal) {
                    count = 1;
                    for (int diag = 1; i+diag < size && j+diag < size; diag++) {
                        if (board[i+diag][j+diag] == symbol) {
                            count++;
                        } else {
                            break;
                        }
                        if (count == goal) {
                            bool h1 = isEmptyHead(board, size, i-1, j-1, symbol);
                            bool h2 = isEmptyHead(board, size, i+count, j+count, symbol);
                            bool win = false;
                            switch (rule) {
                                case EndRule::NONE: win = true; break;
                                case EndRule::OPEN_ONE: if (h1 || h2) win = true; break;
                                case EndRule::OPEN_TWO: if (h1 && h2) win = true; break;
                            }
                            if (win) {
                                WinLine wl;
                                for (int k = 0; k < goal; k++)
                                    wl.cells.push_back({i+k, j+k});
                                return wl;
                            }
                        }
                    }
                }
                // Check cheo phu
                if (size - i >= goal && j + 1 >= goal) {
                    count = 1;
                    for (int diag = 1; i+diag < size && j-diag >= 0; diag++) {
                        if (board[i+diag][j-diag] == symbol) {
                            count++;
                        } else {
                            break;
                        }
                        if (count == goal) {
                            bool h1 = isEmptyHead(board, size, i-1, j+1, symbol);
                            bool h2 = isEmptyHead(board, size, i+count, j-count, symbol);
                            bool win = false;
                            switch (rule) {
                                case EndRule::NONE: win = true; break;
                                case EndRule::OPEN_ONE: if (h1 || h2) win = true; break;
                                case EndRule::OPEN_TWO: if (h1 && h2) win = true; break;
                            }
                            if (win) {
                                WinLine wl;
                                for (int k = 0; k < goal; k++)
                                    wl.cells.push_back({i+k, j-k});
                                return wl;
                            }
                        }
                    }
                }
            }
        }
    }

    return std::nullopt;
}