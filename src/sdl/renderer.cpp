/**
 * SDL Renderer cpp implementation
 *
 */

#include "renderer.h"

/* ---------- Importing ---------- */

#include <format>
#include <iostream>


#include <SDL2/SDL_image.h>

#include "../game/setup.h"
#include "../utils/config.h"


static const SDL_Color COL_TEXT      = {220, 220, 230, 255};  // Chữ chính (trắng xám)
static const SDL_Color COL_TEXT_DIM  = {120, 120, 150, 255};  // Chữ phụ (xám mờ)
static const SDL_Color COL_WIN       = {80,  220, 120, 180};  // Đường thắng (xanh lá, semi-transparent)
static const SDL_Color BOARD_BG_COLOR = {255, 255, 255, 255}; // Màu nền bàn cờ (trắng)

static const int SCREEN_W       = 1000;
static const int SCREEN_H       = 680;

static const int CELL_SIZE      = 50;   // Kích thước mỗi ô bàn cờ (px)
static const int SYMBOL_PADDING = 10;   // Khoảng cách từ ký hiệu X/O đến mép ô (px)
static const int LINE_THICKNESS = 2;    // Độ dày đường kẻ lưới (px)
/* ---------- Definitions ---------- */

SDLRenderer::SDLRenderer() : I_Renderer() {
}

/**
 * Mô tả: Destructor của SDLRenderer.
 * Đầu vào: Không.
 * Đầu ra: Không.
 * Tác dụng phụ: Không (việc giải phóng thực hiện trong close()).
 */
SDLRenderer::~SDLRenderer() {
}

/**
 * Mô tả: Khởi tạo SDL, window và renderer.
 * Đầu vào: config - cấu hình màn hình và layout.
 * Đầu ra: Không.
 * Tác dụng phụ:
 *   - Khởi tạo SDL subsystem.
 *   - Tạo window và renderer.
 *   - Thiết lập chế độ blend.
 * NOTE: Phải gọi trước khi render.
 */
void SDLRenderer::init(const RunConfig& config) {
    // read config
    screenWidth = config.screenWidth;
    screenHeight = config.screenHeight;

    // int boardPadding = config.boardPadding;
    // init(...)

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    window = SDL_CreateWindow(
        "TicTacToe SDL",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        screenWidth,
        screenHeight,
        0);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // load font
    // font = TTF_OpenFont("assets/font.ttf", font_size);
    fontLarge  = TTF_OpenFont("../assets/NotoSans-Regular.ttf", 32);
    fontNormal = TTF_OpenFont("../assets/NotoSans-Regular.ttf", 20);
    fontSmall  = TTF_OpenFont("../assets/NotoSans-Regular.ttf", 15);
}

/**
 * Mô tả: Xóa toàn bộ nội dung màn hình (background tối).
 * Đầu vào: Không.
 * Đầu ra: Không.
 * Tác dụng phụ: Reset frame hiện tại.
 */
void SDLRenderer::clearScreen() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // white background
    SDL_RenderClear(renderer);
}

/**
 * Mô tả: Hiển thị frame đã render lên màn hình.
 * Đầu vào: Không.
 * Đầu ra: Không.
 * Tác dụng phụ: Swap buffer để hiển thị nội dung.
 */
void SDLRenderer::renderPresent() {
    SDL_RenderPresent(renderer);
}

/**
 * Mô tả: Vẽ hình chữ nhật lên renderer.
 * Đầu vào:
 *   - x, y: tọa độ.
 *   - w, h: kích thước.
 *   - color: màu sắc.
 *   - filled: true nếu fill, false nếu vẽ viền.
 * Đầu ra: Không.
 * Tác dụng phụ: Vẽ trực tiếp lên renderer.
 */
