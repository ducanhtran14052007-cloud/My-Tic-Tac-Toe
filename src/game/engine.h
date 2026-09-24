/**
 * Engine header file
 *
 */

#pragma once

/* ---------- Importing ---------- */

#include "../utils/config.h"
#include "interface/i_interaction.h"
#include "interface/i_renderer.h"
#include "setup.h"

/* ---------- Declarations ---------- */

/**
 * Mô tả:
 *   Lớp Engine đóng vai trò điều phối chính (core controller) của game.
 *   Quản lý vòng đời game bao gồm:
 *   - Khởi tạo (init)
 *   - Bắt đầu game (startGame)
 *   - Chạy game loop (playGame)
 *   - Kết thúc game (endGame)
 *   - Dọn dẹp tài nguyên (close)
 *
 *   Engine sử dụng các interface:
 *   - I_Renderer: để render UI
 *   - I_Interaction: để xử lý input từ người chơi
 *
 * Đầu vào:
 *   - RunConfig: cấu hình chạy chương trình
 *   - I_Renderer*: implementation cụ thể (terminal hoặc SDL)
 *   - I_Interaction*: implementation cụ thể (terminal hoặc SDL)
 *
 * Đầu ra:
 *   - Điều khiển toàn bộ quá trình chơi game
 *
 * Tác dụng phụ:
 *   - Gọi render, nhận input, thay đổi trạng thái game
 */
class Engine {
   private:
    const RunConfig* config;      // con trỏ tới cấu hình chạy chương trình (không sở hữu)
    I_Renderer* iRenderer;        // renderer dùng để vẽ UI
    I_Interaction* iInteraction;  // interaction dùng để nhận input

    GameSetup gameSetup;  // chứa thông tin setup ban đầu của game (board size, mode, ...)

    /**
     * Mô tả:
     *   Kiểm tra tính hợp lệ của cấu hình và trạng thái ban đầu trước khi chạy game.
     *
     * Đầu vào:
     *   - Không có
     *
     * Đầu ra:
     *   - bool: true nếu hợp lệ, false nếu có lỗi
     *
     * Tác dụng phụ:
     *   - Có thể log lỗi hoặc cảnh báo
     */
    bool sanity_check();

   public:
    /**
     * Mô tả:
     *   Constructor của Engine.
     *   Nhận các dependency cần thiết và lưu lại để sử dụng trong vòng đời game.
     *
     * Đầu vào:
     *   - _config: con trỏ tới cấu hình chương trình
     *   - _iRenderer: renderer cụ thể
     *   - _iInteraction: interaction cụ thể
     *
     * Đầu ra:
     *   - Không có
     *
     * Tác dụng phụ:
     *   - Thiết lập trạng thái ban đầu của engine
     */
    Engine(const RunConfig* _config, I_Renderer* _iRenderer, I_Interaction* _iInteraction);

    /**
     * Mô tả:
     *   Destructor của Engine.
     *   Dùng để giải phóng hoặc cleanup các tài nguyên nội bộ nếu có.
     *
     * Đầu vào:
     *   - Không có
     *
     * Đầu ra:
     *   - Không có
     *
     * Tác dụng phụ:
     *   - Giải phóng tài nguyên
     */
    ~Engine();

    /**
     * Mô tả:
     *   Khởi tạo engine trước khi bắt đầu game.
     *   Có thể bao gồm:
     *   - Kiểm tra config (sanity_check)
     *   - Thiết lập gameSetup
     *   - Khởi tạo renderer/interaction nếu cần
     *
     * Đầu vào:
     *   - Không có
     *
     * Đầu ra:
     *   - Không có
     *
     * Tác dụng phụ:
     *   - Thay đổi trạng thái nội bộ của engine
     */
    void init();

    /**
     * Mô tả:
     *   Thực hiện các bước chuẩn bị ngay trước khi game bắt đầu.
     *   Ví dụ:
     *   - Render màn hình chào
     *   - Reset trạng thái game
     *
     * Đầu vào:
     *   - Không có
     *
     * Đầu ra:
     *   - Không có
     *
     * Tác dụng phụ:
     *   - Gọi renderer để hiển thị UI ban đầu
     */
    void startGame();

    /**
     * Mô tả:
     *   Chạy game loop chính.
     *   Thực hiện:
     *   - Nhận input từ iInteraction
     *   - Cập nhật logic game
     *   - Render lại UI
     *   - Lặp lại cho đến khi game kết thúc
     *
     * Đầu vào:
     *   - Không có
     *
     * Đầu ra:
     *   - GameResult: kết quả cuối cùng của game
     *
     * Tác dụng phụ:
     *   - Thay đổi trạng thái game liên tục
     *   - Gọi render và interaction nhiều lần
     *
     * NOTE:
     *   Đây là phần quan trọng nhất (core loop).
     */
    GameResult playGame();

    /**
     * Mô tả:
     *   Xử lý sau khi game kết thúc.
     *   Ví dụ:
     *   - Hiển thị kết quả (win/lose/draw)
     *   - Log thông tin game
     *
     * Đầu vào:
     *   - gameResult: kết quả của game
     *
     * Đầu ra:
     *   - Không có
     *
     * Tác dụng phụ:
     *   - Render UI kết thúc
     */
    void endGame(const GameResult& gameResult);

    /**
     * Mô tả:
     *   Dọn dẹp tài nguyên trước khi engine bị hủy.
     *   Ví dụ:
     *   - Giải phóng resource trong renderer/interaction nếu cần
     *
     * Đầu vào:
     *   - Không có
     *
     * Đầu ra:
     *   - Không có
     *
     * Tác dụng phụ:
     *   - Cleanup tài nguyên nội bộ
     */
    void close();
};