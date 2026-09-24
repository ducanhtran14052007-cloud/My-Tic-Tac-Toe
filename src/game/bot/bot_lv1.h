/**
 * Bot LV1 header file
 *
 */

#pragma once

/* ---------- Importing ---------- */

#include "../setup.h"
#include "bot.h"

/* ---------- Declarations ---------- */

/**
 * Mô tả:
 *   BotLevel1 là bot mức độ EASY.
 *   Chiến lược chính:
 *   - Chọn nước đi ngẫu nhiên trong các ô còn trống.
 *
 * Đầu vào:
 *   - Trạng thái board, size, goal
 *
 * Đầu ra:
 *   - Một nước đi hợp lệ (row, col)
 *
 * Tác dụng phụ:
 *   - Không có
 *
 * NOTE:
 *   - Đây là bot đơn giản, không có chiến lược thông minh
 *   - Phù hợp để test hoặc làm baseline
 */
class BotLevel1 : public Bot {
   protected:
    /**
     * Mô tả:
     *   Chọn ngẫu nhiên một ô trống trên bàn cờ.
     *
     * Đầu vào:
     *   - board: trạng thái bàn cờ
     *   - size: kích thước
     *
     * Đầu ra:
     *   - pII: (row, col) của ô được chọn
     *
     * Tác dụng phụ:
     *   - Không có
     *
     * NOTE:
     *   - Cần đảm bảo ô được chọn là hợp lệ
     */
    pII random_pick(char board[][BOARD_N_MAX], const int size);

   public:
    /**
     * Mô tả:
     *   Constructor của BotLevel1.
     *
     * Đầu vào:
     *   - _level: mức độ bot (EASY)
     *   - _symbol: ký hiệu bot
     *
     * Đầu ra:
     *   - Không có
     */
    BotLevel1(const BotLevel& _level, const char& _symbol);

    /**
     * Mô tả:
     *   Destructor của BotLevel1.
     */
    ~BotLevel1() override;

    /**
     * Mô tả:
     *   Lấy nước đi tiếp theo của bot.
     *   Với level EASY → thường chỉ gọi random_pick.
     *
     * Đầu vào:
     *   - board: trạng thái bàn cờ
     *   - size: kích thước
     *   - goal: số quân cần để thắng (ít dùng trong level này)
     *
     * Đầu ra:
     *   - pII: nước đi (row, col)
     *
     * Tác dụng phụ:
     *   - Không có
     */
    pII getMove(char board[][BOARD_N_MAX], const int size, const int goal) override;
};