void SDLRenderer::drawRect(int x, int y, int w, int h, SDL_Color color, bool filled) {
    SDL_Rect rect = {x, y, w, h};
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    if (filled)
        SDL_RenderFillRect(renderer, &rect);
    else
        SDL_RenderDrawRect(renderer, &rect);
}
// My helper func
void SDLRenderer::drawLine(int x1, int y1, int x2, int y2, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void SDLRenderer::drawText(const std::string& text, int x, int y, SDL_Color color, TTF_Font* font) {
    if (!font || text.empty()) return;

    // Bước 1: Render text thành Surface
    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text.c_str(), color);
    if (!surface) return;

    // Bước 2: Chuyển Surface thành Texture
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);  // Surface không cần nữa sau khi có Texture
    if (!texture) return;

    // Bước 3: Lấy kích thước texture
    int tw, th;
    SDL_QueryTexture(texture, nullptr, nullptr, &tw, &th);

    // Bước 4: Vẽ texture tại (x, y)
    SDL_Rect dst = {x, y, tw, th};
    SDL_RenderCopy(renderer, texture, nullptr, &dst); //nullptr để vẽ toàn bộ texture

    // Bước 5: Giải phóng texture
    SDL_DestroyTexture(texture);
}

void SDLRenderer::drawTextCentered(const std::string& text, int x, int y, int w, SDL_Color color, TTF_Font* font) {
    if (!font || text.empty()) return;

    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text.c_str(), color);
    if (!surface) return;

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) return;

    int tw, th;
    SDL_QueryTexture(texture, nullptr, nullptr, &tw, &th);

    int cx = x + (w - tw) / 2;
    SDL_Rect dst = {cx, y, tw, th};
    SDL_RenderCopy(renderer, texture, nullptr, &dst);
    SDL_DestroyTexture(texture);
}

void SDLRenderer::drawImage(const std::string& imagePath, int x, int y, int w, int h) {
    // Bước 1: Load ảnh từ file
    SDL_Surface* surface = IMG_Load(imagePath.c_str());
    if (!surface) {
        return;
    }

    // Bước 2: Chuyển Surface thành Texture
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        return;
    }

    // Bước 3: Vẽ texture tại (x, y) với kích thước (w, h)
    SDL_Rect dst = {x, y, w, h};
    SDL_RenderCopy(renderer, texture, nullptr, &dst);

    // Bước 4: Giải phóng texture
    SDL_DestroyTexture(texture);
}

/**
 * Mô tả: Hiển thị menu lựa chọn.
 * Đầu vào: selectType, context.
 * Đầu ra: Không.
 * Tác dụng phụ: Vẽ UI lên màn hình.
 * TODO:
 *   - Bước 1: Xác định loại menu.
 *   - Bước 2: Render text tương ứng.
 *   - Trường hợp biên: selectType không hợp lệ.
 */
