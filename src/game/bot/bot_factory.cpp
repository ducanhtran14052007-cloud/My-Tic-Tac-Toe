/**
 * Bot Factory cpp implementation
 *
 */

#include "bot_factory.h"

/* ---------- Importing ---------- */

#include "bot_lv1.h"
#include "bot_lv2.h"
#include "bot_lv3.h"

/* ---------- Definitions ---------- */

/**
 * Mô tả:
 *   Tạo đối tượng Bot tương ứng với level.
 *
 * Đầu vào:
 *   - level: mức độ bot (EASY, MEDIUM, HARD)
 *   - symbol: ký hiệu của bot ('X' hoặc 'O')
 *
 * Đầu ra:
 *   - Bot*: con trỏ tới bot được tạo (hoặc nullptr nếu không hợp lệ)
 *
 * Tác dụng phụ:
 *   - Cấp phát bộ nhớ động (new)
 *
 * TODO:
 *   - Bước 1: Kiểm tra level
 *   - Bước 2: Tạo instance bot tương ứng với level
 *   - Bước 3: Trả về con trỏ bot
 *   - Trường hợp biên: level không hợp lệ → trả về nullptr
 */
Bot* BotFactory::createBot(BotLevel level, const char& symbol) {
    // TODO: implement factory logic

    switch (level) {
        case BotLevel::EASY:
            // TODO: tạo bot level EASY (ví dụ BotLevel1)
            return new BotLevel1(level, symbol);

        case BotLevel::MEDIUM:
            // TODO: tạo bot level MEDIUM (ví dụ BotLevel2)
            return new BotLevel2(level, symbol);

        case BotLevel::HARD:
            // TODO: tạo bot level HARD (ví dụ BotLevel3)
            return new BotLevel3(level, symbol);

        default:
            // TODO: xử lý level không hợp lệ
            return nullptr;
    }
}