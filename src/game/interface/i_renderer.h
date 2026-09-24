/**
 * I Renderer header file
 *
 */

#pragma once

/* ---------- Importing ---------- */

#include "../../utils/config.h"
#include "../logic.h"
#include "../setup.h"

/* ---------- Declarations ---------- */

/**
 * Mô tả:
 *   Interface I_Renderer định nghĩa các hàm cần thiết để hiển thị UI của game.
 *   Các implementation cụ thể (TerminalRenderer, SDLRenderer, ...) sẽ kế thừa class này.
 *
 *   Engine sẽ sử dụng interface này để:
 *   - Render board
 *   - Hiển thị menu chọn
 *   - Hiển thị kết quả
 *
 * Đầu vào:
 *   - Các dữ liệu game (board, player, result, ...)
 *
 * Đầu ra:
 *   - Hiển thị UI (console hoặc GUI)
 *
 * Tác dụng phụ:
 *   - In ra màn hình (terminal hoặc cửa sổ đồ họa)
 *
 * NOTE:
 *   - Đây là abstract class (có các hàm pure virtual)
 */
class I_Renderer {
   public:
    /**
     * Mô tả:
     *   Constructor mặc định.
     */
    I_Renderer();

    /**
     * Mô tả:
     *   Destructor ảo để đảm bảo giải phóng đúng khi dùng polymorphism.
     */
    virtual ~I_Renderer();

    /**
     * Mô tả:
     *   Khởi tạo renderer với cấu hình.
     *
     * Đầu vào:
     *   - config: cấu hình chương trình
     *
     * Đầu ra:
     *   - Không có
     *
     * Tác dụng phụ:
     *   - Thiết lập trạng thái ban đầu của renderer
     */
    virtual void init(const RunConfig& config) = 0;

    /**
     * Mô tả:
     *   Xóa màn hình hiện tại (clear console hoặc frame).
     */
    virtual void clearScreen() = 0;

    /**
     * Mô tả:
     *   Hiển thị menu chọn (size, goal, mode, ...)
     *
     * Đầu vào:
     *   - selectType: loại menu cần hiển thị
     *   - context: thông tin bổ sung (ví dụ size hiện tại)
     */
    virtual void showSelectMenu(SelectType selectType, int context = NO_CONTEXT) = 0;

    /**
     * Mô tả:
     *   Hiển thị thông báo khi người dùng chọn không hợp lệ.
     *
     * Đầu vào:
     *   - selectType: loại menu
     *   - context: giá trị đã nhập sai
     */
    virtual void showInvalidSelect(SelectType selectType, int context = NO_CONTEXT) = 0;

    /**
     * Mô tả:
     *   Hiển thị xác nhận khi người dùng chọn hợp lệ.
     *
     * Đầu vào:
     *   - selectType: loại menu
     *   - context: giá trị đã chọn
     */
    virtual void showValidSelect(SelectType selectType, int context = NO_CONTEXT) = 0;

    /**
     * Mô tả:
     *   Hiển thị bàn cờ hiện tại.
     *
     * Đầu vào:
     *   - board: ma trận bàn cờ
     *   - size: kích thước bàn cờ
     */
    virtual void displayBoard(const char board[][BOARD_N_MAX], const int size) = 0;

    /**
     * Mô tả:
     *   Hiển thị nước đi vừa thực hiện.
     *
     * Đầu vào:
     *   - row: hàng
     *   - col: cột
     */
    virtual void showMove(const int row, const int col) = 0;

    /**
     * Mô tả:
     *   Hiển thị thông báo khi nước đi không hợp lệ.
     */
    virtual void showInvalidMove() = 0;

    /**
     * Mô tả:
     *   Hiển thị thông tin người chơi hiện tại.
     *
     * Đầu vào:
     *   - player: chỉ số player (0 hoặc 1)
     *   - is_bot: có phải bot hay không
     */
    virtual void showPlayer(const int player, const bool is_bot) = 0;

    /**
     * Mô tả:
     *   Hiển thị kết quả game (interactive mode).
     *
     * Đầu vào:
     *   - winner: người thắng (-1 nếu hòa)
     *   - is_bot: winner có phải bot hay không
     *   - winLine: thông tin đường thắng (có thể null)
     */
    virtual void showResult(const int winner, const bool is_bot, const WinLine* winLine = nullptr) = 0;

    /**
     * Mô tả:
     *   In kết quả game (judge mode, không interactive).
     *
     * Đầu vào:
     *   - gameResult: kết quả game
     */
    virtual void printResult(const GameResult& gameResult) = 0;

    /**
     * Mô tả:
     *   Dọn dẹp tài nguyên renderer trước khi kết thúc.
     */
    virtual void close() = 0;
};