void SDLRenderer::showSelectMenu(SelectType selectType, int context) {
    // TODO: Render menu UI tương ứng với selectType
    clearScreen();
    int menuY = 100;

    switch (selectType) {
        case SelectType::TITLE_UI: {
            drawImage("../assets/menupic1.png", 0, 0, screenWidth, screenHeight);
            drawTextCentered("TIC TAC TOE", 0, menuY, screenWidth, {255, 255, 255, 255}, fontLarge);
            drawTextCentered("Press Enter to start...", 0, menuY + 120, screenWidth, {255, 255, 220, 255}, fontNormal);
            break;
        }
        case SelectType::SIZE_UI: {
            drawTextCentered("Select Board Size", 0, 100, SCREEN_W, {0, 0, 0, 255}, fontLarge);
            drawTextCentered("Press a number key then Enter", 0, 220, SCREEN_W, {100, 100, 100, 255}, fontNormal);

            // Ô input — căn giữa, rộng 500px, cao 70px
            int boxW = 500, boxH = 70;
            int boxX = (SCREEN_W - boxW) / 2;
            int boxY = 280;

            drawRect(boxX, boxY, boxW, boxH, {255, 255, 255, 120}, true);   // nền trắng trong suốt
            drawRect(boxX, boxY, boxW, boxH, {0, 0, 0, 255}, false);        // viền đen

            // inputBuffer là biến thành viên lưu ký tự người dùng đang gõ
            std::string display = inputBuffer.empty() ? "Choose a number...." : inputBuffer;
            SDL_Color textColor = inputBuffer.empty() ? SDL_Color{150, 150, 150, 255}
                                                    : SDL_Color{0, 0, 0, 255};
            drawText(display, boxX + 20, boxY + 22, textColor, fontNormal);

            renderPresent();
            break;
        }
        case SelectType::GOAL_UI: {
            drawTextCentered("Select Goal", 0, 100, SCREEN_W, {0, 0, 0, 255}, fontLarge);
            drawTextCentered("Press a number key then Enter", 0, 220, SCREEN_W, {100, 100, 100, 255}, fontNormal);

            // Ô input - rộng 500px, cao 70px
            int boxW = 500, boxH = 70;
            int boxX = (SCREEN_W - boxW) / 2;
            int boxY = 280;

            drawRect(boxX, boxY, boxW, boxH, {255, 255, 255, 120}, true);   // nền trắng trong suốt
            drawRect(boxX, boxY, boxW, boxH, {0, 0, 0, 255}, false);        // viền đen

            // inputBuffer là biến thành viên lưu ký tự người dùng đang gõ
            std::string display = inputBuffer.empty() ? "Choose a number...." : inputBuffer;
            SDL_Color textColor = inputBuffer.empty() ? SDL_Color{150, 150, 150, 255}
                                                    : SDL_Color{0, 0, 0, 255};
            drawText(display, boxX + 20, boxY + 22, textColor, fontNormal);

            renderPresent();
            break;
        }
        case SelectType::GAME_MODE_UI: {
            drawImage("../assets/menupic.jpg", 0, 0, screenWidth, screenHeight);
            drawTextCentered("Choose Game Mode", 0, 80, SCREEN_W, {0, 0, 0, 255}, fontLarge);

            struct { const char* key; const char* name; } modes[] = {
                {"1", "PvP"},
                {"2", "PvE"},
                {"3", "EvE"},
            };

            int cardW = 200, cardH = 200, gap = 40;
            int totalW = 3 * cardW + 2 * gap;
            int startX = (SCREEN_W - totalW) / 2;
            int cardY  = 220;

            for (int i = 0; i < 3; i++) {
                int cx = startX + i * (cardW + gap);

                drawRect(cx, cardY, cardW, cardH, {255, 255, 255, 120}, true);  // nền trắng
                drawRect(cx, cardY, cardW, cardH, {0, 0, 0, 255}, false);       // viền đen

                drawTextCentered(modes[i].key, cx, cardY + 60, cardW, {0, 0, 0, 255}, fontNormal);
                drawTextCentered(modes[i].name, cx, cardY + 120, cardW, {0, 0, 0, 255}, fontNormal);
            }

            renderPresent();
            break;
        }
        case SelectType::BOT_LEVEL_UI: {
            drawImage("../assets/askeladd.jpg", 0, 0, screenWidth, screenHeight);
            drawTextCentered("Choose Bot Level", 0, menuY, screenWidth, {0, 0, 0, 255}, fontLarge);

            struct { const char* key; const char* name; } levels[] = {
                {"1", "EASY"},
                {"2", "MEDIUM"},
                {"3", "HARD"},
            };
            SDL_Color lvColors[] = {
                {80, 200, 100, 255},   // Xanh lá (dễ)
                {230, 180, 50, 255},   // Vàng cam (trung bình)
                {220, 80, 80, 255},    // Đỏ (khó)
            };

            int btnW = 120, btnH = 60, gap = 30;
            int totalW = 3 * btnW + 2 * gap;
            int startX = (screenWidth - totalW) / 2;

            for (int i = 0; i < 3; i++) {
                int bx = startX + i * (btnW + gap);
                int by = menuY + 60;
                drawRect(bx, by, btnW, btnH, {255, 255, 255, 255}, true);
                drawRect(bx, by, btnW, btnH, lvColors[i], false); 
                drawText(levels[i].key, bx + 8, by + 8, COL_TEXT_DIM, fontSmall);
                drawTextCentered(levels[i].name, bx, by + 22, btnW, lvColors[i], fontNormal);
            }
            break;
        }
        case SelectType::MUL_BOT_LEVEL_UI: {
            if (context == 0) drawImage("../assets/thorfinn.jpg", 0, 0, screenWidth, screenHeight);
            else              drawImage("../assets/askeladd.jpg", 0, 0, screenWidth, screenHeight);
            std::string title = "Choose Bot Level " + std::to_string(context + 1);
            std::string symbol = (context == 0) ? "Bot 1(X)" : "Bot 2(O)";
            drawTextCentered(title, 0, menuY, screenWidth, {0, 0, 0, 255}, fontLarge);
            drawTextCentered(symbol, 0, menuY + 40, screenWidth, {0, 0, 0, 255}, fontNormal);

            struct { const char* key; const char* name; } levels[] = {
                {"1", "EASY"}, {"2", "MEDIUM"}, {"3", "HARD"},
            };
            int btnW = 120, btnH = 50, gap = 20;
            int totalW = 3 * btnW + 2 * gap;
            int startX = (screenWidth - totalW) / 2;
            for (int i = 0; i < 3; i++) {
                int bx = startX + i * (btnW + gap);
                int by = menuY + 90;
                if (i==0) {
                drawRect(bx, by, btnW, btnH, {255, 255, 255, 255}, true);
                drawRect(bx, by, btnW, btnH, {80, 200, 100, 255}, false);
                drawText(levels[i].key, bx + 8, by + 8, COL_TEXT_DIM, fontSmall);
                drawTextCentered(levels[i].name, bx, by + 15, btnW, {80, 200, 100, 255}, fontNormal);}
                else if (i==1) {
                drawRect(bx, by, btnW, btnH, {255, 255, 255, 255}, true);
                drawRect(bx, by, btnW, btnH, {230, 180, 50, 255}, false);
                drawText(levels[i].key, bx + 8, by + 8, COL_TEXT_DIM, fontSmall);
                drawTextCentered(levels[i].name, bx, by + 15, btnW, {230, 180, 50, 255}, fontNormal);}
                else {
                drawRect(bx, by, btnW, btnH, {255, 255, 255, 255}, true);
                drawRect(bx, by, btnW, btnH, {220, 80, 80, 255}, false);
                drawText(levels[i].key, bx + 8, by + 8, COL_TEXT_DIM, fontSmall);
                drawTextCentered(levels[i].name, bx, by + 15, btnW, {220, 80, 80, 255}, fontNormal);}
            }
            break;
        }
        case SelectType::PLAYER_UI: {
            drawTextCentered("Click to make your move...", 0, menuY, screenWidth, COL_TEXT_DIM, fontNormal);
            break;
        }

        default:
            break;
    }
    renderPresent();
}

