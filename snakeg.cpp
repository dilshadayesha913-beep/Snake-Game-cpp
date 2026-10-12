#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

const int WIDTH = 600;
const int HEIGHT = 540;
const int CELL = 20;
const int Window_H=760;

struct Point
{
    int x;
    int y;
};

enum Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

// ======================================================
// DRAW TEXT
// ======================================================

void drawText(SDL_Renderer* renderer,
              TTF_Font* font,
              string text,
              int x,
              int y)
{
    SDL_Color white = {255, 255, 255, 255};

    SDL_Surface* surface =
        TTF_RenderText_Solid(
            font,
            text.c_str(),
            white
        );

    if (surface == NULL)
        return;

    SDL_Texture* texture =
        SDL_CreateTextureFromSurface(
            renderer,
            surface
        );

    SDL_Rect position;

    position.x = x;
    position.y = y;
    position.w = surface->w;
    position.h = surface->h;

    SDL_RenderCopy(
        renderer,
        texture,
        NULL,
        &position
    );

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

// ======================================================
// CHECK SNAKE
// ======================================================

bool contains(vector<Point>& snake, Point p)
{
    for (int i = 0; i < snake.size(); i++)
    {
        if (snake[i].x == p.x &&
            snake[i].y == p.y)
        {
            return true;
        }
    }

    return false;
}

// ======================================================
// CREATE FOOD
// ======================================================

Point createFood(vector<Point>& snake)
{
    Point food;

    do
    {
        food.x = rand() % (WIDTH / CELL);
        food.y = rand() % (HEIGHT/ CELL);

    } while (contains(snake, food));

    return food;
}

// ======================================================
// CHECK TOUCH BUTTON
// ======================================================

bool insideButton(int x, int y, SDL_Rect button)
{
    return (
        x >= button.x &&
        x <= button.x + button.w &&
        y >= button.y &&
        y <= button.y + button.h
    );
}
//=========================================================
//Restart Game
//==========================================================
void resetGame(vector<Point>&snake,
                          Direction&direction,
                          Point&food,
                       
                          int &score,
                          bool &gameOver,
                          Uint32 &lastMove
                          
                     )
                     {
                     	snake.clear();

                    snake.push_back({15, 10});
                    snake.push_back({14, 10});
                    snake.push_back({13, 10});

                    direction = RIGHT;

                    score = 0;

                    food = createFood(snake);

                    gameOver = false;

                    lastMove = SDL_GetTicks();
                }
                     	

// ======================================================
// MAIN
// ======================================================

int main(int argc,char* argv[])
{
    
    srand(time(0));

    // SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        cout << SDL_GetError() << endl;
        return 1;
    }

    // SDL_ttf
    if (TTF_Init() != 0)
    {
        cout << TTF_GetError() << endl;
        SDL_Quit();
        return 1;
    }

    // Window
    SDL_Window* window =
        SDL_CreateWindow(
            "Snake Game",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            WIDTH,
            Window_H,
            SDL_WINDOW_SHOWN
        );

    if (!window)
    {
        SDL_Quit();
        return 1;
    }

    // Renderer
    SDL_Renderer* renderer =
        SDL_CreateRenderer(
            window,
            -1,
            SDL_RENDERER_ACCELERATED
        );

    // Font
    TTF_Font* font =
        TTF_OpenFont("arial.ttf", 24);

    if (!font)
    {
        cout << "Font Error: "
             << TTF_GetError() << endl;

        return 1;
    }

    // ==================================================
    // VECTOR SNAKE
    // ==================================================

    vector<Point> snake;

    snake.push_back({15, 10});
    snake.push_back({14, 10});
    snake.push_back({13, 10});

    Direction direction = RIGHT;

    Point food = createFood(snake);

    int score = 0;

    bool running = true;
    bool gameOver = false;

    Uint32 lastMove = SDL_GetTicks();

    const Uint32 moveDelay = 120;

    SDL_Event event;

    // ==================================================
    // TOUCH BUTTONS
    // ==================================================

    SDL_Rect upButton =
    {
        250, 540, 100, 40
    };

    SDL_Rect downButton =
    {
        250, 620, 100, 40
    };

    SDL_Rect leftButton =
    {
        140, 580, 100, 40
    };

    SDL_Rect rightButton =
    {
        360, 580, 100, 40
    };
    
    SDL_Rect resetButton=
    {
    	210,400,180,50
    };

    // ==================================================
    // GAME LOOP
    // ==================================================

    while (running)
    {
        // ==================================================
        // EVENTS
        // ==================================================

        while (SDL_PollEvent(&event))
        {
            // -------------------------
            // Close window
            // -------------------------

            if (event.type == SDL_QUIT)
            {
                running = false;
            }

            // ==================================================
            // KEYBOARD
            // ==================================================

            if (event.type == SDL_KEYDOWN &&
                !gameOver)
            {
                SDL_Keycode key =
                    event.key.keysym.sym;

                if (key == SDLK_UP &&
                    direction != DOWN)
                {
                    direction = UP;
                }

                if (key == SDLK_DOWN &&
                    direction != UP)
                {
                    direction = DOWN;
                }

                if (key == SDLK_LEFT &&
                    direction != RIGHT)
                {
                    direction = LEFT;
                }

                if (key == SDLK_RIGHT &&
                    direction != LEFT)
                {
                    direction = RIGHT;
                }
            }

            // ==================================================
            // TOUCH / MOUSE
            // ==================================================

            if (event.type == SDL_MOUSEBUTTONDOWN
                )
            {
                int x = event.button.x;
                int y = event.button.y;
                
                if(gameOver &&
                insideButton(x,y,resetButton))
                {
                	resetGame(
                	snake,
                	direction,
                	food,
                	score,
                	gameOver,
                	lastMove
                	);
                }
                	
                	
                if(!gameOver)
                {
                if (insideButton(x, y, upButton))
                {
                    if (direction != DOWN)
                        direction = UP;
                }

                else if (insideButton(x, y, downButton))
                {
                    if (direction != UP)
                        direction = DOWN;
                }

                else if (insideButton(x, y, leftButton))
                {
                    if (direction != RIGHT)
                        direction = LEFT;
                }

                else if (insideButton(x, y, rightButton))
                {
                    if (direction != LEFT)
                        direction = RIGHT;
                }
            }
            }

            // ==================================================
            // RESTART
            // ==================================================

            if (event.type == SDL_KEYDOWN &&
                gameOver)
            {
                if (event.key.keysym.sym == SDLK_r)
                {
                    resetGame(
                	snake,
                	direction,
                	food,
                	score,
                	gameOver,
                	lastMove
                	);
                }
            }
        }

        // ==================================================
        // MOVE SNAKE
        // ==================================================

        Uint32 currentTime = SDL_GetTicks();

        if (!gameOver &&
            currentTime - lastMove >= moveDelay)
        {
            lastMove = currentTime;

            // New head
            Point newHead = snake[0];

            // Direction
            if (direction == UP)
                newHead.y--;

            if (direction == DOWN)
                newHead.y++;

            if (direction == LEFT)
                newHead.x--;

            if (direction == RIGHT)
                newHead.x++;

            // ==================================================
            // BOUNDARY COLLISION
            // ==================================================

            if (newHead.x < 0 ||
                newHead.x >= WIDTH / CELL ||
                newHead.y < 0 ||
                newHead.y >= HEIGHT / CELL)
            {
                gameOver = true;
            }

            // ==================================================
            // SELF COLLISION
            // ==================================================

            if (!gameOver &&
                contains(snake, newHead))
            {
                gameOver = true;
            }

            if (!gameOver)
            {
                // Add head
                snake.insert(
                    snake.begin(),
                    newHead
                );

                // ==================================================
                // FOOD
                // ==================================================

                if (newHead.x == food.x &&
                    newHead.y == food.y)
                {
                    score+=10;;

                    food = createFood(snake);
                }
                else
                {
                    // Remove tail
                    snake.pop_back();
                }
            }
        }

        // ==================================================
        // DRAW
        // ==================================================

        // Background
        SDL_SetRenderDrawColor(
            renderer,
            20, 20, 20, 255
        );

        SDL_RenderClear(renderer);

        // ==================================================
        // BOUNDARY
        // ==================================================

        SDL_SetRenderDrawColor(
            renderer,
            255, 255, 255, 255
        );

        SDL_Rect border =
        {
            0,
            0,
            WIDTH,
            HEIGHT
        };

        SDL_RenderDrawRect(
            renderer,
            &border
        );

        // ==================================================
        // SNAKE
        // ==================================================

      

        for (int i = 0;
             i < snake.size();
             i++)
        {
            SDL_Rect body;

            body.x = snake[i].x * CELL;
            body.y = snake[i].y * CELL;

            body.w = CELL-1;
            body.h = CELL-1;
            if(i==0)
            {
            	SDL_SetRenderDrawColor(
            renderer,
            0, 255, 0, 255
        );
            	

            
        }
        else
        {
        	SDL_SetRenderDrawColor(
            renderer,
            0, 155, 0, 255
        );
        	
        }
        	SDL_RenderFillRect(
                renderer,
                &body
            );
        
        }

        // ==================================================
        // FOOD
        // ==================================================

        SDL_SetRenderDrawColor(
            renderer,
            255, 0, 0, 255
        );

        SDL_Rect foodRect;

        foodRect.x = food.x * CELL;
        foodRect.y = food.y * CELL;

        foodRect.w = CELL;
        foodRect.h = CELL;

        SDL_RenderFillRect(
            renderer,
            &foodRect
        );

        // ==================================================
        // SCORE
        // ==================================================

        string scoreText =
            "Score: " + to_string(score);

        drawText(
            renderer,
            font,
            scoreText,
            10,
            20
        );

        // ==================================================
        // TOUCH BUTTONS
        // ==================================================

        SDL_SetRenderDrawColor(
            renderer,
            70, 70, 70, 255
        );

        SDL_RenderFillRect(
            renderer,
            &upButton
        );

        SDL_RenderFillRect(
            renderer,
            &downButton
        );

        SDL_RenderFillRect(
            renderer,
            &leftButton
        );

        SDL_RenderFillRect(
            renderer,
            &rightButton
        );

        // Button text
        drawText(
            renderer,
            font,
            "UP",
            285,
            547
        );

        drawText(
            renderer,
            font,
            "DOWN",
            270,
            627
        );

        drawText(
            renderer,
            font,
            "LEFT",
            160,
            587
        );

        drawText(
            renderer,
            font,
            "RIGHT",
            375,
            587
        );

        // ==================================================
        // GAME OVER
        // ==================================================

        if (gameOver)
        {
            drawText(
                renderer,
                font,
                "GAME OVER!",
                220,
                270
            );

        
        
        SDL_SetRenderDrawColor(
                renderer,
                0, 0, 255, 255
            );

            SDL_RenderFillRect(
                renderer,
                &resetButton
            );

            drawText(
                renderer,
                font,
                "RESTART",
                250,
                412
            );
        }
        // ==================================================
        // SHOW SCREEN
        // ==================================================

        SDL_RenderPresent(renderer);


        SDL_Delay(5);
    
    }

    // ==================================================
    // CLEAN UP
    // ==================================================
    
    

    TTF_CloseFont(font);

    SDL_DestroyRenderer(renderer);

    SDL_DestroyWindow(window);

    TTF_Quit();

    SDL_Quit();
    

    return 0;
}