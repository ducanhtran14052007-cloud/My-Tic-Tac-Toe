/**
 * Terminal Interaction header file
 *
 */

#pragma once

/* ---------- Importing ---------- */

#include <fstream>

#include "../game/interface/i_interaction.h"
#include "../game/setup.h"
#include "../utils/config.h"

/* ---------- Declarations ---------- */

/**
 * Mô tả:
 *   TerminalInteraction là lớp triển khai I_Interaction cho môi trường terminal.
 *
 *   Nhiệm vụ:
 *   - Nhận input từ người dùng qua console
 *   - Hỗ trợ đọc input từ file (judge mode hoặc test)
 *   - Validate và parse input thành dữ liệu hợp lệ
 *
 * Đầu vào:
 *   - Dữ liệu từ stdin hoặc file
 *
 * Đầu ra:
 *   - Giá trị nhập hợp lệ (size, goal, move, ...)
 *
 * Tác dụng phụ:
 *   - Có thể thay đổi nguồn input (redirect cin)
 *
 * NOTE:
 *   - Hỗ trợ cả interactive mode và judge mode
 */
class TerminalInteraction : public I_Interaction {
   private:
    std::ifstream global_file_in;  // stream đọc từ file input
    std::streambuf* cin_backup;    // backup buffer của std::cin để restore sau

   public:
    /**
     * Mô tả:
     *   Constructor của TerminalInteraction.
     */
    TerminalInteraction();

    /**
     * Mô tả:
     *   Destructor của TerminalInteraction.
     */
    ~TerminalInteraction();

    /**
     * Mô tả:
     *   Khởi tạo interaction dựa trên config.
     *
     * Đầu vào:
     *   - config: cấu hình chạy chương trình
     *
     * Đầu ra:
     *   - Không có
     *
     * Tác dụng phụ:
     *   - Có thể redirect input sang file
     */
    void init(const RunConfig& config) override;

    /**
     * Mô tả:
     *   Thiết lập nguồn input (console hoặc file).
     *
     * Đầu vào:
     *   - config
     *
     * Đầu ra:
     *   - Không có
     *
     * Tác dụng phụ:
     *   - Thay đổi buffer của std::cin
     */
    void initInteraction(const RunConfig& config);

    /**
     * Mô tả:
     *   Đóng interaction và khôi phục lại trạng thái ban đầu của cin.
     *
     * Đầu vào:
     *   - Không có
     *
     * Đầu ra:
     *   - Không có
     *
     * Tác dụng phụ:
     *   - Restore cin buffer
     */
    void closeInteraction();

    /**
     * Mô tả:
     *   Kiểm tra tính hợp lệ của chuỗi input.
     *
     * Đầu vào:
     *   - input: chuỗi nhập vào
     *
     * Đầu ra:
     *   - true nếu hợp lệ, false nếu không
     *
     * NOTE:
     *   - Có thể dùng để kiểm tra format số hoặc tuple
     */
    bool validateInput(const std::string& input);

    /**
     * Mô tả:
     *   Đọc một giá trị số nguyên từ input.
     *
     * Đầu vào:
     *   - val: con trỏ lưu kết quả
     *
     * Đầu ra:
     *   - true nếu đọc thành công
     *   - false nếu lỗi
     *
     * Tác dụng phụ:
     *   - Đọc dữ liệu từ cin hoặc file
     */
    bool getInput(int* val);

    /**
     * Mô tả:
     *   Tạm dừng chương trình (chờ user hoặc timeout).
     *
     * Đầu vào:
     *   - timeout: thời gian chờ (ms)
     *
     * Đầu ra:
     *   - Không có
     */
    void pause(int timeout = 0) override;

    /**
     * Mô tả:
     *   Nhập kích thước board.
     *
     * Đầu vào:
     *   - size: con trỏ lưu kết quả
     *
     * Đầu ra:
     *   - true nếu hợp lệ
     *   - false nếu không
     */
    bool selectSize(int* size) override;

    /**
     * Mô tả:
     *   Nhập goal (số quân cần để thắng).
     *
     * Đầu vào:
     *   - goal: con trỏ lưu kết quả
     *   - size: kích thước board (để validate)
     *
     * Đầu ra:
     *   - true nếu hợp lệ
     *   - false nếu không
     */
    bool selectGoal(int* goal, const int size) override;

    /**
     * Mô tả:
     *   Chọn game mode (PVP, PVE, EVE).
     *
     * Đầu vào:
     *   - mode: con trỏ lưu kết quả
     *
     * Đầu ra:
     *   - true nếu hợp lệ
     *   - false nếu không
     */
    bool selectGameMode(GameMode* mode) override;

    /**
     * Mô tả:
     *   Chọn level cho bot.
     *
     * Đầu vào:
     *   - levels: mảng lưu level
     *   - index: vị trí bot (0 hoặc 1)
     *
     * Đầu ra:
     *   - true nếu hợp lệ
     *   - false nếu không
     */
    bool selectBotLevel(BotLevel* levels, const int index) override;

    /**
     * Mô tả:
     *   Nhập nước đi của người chơi (row, col).
     *
     * Đầu vào:
     *   - row, col: con trỏ lưu kết quả
     *
     * Đầu ra:
     *   - true nếu parse thành công
     *   - false nếu lỗi format
     */
    bool getPlayerMove(int* row, int* col) override;

    /**
     * Mô tả:
     *   Đóng interaction.
     *
     * Đầu vào:
     *   - Không có
     *
     * Đầu ra:
     *   - Không có
     *
     * Tác dụng phụ:
     *   - Gọi closeInteraction()
     */
    void close() override;
};