#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <SDL.h>
using namespace std;
const string WINDOW_TITLE = "Sudoku";
const int SCREEN_WIDTH = 726;
const int SCREEN_HEIGHT = 594;

void load_SDL_and_Images();
void unload_SDL_and_Images();
SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture *gameImg,*youWin, *numberMsg[10], *falseNumber;
int fillit(int **grid, int row, int col);
int getChooseNumber();

void showGame();
void pause();
void printGameImg();
void printYouWin();
void printNumberMsg(int gridNumber,int x, int y);
void printFalseNumber(int x, int y);
int checkzeros(int **grid);
int check(int **grid, int row, int col, int num);
void fillinrandom(int **grid);
void printChooseNumberComputer();
int chooseNumberComputer(int x, int y);
void printChooseNumber( int chooseNumber, int **grid);

void logSDLError(std::ostream& os, const std::string &msg, bool fatal = false);
void initSDL(SDL_Window* &window, SDL_Renderer* &renderer);
void quitSDL(SDL_Window* window, SDL_Renderer* renderer);
void waitUntilKeyPressed();
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren);
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y);
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h);


int checkzeros(int **grid)
{
    int row, col, zero = 0;
    for( row = 0; row < 9; row++)
    {
        for(col = 0; col < 9; col++)
        {
            if(grid[row][col] == 0)
                zero++;
        }
    }
    return zero;
}

int check(int **grid, int row, int col, int num)
{
    int rowStart = (row/3) * 3;
    int colStart = (col/3) * 3;
    int i, j;

    for(i=0; i<9; ++i)
    {
        if (grid[row][i] == num)
            return 0;
        if (grid[i][col] == num)
            return 0;
        if (grid[rowStart + (i%3)][colStart + (i/3)] == num)
            return 0;
    }
    return 1;
}

int fillit(int **grid, int row, int col)
{
    int i;
    if(row<9 && col<9)
    {
        if(grid[row][col] != 0)
        {
            if((col+1)<9)
                return fillit(grid, row, col+1);
            else if((row+1)<9)
                return fillit(grid, row+1, 0);
            else
                return 1;
        }
        else
        {
            for(i=0; i<9; ++i)
            {
                if(check(grid, row, col, i+1))
                {
                    grid[row][col] = i+1;
                    if((col+1)<9)
                    {
                        if(fillit(grid, row, col +1))
                            return 1;
                        else
                            grid[row][col] = 0;
                    }
                    else if((row+1) < 9)
                    {
                        if(fillit(grid, row+1, 0))
                            return 1;
                        else
                            grid[row][col] = 0;
                    }
                    else
                        return 1;
                }
            }
        }
        return 0;
    }
    else return 1;
}

void fillinrandom(int **grid)
{
    int max = 9,
        srand(time(0));

    int k;
    for( k = 0; k < 11 ; k++ )
    {
        int i, j;
        i = rand() % max;
        j = rand() % max;
        if( !grid[i][j] )
        {
            int num = rand() % max + 1;
            while(1)
            {
                if( check(grid, i, j, num) )
                {
                    grid[i][j] = num;
                    break;
                }
                else
                {
                    num = (num + 1)	% max;
                }
            }
        }
    }
}

void copy(int **grid, int **solutiongrid)
{
    int i, j;
    for( i = 0; i < 9; i++ )
    {
        for( j = 0; j < 9; j++ )
            solutiongrid[i][j] = grid[i][j];
    }

}

void showGame()
{
    SDL_RenderClear(renderer);
    printGameImg();
    printChooseNumberComputer();
    SDL_Delay(500);
    SDL_RenderPresent(renderer);
}

void printGameImg()
{
    renderTexture(gameImg, renderer, 0, 0);
    SDL_RenderPresent(renderer);
}

