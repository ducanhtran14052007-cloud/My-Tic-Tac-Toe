/**
 * Setup header file
 *
 */

#pragma once

/* ---------- Importing ---------- */

#include <exception>
#include <random>
#include <utility>

/* ---------- Global Variables ---------- */

// ---------- Flag cấu hình ----------

inline const bool ALGORITHM_FLAG = true;  // bật/tắt gỡ lỗi các thuật toán
inline const bool TIME_ENABLED = true;    // bật/tắt đo thời gian thực thi
inline const int RANDOM_SEED = 8702;      // seed cố định cho random (đảm bảo reproducible)

// ---------- Constants ----------

inline const int BOARD_N_MIN = 3;    // kích thước bàn cờ nhỏ nhất
inline const int BOARD_N_MAX = 12;   // kích thước bàn cờ lớn nhất
inline const int GOAL_MAX = 5;       // số lượng quân cần để thắng tối đa
inline const int SLEEP_TIME = 1500;  // thời gian delay (ms) khi bot chơi

// ---------- Minimax / AI ----------

inline const int SCORE_INF = 1000;     // giá trị vô cùng (dùng trong minimax)
inline const int EVALUATE_SCORE = 10;  // điểm đánh giá cơ bản

// ---------- Enum ----------

/**
 * Mô tả:
 *   Mức độ khó của bot.
 */
enum class BotLevel {
    EASY,
    MEDIUM,
    HARD,
    INVALID_LV
};

/**
 * Mô tả:
 *   Chuyển BotLevel sang string để hiển thị/log.
 */
inline static std::string botToString(int v) {
    switch ((BotLevel)v) {
        case BotLevel::EASY:
            return "EASY";
        case BotLevel::MEDIUM:
            return "MEDIUM";
        case BotLevel::HARD:
            return "HARD";
        default:
            return "?";
    }
}

/**
 * Mô tả:
 *   Chế độ chơi của game.
 */
enum class GameMode {
    PVP,  // Player vs Player
    PVE,  // Player vs Bot
    EVE,  // Bot vs Bot
    INVALID_MODE
};

/**
 * Mô tả:
 *   Chuyển GameMode sang string.
 */
inline static std::string modeToString(int v) {
    switch ((GameMode)v) {
        case GameMode::PVP:
            return "PVP";
        case GameMode::PVE:
            return "PVE";
        case GameMode::EVE:
            return "EVE";
        default:
            return "?";
    }
}

/**
 * Mô tả:
 *   Loại UI selection đang hiển thị.
 *   Dùng để renderer quyết định vẽ gì.
 */
enum class SelectType {
    TITLE_UI,
    SIZE_UI,
    GOAL_UI,
    GAME_MODE_UI,
    BOT_LEVEL_UI,
    PLAYER_UI,
    MUL_BOT_LEVEL_UI,
    INVALID_UI
};

constexpr int NO_CONTEXT = -1;  // giá trị mặc định khi không có context

/**
 * Mô tả:
 *   Quy tắc kết thúc (dùng trong logic nâng cao, ví dụ Gomoku).
 */
enum class EndRule {
    NONE,      // không kiểm tra đầu mở
    OPEN_ONE,  // có ít nhất 1 đầu mở
    OPEN_TWO   // có đúng 2 đầu mở
};

// ---------- Random ----------

/**
 * Mô tả:
 *   Generator random toàn cục.
 *
 * NOTE:
 *   - Sử dụng seed cố định để đảm bảo kết quả reproducible
 *   - TODO: nên chuyển thành function để tránh global state
 */
inline std::mt19937 generator(RANDOM_SEED);

/* ---------- Type Definitions ---------- */

// alias cho pair<int, int> (tọa độ)
using pII = std::pair<int, int>;

/**
 * Mô tả:
 *   Struct chứa thông tin setup của game.
 *
 * Đầu vào:
 *   - Được thiết lập trong startGame()
 *
 * Đầu ra:
 *   - Dùng xuyên suốt game để xác định luật chơi
 *
 * Tác dụng phụ:
 *   - Không có
 */
struct GameSetup {
    char board[BOARD_N_MAX][BOARD_N_MAX];  // bàn cờ
    int size;                              // kích thước board
    int goal;                              // số quân cần để thắng
    GameMode mode;                         // chế độ chơi
    BotLevel levels[2];                    // level của 2 player (nếu là bot)
};

constexpr int DRAW_RESULT = -1;  // giá trị biểu diễn hòa

/**
 * Mô tả:
 *   Struct chứa kết quả của game.
 *
 * Đầu vào:
 *   - Được cập nhật trong game loop
 *
 * Đầu ra:
 *   - Trả về từ Engine::playGame()
 *
 * Tác dụng phụ:
 *   - Không có
 */
struct GameResult {
    int winner;  // 0 hoặc 1 là player thắng, -1 là hòa
    bool isBot;  // người thắng có phải bot không
    int turns;   // số lượt đã chơi

    /**
     * Mô tả:
     *   Constructor khởi tạo kết quả game.
     */
    GameResult(int _winner, bool _isBot, int _turns)
        : winner(_winner), isBot(_isBot), turns(_turns) {};
};

/**
 * Mô tả:
 *   Exception dùng để signal việc người dùng muốn thoát game.
 *
 * Đầu vào:
 *   - Được throw khi user yêu cầu quit
 *
 * Đầu ra:
 *   - Được catch ở tầng Engine hoặc main
 *
 * Tác dụng phụ:
 *   - Dừng flow bình thường của chương trình
 */
class QuitException : public std::exception {
   public:
    /**
     * Mô tả:
     *   Trả về thông điệp lỗi.
     */
    const char* what() const noexcept override {
        return "User requested quit";
    }
};

/**
 * Mô tả: Exception dùng để báo hiệu một chức năng chưa được cài đặt.
 *        Thường được sử dụng trong giai đoạn phát triển (development) hoặc skeleton code.
 * Đầu vào: Không có.
 * Đầu ra: Không có.
 * Tác dụng phụ:
 *   - Khi được ném (throw), sẽ truyền thông báo lỗi "Functionality not implemented yet".
 */
class NotImplementedException : public std::exception {
   public:
    /**
     * Mô tả: Trả về thông điệp lỗi của exception.
     * Đầu vào: Không có.
     * Đầu ra:
     *   - const char*: chuỗi mô tả lỗi.
     * Tác dụng phụ: Không có.
     */
    const char* what() const noexcept override {
        return "Functionality not implemented yet";
    }
};