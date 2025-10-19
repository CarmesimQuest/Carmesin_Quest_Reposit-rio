#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <assert.h>

typedef struct Fade{

    bool ativo;
    bool fade_in; //se for verdadeiro = fade in, se falso = fade out
    float alpha;
    float vlc;
    Uint32 duracao;
    Uint32 tempo_inicio;
}Fade;

//funções para texto abaixo
typedef struct TextTexture{

    SDL_Texture* texture;
    int largura;
    int altura;

}TTR;

TTR CarregaTexto(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color color){

    TTR result = {NULL, 0, 0};

    SDL_Surface* surface = TTF_RenderText_Blended(font, text, color);

    if(!surface){
        return result;
    }

    result.texture = SDL_CreateTextureFromSurface(renderer, surface);

    if(result.texture){

        result.largura = surface -> w;
        result.altura = surface -> h;

    }

    SDL_FreeSurface(surface);

    return result;
}

void renderText(SDL_Renderer* renderer, TTR text){

    if(!text.texture){
        return;
    }

    SDL_Rect dest = {0, 0, text.largura, text.altura};
    SDL_RenderCopy(renderer, text.texture, NULL, &dest);

}

void renderTextAt(SDL_Renderer* renderer, TTR text, int x, int y){

    if(!text.texture){
        return;
    }

    SDL_Rect dest = {x, y, text.largura, text.altura};
    SDL_RenderCopy(renderer, text.texture, NULL, &dest);
}

void freeTextTexture(TTR text){

    if(text.texture){
        SDL_DestroyTexture(text.texture);
    }
}

//funções para imagens abaixo
SDL_Texture* carregarTextura(const char* caminho, SDL_Renderer* renderer){

    SDL_Texture* textura = NULL;
    SDL_Surface* superficie = IMG_Load(caminho);

    if(!superficie){
        printf("Erro ao abrir carregar imagem %s: %s\n", caminho, IMG_GetError());
        return NULL;
    }

    textura = SDL_CreateTextureFromSurface(renderer, superficie);
    SDL_FreeSurface(superficie);

    return textura;
}

typedef struct ObjImage{

    SDL_Texture* textura;
    SDL_Rect posicao;

}OBI;

OBI criarObjetoImagem(const char* caminho, SDL_Renderer* renderer, int x, int y, int w, int h){

    OBI obj = {0};
    obj.textura = carregarTextura(caminho, renderer);
    obj.posicao = (SDL_Rect){x, y, w, h};
    return obj;

}

//funções para o controle do fade

void iniciarFade(Fade* fade, bool fade_in, Uint32 duracao_ms){

    fade -> ativo = true;
    fade -> fade_in = fade_in;
    fade -> duracao = duracao_ms;
    fade -> tempo_inicio = SDL_GetTicks();

    if(fade_in){
        fade -> alpha = 0.0f; //começa transparente
    }

    else{
        fade -> alpha = 1.0f;
    }
}

void atualizarFade(Fade* fade){

    if(!fade -> ativo){
        return;
    }

    Uint32 tempo_atual = SDL_GetTicks();
    Uint32 tempo_decorrido = tempo_atual - fade -> tempo_inicio;
    float progresso = (float)tempo_decorrido / fade -> duracao;

    if(progresso >= 1.0f){
        progresso = 1.0f;
        fade -> ativo = false;
    }

    if(fade -> fade_in){
        fade -> alpha = progresso;//0 -> 1
    }

    else{
        fade -> alpha = 1.0f - progresso; //1 -> 0
    }
}

void renderizaFade(SDL_Renderer* renderer, Fade* fade){

    if(!fade -> ativo && fade -> alpha == 0.0f){
        return;
    }

    // Criar uma textura preta semitransparente para o fade
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, (Uint8)(fade -> alpha * 255));
    SDL_RenderFillRect(renderer, NULL);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

