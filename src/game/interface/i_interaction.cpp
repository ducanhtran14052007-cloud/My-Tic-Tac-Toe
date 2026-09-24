/**
 * Game Interaction cpp implementation
 *
 */

#include "i_interaction.h"

/* ---------- Importing ---------- */

/* ---------- Definitions ---------- */

/**
 * Mô tả:
 *   Constructor mặc định của interface I_Interaction.
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
 *   - Class này là abstract nên constructor thường không chứa logic
 */
I_Interaction::I_Interaction() {
}

/**
 * Mô tả:
 *   Destructor ảo của I_Interaction.
 *   Đảm bảo khi xóa đối tượng thông qua con trỏ base sẽ gọi đúng destructor của lớp con.
 *
 * Đầu vào:
 *   - Không có
 *
 * Đầu ra:
 *   - Không có
 *
 * Tác dụng phụ:
 *   - Có thể được override ở lớp con để cleanup tài nguyên
 */
I_Interaction::~I_Interaction() {
}