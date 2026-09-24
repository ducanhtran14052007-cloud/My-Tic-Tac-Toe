/**
 * Interaction header file
 *
 */

#pragma once

/* ---------- Importing ---------- */

#include "../../utils/config.h"
#include "../setup.h"

/* ---------- Declarations ---------- */

/**
 * Mô tả:
 *   Interface I_Interaction định nghĩa cách hệ thống nhận input từ người dùng hoặc nguồn khác.
 *   Các implementation cụ thể (TerminalInteraction, SDLInteraction, ...) sẽ kế thừa class này.
 *
 *   Engine sử dụng interface này để:
 *   - Lấy lựa chọn cấu hình (size, goal, mode, ...)
 *   - Lấy nước đi của player
 *   - Điều khiển pause/wait
 *
 * Đầu vào:
 *   - Input từ user hoặc từ file (judge mode)
 *
 * Đầu ra:
 *   - Dữ liệu đầu vào cho game (size, move, mode, ...)
 *
 * Tác dụng phụ:
 *   - Đọc input từ console hoặc hệ thống UI
 *
 * NOTE:
 *   - Đây là abstract class (pure virtual)
 */
class I_Interaction {
   public:
    /**
     * Mô tả:
     *   Constructor mặc định.
     */
    I_Interaction();

    /**
     * Mô tả:
     *   Destructor ảo để đảm bảo giải phóng đúng khi dùng polymorphism.
     */
    virtual ~I_Interaction();

    /**
     * Mô tả:
     *   Khởi tạo interaction theo cấu hình.
     *
     * Đầu vào:
     *   - config: cấu hình chương trình
     *
     * Đầu ra:
     *   - Không có
     *
     * Tác dụng phụ:
     *   - Chuẩn bị nguồn input (stdin, file, event system, ...)
     */
    virtual void init(const RunConfig& config) = 0;

    /**
     * Mô tả:
     *   Tạm dừng chương trình (chờ user hoặc delay).
     *
     * Đầu vào:
     *   - timeout: thời gian chờ (ms), nếu = 0 thì chờ user
     *
     * Đầu ra:
     *   - Không có
     *
     * Tác dụng phụ:
     *   - Block luồng hiện tại
     */
    virtual void pause(int timeout = 0) = 0;

    /**
     * Mô tả:
     *   Nhận kích thước bàn cờ từ user.
     *
     * Đầu vào:
     *   - size: con trỏ để ghi giá trị
     *
     * Đầu ra:
     *   - bool: true nếu hợp lệ, false nếu không hợp lệ
     *
     * Tác dụng phụ:
     *   - Ghi giá trị vào *size
     */
    virtual bool selectSize(int* size) = 0;

    /**
     * Mô tả:
     *   Nhận điều kiện thắng (goal) từ user.
     *
     * Đầu vào:
     *   - goal: con trỏ để ghi giá trị
     *   - size: kích thước board (để validate)
     *
     * Đầu ra:
     *   - bool: true nếu hợp lệ
     *
     * Tác dụng phụ:
     *   - Ghi giá trị vào *goal
     */
    virtual bool selectGoal(int* goal, const int size) = 0;

    /**
     * Mô tả:
     *   Nhận chế độ chơi (PVP, PVE, EVE).
     *
     * Đầu vào:
     *   - mode: con trỏ để ghi giá trị
     *
     * Đầu ra:
     *   - bool: true nếu hợp lệ
     *
     * Tác dụng phụ:
     *   - Ghi giá trị vào *mode
     */
    virtual bool selectGameMode(GameMode* mode) = 0;

    /**
     * Mô tả:
     *   Nhận level của bot.
     *
     * Đầu vào:
     *   - levels: mảng level của các bot
     *   - index: bot cần set (0 hoặc 1)
     *
     * Đầu ra:
     *   - bool: true nếu hợp lệ
     *
     * Tác dụng phụ:
     *   - Ghi giá trị vào levels[index]
     */
    virtual bool selectBotLevel(BotLevel* levels, const int index) = 0;

    /**
     * Mô tả:
     *   Lấy nước đi của player (row, col).
     *
     * Đầu vào:
     *   - row: con trỏ lưu hàng
     *   - col: con trỏ lưu cột
     *
     * Đầu ra:
     *   - bool: true nếu input hợp lệ
     *
     * Tác dụng phụ:
     *   - Ghi giá trị vào row và col
     */
    virtual bool getPlayerMove(int* row, int* col) = 0;

    /**
     * Mô tả:
     *   Dọn dẹp tài nguyên của interaction.
     */
    virtual void close() = 0;
};