/**
 * Mô tả: Hiển thị thông báo lựa chọn không hợp lệ.
 * Đầu vào: selectType, context.
 * Đầu ra: Không.
 * Tác dụng phụ: Hiển thị thông báo lỗi.
 * TODO:
 *   - Bước 1: Xác định loại lỗi.
 *   - Bước 2: Render text cảnh báo.
 */
void SDLRenderer::showInvalidSelect(SelectType selectType, int context) {
    (void)selectType; (void)context;
    // TODO: Render thông báo lỗi
    drawRect(0, screenHeight - 60, screenWidth, 60, {180, 50, 50, 200}, true);
    drawTextCentered("! Invalid selection, please try again !", 0, screenHeight - 40, screenWidth, COL_TEXT, fontNormal);
    renderPresent();
    SDL_Delay(400);
}

/**
 * Mô tả: Hiển thị thông báo lựa chọn hợp lệ.
 * Đầu vào: selectType, context.
 * Đầu ra: Không.
 * Tác dụng phụ: Hiển thị xác nhận.
 * TODO:
 *   - Bước 1: Xác định loại selection.
 *   - Bước 2: Render thông báo thành công.
 */
void SDLRenderer::showValidSelect(SelectType selectType, int context) {
    (void)selectType; (void)context;
    // TODO: Render thông báo thành công
    drawRect(0, screenHeight - 60, screenWidth, 60, {50, 180, 80, 180}, true);
    drawTextCentered("Processing...", 0, screenHeight - 40, screenWidth, COL_TEXT, fontNormal);
    renderPresent();
    SDL_Delay(400);
}

/**
 * Mô tả: Vẽ bàn cờ lên màn hình.
 * Đầu vào: board, size.
 * Đầu ra: Không.
 * Tác dụng phụ: Render grid và ký hiệu.
 * TODO:
 *   - Bước 1: Tính toán layout ô.
 *   - Bước 2: Vẽ grid.
 *   - Bước 3: Vẽ X/O.
 */
