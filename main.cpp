#include <bits/stdc++.h>
#include <SDL.h>

using namespace std;

const int SCREEN_WIDTH = 720;
const int SCREEN_HEIGHT = 720;
const string WINDOW_TITLE = "queen_chess";
SDL_Event e;
SDL_Window* window;
SDL_Renderer* renderer;
    int col = 8;
    int row = 8;

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer);
void logSDLError(std::ostream& os, const std::string &msg, bool fatal = false);
void quitSDL(SDL_Window* window, SDL_Renderer* renderer);
void waitUntilKeyPressed();

SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren);
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y);
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h);

void refreshScreen(SDL_Window* window, SDL_Renderer* renderer,
    const SDL_Rect& filled_rect, const bool leftMouse = false);

void LoadImage();
void VebanCo();

int main(int argc, char* argv[])
{
    initSDL(window, renderer);

    VebanCo();
    LoadImage();
    SDL_RenderPresent(renderer);

    // Định nghĩa toạ độ ban đầu và kích cỡ hình chữ nhật
    SDL_Rect filled_rect;
    filled_rect.x = 0;
    filled_rect.y = 0;
    filled_rect.w = SCREEN_HEIGHT/8;
    filled_rect.h = SCREEN_HEIGHT/8;

    while (true) {
        // Đợi 10 mili giây
        SDL_Delay(10);

        // Nếu không có sự kiện gì thì tiếp tục trở về đầu vòng lặp
        if ( SDL_WaitEvent(&e) == 0) continue;

        // Nếu sự kiện là kết thúc (như đóng cửa sổ) thì thoát khỏi vòng lặp
        if (e.type == SDL_QUIT) break;

        // Nếu nhấn phìm ESC thì thoát khỏi vòng lặp
        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) break;

        // Nếu chuột (mouse) được nhấn (xuống)
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            filled_rect.x = e.button.x; // Lấy hoành độ x của chuột
            filled_rect.y = e.button.y; // Lấy tung độ y của chuột
            // Xoá toàn bộ màn hình và vẽ lại
            if (e.button.button == SDL_BUTTON_LEFT)
                refreshScreen(window, renderer, filled_rect, true);
        }
    }

    waitUntilKeyPressed();
    quitSDL(window, renderer);
    return 0;
}

void logSDLError(std::ostream& os, const std::string &msg, bool fatal)
{
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    logSDLError(std::cout, "SDL_Init", true);
    window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    //window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
    //SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (window == nullptr) logSDLError(std::cout, "CreateWindow", true);
    //Khi chạy trong môi trường bình thường (không chạy trong máy ảo)
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
    SDL_RENDERER_PRESENTVSYNC);
    //Khi chạy ở máy ảo (ví dụ tại máy tính trong phòng thực hành ở trường)
    //renderer = SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(window));
    if (renderer == nullptr) logSDLError(std::cout, "CreateRenderer", true);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void waitUntilKeyPressed()
{
    SDL_Event e;
    while (true) {
        if ( SDL_WaitEvent(&e) != 0 && (e.type == SDL_KEYDOWN || e.type == SDL_QUIT) )
        return;
        SDL_Delay(100);
    }
}

SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren)
{
	//Khởi tạo là nullptr để tránh lỗi 'dangling pointer'
	SDL_Texture *texture = nullptr;
	//Nạp ảnh từ tên file (với đường dẫn)
	SDL_Surface *loadedImage = SDL_LoadBMP(file.c_str());
	//Nếu không có lỗi, chuyển đổi về dạng texture and và trả về
	if (loadedImage != nullptr){
		texture = SDL_CreateTextureFromSurface(ren, loadedImage);
		SDL_FreeSurface(loadedImage);
		//Đảm bảo việc chuyển đổi không có lỗi
		if (texture == nullptr){
			logSDLError(std::cout, "CreateTextureFromSurface");
		}
	}
	else {
		logSDLError(std::cout, "LoadBMP");
	}
	return texture;
}

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y)
{
	//Thiết lập hình chữ nhật đích mà chúng ta muốn vẽ ảnh vào trong
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	//Truy vẫn texture để lấy chiều rộng và cao (vào chiều rộng và cao tương ứng của hình chữ nhật đích)
	SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
    //Đưa toàn bộ ảnh trong texture vào hình chữ nhật đích
	SDL_RenderCopy(ren, tex, NULL, &dst);
}

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h)
{
	//Thiết lập hình chữ nhật đích mà chúng ta muốn vẽ ảnh vào trong
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
    dst.w = w;
    dst.h = h;
    //Đưa toàn bộ ảnh trong texture vào hình chữ nhật đích
    //(ảnh sẽ co dãn cho khớp với kích cỡ mới)
	SDL_RenderCopy(ren, tex, NULL, &dst);
}

void refreshScreen(SDL_Window* window, SDL_Renderer* renderer,
    const SDL_Rect& filled_rect, const bool leftMouse)
{
    VebanCo();
    SDL_Texture *image = loadTexture("queen.bmp", renderer);

    // Vẽ hình chữ nhật với màu tuỳ thuộc nút chuột trái hay phải được nhấn
    if (leftMouse)
        renderTexture(image, renderer, (e.button.x/90)*90, (e.button.y/90)*90, SCREEN_HEIGHT/col, SCREEN_HEIGHT/col);
    // Dùng lệnh hiển thị (đưa) hình đã vẽ ra mành hình
    SDL_RenderPresent(renderer);
}

void LoadImage(){
    //Nạp ảnh nền vào một biến texture
    //SDL_Texture *background = loadTexture("background.bmp", renderer);
    //Nạp ảnh vào một biến texture
    SDL_Texture *image = loadTexture("queen.bmp", renderer);
    //Nếu có lỗi thì giải phóng hai texture đã nạp, rồi kết thúc
    if (image == nullptr){
        //SDL_DestroyTexture(background);
        SDL_DestroyTexture(image);
        quitSDL(window, renderer);
    }
    renderTexture(image, renderer, 0, 0, SCREEN_HEIGHT/col, SCREEN_HEIGHT/col);
}

void VebanCo(){
    SDL_RenderClear(renderer);
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            SDL_Rect fill_rect;
                fill_rect.w = SCREEN_HEIGHT/col;
                fill_rect.h = SCREEN_HEIGHT/row;
                fill_rect.x = j*fill_rect.w;
                fill_rect.y = i*fill_rect.h;
            if((i+j)%2 == 0){
                SDL_SetRenderDrawColor(renderer,255,255,255,255);
                SDL_RenderFillRect(renderer, &fill_rect);
            }
            else{
                SDL_SetRenderDrawColor(renderer,255,0,0,255);
                SDL_RenderFillRect(renderer, &fill_rect);
            }
        }
    }
}

