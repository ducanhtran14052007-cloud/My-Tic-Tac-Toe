/**
 * Bot header file
 *
 */

#pragma once

/* ---------- Importing ---------- */

#include "../setup.h"

/* ---------- Declarations ---------- */

/**
 * Mô tả:
 *   Class Bot là abstract base class cho tất cả các bot trong game.
 *   Các bot cụ thể (Easy, Medium, Hard, ...) sẽ kế thừa class này
 *   và implement hàm getMove().
 *
 *   Bot chịu trách nhiệm:
 *   - Phân tích trạng thái board
 *   - Quyết định nước đi tiếp theo
 *
 * Đầu vào:
 *   - Trạng thái board, kích thước, goal
 *
 * Đầu ra:
 *   - Một nước đi (row, col)
 *
 * Tác dụng phụ:
 *   - Không có (chỉ đọc board)
 *
 * NOTE:
 *   - Đây là abstract class (có hàm pure virtual)
 */
class Bot {
   protected:
    const BotLevel level;  // mức độ khó của bot
    const char symbol;     // ký hiệu của bot ('X' hoặc 'O')
    const char op_symbol;  // ký hiệu đối thủ

   public:
    /**
     * Mô tả:
     *   Constructor khởi tạo bot.
     *
     * Đầu vào:
     *   - _level: mức độ bot
     *   - _symbol: ký hiệu bot
     *
     * Đầu ra:
     *   - Không có
     *
     * Tác dụng phụ:
     *   - Thiết lập symbol và op_symbol
     *
     * NOTE:
     *   - op_symbol thường là ký hiệu còn lại ('X' <-> 'O')
     */
    Bot(const BotLevel& _level, const char& _symbol);

    /**
     * Mô tả:
     *   Destructor ảo để đảm bảo polymorphism hoạt động đúng.
     */
    virtual ~Bot();

    /**
     * Mô tả:
     *   Hàm quyết định nước đi của bot.
     *
     * Đầu vào:
     *   - board: trạng thái bàn cờ
     *   - size: kích thước board
     *   - goal: số quân cần để thắng
     *
     * Đầu ra:
     *   - pII: cặp (row, col) là nước đi
     *
     * Tác dụng phụ:
     *   - Không có
     *
     * NOTE:
     *   - Phải được implement bởi các class kế thừa
     */
    virtual pII getMove(char board[][BOARD_N_MAX], const int size, const int goal) = 0;
};