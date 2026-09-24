/**
 * Terminal Interaction cpp implementation
 *
 */

#include "interaction.h"

/* ---------- Importing ---------- */

#include <chrono>
#include <format>
#include <iostream>
#include <thread>

#include "../utils/logger.h"

/* ---------- Definitions ---------- */

TerminalInteraction::TerminalInteraction() {
}

/**
 * Mô tả: Destructor của TerminalInteraction.
 * Đầu vào: Không.
 * Đầu ra: Không.
 * Tác dụng phụ: Không.
 */
TerminalInteraction::~TerminalInteraction() {
}

/**
 * Mô tả: Khởi tạo interaction (thiết lập nguồn input).
 * Đầu vào: config - cấu hình runtime.
 * Đầu ra: Không.
 * Tác dụng phụ: Có thể redirect std::cin sang file input.
 */
void TerminalInteraction::init(const RunConfig& config) {
    initInteraction(config);
}

/**
 * Mô tả: Thiết lập nguồn input cho chương trình (console hoặc file).
 * Đầu vào: config - cấu hình runtime.
 * Đầu ra: Không.
 * Tác dụng phụ:
 *   - Redirect std::cin sang file nếu chạy non-interactive.
 *   - Ghi log trạng thái.
 * NOTE: Nếu file không mở được, sẽ fallback về console.
 */
void TerminalInteraction::initInteraction(const RunConfig& config) {
    cin_backup = nullptr;

    if (!config.interactive && !config.input_file.empty()) {
        global_file_in.open(config.input_file);
        if (global_file_in.is_open()) {
            cin_backup = std::cin.rdbuf();
            std::cin.rdbuf(global_file_in.rdbuf());

            Logger::log(std::format("redirected cin to: {}", config.input_file));
        } else {
            Logger::log("failed to open input file, using console.", Logger::Level::ERROR);
        }
    }
}

/**
 * Mô tả: Khôi phục lại nguồn input ban đầu và đóng file nếu có.
 * Đầu vào: Không.
 * Đầu ra: Không.
 * Tác dụng phụ:
 *   - Reset std::cin về buffer gốc.
 *   - Đóng file input.
 */
void TerminalInteraction::closeInteraction() {
    if (cin_backup) {
        std::cin.rdbuf(cin_backup);
        Logger::log("fallback using 'std::cin' input stream.");
    }

    if (global_file_in.is_open()) {
        global_file_in.close();
    }
}

/**
 * Mô tả: Kiểm tra chuỗi input có hợp lệ (chỉ chứa ký tự số) hay không.
 * Đầu vào: input - chuỗi nhập từ người dùng.
 * Đầu ra: true nếu hợp lệ, false nếu chứa ký tự không phải số hoặc rỗng.
 * Tác dụng phụ: Không.
 * TODO:
 *   - Bước 1: Kiểm tra chuỗi rỗng.
 *   - Bước 2: Duyệt từng ký tự và kiểm tra có phải số.
 *   - Trường hợp biên: input rỗng hoặc chứa ký tự đặc biệt.
 */
bool TerminalInteraction::validateInput(const std::string& input) {
    // TODO: Kiểm tra chuỗi rỗng
    // TODO: Duyệt từng ký tự để đảm bảo toàn bộ là chữ số
    // TODO: Trả về true nếu hợp lệ, ngược lại false
    if (input.empty()) {
        return false;
    }
    for (const char &ch : input) {
        if (!isdigit(ch)) {
            return false;
        }
    }
    return true;
}

/**
 * Mô tả: Đọc input từ luồng std::cin và chuyển sang số nguyên.
 * Đầu vào: val - con trỏ lưu giá trị đọc được.
 * Đầu ra: true nếu đọc và chuyển đổi thành công, false nếu lỗi.
 * Tác dụng phụ: Ghi log khi input không hợp lệ.
 * TODO:
 *   - Bước 1: Đọc chuỗi từ std::cin.
 *   - Bước 2: Kiểm tra hợp lệ bằng validateInput.
 *   - Bước 3: Chuyển sang số bằng stoi.
 *   - Trường hợp biên: input không phải số hoặc lỗi chuyển đổi.
 */
bool TerminalInteraction::getInput(int* val) {
    // TODO: Đọc dữ liệu dạng chuỗi từ std::cin
    // TODO: Validate input
    // TODO: Chuyển đổi sang int và gán vào *val
    // TODO: Xử lý exception nếu có
    std::string input;
    if (!(std::cin >> input)) return false;
    if (!validateInput(input)) return false;
    
    try {
        *val = std::stoi(input);
    } catch (...) {
        return false;
    }
    return true;
}

/**
 * Mô tả: Tạm dừng chương trình (chờ người dùng hoặc delay theo thời gian).
 * Đầu vào: timeout - thời gian chờ (ms), nếu = 0 thì chờ input.
 * Đầu ra: Không.
 * Tác dụng phụ: Làm chậm luồng thực thi.
 * TODO:
 *   - Bước 1: Nếu timeout = 0, chờ người dùng nhấn Enter.
 *   - Bước 2: Nếu timeout > 0, sleep trong khoảng thời gian tương ứng.
 *   - Trường hợp biên: buffer input còn dữ liệu.
 */