int main (int argc, char* args[])
{
    srand(time(NULL));

    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init(); // Inicializar TTF

    SDL_Window* win = SDL_CreateWindow("Teste Carmim",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         800, 600, SDL_WINDOW_SHOWN
                      );

    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    //carrgar imagens
    OBI img1 = criarObjetoImagem("teste_fundo_Menu.png", ren, 0, 0, 800, 600);
    OBI img2 = criarObjetoImagem("vila_place_holder.png", ren, 0, 0, 800, 600);

    // Carregar a fonte uma única vez no início
    TTF_Font* fnt = TTF_OpenFont("minecraft_font.ttf", 12);
    SDL_Color corTexto = { 0, 255, 255, 255};
    int texto_visivel = 0;

    TTR hello = CarregaTexto(ren, fnt, "Deu certo", corTexto);
    TTR sdl = CarregaTexto(ren, fnt, "SDL2 com texto", corTexto);

    //blocos
    SDL_Rect r = { 0, 500, 200, 200 };
    SDL_Rect r2 = { 600, 500, 200, 200 };

    //bloco do fade
    Fade fade = {false, true, 0.0f, 0.0f, 0, 0};
    bool em_transicao = false;
    int prox_tela = 0; //0 = tela, 1 = tela2

    //permite a aparição da imagem
    bool vrfim1 = true;
    bool vrfim2 = false;

    int stop = 0;
    int espera = 500;
    int mouseX, mouseY;
    int red = 255;
    int blue = 255;
    int green = 255;
    SDL_Event evt;

    while(stop == 0){
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
                    case SDLK_f://usar o f pra testar o fade
                        if(!em_transicao){
                            em_transicao = true;
                            iniciarFade(&fade, false, 1000);//fade out por 1 seg
                        }
                        break;
                }
            }

            else if(evt.type == SDL_QUIT){
                stop++;
            }

            // MOVER a verificação do clique do mouse para DENTRO do if(isevt)
            if(evt.type == SDL_MOUSEBUTTONDOWN){
                SDL_GetMouseState(&mouseX, &mouseY);

                //click em r
                if(mouseX >= r.x && mouseX <= r.x + r.w && mouseY >= r.y && mouseY <= r.y + r.h){

                    em_transicao = true;
                    if(vrfim1 == true){
                        prox_tela = 2;
                    }
                    else{
                        prox_tela = 1;
                    }

                    iniciarFade(&fade, false, 500);

                    /*printf("\nX:%d, Y:%d", mouseX, mouseY);
                    red = rand() % 255;
                    green = rand() % 255;
                    blue = rand() % 255;

                    r2.x = rand() % 400;
                    r2.y = rand() % 400;*/

                    printf("\nposição X r2:%d\n", r2.x);
                    printf("\nposição Y r2:%d\n", r2.y);

                }

                //click em r2
                if(mouseX >= r2.x && mouseX <= r2.x + r2.w && mouseY >= r2.y && mouseY <= r2.y + r2.h){
                    texto_visivel = 1;
                }
            }
        }
        else{
            espera = 500;
        }

        //atualizar efeito de fade
        if(fade.ativo){
            atualizarFade(&fade);

            //Quando o fade out terminhar, iniciar fade in
            if(!fade.ativo && fade.fade_in == false && em_transicao){
                //troca a tela
                if(prox_tela == 1){
                    vrfim1 = true;
                    vrfim2 = false;
                }
                else if(prox_tela == 2){
                    vrfim1 = false;
                    vrfim2 = true;
                }

                iniciarFade(&fade, true, 500); //inicia o fade após a troca de tela
            }

            //quando o fade in terminar, finalizar transição
            if(!fade.ativo && fade.fade_in == true && em_transicao){
                em_transicao = false;
            }
        }

        if(SDL_HasIntersection(&r, &r2)){
            printf("\nencostou\n");
        }

        //renderização abaixo tela
        SDL_SetRenderDrawColor(ren, red, green, blue, 0x00);
        SDL_RenderClear(ren);

        //renderiza imagem

        if(img1.textura && vrfim1){
            SDL_RenderCopy(ren, img1.textura, NULL, &img1.posicao);
        }

        if(img1.textura && vrfim2){
            SDL_RenderCopy(ren, img2.textura, NULL, &img2.posicao);
        }

        //renderizando texto
        if(texto_visivel == 1){
            renderTextAt(ren, hello, 50, 50);
            renderTextAt(ren, sdl, 50, 100);
        }

        //renderiza bloco

        SDL_SetRenderDrawColor(ren, 0, 0, blue, 0x00);
        SDL_RenderFillRect(ren, &r);

        SDL_SetRenderDrawColor(ren, 255, 0, 0, 0);
        SDL_RenderFillRect(ren, &r2);

        //renderizar efeito de fade por ultimo(sobre tudo)
        if(fade.ativo || fade.alpha > 0.0f){
            renderizaFade(ren, &fade);
        }

        SDL_RenderPresent(ren);
    }

    //limpar imagens
    if(img1.textura){
        SDL_DestroyTexture(img1.textura);
    }
    if(img2.textura){
        SDL_DestroyTexture(img2.textura);
    }

    freeTextTexture(hello);
    freeTextTexture(sdl);
    TTF_CloseFont(fnt);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();

    return 0;
}
