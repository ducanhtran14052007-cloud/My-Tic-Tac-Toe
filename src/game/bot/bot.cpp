/**
 * Bot cpp implementation
 *
 */

#include "bot.h"

/* ---------- Importing ---------- */

/* ---------- Definitions ---------- */

/**
 * Mô tả:
 *   Constructor của Bot.
 *   - Khởi tạo level, symbol và xác định symbol của đối thủ (op_symbol).
 *
 * Đầu vào:
 *   - _level: mức độ bot
 *   - _symbol: ký hiệu của bot ('X' hoặc 'O')
 *
 * Đầu ra:
 *   - Không có
 *
 * Tác dụng phụ:
 *   - Gán giá trị cho các biến thành viên level, symbol, op_symbol
 *
 * NOTE:
 *   - op_symbol được suy ra từ symbol:
 *       + nếu bot là 'O' thì đối thủ là 'X'
 *       + ngược lại là 'O'
 */
Bot::Bot(const BotLevel& _level, const char& _symbol)
    : level(_level), symbol(_symbol), op_symbol((symbol == 'O') ? 'X' : 'O') {
}

/**
 * Mô tả:
 *   Destructor ảo của Bot.
 *   Đảm bảo khi delete thông qua con trỏ base sẽ gọi đúng destructor lớp con.
 *
 * Đầu vào:
 *   - Không có
 *
 * Đầu ra:
 *   - Không có
 *
 * Tác dụng phụ:
 *   - Có thể được override ở lớp con để giải phóng tài nguyên nếu cần
 */
Bot::~Bot() {
}