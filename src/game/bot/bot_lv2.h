/**
 * Bot LV2 header file
 *
 */

#pragma once

/* ---------- Importing ---------- */

#include "../setup.h"
#include "bot_lv1.h"

/* ---------- Declarations ---------- */

/**
 * Mô tả:
 *   BotLevel2 là bot mức độ MEDIUM.
 *   Kế thừa từ BotLevel1 (random) nhưng bổ sung chiến lược cơ bản:
 *   - Ưu tiên nước đi thắng ngay
 *   - Chặn nước đi thắng của đối thủ
 *   - Nếu không có thì dùng heuristic đơn giản hoặc fallback random
 *
 * Đầu vào:
 *   - board, size, goal
 *
 * Đầu ra:
 *   - Một nước đi hợp lệ (row, col)
 *
 * Tác dụng phụ:
 *   - Không có
 *
 * NOTE:
 *   - Đây là bot trung cấp, chưa dùng minimax
 *   - Logic chủ yếu dựa trên kiểm tra cục bộ
 */
class BotLevel2 : public BotLevel1 {
   protected:
    /**
     * Mô tả:
     *   Áp dụng heuristic đơn giản để chọn nước đi tốt hơn random.
     *
     * Đầu vào:
     *   - board: trạng thái bàn cờ
     *   - size: kích thước
     *   - goal: điều kiện thắng
     *   - botSymbol: ký hiệu bot
     *   - playerSymbol: ký hiệu đối thủ
     *
     * Đầu ra:
     *   - pII: nước đi được đánh giá là tốt
     *
     * Tác dụng phụ:
     *   - Không có
     */
    pII simple_heuristic(char board[][BOARD_N_MAX], const int size, const int goal, const char botSymbol, const char playerSymbol);

   public:
    /**
     * Mô tả:
     *   Constructor của BotLevel2.
     */
    BotLevel2(const BotLevel& _level, const char& _symbol);

    /**
     * Mô tả:
     *   Destructor của BotLevel2.
     */
    ~BotLevel2() override;

    /**
     * Mô tả:
     *   Lấy nước đi của bot MEDIUM.
     *   Sử dụng heuristic
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