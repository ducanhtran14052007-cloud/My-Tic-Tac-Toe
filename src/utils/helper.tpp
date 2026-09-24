/**
 * Helper tpp template implementation
 *
 */

#include "helper.h"

/* ---------- Importing ---------- */

#include <chrono>
#include <sstream>

#include "logger.h"

/* ---------- Definitions ---------- */

/**
 * Mô tả:
 *   Cài đặt hàm template đo thời gian thực thi của một function.
 *   - Hỗ trợ cả function có return value và void
 *   - Có thể bật/tắt việc log thời gian bằng flag "enabled"
 *
 * Đầu vào:
 *   - label: tên/nhãn của function (dùng để log)
 *   - func: function/lambda cần thực thi
 *   - enabled: bật/tắt đo thời gian
 *
 * Đầu ra:
 *   - Nếu func có return → trả về kết quả của func
 *   - Nếu func void → không trả về gì
 *
 * Tác dụng phụ:
 *   - Ghi log thời gian thực thi (DEBUG level) nếu enabled = true
 *
 * NOTE:
 *   - Sử dụng std::invoke_result_t để suy ra kiểu trả về
 *   - Dùng if constexpr để xử lý compile-time cho void/non-void
 */
template <typename Function>
auto measureExecutionTime(const std::string& label, Function func, bool enabled)
    -> std::invoke_result_t<Function> {
    // suy ra kiểu trả về của function
    using ReturnT = std::invoke_result_t<Function>;

    // lấy thời điểm bắt đầu
    auto start = std::chrono::high_resolution_clock::now();

    // trường hợp function không trả về giá trị
    if constexpr (std::is_void_v<ReturnT>) {
        func();  // thực thi function

        auto end = std::chrono::high_resolution_clock::now();  // thời điểm kết thúc

        if (enabled) {
            // tính thời gian chạy (giây)
            std::chrono::duration<double> duration = end - start;

            // format message log
            std::stringstream msg;
            msg << "execution time of "
                << "[" << label << "()]"
                << " = " << duration.count() << "s";

            Logger::log(msg.str(), Logger::Level::DEBUG);
        }

        return;  // void

    } else {
        // trường hợp function có trả về giá trị
        ReturnT result = func();  // lưu kết quả

        auto end = std::chrono::high_resolution_clock::now();

        if (enabled) {
            std::chrono::duration<double> duration = end - start;

            std::stringstream msg;
            msg << "execution time of "
                << "[" << label << "()]"
                << " = " << duration.count() << "s";

            Logger::log(msg.str(), Logger::Level::DEBUG);
        }

        return result;  // trả về kết quả ban đầu
    }
}