/**
 * Ternimal Renderer header file
 *
 */

#pragma once

/* ---------- Importing ---------- */

#include "../game/interface/i_renderer.h"

/* ---------- Declarations ---------- */

/**
 * Mô tả:
 *   TerminalRenderer là lớp triển khai (implementation) của I_Renderer
 *   dành cho môi trường terminal (console).
 *
 *   Nhiệm vụ:
 *   - Hiển thị UI dạng text
 *   - In bàn cờ, menu, thông báo
 *   - Xử lý render đơn giản không cần đồ họa
 *
 * Đầu vào:
 *   - Dữ liệu game (board, player, trạng thái)
 *
 * Đầu ra:
 *   - In ra màn hình console
 *
 * Tác dụng phụ:
 *   - Ghi ra stdout (console)
 *
 * NOTE:
 *   - Dùng khi không bật GUI (SDL)
 *   - Phù hợp cho debug hoặc judge mode
 */
class TerminalRenderer : public I_Renderer {
   public:
    /**
     * Mô tả:
     *   Constructor của TerminalRenderer.
     */
    TerminalRenderer();

    /**
     * Mô tả:
     *   Destructor của TerminalRenderer.
     */
    ~TerminalRenderer();

    /**
     * Mô tả:
     *   Khởi tạo renderer với config.
     *
     * Đầu vào:
     *   - config: cấu hình chạy chương trình
     *
     * Đầu ra:
     *   - Không có
     *
     * Tác dụng phụ:
     *   - Có thể thiết lập trạng thái ban đầu cho UI
     */
    void init(const RunConfig& config) override;

    /**
     * Mô tả:
     *   Xóa màn hình terminal.
     *
     * Đầu vào:
     *   - Không có
     *
     * Đầu ra:
     *   - Không có
     *
     * Tác dụng phụ:
     *   - Làm sạch nội dung console
     */
    void clearScreen() override;

    /**
     * Mô tả:
     *   Hiển thị menu lựa chọn (size, mode, bot level, ...).
     *
     * Đầu vào:
     *   - selectType: loại menu
     *   - context: thông tin bổ sung (ví dụ size)
     *
     * Đầu ra:
     *   - Không có
     */
    void showSelectMenu(SelectType selectType, int context = NO_CONTEXT) override;

    /**
     * Mô tả:
     *   Hiển thị thông báo lựa chọn không hợp lệ.
     *
     * Đầu vào:
     *   - selectType: loại menu
     *   - context: giá trị người dùng nhập
     *
     * Đầu ra:
     *   - Không có
     */
    void showInvalidSelect(SelectType selectType, int context = NO_CONTEXT) override;

    /**
     * Mô tả:
     *   Hiển thị thông báo lựa chọn hợp lệ.
     *
     * Đầu vào:
     *   - selectType: loại menu
     *   - context: giá trị hợp lệ
     *
     * Đầu ra:
     *   - Không có
     */
    void showValidSelect(SelectType selectType, int context = NO_CONTEXT) override;

    /**
     * Mô tả:
     *   Hiển thị bàn cờ hiện tại.
     *
     * Đầu vào:
     *   - board: trạng thái bàn cờ
     *   - size: kích thước
     *
     * Đầu ra:
     *   - Không có
     *
     * Tác dụng phụ:
     *   - In board ra console
     */
    void displayBoard(const char board[][BOARD_N_MAX], const int size) override;

    /**
     * Mô tả:
     *   Hiển thị nước đi vừa thực hiện.
     *
     * Đầu vào:
     *   - row, col: vị trí nước đi
     *
     * Đầu ra:
     *   - Không có
     */
    void showMove(const int row, const int col) override;

    /**
     * Mô tả:
     *   Hiển thị thông báo nước đi không hợp lệ.
     *
     * Đầu vào:
     *   - Không có
     *
     * Đầu ra:
     *   - Không có
     */
    void showInvalidMove() override;

    /**
     * Mô tả:
     *   Hiển thị thông tin người chơi hiện tại.
     *
     * Đầu vào:
     *   - player: index người chơi (0 hoặc 1)
     *   - is_bot: có phải bot không
     *
     * Đầu ra:
     *   - Không có
     */
    void showPlayer(const int player, const bool is_bot) override;

    /**
     * Mô tả:
     *   Hiển thị kết quả game (UI đầy đủ).
     *
     * Đầu vào:
     *   - winner: người thắng (-1 nếu hòa)
     *   - is_bot: người thắng có phải bot không
     *   - winLine: đường thắng (nếu có)
     *
     * Đầu ra:
     *   - Không có
     */
    void showResult(const int winner, const bool is_bot, const WinLine* winLine = nullptr) override;

    /**
     * Mô tả:
     *   In kết quả game dạng đơn giản (phục vụ judge mode).
     *
     * Đầu vào:
     *   - gameResult: kết quả game
     *
     * Đầu ra:
     *   - Không có
     *
     * NOTE:
     *   - Không in UI phức tạp, chỉ output cần thiết
     */
    void printResult(const GameResult& gameResult) override;

    /**
     * Mô tả:
     *   Đóng renderer, giải phóng tài nguyên nếu có.
     *
     * Đầu vào:
     *   - Không có
     *
     * Đầu ra:
     *   - Không có
     */
    void close() override;
};