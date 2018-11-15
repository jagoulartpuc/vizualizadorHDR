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

// Constante do tonemapping
float toneMappingEscala = 0.6;
float correcaoGama = 2.1;

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

void process()
{


    printf("Exposure: %.3f\n", exposure);

    //1 para Tone Mapping por correção gama, 2 para Tone Mapping
    modo = 3;

    int pos;
    for(pos=0; pos<sizeX*sizeY; pos++) {

        float redExposicao = (image[pos].r * exposure);
        float greenExposicao = (image[pos].g * exposure);
        float blueExposicao = (image[pos].b * exposure);

        float redToneMapping;
        float greenToneMapping;
        float blueToneMapping;

        if (modo != 2){
            redToneMapping = fastpow(redExposicao,(1 / correcaoGama));
            greenToneMapping = fastpow(greenExposicao,(1 / correcaoGama));
            blueToneMapping = fastpow(blueExposicao,(1 / correcaoGama));
        }
        else if(modo != 1) {
            redToneMapping =  redExposicao/(redExposicao + toneMappingEscala);
            greenToneMapping = greenExposicao/(greenExposicao + toneMappingEscala);
            blueToneMapping = blueExposicao/(blueExposicao + toneMappingEscala);
        }

        if(redToneMapping > 1) {
            redToneMapping = 1;
        }

        int red8 = (int)((((1.0) < (redToneMapping)) ? (1.0) : (redToneMapping))* 255);
        int green8 = (int)((((1.0) < (greenToneMapping)) ? (1.0) : (greenToneMapping))* 255);
        int blue8 = (int)((((1.0) < (blueToneMapping)) ? (1.0) : (blueToneMapping))* 255);

        image8[pos].r = red8;
        image8[pos].g = green8;
        image8[pos].b = blue8;

    }
    buildTex();
}

int main(int argc, char** argv)
{
    if(argc==1) {
        printf("hdrvis [image file.hdr]\n");
        exit(1);
    }

    printf("Digite o modo: ");
    scanf("%d", &modo);

    // Inicialização da janela gráfica
    init(argc,argv);

    FILE* arq = fopen("memorial.hdr","rb");

    RGBE_ReadHeader(arq, &sizeX, &sizeY, NULL);

    //Alocar memórias
    image8 = (RGB8*) malloc(sizeof(RGB8) * sizeX * sizeY);
    image = (RGBf*)malloc(sizeof(RGBf) * sizeX * sizeY);

    int result = RGBE_ReadPixels_RLE(arq, (float*)image, sizeX, sizeY);
    if (result == RGBE_RETURN_FAILURE) {
    }
    fclose(arq);

    printf("%d x %d\n", sizeX, sizeY);

    exposure = 1.0f;
    process();

    glutMainLoop();
    return 0;
}

