/**
 * Bot LV3 header file
 *
 */

#pragma once

/* ---------- Importing ---------- */

#include "../setup.h"
#include "bot_lv2.h"

/* ---------- Declarations ---------- */

/**
 * Mô tả:
 *   BotLevel3 là bot mức độ HARD.
 *   Kế thừa từ BotLevel2 và mở rộng thêm chiến lược nâng cao.
 *
 *   Chiến lược dự kiến:
 *   - Sử dụng heuristic mạnh hơn hoặc thuật toán như minimax/alpha-beta
 *   - Đánh giá trạng thái board để chọn nước đi tối ưu
 *
 * Đầu vào:
 *   - board, size, goal
 *
 * Đầu ra:
 *   - Một nước đi tốt nhất có thể (row, col)
 *
 * Tác dụng phụ:
 *   - Không có
 *
 * NOTE:
 *   - Đây là level cao nhất → nên có chiến lược "có suy nghĩ"
 *   - Có thể tốn nhiều thời gian tính toán hơn
 */
class BotLevel3 : public BotLevel2 {
   protected:
    /**
     * Mô tả:
     *   Hàm xử lý logic nâng cao cho bot HARD.
     *   Có thể chứa:
     *   - minimax
     *   - alpha-beta pruning
     *   - heuristic scoring
     *
     * Đầu vào:
     *   - board: trạng thái bàn cờ
     *   - size: kích thước
     *   - goal: điều kiện thắng
     *
     * Đầu ra:
     *   - pII: nước đi tối ưu
     *
     * Tác dụng phụ:
     *   - Không có
     */
    pII hard_level(char board[][BOARD_N_MAX], const int size, const int goal);

   public:
    /**
     * Mô tả:
     *   Constructor của BotLevel3.
     */
    BotLevel3(const BotLevel& _level, const char& _symbol);

    /**
     * Mô tả:
     *   Destructor của BotLevel3.
     */
    ~BotLevel3() override;

    /**
     * Mô tả:
     *   Lấy nước đi của bot HARD.
     *
     *   Thứ tự ưu tiên có thể:
     *   1. Dùng thuật toán nâng cao (hard_level)
     *   2. Fallback về logic của BotLevel2 nếu cần
     *
     * Đầu vào:
     *   - board, size, goal
     *
     * Đầu ra:
     *   - pII: nước đi
     *
     * Tác dụng phụ:
     *   - Không có
     */
    pII getMove(char board[][BOARD_N_MAX], const int size, const int goal) override;
};