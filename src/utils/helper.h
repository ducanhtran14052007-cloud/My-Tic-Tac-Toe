/**
 * Helper header file
 *
 */

#pragma once

/* ---------- Importing ---------- */

#include <functional>
#include <string>
#include <type_traits>

/* ---------- Declarations ---------- */

/**
 * Mô tả:
 *   Hàm template dùng để đo thời gian thực thi của một function bất kỳ.
 *   - Có thể bật/tắt việc đo bằng flag "enabled"
 *   - Trả về kết quả của function sau khi thực thi
 *
 * Đầu vào:
 *   - label: tên/nhãn của đoạn code (dùng để log)
 *   - func: function/lambda cần đo thời gian
 *   - enabled: nếu false thì bỏ qua việc đo thời gian
 *
 * Đầu ra:
 *   - Kiểu trả về của func (suy ra bằng std::invoke_result_t)
 *
 * Tác dụng phụ:
 *   - Có thể ghi log thời gian thực thi
 *
 * NOTE:
 *   - Dùng template để hỗ trợ mọi kiểu function (lambda, function pointer, ...)
 *   - Phải đặt implementation trong header/tpp do là template
 */
template <typename Function>
auto measureExecutionTime(const std::string& label, Function func, bool enabled)
    -> std::invoke_result_t<Function>;

// include phần implementation template
// NOTE: template phải có full definition khi compile
#include "helper.tpp"