void SDLRenderer::displayBoard(const char board[][BOARD_N_MAX], const int size) {
    currentSize = size;
    // TODO: Render board
    int boardW = size * CELL_SIZE;
    int boardH = size * CELL_SIZE;

    int offsetX = (SCREEN_W - boardW) / 2;
    int offsetY = (SCREEN_H - boardH) / 2;

    drawRect(offsetX, offsetY, boardW, boardH, {255, 255, 255, 255}, true);
    drawRect(offsetX, offsetY, boardW, boardH, {0, 0, 0, 255}, false);

    for (int i = 1; i < size; i++) {
        // Ngang
        drawLine(offsetX, offsetY + i * CELL_SIZE,
                 offsetX + boardW, offsetY + i * CELL_SIZE,
                 {0, 0, 0, 255});
        // Dọc
        drawLine(offsetX + i * CELL_SIZE, offsetY,
                 offsetX + i * CELL_SIZE, offsetY + boardH,
                 {0, 0, 0, 255});
    }

    // Vẽ X và O
    for (int r = 0; r < size; r++) {
        for (int c = 0; c < size; c++) {
            char sym = board[r][c];
            if (sym == '-') continue;

            int cx = offsetX + c * CELL_SIZE;
            int cy = offsetY + r * CELL_SIZE;
            int p  = SYMBOL_PADDING;

            if (sym == 'X') {
                SDL_SetRenderDrawColor(renderer, 200, 50, 50, 255);
                for (int t = -LINE_THICKNESS; t <= LINE_THICKNESS; t++) {
                    SDL_RenderDrawLine(renderer, cx+p, cy+p+t, cx+CELL_SIZE-p, cy+CELL_SIZE-p+t);
                    SDL_RenderDrawLine(renderer, cx+p+t, cy+CELL_SIZE-p, cx+CELL_SIZE-p+t, cy+p);
                }
            } else if (sym == 'O') {
                int rc = CELL_SIZE / 2 - p;
                int ox = cx + CELL_SIZE / 2;
                int oy = cy + CELL_SIZE / 2;
                SDL_SetRenderDrawColor(renderer, 50, 100, 200, 255);
                for (int dr = -LINE_THICKNESS; dr <= LINE_THICKNESS; dr++) {
                    int r2 = rc + dr;
                    for (int deg = 0; deg < 360; deg++) {
                        float rad = deg * 3.14159f / 180.0f;
                        SDL_RenderDrawPoint(renderer,
                            ox + (int)(r2 * cosf(rad)),
                            oy + (int)(r2 * sinf(rad)));
                    }
                }
            }
        }
    }

    renderPresent();
}

/**
 * Mô tả: Hiển thị nước đi vừa thực hiện.
 * Đầu vào: row, col.
 * Đầu ra: Không.
 * Tác dụng phụ: Highlight ô.
 * TODO:
 *   - Bước 1: Xác định vị trí ô.
 *   - Bước 2: Vẽ highlight.
 */
void SDLRenderer::showMove(const int row, const int col) {
    // TODO: Highlight move
    
    int boardW = currentSize * CELL_SIZE;
    int offsetX = (SCREEN_W - boardW) / 2;
    int offsetY = (SCREEN_H - boardW) / 2;

    int x = offsetX + col * CELL_SIZE;
    int y = offsetY + row * CELL_SIZE;

    // Vẽ highlight vàng bán trong suốt lên ô vừa đi
    drawRect(x + 1, y + 1, CELL_SIZE - 2, CELL_SIZE - 2, {255, 215, 0, 40}, true);
    renderPresent();
}

/**
 * Mô tả: Hiển thị thông báo nước đi không hợp lệ.
 * Đầu vào: Không.
 * Đầu ra: Không.
 * Tác dụng phụ: Hiển thị lỗi.
 * TODO:
 *   - Bước 1: Render thông báo lỗi.
 */
void SDLRenderer::showInvalidMove() {
    // TODO: Render invalid move message
    drawRect(0, screenHeight - 60, screenWidth, 60, {180, 50, 50, 200}, true);
    drawTextCentered("! Invalid move, please try again !", 0, screenHeight - 40, screenWidth, COL_TEXT, fontNormal);
    renderPresent();
}

