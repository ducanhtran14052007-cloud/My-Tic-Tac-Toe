/**
 * I Renderer cpp implementation
 *
 */

#include "i_renderer.h"

/* ---------- Importing ---------- */

#include "../../utils/logger.h"

/* ---------- Definitions ---------- */

/**
 * Mô tả:
 *   Constructor mặc định của interface I_Renderer.
 *
 * Đầu vào:
 *   - Không có
 *
 * Đầu ra:
 *   - Không có
 *
 * Tác dụng phụ:
 *   - Không có
 *
 * NOTE:
 *   - Đây là abstract class nên constructor thường không chứa logic
 *   - Các lớp con sẽ kế thừa và sử dụng interface này
 */
I_Renderer::I_Renderer() {
}

/**
 * Mô tả:
 *   Destructor ảo của I_Renderer.
 *   Đảm bảo khi delete thông qua con trỏ base sẽ gọi đúng destructor của lớp con.
 *
 * Đầu vào:
 *   - Không có
 *
 * Đầu ra:
 *   - Không có
 *
 * Tác dụng phụ:
 *   - Có thể được override ở lớp con để giải phóng tài nguyên (SDL, buffer, ...)
 */
I_Renderer::~I_Renderer() {
}