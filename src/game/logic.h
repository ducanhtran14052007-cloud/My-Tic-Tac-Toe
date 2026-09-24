/**
 * Logic header file
 *
 */

#pragma once

/* ---------- Importing ---------- */

#include <optional>

#include "setup.h"

/* ---------- Declarations ---------- */

/**
 * Mô tả:
 *   Struct biểu diễn một đường thắng (win line).
 *   Bao gồm danh sách các ô (row, col) tạo thành chuỗi thắng.
 *
 * Đầu vào:
 *   - Không có (được tạo từ logic checkWin/getWinLine)
 *
 * Đầu ra:
 *   - Danh sách tọa độ các ô thắng
 *
 * Tác dụng phụ:
 *   - Không có
 */
struct WinLine {
    std::vector<std::pair<int, int>> cells;  // danh sách các ô tạo thành đường thắng
};

namespace Logic {

/**
 * Mô tả:
 *   Khởi tạo bàn cờ rỗng với kích thước cho trước.
 *
 * Đầu vào:
 *   - board: ma trận bàn cờ
 *   - size: kích thước bàn cờ
 *
 * Đầu ra:
 *   - Không có
 *
 * Tác dụng phụ:
 *   - Ghi dữ liệu vào board (set về trạng thái ban đầu)
 */
void initBoard(char board[][BOARD_N_MAX], const int size);

/**
 * Mô tả:
 *   Kiểm tra một nước đi có hợp lệ hay không.
 *
 * Đầu vào:
 *   - board: trạng thái bàn cờ
 *   - size: kích thước
 *   - row, col: vị trí cần kiểm tra
 *
 * Đầu ra:
 *   - true nếu hợp lệ (trong phạm vi và ô trống)
 *
 * Tác dụng phụ:
 *   - Không có
 */
bool isValidMove(const char board[][BOARD_N_MAX], const int size, const int row, const int col);

/**
 * Mô tả:
 *   Thực hiện một nước đi lên bàn cờ.
 *
 * Đầu vào:
 *   - board: bàn cờ
 *   - row, col: vị trí
 *   - symbol: ký hiệu ('X' hoặc 'O')
 *
 * Đầu ra:
 *   - Không có
 *
 * Tác dụng phụ:
 *   - Ghi giá trị vào board[row][col]
 */
void makeMove(char board[][BOARD_N_MAX], const int row, const int col, const char symbol);

/**
 * Mô tả:
 *   Kiểm tra một ô có phải là "đầu mở" (empty head) hay không.
 *   Thường dùng trong logic nâng cao (Gomoku).
 *
 * Đầu vào:
 *   - board: bàn cờ
 *   - size: kích thước
 *   - x, y: vị trí cần kiểm tra
 *   - symbol: ký hiệu của player
 *
 * Đầu ra:
 *   - true nếu là đầu mở hợp lệ
 *
 * Tác dụng phụ:
 *   - Không có
 */
bool isEmptyHead(const char board[][BOARD_N_MAX], const int size, int x, int y, const char symbol);

/**
 * Mô tả:
 *   Kiểm tra điều kiện thắng của một player.
 *
 * Đầu vào:
 *   - board: bàn cờ
 *   - size: kích thước
 *   - symbol: ký hiệu player
 *   - goal: số quân cần để thắng
 *   - rule: quy tắc kiểm tra đầu mở (mặc định OPEN_TWO)
 *
 * Đầu ra:
 *   - true nếu player thắng
 *
 * Tác dụng phụ:
 *   - Không có
 *
 * NOTE:
 *   - Có thể dùng cho cả Tic-tac-toe và Gomoku
 */
bool checkWin(char board[][BOARD_N_MAX], const int size, const char symbol, const int goal, EndRule rule = EndRule::OPEN_TWO);

/**
 * Mô tả:
 *   Kiểm tra trạng thái hòa (draw).
 *
 * Đầu vào:
 *   - board: bàn cờ
 *   - size: kích thước
 *
 * Đầu ra:
 *   - true nếu không còn nước đi hợp lệ và không có người thắng
 *
 * Tác dụng phụ:
 *   - Không có
 */
bool checkDraw(char board[][BOARD_N_MAX], const int size);

/**
 * Mô tả:
 *   Lấy thông tin đường thắng nếu tồn tại.
 *
 * Đầu vào:
 *   - board: bàn cờ
 *   - size: kích thước
 *   - symbol: player cần kiểm tra
 *   - goal: số quân để thắng
 *   - rule: quy tắc đầu mở
 *
 * Đầu ra:
 *   - std::optional<WinLine>:
 *       + Có giá trị nếu tồn tại đường thắng
 *       + nullopt nếu không có
 *
 * Tác dụng phụ:
 *   - Không có
 *
 * NOTE:
 *   - Dùng để highlight đường thắng trên UI
 */
std::optional<WinLine> getWinLine(
    const char board[][BOARD_N_MAX],
    const int size,
    const char symbol,
    const int goal,
    EndRule rule = EndRule::OPEN_TWO);

}  // namespace Logic