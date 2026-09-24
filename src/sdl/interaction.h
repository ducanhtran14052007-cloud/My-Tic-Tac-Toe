/**
 * SDL Interaction header file
 *
 */

#pragma once

/* ---------- Importing ---------- */

#include <SDL2/SDL.h>

#include "../game/interface/i_interaction.h"
#include "../game/setup.h"
#include "../utils/config.h"
#include "renderer.h"

/* ---------- Declarations ---------- */

/**
 * Mô tả: Lớp xử lý tương tác người dùng sử dụng SDL (event-based).
 * Đầu vào: Không (khởi tạo qua init với RunConfig).
 * Đầu ra: Không.
 * Tác dụng phụ:
 *   - Lắng nghe và xử lý SDL events (keyboard, mouse, quit,...).
 *   - Điều khiển luồng input cho game GUI.
 * NOTE: Hoạt động theo mô hình event loop, khác với terminal input.
 */
class SDLInteraction : public I_Interaction {
   private:
    /**
     * Mô tả: Chờ event quit (đóng cửa sổ) từ người dùng.
     * Đầu vào: e - SDL_Event để lưu sự kiện nhận được.
     * Đầu ra: true nếu nhận được quit event, false nếu không.
     * Tác dụng phụ: Block cho đến khi có event.
     */
    bool waitForQuit(SDL_Event& e);
    SDLRenderer* sdlRenderer = nullptr;
    int boardSize = 0; 

    
   public:
    void setRenderer(SDLRenderer* r) { sdlRenderer = r; }

    /**
     * Mô tả: Constructor khởi tạo SDLInteraction.
     * Đầu vào: Không.
     * Đầu ra: Không.
     * Tác dụng phụ: Không.
     */
    SDLInteraction();

    /**
     * Mô tả: Destructor của SDLInteraction.
     * Đầu vào: Không.
     * Đầu ra: Không.
     * Tác dụng phụ: Không.
     */
    ~SDLInteraction();

    /**
     * Mô tả: Khởi tạo hệ thống interaction cho SDL.
     * Đầu vào: config - cấu hình runtime.
     * Đầu ra: Không.
     * Tác dụng phụ:
     *   - Chuẩn bị môi trường nhận event.
     */
    void init(const RunConfig& config) override;

    /**
     * Mô tả: Tạm dừng chương trình.
     * Đầu vào: timeout - thời gian chờ (ms), nếu = 0 thì chờ event.
     * Đầu ra: Không.
     * Tác dụng phụ:
     *   - Có thể block thread.
     * NOTE: Có thể dùng để chờ click hoặc delay frame.
     */
    void pause(int timeout = 0) override;

    /**
     * Mô tả: Nhận input kích thước bàn cờ từ người dùng (qua event).
     * Đầu vào: size - con trỏ lưu kết quả.
     * Đầu ra: true nếu hợp lệ, false nếu không.
     * Tác dụng phụ: Đọc event từ SDL.
     */
    bool selectSize(int* size) override;

    /**
     * Mô tả: Nhận input goal (điều kiện thắng).
     * Đầu vào:
     *   - goal: con trỏ lưu kết quả.
     *   - size: kích thước bàn cờ.
     * Đầu ra: true nếu hợp lệ, false nếu không.
     * Tác dụng phụ: Đọc event từ SDL.
     */
    bool selectGoal(int* goal, const int size) override;

    /**
     * Mô tả: Nhận input chế độ chơi.
     * Đầu vào: mode - con trỏ lưu GameMode.
     * Đầu ra: true nếu hợp lệ, false nếu không.
     * Tác dụng phụ: Đọc event từ SDL.
     */
    bool selectGameMode(GameMode* mode) override;

    /**
     * Mô tả: Nhận input level cho bot.
     * Đầu vào:
     *   - levels: mảng BotLevel.
     *   - index: vị trí bot cần set.
     * Đầu ra: true nếu hợp lệ, false nếu không.
     * Tác dụng phụ: Đọc event từ SDL.
     */
    bool selectBotLevel(BotLevel* levels, const int index) override;

    /**
     * Mô tả: Nhận nước đi của người chơi (qua mouse/keyboard).
     * Đầu vào: row, col - con trỏ lưu tọa độ.
     * Đầu ra: true nếu lấy được nước đi hợp lệ, false nếu không.
     * Tác dụng phụ:
     *   - Đọc event chuột hoặc bàn phím.
     * NOTE: Cần map tọa độ pixel -> tọa độ board.
     */
    bool getPlayerMove(int* row, int* col) override;

    /**
     * Mô tả: Đóng hệ thống interaction SDL.
     * Đầu vào: Không.
     * Đầu ra: Không.
     * Tác dụng phụ:
     *   - Giải phóng tài nguyên liên quan đến event (nếu có).
     */
    void close() override;
};