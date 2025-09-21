#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>


int main (int argc, char* args[])
{
    srand(time(NULL));

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Teste Carmim",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         400, 400, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);
    SDL_Rect r = { 40,20, 10,10 };
    SDL_Rect r2 = { 100, 100, 10, 10 };
    SDL_Rect r3 =  { 200, 200, 90, 70 };

    bool vrfqudrado = false;

    int stop = 0;
    int espera = 500;
    int mouseX, mouseY;
    int red = 255;
    int blue = 255;
    int green = 255;
    //SDL_Point ponto;


    while(stop == 0){

        //SDL_GetMouseState(&ponto.x, &ponto.y);

        SDL_SetRenderDrawColor(ren, red, green, blue, 0x00);
        SDL_RenderClear(ren);

        SDL_SetRenderDrawColor(ren, 0, 0, blue, 0x00);
        SDL_RenderFillRect(ren, &r);
        SDL_RenderPresent(ren);

        SDL_SetRenderDrawColor(ren, 50, 100, 150, 0x00);
        SDL_RenderFillRect(ren, &r2);
        SDL_RenderPresent(ren);

        if(vrfqudrado == true){
            SDL_SetRenderDrawColor(ren, red + 50, green + 100, blue + 150, 0x00);
            SDL_RenderFillRect(ren, &r3);
            SDL_RenderPresent(ren);
        }

        /*SDL_SetRenderDrawColor(ren, red + 50, green + 70, blue + 20, 0x00);
        SDL_RenderFillRect(ren, &r3);
        SDL_RenderPresent(ren);*/


        SDL_Event evt;
        Uint32 antes = SDL_GetTicks();
        int isevt = SDL_WaitEventTimeout(&evt, espera);

        if(isevt){
            espera -= (SDL_GetTicks() - antes);
            if(evt.type == SDL_KEYDOWN){
                switch(evt.key.keysym.sym){

                case SDLK_ESCAPE:
                    stop++;
                    break;

                case SDLK_UP:
                    r.y -= 10;
                    break;

                case SDLK_DOWN:
                    r.y += 10;
                    break;

                case SDLK_LEFT:
                    r.x -= 10;
                    break;

                case SDLK_RIGHT:
                    r.x += 10;
                    break;

                }
            }
            else{
                espera = 500;
            }

            if(evt.type == SDL_MOUSEBUTTONDOWN){
                SDL_GetMouseState(&mouseX, &mouseY);

                if(mouseX >= r.x && mouseX <= r.x + r.w && mouseY >= r.y && mouseY <= r.y + r.h){

                    vrfqudrado = true;

                    printf("\nX:%d, Y:%d", mouseX, mouseY);
                    red = rand() % 255;
                    green = rand() % 255;
                    blue = rand() % 255;

                    r2.x = rand() % 200;
                    r2.y = rand() % 200;

                    SDL_SetRenderDrawColor(ren, red, green, blue, 0x00);
                }

            }

            else{
                espera = 500;
            }

        }

        if(SDL_HasIntersection(&r, &r2)){
            printf("\nencostou\n");
        }

        /*if(SDL_PointInRect(&ponto, &r3)){

            printf("\nApareceu\n");
            SDL_SetRenderDrawColor(ren, red, green, blue, 0x00);
            SDL_RenderFillRect(ren, &r3);
            SDL_RenderPresent(ren);

        }*/

    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
