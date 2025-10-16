#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <assert.h>

int main (int argc, char* args[])
{
    srand(time(NULL));

    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init(); // Inicializar TTF

    SDL_Window* win = SDL_CreateWindow("Teste Carmim",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         400, 400, SDL_WINDOW_SHOWN
                      );

    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    // Carregar a fonte uma única vez no início
    TTF_Font* fnt = TTF_OpenFont("tiny.ttf", 20);
    SDL_Texture* txt = NULL;
    int texto_visivel = 0;

    //blocos
    SDL_Rect r = { 40,20, 10,10 };
    SDL_Rect r2 = { 100, 100, 50, 50 };
    SDL_Rect r3 =  { 200, 200, 90, 70 };
    SDL_Rect r4 = { 150, 150, 70, 70 };

    bool vrfqudrado = false;
    bool vrfqudrado2 = false;

    int stop = 0;
    int espera = 500;
    int mouseX, mouseY;
    int red = 255;
    int blue = 255;
    int green = 255;

    while(stop == 0){
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

            // MOVER a verificação do clique do mouse para DENTRO do if(isevt)
            if(evt.type == SDL_MOUSEBUTTONDOWN){
                SDL_GetMouseState(&mouseX, &mouseY);

                if(mouseX >= r.x && mouseX <= r.x + r.w && mouseY >= r.y && mouseY <= r.y + r.h){
                    vrfqudrado2 = true;
                    vrfqudrado = false;

                    printf("\nX:%d, Y:%d", mouseX, mouseY);
                    red = rand() % 255;
                    green = rand() % 255;
                    blue = rand() % 255;

                    r2.x = rand() % 400;
                    r2.y = rand() % 400;

                    printf("\nposição X r2:%d\n", r2.x);
                    printf("\nposição Y r2:%d\n", r2.y);
                }

                if(mouseX >= r2.x && mouseX <= r2.x + r2.w && mouseY >= r2.y && mouseY <= r2.y + r2.h){
                    printf("Tá clicando r2");
                    vrfqudrado = true;
                }

                if(mouseX >= r4.x && mouseX <= r4.x + r4.w && mouseY >= r4.y && mouseY <= r4.y + r4.h){
                    texto_visivel = 1;
                    // Criar a textura do texto apenas uma vez quando o quadrado for clicado
                    if(fnt && !txt){
                        SDL_Color clr = {0xFF, 0x00, 0x00, 0xFF};
                        SDL_Surface* sfc = TTF_RenderText_Blended(fnt, "deu certo!", clr);
                        txt = SDL_CreateTextureFromSurface(ren, sfc);
                        SDL_FreeSurface(sfc);
                    }
                }
            }
        }
        else{
            espera = 500;
        }

        if(SDL_HasIntersection(&r, &r2)){
            printf("\nencostou\n");
        }

        //renderização abaixo
        SDL_SetRenderDrawColor(ren, red, green, blue, 0x00);
        SDL_RenderClear(ren);

        SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
        SDL_RenderFillRect(ren, &r4);

        SDL_SetRenderDrawColor(ren, 0, 0, blue, 0x00);
        SDL_RenderFillRect(ren, &r);

        if(vrfqudrado2 == true){
            SDL_SetRenderDrawColor(ren, red + 70, green + 170, blue + 90, 0x00);
            SDL_RenderFillRect(ren, &r2);
        }

        if(vrfqudrado == true){
            SDL_SetRenderDrawColor(ren, red + 50, green + 100, blue + 150, 0x00);
            SDL_RenderFillRect(ren, &r3);
        }

        //renderizando texto
        if(texto_visivel == 1 && txt){
            SDL_Rect text_rect = {80, 80, 200, 40};
            SDL_RenderCopy(ren, txt, NULL, &text_rect);
        }

        SDL_RenderPresent(ren);
    }

    if (txt) SDL_DestroyTexture(txt);
    if (fnt) TTF_CloseFont(fnt);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
