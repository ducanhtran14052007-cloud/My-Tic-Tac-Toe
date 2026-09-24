/**
 * Logger header file
 *
 */

#pragma once

/* ---------- Importing ---------- */

#include <fstream>
#include <string>

/* ---------- Declarations ---------- */

namespace Logger {

/**
 * Mô tả:
 *   Enum Level định nghĩa các mức độ log khác nhau.
 *
 * Đầu vào:
 *   - Không có
 *
 * Đầu ra:
 *   - Không có
 *
 * Tác dụng phụ:
 *   - Được sử dụng để phân loại log và lọc log theo mức độ
 */
enum class Level {
    DEBUG,    // log chi tiết (debug)
    INFO,     // thông tin chung
    WARNING,  // cảnh báo
    ERROR,    // lỗi nghiêm trọng
    MSG,      // message thuần (không có prefix level)
};

/**
 * Mô tả:
 *   Chuyển Level thành chuỗi string tương ứng.
 *
 * Đầu vào:
 *   - level: mức log
 *
 * Đầu ra:
 *   - std::string: tên của level
 *
 * Tác dụng phụ:
 *   - Không có
 */
inline std::string levelToString(Level level) {
    switch (level) {
        case Level::DEBUG:
            return "DEBUG";
        case Level::INFO:
            return "INFO";
        case Level::WARNING:
            return "WARN";
        case Level::ERROR:
            return "ERROR";
        case Level::MSG:
            return "";  // không hiển thị label
        default:
            return "UNKNOWN";
    }
}

/**
 * Mô tả:
 *   Mức log tối thiểu sẽ được hiển thị.
 *   Ví dụ: nếu là INFO thì DEBUG sẽ bị bỏ qua.
 *
 * NOTE:
 *   Có thể thay đổi giá trị này khi init (ví dụ verbose_flag)
 */
inline static Level min_level = Level::INFO;

// ---------- Level Color (ANSI escape codes) ----------

const std::string RESET = "\033[0m";    // reset màu
const std::string RED = "\033[31m";     // màu đỏ (error)
const std::string GREEN = "\033[32m";   // màu xanh lá (info)
const std::string YELLOW = "\033[33m";  // màu vàng (warning)
const std::string BLUE = "\033[34m";    // màu xanh dương (debug)
const std::string CYAN = "\033[36m";    // màu cyan (ít dùng)

/**
 * Mô tả:
 *   Trả về màu tương ứng với từng mức log.
 *
 * Đầu vào:
 *   - level: mức log
 *
 * Đầu ra:
 *   - std::string: mã màu ANSI
 *
 * Tác dụng phụ:
 *   - Không có
 */
inline std::string getColor(Level level) {
    switch (level) {
        case Level::DEBUG:
            return BLUE;
        case Level::INFO:
            return GREEN;
        case Level::WARNING:
            return YELLOW;
        case Level::ERROR:
            return RED;
        default:
            return RESET;
    }
}

// ---------- Static Variables ----------

// file stream để ghi log ra file
inline static std::ofstream log_file;

// bật/tắt ghi log ra file
inline static bool write_to_file = false;

// xác định có đang ở judge mode hay không (ảnh hưởng format/log behavior)
inline static bool is_judge_mode = false;

// ---------- Functions ----------

/**
 * Mô tả:
 *   Khởi tạo hệ thống logger.
 *   Thiết lập:
 *   - chế độ judge
 *   - ghi log ra file
 *   - mức log (verbose)
 *
 * Đầu vào:
 *   - judge_mode: có phải chế độ chấm hay không
 *   - to_file: có ghi log ra file hay không
 *   - path: đường dẫn file log
 *   - verbose_flag: nếu true thì bật DEBUG level
 *
 * Đầu ra:
 *   - Không có
 *
 * Tác dụng phụ:
 *   - Mở file log nếu cần
 *   - Thiết lập biến global của logger
 */
void init(bool judge_mode, bool to_file = true, const std::string& path = "log.txt", bool verbose_flag = false);

/**
 * Mô tả:
 *   Ghi một dòng log.
 *   - Có thể in ra console
 *   - Có thể ghi ra file
 *   - Có lọc theo level
 *
 * Đầu vào:
 *   - msg: nội dung log
 *   - level: mức log (mặc định INFO)
 *
 * Đầu ra:
 *   - Không có
 *
 * Tác dụng phụ:
 *   - In ra console
 *   - Ghi ra file nếu bật
 *
 * NOTE:
 *   - Log có level < min_level sẽ bị bỏ qua
 */
void log(const std::string& msg, Level level = Level::INFO);

/**
 * Mô tả:
 *   Đóng logger, giải phóng tài nguyên.
 *
 * Đầu vào:
 *   - Không có
 *
 * Đầu ra:
 *   - Không có
 *
 * Tác dụng phụ:
 *   - Đóng file log nếu đang mở
 */
void close();

}  // namespace Logger