void TerminalInteraction::pause(int timeout) {
    // TODO: Xử lý pause theo chế độ interactive hoặc delay
    if (timeout > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
        return;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

/**
 * Mô tả: Nhận input kích thước bàn cờ từ người dùng.
 * Đầu vào: size - con trỏ lưu kích thước bàn cờ.
 * Đầu ra: true nếu hợp lệ, false nếu không hợp lệ.
 * Tác dụng phụ: Ghi log nếu input sai.
 * TODO:
 *   - Bước 1: Gọi getInput để đọc giá trị.
 *   - Bước 2: Kiểm tra nằm trong [BOARD_N_MIN, BOARD_N_MAX].
 *   - Trường hợp biên: giá trị ngoài phạm vi.
 */
bool TerminalInteraction::selectSize(int* size) {
    // TODO: Đọc input size
    // TODO: Validate range hợp lệ
    if (!getInput(size)) {
        return false;
    }
    if (*size < 3 || *size > BOARD_N_MAX) {
        return false;
    }
    return true;
}

/**
 * Mô tả: Nhận input điều kiện thắng (goal).
 * Đầu vào: goal - con trỏ lưu giá trị goal, size - kích thước bàn cờ.
 * Đầu ra: true nếu hợp lệ, false nếu không.
 * Tác dụng phụ: Ghi log nếu sai.
 * TODO:
 *   - Bước 1: Đọc input goal.
 *   - Bước 2: Kiểm tra goal >= 3 và <= size.
 *   - Trường hợp biên: goal không hợp lệ.
 */
bool TerminalInteraction::selectGoal(int* goal, const int size) {
    // TODO: Đọc input goal
    // TODO: Validate điều kiện goal
    if (!getInput(goal)) {
        return false;
    }
    if (*goal < 3 || *goal > size) {
        return false;
    }
    if (*goal > GOAL_MAX) {
        return false;
    }
    return true;
}

/**
 * Mô tả: Nhận input chế độ chơi (PVP, PVE, EVE).
 * Đầu vào: mode - con trỏ lưu GameMode.
 * Đầu ra: true nếu hợp lệ, false nếu không.
 * Tác dụng phụ: Ghi log nếu sai.
 * TODO:
 *   - Bước 1: Đọc input số.
 *   - Bước 2: Map sang enum GameMode.
 *   - Trường hợp biên: giá trị ngoài [1,3].
 */
bool TerminalInteraction::selectGameMode(GameMode* mode) {
    // TODO: Đọc input mode
    // TODO: Mapping sang enum tương ứng
    int mode_input;
    if (!getInput(&mode_input)) {
        return false;
    }
    switch (mode_input) {
        case 1:
            *mode = GameMode::PVP;
            return true;
        case 2:
            *mode = GameMode::PVE;
            return true;
        case 3:
            *mode = GameMode::EVE;
            return true;
        default:
            break;
    }

    return false;
}

/**
 * Mô tả: Nhận input level cho bot.
 * Đầu vào: levels - mảng BotLevel, index - vị trí cần gán.
 * Đầu ra: true nếu hợp lệ, false nếu không.
 * Tác dụng phụ: Ghi log nếu sai.
 * TODO:
 *   - Bước 1: Đọc input level.
 *   - Bước 2: Kiểm tra index hợp lệ (0 hoặc 1).
 *   - Bước 3: Map sang enum BotLevel.
 *   - Trường hợp biên: index ngoài phạm vi hoặc level sai.
 */
bool TerminalInteraction::selectBotLevel(BotLevel* levels, const int index) {
    // TODO: Đọc input bot level
    // TODO: Validate index
    // TODO: Mapping sang BotLevel
    int level_input;
    if (!getInput(&level_input)) {
        return false;
    }
    if (index < 0 || index > 1) {
        return false;
    }
    switch (level_input) {
        case 1:
            levels[index] = BotLevel::EASY;
            return true;
        case 2:
            levels[index] = BotLevel::MEDIUM;
            return true;
        case 3:
            levels[index] = BotLevel::HARD;
            return true;
        default:
            break;
    }
    return false;
}

/**
 * Mô tả: Nhận nước đi của người chơi (row, col).
 * Đầu vào: row, col - con trỏ lưu tọa độ.
 * Đầu ra: true nếu đọc thành công, false nếu lỗi.
 * Tác dụng phụ: Không.
 * TODO:
 *   - Bước 1: Gọi getInput cho row.
 *   - Bước 2: Gọi getInput cho col.
 *   - Trường hợp biên: input không hợp lệ.
 */
bool TerminalInteraction::getPlayerMove(int* row, int* col) {
    // TODO: Đọc row và col
    if (!getInput(row)) return false;
    if (!getInput(col)) return false;
    return true;
}

/**
 * Mô tả: Đóng interaction và giải phóng tài nguyên liên quan.
 * Đầu vào: Không.
 * Đầu ra: Không.
 * Tác dụng phụ:
 *   - Khôi phục std::cin nếu đã bị redirect.
 *   - Đóng file input nếu đang mở.
 */
void TerminalInteraction::close() {
    closeInteraction();
}