/**
 * Mô tả: Hiển thị người chơi hiện tại.
 * Đầu vào: player, is_bot.
 * Đầu ra: Không.
 * Tác dụng phụ: Hiển thị thông tin turn.
 * TODO:
 *   - Bước 1: Xác định text.
 *   - Bước 2: Render lên màn hình.
 */
void SDLRenderer::showPlayer(const int player, const bool is_bot) {
    // TODO: Render player info
    drawRect(0, 0, SCREEN_W, 60, {255, 255, 255, 255}, true);
    drawRect(0, 55, SCREEN_W, 2, {0, 0, 0, 255}, true);

    std::string symbol = (player == 0) ? "X" : "O";
    std::string type   = is_bot ? "Bot" : "Player";
    std::string msg    = "Player " + std::to_string(player + 1) + " (" + symbol + ")'s turn  (" + type + ")";

    SDL_Color col = (player == 0) ? SDL_Color{200, 50, 50, 255}
                                  : SDL_Color{50, 100, 200, 255};

    drawTextCentered(msg, 0, 15, SCREEN_W, col, fontNormal);

    renderPresent();
}

/**
 * Mô tả: Hiển thị kết quả game.
 * Đầu vào: winner, is_bot, winLine.
 * Đầu ra: Không.
 * Tác dụng phụ: Hiển thị kết quả và highlight đường thắng.
 * TODO:
 *   - Bước 1: Kiểm tra draw hoặc win.
 *   - Bước 2: Render text.
 *   - Bước 3: Highlight winLine nếu có.
 */
void SDLRenderer::showResult(const int winner, const bool is_bot, const WinLine* winLine) {
    // TODO: Render result
    if (winLine != nullptr) {
        int boardW  = currentSize * CELL_SIZE;
        int offsetX = (SCREEN_W - boardW) / 2;
        int offsetY = (SCREEN_H - boardW) / 2;

        for (auto& [r, c] : winLine->cells) {
            int x = offsetX + c * CELL_SIZE;
            int y = offsetY + r * CELL_SIZE;
            drawRect(x + 1, y + 1, CELL_SIZE - 2, CELL_SIZE - 2, COL_WIN, true);
        }
    }

    // Bước 2: Vẽ text kết quả ở status bar
    drawRect(0, 0, SCREEN_W, 60, {255, 255, 255, 255}, true);
    drawRect(0, 55, SCREEN_W, 2, {0, 0, 0, 255}, true);

    std::string msg;
    SDL_Color col;

    if (winner == -1) {
        msg = "Draw!";
        col = {100, 100, 100, 255};
    } else {
        std::string sym  = (winner == 0) ? "X" : "O";
        std::string type = is_bot ? "Bot" : "Player";
        msg = "Player " + std::to_string(winner + 1) + " (" + sym + ") wins!  (" + type + ")";
        col = (winner == 0) ? SDL_Color{200, 50, 50, 255}
                            : SDL_Color{50, 100, 200, 255};
    }

    drawTextCentered(msg, 0, 15, SCREEN_W, col, fontLarge);
    drawTextCentered("Press any key to exit...", 0, SCREEN_H - 35, SCREEN_W, {150, 150, 150, 255}, fontSmall);
    
    renderPresent();
}

/**
 * Mô tả: In kết quả ra stdout (judge mode).
 * Đầu vào: gameResult.
 * Đầu ra: Không.
 * Tác dụng phụ: In console.
 * TODO:
 *   - Bước 1: Format output.
 *   - Bước 2: In ra std::cout.
 */
void SDLRenderer::printResult(const GameResult& gameResult) {
    // TODO: Print result
    std::cout << gameResult.winner << " " << gameResult.turns << "\n";
}


/**
 * Mô tả: Giải phóng tài nguyên SDL.
 * Đầu vào: Không.
 * Đầu ra: Không.
 * Tác dụng phụ:
 *   - Destroy renderer và window.
 *   - Shutdown SDL subsystem.
 */
void SDLRenderer::close() {
    // if (font) {
    //     TTF_CloseFont(font);
    //     font = nullptr;
    // }
    if (fontLarge)  { TTF_CloseFont(fontLarge);  fontLarge  = nullptr; }
    if (fontNormal) { TTF_CloseFont(fontNormal); fontNormal = nullptr; }
    if (fontSmall)  { TTF_CloseFont(fontSmall);  fontSmall  = nullptr; }

    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}