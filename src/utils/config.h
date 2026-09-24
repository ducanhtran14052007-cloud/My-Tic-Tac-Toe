/**
 * Config header file
 *
 */

#pragma once

/* ---------- Importing ---------- */

#include <string>

/* ---------- Global Variables ---------- */

// version của chương trình
// TODO: sửa <STUDENT_ID> bằng mã số sinh viên
inline const std::string VERSION = "0.4.<25022777>";

/* ---------- Type Definitions ---------- */

/**
 * Mô tả:
 *   Struct RunConfig chứa toàn bộ cấu hình chạy của chương trình.
 *   Các giá trị này thường được parse từ command line arguments.
 *
 * Đầu vào:
 *   - Được khởi tạo thông qua parseArgs()
 *
 * Đầu ra:
 *   - Cung cấp thông tin cấu hình cho các module khác (Engine, Renderer, ...)
 *
 * Tác dụng phụ:
 *   - Không có (chỉ là container dữ liệu)
 */
struct RunConfig {
    // ---------- Core ----------

    bool interactive = true;  // bật/tắt chế độ tương tác (UI + input từ người dùng)
    bool judge_mode = false;  // chế độ chấm (không interactive, dùng để test/auto-run)
    std::string input_file;   // file input (dùng trong judge mode)

    bool to_file = true;               // ghi log ra file hay không
    std::string log_file = "log.txt";  // tên file log

    bool verbose_flag = false;  // bật log chi tiết (DEBUG)
    bool gui_flag = false;      // bật giao diện GUI (SDL) thay vì terminal

    bool is_help = false;  // nếu true thì chỉ hiển thị help và thoát

    // ---------- SDL Layout ----------

    int screenWidth = 1000;  // chiều rộng cửa sổ SDL
    int screenHeight = 680;  // chiều cao cửa sổ SDL
    int boardPadding = 100;  // khoảng cách padding của board trong màn hình

    // ---------- Helper ----------

    /**
     * Mô tả:
     *   Overload operator<< để in RunConfig ra stream (ví dụ: std::cout).
     *
     * Đầu vào:
     *   - os: output stream
     *   - config: đối tượng RunConfig
     *
     * Đầu ra:
     *   - std::ostream&: stream sau khi ghi
     *
     * Tác dụng phụ:
     *   - Ghi dữ liệu vào stream
     */
    friend std::ostream& operator<<(std::ostream& os, const RunConfig& config);

    /**
     * Mô tả:
     *   Chuyển RunConfig thành chuỗi string để phục vụ logging/debug.
     *
     * Đầu vào:
     *   - Không có
     *
     * Đầu ra:
     *   - std::string: biểu diễn dạng text của config
     *
     * Tác dụng phụ:
     *   - Không có
     */
    std::string toString() const;
};

/* ---------- Declarations ---------- */

/**
 * Mô tả:
 *   Parse các tham số dòng lệnh (argc, argv) thành RunConfig.
 *
 * Đầu vào:
 *   - argc: số lượng tham số
 *   - argv: mảng tham số
 *
 * Đầu ra:
 *   - RunConfig: cấu hình đã được parse
 *
 * Tác dụng phụ:
 *   - Có thể thiết lập các flag như help, gui, judge_mode, ...
 */
RunConfig parseArgs(int argc, char* argv[]);

/**
 * Mô tả:
 *   Trả về chuỗi hướng dẫn sử dụng chương trình (help message).
 *
 * Đầu vào:
 *   - Không có
 *
 * Đầu ra:
 *   - std::string: nội dung hướng dẫn
 *
 * Tác dụng phụ:
 *   - Không có
 */
std::string configHelpStr();