void printYouWin()
{
    renderTexture(youWin, renderer, 0, 0, 594, 726);
    SDL_RenderPresent(renderer);
}
void printNumberMsg(int gridNumber,int x, int y)
{
    renderTexture(numberMsg[gridNumber], renderer, x, y, 66, 66);
    SDL_RenderPresent(renderer);
}
void printFalseNumber(int x, int y)
{
    renderTexture(falseNumber, renderer, x, y, 66, 66);
    SDL_RenderPresent(renderer);
}
void printChooseNumberComputer()
{
    printNumberMsg(1,660,0);
    printNumberMsg(2,660,66);
    printNumberMsg(3,660,132);
    printNumberMsg(4,660,198);
    printNumberMsg(5,660,264);
    printNumberMsg(6,660,330);
    printNumberMsg(7,660,396);
    printNumberMsg(8,660,462);
    printNumberMsg(9,660,528);
}
void load_SDL_and_Images()
{
    initSDL(window, renderer);
    gameImg = loadTexture("images/Grid.bmp", renderer);
    youWin = loadTexture("images/youwin.bmp", renderer);
    for(int i=1; i<=9; i++)
    {
        numberMsg[i] = loadTexture("images/"+to_string(i) + ".bmp", renderer);
    }
    falseNumber = loadTexture("images/0.bmp", renderer);
}

void unload_SDL_and_Images()
{
    SDL_DestroyTexture(gameImg);
    SDL_DestroyTexture(youWin);
    SDL_DestroyTexture(falseNumber);
    for (int i=1; i<=9; i++)
    {
        SDL_DestroyTexture(numberMsg[i]);
    }
    quitSDL(window, renderer);
}
int chooseNumberComputer(int x, int y)
{
    int chooseNumber[1][9];
    if(x >= 660)
    {
        x = (x-660)/66;
        y = y/66;
        chooseNumber[0][0] = 1;
        chooseNumber[0][1] = 2;
        chooseNumber[0][2] = 3;
        chooseNumber[0][3] = 4;
        chooseNumber[0][4] = 5;
        chooseNumber[0][5] = 6;
        chooseNumber[0][6] = 7;
        chooseNumber[0][7] = 8;
        chooseNumber[0][8] = 9;
    }
    return chooseNumber[x][y];
}

int getChooseNumber()
{
    int number = 0;
    SDL_Event e;
    while (number<'1' || number>'9')
    {
        SDL_Delay(10);
        if ( SDL_WaitEvent(&e) == 0) continue;
        if ((e.type == SDL_QUIT) || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
        {
            unload_SDL_and_Images();
            exit(1);
        }

        if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if(e.button.x>=660)
            {
                number = chooseNumberComputer(e.button.x, e.button.y);
                break;
            }
        }

    }
    return number;
}


int main(int argc, char* argv[])
{
    load_SDL_and_Images();
    showGame();
    int grid[9][9];
    int *gridNumber[9];
    int chooseNumber;
    int i, j;
    for( i = 0; i < 9; i++ )
    {
        gridNumber[i]=grid[i];
    }
    for( i = 0; i < 9; i++ )
    {
        for( j = 0; j < 9; j++ )
            gridNumber[i][j] = 0;
    }
    fillinrandom(gridNumber);
    for( i = 0; i < 9; i++ )
    {
        for( j = 0; j < 9; j++ )
            if(gridNumber[i][j] != 0)
            {
                printNumberMsg(gridNumber[i][j],i*66,j*66);
            }
    }


    do
    {
        cout << "hello";
        chooseNumber = getChooseNumber();
        cout << "so ="<<chooseNumber;
        if(chooseNumber != 0 )
        {
            cout<< "huyen";
            SDL_Event e;
            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                if(e.button.x <= 594 )
                {
                    i = e.button.x / 66;
                    j = e.button.y / 66;

                    if(gridNumber[i][j] == 0)
                    {
                        gridNumber[i][j] = chooseNumber;
                        SDL_Delay(500);
                        printNumberMsg(chooseNumber,i*66,j*66);
                    }
                }
            }
        }
        chooseNumber = 0;
    }
    while (checkzeros(gridNumber) != 0);
    for( i = 0; i < 9; i++ )
    {
        for( j = 0; j < 9; j++ ){
            if(gridNumber[i][j] != fillit(gridNumber,i,j)){
                printFalseNumber(i*66,j*66);
            }
        }

    }
    waitUntilKeyPressed();
    unload_SDL_and_Images();
    return 0;
}
void logSDLError(std::ostream& os,
                 const std::string &msg, bool fatal)
{
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal)
    {
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
    //   SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (window == nullptr) logSDLError(std::cout, "CreateWindow", true);


    //Khi th??ng th?????ng ch???y v???i m??i tr?????ng b??nh th?????ng ??? nh??
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
                                  SDL_RENDERER_PRESENTVSYNC);
    //Khi ch???y ??? m??y th???c h??nh WinXP ??? tr?????ng (m??y ???o)
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
    while (true)
    {
        if ( SDL_WaitEvent(&e) != 0 &&
                (e.type == SDL_KEYDOWN || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_QUIT) )
            return;
        SDL_Delay(100);
    }
}

