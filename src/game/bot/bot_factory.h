/**
 * Bot Factory header file
 *
 */

#pragma once

/* ---------- Importing ---------- */

#include "../setup.h"
#include "bot.h"

/* ---------- Declarations ---------- */

/**
 * Mô tả:
 *   BotFactory là lớp chịu trách nhiệm tạo (instantiate) các đối tượng Bot
 *   dựa trên BotLevel được chọn.
 *
 *   Giúp tách logic khởi tạo bot ra khỏi Engine:
 *   - Engine không cần biết chi tiết các class bot cụ thể
 *   - Dễ mở rộng thêm bot mới (chỉ cần sửa factory)
 *
 * Đầu vào:
 *   - level: mức độ bot (EASY, MEDIUM, HARD, ...)
 *   - symbol: ký hiệu của bot ('X' hoặc 'O')
 *
 * Đầu ra:
 *   - Con trỏ Bot* tương ứng với level
 *
 * Tác dụng phụ:
 *   - Cấp phát bộ nhớ động (new Bot)
 *
 * NOTE:
 *   - Caller chịu trách nhiệm delete con trỏ trả về
 *   - Có thể mở rộng bằng cách thêm case trong createBot
 */
class BotFactory {
   public:
    /**
     * Mô tả:
     *   Tạo bot tương ứng với level.
     *
     * Đầu vào:
     *   - level: mức độ bot
     *   - symbol: ký hiệu bot
     *
     * Đầu ra:
     *   - Bot*: con trỏ tới bot được tạo
     *
     * Tác dụng phụ:
     *   - Cấp phát bộ nhớ động
     *
     * TODO:
     *   - Thêm hỗ trợ cho nhiều loại bot hơn
     *   - Xử lý trường hợp level không hợp lệ
     */
    static Bot* createBot(BotLevel level, const char& symbol);
};