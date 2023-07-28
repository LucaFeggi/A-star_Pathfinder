const int MapWidth = 24;
const int MapHeight = 24;
const int BlockSize = 32;
const int WindowWidth = (MapWidth + MapWidth / 2) * BlockSize;
const int WindowHeight = MapHeight * BlockSize;

SDL_Window *Window = NULL;
SDL_Surface *Surface = NULL;
TTF_Font *Font = NULL;

const SDL_Color White = {0xff, 0xff, 0xff};
const SDL_Color Black = {0x00, 0x00, 0x00};
const SDL_Color Red = {0xff, 0x00, 0x00};    
const SDL_Color Green = {0x00, 0xff, 0x00};   
const SDL_Color Blue = {0x00, 0x00, 0xff};
const SDL_Color EdgeColor = {84, 153, 199};
const SDL_Color BackgroundColor = {169, 204, 227};
const SDL_Color ExpansionColor = {40, 116, 166};
const SDL_Color PathColor = {0xff, 0xff, 0x00};

void DrawSquare(int x, int y, const SDL_Color Color, const SDL_Color EdgeColor = EdgeColor){
	uint8_t* PixelPtr = (uint8_t*)Surface->pixels + (y * BlockSize * WindowWidth + x * BlockSize) * 4;
	for (int i = 0; i < BlockSize; i++){
		for (int j = 0; j < BlockSize; j++){
			if(i < BlockSize / 8 || j < BlockSize / 8 || i > BlockSize - BlockSize / 8  || j > BlockSize - BlockSize / 8 ){
				*(PixelPtr + j * 4) = EdgeColor.b;
				*(PixelPtr + j * 4 + 1) = EdgeColor.g;
				*(PixelPtr + j * 4 + 2) = EdgeColor.r;
			}
			else{
				*(PixelPtr + j * 4) = Color.b;
				*(PixelPtr + j * 4 + 1) = Color.g;
				*(PixelPtr + j * 4 + 2) = Color.r;
			}
		}
		PixelPtr += WindowWidth * 4;
	}
}

void DrawPixel(int x, int y, SDL_Color Color = White){
	uint8_t* PixelPtr = (uint8_t*)Surface->pixels + (y * WindowWidth + x) * 4;
	if(PixelPtr == nullptr)
		return;
	*(PixelPtr + 2) = Color.r;
	*(PixelPtr + 1) = Color.g;
	*(PixelPtr) = Color.b;
}

void InitializeSDL(){
	SDL_Init(SDL_INIT_VIDEO);
	Window = SDL_CreateWindow("A* pathfinder", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WindowWidth, WindowHeight, SDL_WINDOW_SHOWN);
	Surface = SDL_GetWindowSurface(Window);
	TTF_Init();
	Font = TTF_OpenFont("font/pixelated.ttf", BlockSize);
}

void CloseSDL(){
	SDL_FreeSurface(Surface);
	SDL_DestroyWindow(Window);
	TTF_Quit();
	SDL_Quit();
}