/**
* N???p m???t ???nh bitmap (BMP) v??o m???t texture tr??n thi???t b??? hi???n th??? (rendering device)
* @param file: ???????ng d???n v?? t??n file ???nh BMP
* @param ren: renderer ????? n???p texture l??n
* @return tr??? v??? texture ???? n???p, ho???c nullptr n???u c?? l???i.
*/
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren)
{
    //Kh???i t???o l?? nullptr ????? tr??nh l???i 'dangling pointer'
    SDL_Texture *texture = nullptr;
    //N???p ???nh t??? t??n file (v???i ???????ng d???n)
    SDL_Surface *loadedImage = SDL_LoadBMP(file.c_str());
    //N???u kh??ng c?? l???i, chuy???n ?????i v??? d???ng texture and v?? tr??? v???
    if (loadedImage != nullptr)
    {
        texture = SDL_CreateTextureFromSurface(ren, loadedImage);
        SDL_FreeSurface(loadedImage);
        //?????m b???o vi???c chuy???n ?????i kh??ng c?? l???i
        if (texture == nullptr)
        {
            logSDLError(std::cout, "CreateTextureFromSurface");
        }
    }
    else
    {
        logSDLError(std::cout, "LoadBMP");
    }
    return texture;
}

/**
* V??? m???t SDL_Texture l??n m???t SDL_Renderer t???i to??? ????? (x, y), trong khi
* gi??? nguy??n chi???u r???ng v?? cao c???a ???nh
* @param tex: texture ngu???n ch??ng ta mu???n v??? ra
* @param ren: thi???t b??? renderer ch??ng ta mu???n v??? v??o
* @param x: ho??nh ?????
* @param y: tung ?????
*/
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y)
{
    //Thi???t l???p h??nh ch??? nh???t ????ch m?? ch??ng ta mu???n v??? ???nh v??o trong
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    //Truy v???n texture ????? l???y chi???u r???ng v?? cao (v??o chi???u r???ng v?? cao t????ng ???ng c???a h??nh ch??? nh???t ????ch)
    SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
    //????a to??n b??? ???nh trong texture v??o h??nh ch??? nh???t ????ch
    SDL_RenderCopy(ren, tex, NULL, &dst);
}

/**
* V??? m???t SDL_Texture l??n m???t SDL_Renderer t???i to??? ????? (x, y), v???i
* chi???u r???ng v?? cao m???i
* @param tex: texture ngu???n ch??ng ta mu???n v??? ra
* @param ren: thi???t b??? renderer ch??ng ta mu???n v??? v??o
* @param x: ho??nh ?????
* @param y: tung ?????
* @param w: chi???u r???ng (m???i)
* @param h: ????? cao (m???i)
*/
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h)
{
    //Thi???t l???p h??nh ch??? nh???t ????ch m?? ch??ng ta mu???n v??? ???nh v??o trong
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = w;
    dst.h = h;
    //????a to??n b??? ???nh trong texture v??o h??nh ch??? nh???t ????ch
    //(???nh s??? co d??n cho kh???p v???i k??ch c??? m???i)
    SDL_RenderCopy(ren, tex, NULL, &dst);
}
