typedef struct GameTexture{
    SDL_Texture *texture;
    int tWidth;
    int tHeight;
}GameTexture;

# init font
    if (TTF_Init() < 0){
        SDL_Log("%s", TTF_GetError());
        success = false;

    }
    font = TTF_OpenFont("font/arial.ttf", 30);
    if ( !font ) {
        SDL_Log("%s", TTF_GetError());
        success = false;
    }

# render chữ
void InitText( sdl_color, text, sdl_renderer, kích thước)
    <!-- tạo màu -->
    SDL_Color fg = { 255, 255, 255};
    <!-- định hình font và cỡ chữ và text -->
    TTF_RenderUTF8_Blended
    Tìm hiểu về tft render
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, fg);
    <!-- Text -->
    SDL_Texture* texture = SDL_CreateTextureFromSurface(screen, surface);
    Rect rectName = { x, y, w, h}
    rectName.y = y_finish - surface->h;
    rectName.w = surface->w;
    rectName.h = surface->h;
    <!-- render lên màn hình
    screen SDL_Renderer -->
    <!-- SDL_RenderCopy( screen, texture, NULL, &rectName );
     -->
    return GameTexture

# render image
void InitImg( path image, sdl_renderer)
    SDL_Surface *screen = NULL;
    screen = IMG_Load(path);
    sdl texture background = SDL_CreateTextureFromSurface
    Rect {0, 0 SCREEN_WIDTH SCREEN_HEIGHT}

# Exit
    sdl_destroy texture

# Cấu trúc của Render
    SDL_SetRenderDrawColor
    SDL_RenderClear

    rendercopy ...
    <!-- SDL_RenderCopy( screen, texture, NULL, &rectName );
    -->
    render present