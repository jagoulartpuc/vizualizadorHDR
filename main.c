#include <math.h>
#include <string.h>		// para usar strings

// Rotinas para acesso da OpenGL
#include "opengl.h"

// Rotinas para leitura de arquivos .hdr
#include "rgbe.h"

// Variáveis globais a serem utilizadas:

// Dimensões da imagem de entrada
int sizeX, sizeY;

// Imagem de entrada
RGBf* image;

// Imagem de saída
RGB8* image8;

// Fator de exposição inicial
float exposure = 1.0;

// Modo de exibição atual
int modo;

// Função pow mais eficiente (cerca de 7x mais rápida)
float fastpow(float a, float b);
float fastpow(float a, float b) {
     union { float f; int i; }
      u = { a };
      u.i = (int)(b * (u.i - 1065307417) + 1065307417);
      return u.f;
}

// Função principal de processamento: ela deve chamar outras funções
// quando for necessário (ex: algoritmos de tone mapping, etc)
void process()
{
    printf("Exposure: %.3f\n", exposure);
    //
    // EXEMPLO: preenche a imagem com pixels cor de laranja...
    //
    //
    // SUBSTITUA este código pelos algoritmos a serem implementados
    //
    int pos;
    for(pos=0; pos<sizeX*sizeY; pos++) {
        image[pos].r = image[pos].r*exposure;
        image[pos].g = image[pos].g*exposure;
        image[pos].b = image[pos].b*exposure;
    }
    float c = 0.5;
    for(pos=0; pos<sizeX*sizeY; pos++) {
        image[pos].r = (image[pos].r/(image[pos].r + c);
        image[pos].g = (image[pos].g/(image[pos].g + c);
        image[pos].b = (image[pos].b/(image[pos].b + c);
    }



    //
    // NÃO ALTERAR A PARTIR DAQUI!!!!
    //
    buildTex();
}

int main(int argc, char** argv)
{
    if(argc==1) {
        printf("hdrvis [image file.hdr]\n");
        exit(1);
    }

    // Inicialização da janela gráfica
    init(argc,argv);

    //
    // INCLUA aqui o código para LER a imagem de entrada
    //
    // Siga as orientações no enunciado para:
    //
    // 1. Descobrir o tamanho da imagem (ler header)
    // 2. Ler os pixels
    //

    typedef struct {
    float r, g, b;
    } RGBf;

    // Largura e altura da imagem
    int width, height;

    // Ponteiro para o início da imagem na memória
    RGBf* image;

    // Abre o arquivo
    FILE* arq = fopen("cathedral.hdr","rb");

    // Lê o header do arquivo, de onde são extraídas a largura e altura
    RGBE_ReadHeader(arq, &sizeX, &sizeY, NULL);

    // Aloca memória para a imagem inteira
    image = (RGBf*) malloc(sizeof(RGBf) * sizeX * sizeY);
    printf("%i\n", sizeX);
    printf("%i\n", sizeY);
    // Finalmente, lê a imagem para a memória
    int result = RGBE_ReadPixels_RLE(arq, (float*)image, sizeX, sizeY);
    if (result == RGBE_RETURN_FAILURE) {
   /// Tratamento de erro...
    }
    fclose(arq);

    exposure = 1.0f;

    /*
    // TESTE: cria uma imagem de 800x600
    sizeX = 800;
    sizeY = 600;

    printf("%d x %d\n", sizeX, sizeY);

    // Aloca imagem float
    image = (RGBf *)malloc(sizeof(RGBf) * sizeX * sizeY);

    // Aloca memória para imagem de 24 bits
    image8 = (RGB8*) malloc(sizeof(RGB8) * sizeX * sizeY);

    exposure = 1.0f; // exposição inicial
    */
    // Aplica processamento inicial

    process();

    // Não retorna... a partir daqui, interação via teclado e mouse apenas, na janela gráfica
    glutMainLoop();
    return 0;
