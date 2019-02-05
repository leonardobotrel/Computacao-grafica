//
//  main.c
//  quadrados_coloridos
//
//  Created by Leonardo Botrel Tobias on 16/08/2018.
//  Copyright © 2018 Leonardo Botrel. All rights reserved.
//
#include <SOIL/SOIL.h>
#include <GL/glew.h>      // use sinal de maior,menor Descomentar
#include <GL/freeglut.h>  // em vez de áspas Descomentar
//#include <GLUT/GLUT.h> //Comentar
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


//Texturas
GLuint texturaChao;
GLuint texturaCeu;
GLuint texturaCristal;
GLuint texturaPlataforma;
GLuint texturaPersonagem;
GLuint texturaPersonagema;
//Personagem
#define ALTURA_PERSONAGEM_Y1 20
#define ALTURA_PERSONAGEM_Y2 40
#define DISTANCIA_MINIMA_OBSTACULO 70
#define TEMPO_AUMENTA_DIFICULDADE 5000

//structs
typedef struct posicao {
    float x1, x2, y1, y2;
} tipo_posicao;

struct personagem {
    tipo_posicao posicao;
    int pontuacao;
    char* pontuacaoChar;
};

struct objeto {
    tipo_posicao posicao;
};

struct cenario{
    tipo_posicao posicao;
};
struct chao{
    tipo_posicao posicao;
};

typedef struct JOGADOR{

    char* nome;
    int score;

}t_jogador;

#define TAMANHO_VETOR 10
struct JOGADOR vetor[TAMANHO_VETOR];

struct personagem personagem;
struct objeto objeto1, objeto2, objeto3, objeto4;
struct chao chao;
struct cenario cenario;

enum telas {
    INICIO,
    JOGO,
    PAUSE,
    GAMEOVER,
    PONTUACAO,
    NOME
}tela;

//Variaveis globais
int corCeu = 1;
float tempo = 0.0;
int auxtempo = 10;
int escolha = 1;//Escolhe entre diferentes opcoes
int enter = -1;//Da enter em uma opcao
char nomeProvisorio [250] = "";
int posicaoVetor = 0;
int verifica_aumento_velocidade = 500;

//Controle persongem
int pulo = -1, abaixar = -1;
float v = 0.0, vo = 20.0, g = -2.8;

//Obstaculos
float vO = -1;

void setup()
{
    // define qual é a cor do fundo
    //glClearColor(0.0, 0.7, 1.0, 0.5); // azul claro
    //corCeu *= -1;

    glutPostRedisplay(); //redesenha a tela
}

// NOVIDADE: callback de "keyboard"
void teclaPressionada(unsigned char key, int x, int y)
{
    // vê qual tecla foi pressionada
    switch(key)
    {
        case 27:      // Tecla "ESC"
            exit(0);  // Sai da aplicação
            break;
        case 'w':
        case 'W':
            if (pulo == -1 && abaixar == -1) {
                pulo *= -1;
                glutPostRedisplay();
            }
            break;
        case 's':
        case 'S':
            if (pulo == -1) {
                abaixar = 1;
                glutPostRedisplay();
            }
            break;

        case 'p':
        case 'P':
            if (tela == JOGO) {
                tela = PAUSE;
            }
            break;

        //case 32:
         //   escolha *= -1;
         //   break;

        case 13:
            enter *= -1;
        default:
            break;
    }
}

void inicializa () {
    //telas
    tela = INICIO;

    corCeu = 1;
    vO = -4;
    vo = 25.0;
    g = -9.8;

    glEnable(GL_BLEND );
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   texturaChao = SOIL_load_OGL_texture(
       "texchao.png",
       SOIL_LOAD_AUTO,
       SOIL_CREATE_NEW_ID,
   SOIL_FLAG_INVERT_Y
 );
 if (texturaChao == 0) {
        printf("Erro do SOIL: '%s'\n", SOIL_last_result());
    }

    texturaCeu = SOIL_load_OGL_texture(
        "texceu.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
    SOIL_FLAG_INVERT_Y
  );
  if (texturaCeu == 0) {
         printf("Erro do SOIL: '%s'\n", SOIL_last_result());
     }
     texturaCristal = SOIL_load_OGL_texture(
         "Crystal.png",
         SOIL_LOAD_AUTO,
         SOIL_CREATE_NEW_ID,
     SOIL_FLAG_INVERT_Y
   );
   if (texturaCristal == 0) {
          printf("Erro do SOIL: '%s'\n", SOIL_last_result());
      }
      texturaPlataforma = SOIL_load_OGL_texture(
          "plataforma.png",
          SOIL_LOAD_AUTO,
          SOIL_CREATE_NEW_ID,
      SOIL_FLAG_INVERT_Y
    );
    if (texturaPlataforma == 0) {
           printf("Erro do SOIL: '%s'\n", SOIL_last_result());
       }

       texturaPersonagem = SOIL_load_OGL_texture(
           "hippo_spritesm1.png",
           SOIL_LOAD_AUTO,
           SOIL_CREATE_NEW_ID,
       SOIL_FLAG_INVERT_Y
     );
     if (texturaPersonagem == 0) {
            printf("Erro do SOIL: '%s'\n", SOIL_last_result());
        }
        texturaPersonagema = SOIL_load_OGL_texture(
            "abaixar.png",
            SOIL_LOAD_AUTO,
            SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
      );
      if (texturaPersonagema == 0) {
             printf("Erro do SOIL: '%s'\n", SOIL_last_result());
         }

    verifica_aumento_velocidade = 500;

    //personagem
    personagem.posicao.x1 = 10;
    personagem.posicao.x2 = 20;
    personagem.posicao.y1 = ALTURA_PERSONAGEM_Y1;
    personagem.posicao.y2 = ALTURA_PERSONAGEM_Y2;

    //Obstaculo
    objeto1.posicao.x1 = 161;
    objeto1.posicao.x2 = 171;
    objeto1.posicao.y1 = 20;
    objeto1.posicao.y2 = 35;

    objeto2.posicao.x1 = 211;
    objeto2.posicao.x2 = 231;
    objeto2.posicao.y1 = 38;
    objeto2.posicao.y2 = 50;

    objeto3.posicao.x1 = 301;
    objeto3.posicao.x2 = 311;
    objeto3.posicao.y1 = 20;
    objeto3.posicao.y2 = 35;

    objeto4.posicao.x1 = 381;
    objeto4.posicao.x2 = 401;
    objeto4.posicao.y1 = 38;
    objeto4.posicao.y2 = 50;
/*
    glTexCoord2f(0, 0);glVertex3f(0.0, 0.0, 0.0);
    glTexCoord2f(0, 1);glVertex3f(0.0, 20.0, 0.0);
    glTexCoord2f(1, 1);glVertex3f(200.0, 20.0, 0.0);
    glTexCoord2f(1,0);glVertex3f(200.0, 00.0, 0.0);
*/
    chao.posicao.x1 = 0.0;
    chao.posicao.x2 = 160.0;
    chao.posicao.y1 = 0.0;
    chao.posicao.y2 = 20.0;

    cenario.posicao.x1 = 0.0;
    cenario.posicao.x2 = 500.0;
    cenario.posicao.y1 = 20.0;
    cenario.posicao.y2 = 100.0;
/*
    glTexCoord2f(0, 0);glVertex3f(0.0, 20.0, 0.0);
    glTexCoord2f(0,1);glVertex3f(0.0, 100.0, 0.0);
    glTexCoord2f(0.3,1);glVertex3f(200.0, 100.0, 0.0);
    glTexCoord2f(0.3,0);glVertex3f(200.0, 20.0, 0.0);
*/

    //glutKeyboardFunc(teclaPressionada);

    setup();
}

void chaof()
{

  chao.posicao.x1 = 0.0;
  chao.posicao.x2 = 200.0;
  chao.posicao.y1 = 0.0;
  chao.posicao.y2 = 20.0;
}

void cenariof()
{

  cenario.posicao.x1 = 0.0;
  cenario.posicao.x2 = 500.0;
  cenario.posicao.y1 = 20.0;
  cenario.posicao.y2 = 100.0;
}

void dificuldade ()
{
    if (personagem.pontuacao > verifica_aumento_velocidade) {
        corCeu *= -1;
        if (corCeu == 1)
            glClearColor(0.0, 0.7, 1.0, 0.5); // azul claro
        else
            glClearColor(0.0, 0.0, 0.5, 0.0); //azul escuro

        //corCeu *= -1;
        vO += -2;
        vo += 3;
        g -= 5;
        verifica_aumento_velocidade += 500;
    }
}

//callback de desenho do chao (display)
void desenhaChao()
{
    //Define a cor do chao
    glColor3f(0.8, 0.4, 0.2);

    //desenha o chao
    glBegin(GL_TRIANGLE_STRIP);

    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 20.0, 0.0);
    glVertex3f(100.0, 0.0, 0.0);
    glVertex3f(100.0, 20.0, 0.0);

    glEnd();
glClear(GL_COLOR_BUFFER_BIT);
}

void texto(void* font, char* s, float x, float y) {
    glDisable(GL_TEXTURE_2D);
      glColor3f(1.0, 0.0, 0.0);//Amarelo
    unsigned int i;
    glRasterPos2f(x, y);

    for (i = 0; i  < strlen (s); i++) {// menor que
        glutBitmapCharacter(font, s[i]);
    }
}


void specialKeyboardInput(int key, int x, int y){
    switch (key) {
        case GLUT_KEY_UP:
            escolha *= -1;
            break;
        case GLUT_KEY_DOWN:
            escolha *= -1;
            break;
    }
}

void nomeJogador (unsigned char key, int x, int y) {
    char* c = malloc(sizeof(char));
    *c = key;
    strcat(nomeProvisorio, c);
    free (c);

    switch (key) {
        case 27:
            exit(0);
            break;

        case 13:
            enter *= -1;
            break;
        default:
            break;
    }
}

void salvaArquivo(t_jogador players[]){
    FILE * arq;

    arq = fopen("dados.bin", "wb");

    if(arq != NULL)
    {
        for (int i = 0; i < TAMANHO_VETOR; i++)
        fwrite(&players[i], sizeof(players[i]), 1, arq);
        fclose(arq);
    }
    else
    {
        printf("\nErro ao abrir o arquivo para leitura!\n");
        exit(1); // aborta o programa
    }

}

void ordenaJogador(){
    t_jogador aux;
    for (int i = 0; i < TAMANHO_VETOR; i++){
        for (int j = 0; j < TAMANHO_VETOR; j++){
            if (vetor[i].score > vetor[j].score){
                aux = vetor[i];
                vetor[i] = vetor[j];
                vetor[j] = aux;
            }
        }
    }

    salvaArquivo(vetor);
}

void incluirJogador(){
    int posicaoAux = 0;
    //ler_arquivo();
    printf("Passou\n");

    for (int i = 0; i < TAMANHO_VETOR; i++) {
        if (vetor[i].nome != NULL) {
            if (strcmp(vetor[i].nome, "") != 0) {
                posicaoAux++;
            }
        }
    }
    posicaoVetor = posicaoAux;

    if (posicaoVetor == TAMANHO_VETOR) {
        posicaoVetor = 0;
        free (vetor[posicaoVetor].nome);
    }

    vetor[posicaoVetor].nome = (char *)malloc(strlen(nomeProvisorio)+1);
    vetor[posicaoVetor].score = personagem.pontuacao;
    strcpy(vetor[posicaoVetor].nome, nomeProvisorio);
    //printf("%d - %d\n", personagem.pontuacao, vetor[posicaoVetor].score);
    personagem.pontuacao = 0;
    strcpy(nomeProvisorio, "");

    posicaoVetor++;
    ordenaJogador();
  }

int ler_arquivo()
{
    // abre o arquivo para leitura
    FILE * arq = fopen("dados.bin", "rb");

    if(arq != NULL)
    {
        int indice = 0;
        for (int i = 0; i < TAMANHO_VETOR; i++)
        {
            t_jogador p;

            size_t r = fread(&p, sizeof(t_jogador), 1, arq);

            // se retorno for menor que o count, então sai do loop
            if(r < 1) {
                //printf("break\n");
                break;
            }
            else {
                if (vetor[indice].nome == NULL) {
                    /*vetor[indice].nome = (char *)malloc(strlen(nomeProvisorio)+1);
                    vetor[indice].nome = "";*/
                    break;
                }

                //vetor[indice] = p;
                //strcpy(vetor[indice].nome, p.nome);
                vetor[indice].nome = p.nome;
                vetor[indice].score = p.score;
                //printf("%s: %d\n", vetor[indice].nome, vetor[indice].score);
                indice++;
            }
        }
        fclose(arq);
        return indice;
    }
    else
    {
        printf("\nErro ao abrir o arquivo para leitura!\n");
        return 0;
    }
}

void inicio() {
    glColor3f(1.0, 0.0, 0.0);//Amarelo
      texto(GLUT_BITMAP_HELVETICA_18, "-> Setas para selecionar a opcao, enter para escolher", 40, 12);
      texto(GLUT_BITMAP_HELVETICA_18, "-> W para pular e S para abaixar", 45, 8);
    switch (escolha) {
        case 1:
            texto(GLUT_BITMAP_HELVETICA_18, "JOGAR", 75, 60);
            texto(GLUT_BITMAP_HELVETICA_12, "PONTUACAO", 74, 50);
            if (enter == 1) {
                tela = JOGO;
                enter *= -1;
            }
            break;

        case -1:
            texto(GLUT_BITMAP_HELVETICA_12, "JOGAR", 77, 60);
            texto(GLUT_BITMAP_HELVETICA_18, "PONTUACAO", 71, 50);
            if (enter == 1) {
                tela = PONTUACAO;
                enter *= -1;
            }
            break;

        default:
            break;
    }

}

void pausaJogo() {
    glColor3f(1.0, 1.0, 1.0);
    texto(GLUT_BITMAP_HELVETICA_18, "PAUSE", 75, 80);
    glColor3f(1.0, 1.0, 0.0);
    texto(GLUT_BITMAP_HELVETICA_18, "RESUME", 74, 50);

    if (enter == 1) {
        tela = JOGO;
        enter *= -1;
    }
}

void gameOver () {
    glColor3f(1.0, 0.0, 0.0);//Branco
    texto(GLUT_BITMAP_HELVETICA_18, "TOMOU PAU", 70, 80);
    glColor3f(1.0, 0.0, 0.0);//Amarelo
    texto(GLUT_BITMAP_HELVETICA_18, "PONTUACAO:", 65, 60);
    texto(GLUT_BITMAP_HELVETICA_18, personagem.pontuacaoChar, 95, 60);

    switch (escolha) {
        case 1:
            glColor3f(1.0, 0.0, 0.0);//Branco
            texto(GLUT_BITMAP_HELVETICA_18, "INICIO", 75, 50);
            texto(GLUT_BITMAP_HELVETICA_12, "NOME", 77, 40);
            if (enter == 1) {
                enter *= -1;
                glDisable(GL_TEXTURE_2D);
                inicializa();
                incluirJogador();
                glEnable(GL_TEXTURE_2D);
                tela = INICIO;
            }
            break;

        case -1:
            texto(GLUT_BITMAP_HELVETICA_12, "INICIO", 77, 50);
            texto(GLUT_BITMAP_HELVETICA_18, "NOME", 75, 40);
            if (enter == 1) {
                strcpy(nomeProvisorio, "");
                tela = NOME;
                enter *= -1;
            }
        default:
            break;
    }
}

void nome () {
    glColor3f(1.0, 0.0, 0.0);//Amarelo

    texto(GLUT_BITMAP_HELVETICA_18, "DIGITE SEU NOME:", 65, 60);

    glColor3f(1.0, 0.0, 0.0);//Amarelo
    texto(GLUT_BITMAP_HELVETICA_18, nomeProvisorio, 70, 50);
    glutKeyboardFunc(nomeJogador);

    if (enter == 1) {
        tela = GAMEOVER;
        enter *= -1;
    }
}

void pontuacao () {
    int posicaoy = 70;
    glColor3f(1.0, 0.0, 0.0);//Amarelo

    texto(GLUT_BITMAP_HELVETICA_18, "INICIO", 75, 80);

    int teste = ler_arquivo();

    glColor3f(1.0, 1.0, 0.0);//Amarelo
    if (teste != 0) {
        for (int i = 0; i < TAMANHO_VETOR; i++) {
            if (vetor[i].nome != NULL) {
                texto(GLUT_BITMAP_HELVETICA_18, vetor[i].nome, 60, posicaoy);
                asprintf (&personagem.pontuacaoChar, "%i", vetor[i].score);
                texto(GLUT_BITMAP_HELVETICA_18, personagem.pontuacaoChar, 95, posicaoy);
                posicaoy -=5;
            }
        }
    }

    if (enter == 1) {
        tela = INICIO;
        enter *= -1;
    }
}
void abaixarr()
{glClear(GL_COLOR_BUFFER_BIT);


 glClear(GL_COLOR_BUFFER_BIT);
 glColor4f(1, 1, 1,1);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, texturaChao);
//glBegin(GL_TRIANGLE_FAN);
//desenha o chao
  glBegin(GL_TRIANGLE_STRIP);
glTexCoord2f(0,0.55);glVertex3f(personagem.posicao.x1, personagem.posicao.y1, 0.0);
glTexCoord2f(0.5,0.55);glVertex3f(personagem.posicao.x2, personagem.posicao.y1, 0.0);
glTexCoord2f(0,1);glVertex3f(personagem.posicao.x1, personagem.posicao.y2, 0.0);
glTexCoord2f(0.5,1);glVertex3f(personagem.posicao.x2, personagem.posicao.y2, 0.0);





glEnd();
glDisable(GL_TEXTURE_2D);
  glColor3f(1.0, 1.0, 1.0);

}

void desenhaPersonagem () {
    //Desenha o personagem
    //glBegin(GL_TRIANGLE_STRIP);
    glColor4f(1, 1, 1,1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texturaPersonagem);
   //glBegin(GL_TRIANGLE_FAN);

     glBegin(GL_TRIANGLE_STRIP);
/*
   glTexCoord2f(0,0);glVertex3f(personagem.posicao.x1, personagem.posicao.y1, 0.0);
   glTexCoord2f(0,1);glVertex3f(personagem.posicao.x2, personagem.posicao.y1, 0.0);
   glTexCoord2f(1,1);glVertex3f(personagem.posicao.x1, personagem.posicao.y2, 0.0);
   glTexCoord2f(1,0);glVertex3f(personagem.posicao.x2, personagem.posicao.y2, 0.0);
   */

  // glEnd();

    //Abaixar e Pular
    if (abaixar == 1) {


      glTexCoord2f(0.5,0);glVertex3f(personagem.posicao.x1, personagem.posicao.y1, 0.0);
      glTexCoord2f(1,0);glVertex3f(personagem.posicao.x2, personagem.posicao.y1, 0.0);
      glTexCoord2f(0.5,0.42);glVertex3f(personagem.posicao.x1, personagem.posicao.y2-3, 0.0);
      glTexCoord2f(1,0.42);glVertex3f(personagem.posicao.x2, personagem.posicao.y2-3, 0.0);



    }
    else if (pulo == -1) {
       if(auxtempo >= 0){
        //  printf("entrou1\n");
        glTexCoord2f(0,0.55);glVertex3f(personagem.posicao.x1, personagem.posicao.y1, 0.0);
        glTexCoord2f(0.5,0.55);glVertex3f(personagem.posicao.x2, personagem.posicao.y1, 0.0);
        glTexCoord2f(0,1);glVertex3f(personagem.posicao.x1, personagem.posicao.y2, 0.0);
        glTexCoord2f(0.5,1);glVertex3f(personagem.posicao.x2, personagem.posicao.y2, 0.0);
       auxtempo = auxtempo -1;
      }
       //printf("%d\n",auxtempo );
      /*  if(auxtempo <  0 && auxtempo >  -5  ){
      //if(auxtempo>0){
printf("entrou2\n");

          glTexCoord2f(0.5,0.6);glVertex3f(personagem.posicao.x1, personagem.posicao.y1, 0.0);
          glTexCoord2f(1,0.6);glVertex3f(personagem.posicao.x2, personagem.posicao.y1, 0.0);
          glTexCoord2f(0.5,1);glVertex3f(personagem.posicao.x1, personagem.posicao.y2, 0.0);
          glTexCoord2f(1,1);glVertex3f(personagem.posicao.x2, personagem.posicao.y2, 0.0);
          auxtempo = auxtempo -1;

        }*/
        if(auxtempo <  0 && auxtempo >  -20 ){

//printf("entrou3\n");

          glTexCoord2f(0,0);glVertex3f(personagem.posicao.x1, personagem.posicao.y1, 0.0);
          glTexCoord2f(0.5,0);glVertex3f(personagem.posicao.x2, personagem.posicao.y1, 0.0);
          glTexCoord2f(0,0.42);glVertex3f(personagem.posicao.x1, personagem.posicao.y2, 0.0);
          glTexCoord2f(0.5,0.42);glVertex3f(personagem.posicao.x2, personagem.posicao.y2, 0.0);
          auxtempo = auxtempo -1;

        }
      //  if(auxtempo <=  -20){
    /*  if(auxtempo<=0){
printf("entrou4\n");

          glTexCoord2f(0.5,0);glVertex3f(personagem.posicao.x1, personagem.posicao.y1, 0.0);
          glTexCoord2f(1,0);glVertex3f(personagem.posicao.x2, personagem.posicao.y1, 0.0);
          glTexCoord2f(0.5,0.42);glVertex3f(personagem.posicao.x1, personagem.posicao.y2, 0.0);
          glTexCoord2f(1,0.42);glVertex3f(personagem.posicao.x2, personagem.posicao.y2, 0.0);
          auxtempo = auxtempo -1;

        }*/
        if(auxtempo <= -20){
          printf("entrou5\n");
          auxtempo = 10;
        }
    }
    else {
/*
      glTexCoord2f(0,0);glVertex3f(personagem.posicao.x1, personagem.posicao.y1, 0.0);
      glTexCoord2f(1,0);glVertex3f(personagem.posicao.x2, personagem.posicao.y1, 0.0);
      glTexCoord2f(0,0.5);glVertex3f(personagem.posicao.x1, personagem.posicao.y2, 0.0);
      glTexCoord2f(1,0.5);glVertex3f(personagem.posicao.x2, personagem.posicao.y2, 0.0);*/
      glTexCoord2f(0,0.55);glVertex3f(personagem.posicao.x1, personagem.posicao.y1, 0.0);
      glTexCoord2f(0.5,0.55);glVertex3f(personagem.posicao.x2, personagem.posicao.y1, 0.0);
      glTexCoord2f(0,1);glVertex3f(personagem.posicao.x1, personagem.posicao.y2, 0.0);
      glTexCoord2f(0.5,1);glVertex3f(personagem.posicao.x2, personagem.posicao.y2, 0.0);

        personagem.posicao.y1 = ALTURA_PERSONAGEM_Y1 + ((v*tempo)-0.5*(g)*(tempo*tempo));
        personagem.posicao.y2 = ALTURA_PERSONAGEM_Y2 + ((v*tempo)-0.5*(g)*(tempo*tempo));

        v = vo + (g)*tempo;
        tempo = tempo + 0.14;


        if (personagem.posicao.y1 < ALTURA_PERSONAGEM_Y1)//Voltou a posicao inicial
        {
            v = 0;
            pulo = -1;
            tempo = 0.0;
            personagem.posicao.y1 = ALTURA_PERSONAGEM_Y1;
            personagem.posicao.y2 = ALTURA_PERSONAGEM_Y2;
        }
    }

    glEnd();

    //Colisao

    if (personagem.posicao.x2 > objeto1.posicao.x1 && personagem.posicao.x1 < objeto1.posicao.x2 && personagem.posicao.y1 < objeto1.posicao.y2) {
        tela = GAMEOVER;
     }

    if (personagem.posicao.x2 > objeto2.posicao.x1 && personagem.posicao.x1 < objeto2.posicao.x2 && abaixar != 1) {
        tela = GAMEOVER;
    }

    if (personagem.posicao.x2 > objeto3.posicao.x1 && personagem.posicao.x1 < objeto3.posicao.x2 && personagem.posicao.y1 < objeto3.posicao.y2) {
        tela = GAMEOVER;
    }

    if (personagem.posicao.x2 > objeto4.posicao.x1 && personagem.posicao.x1 < objeto4.posicao.x2 && abaixar != 1) {
        tela = GAMEOVER;
    }
      glColor4f(1, 1, 1,1);
    personagem.pontuacao += 1;
    asprintf (&personagem.pontuacaoChar, "%i", personagem.pontuacao);
    texto(GLUT_BITMAP_HELVETICA_18, personagem.pontuacaoChar, 140, 85);

}

void obstaculo () {
    int naleatorio; //numero aleatorio para um objeto entrar em cena
    int const seed1 = 150;
    int const seed2 = 75;




    // glClear(GL_COLOR_BUFFER_BIT);
     glColor4f(1, 1, 1,1);
     glEnable(GL_TEXTURE_2D);
     glBindTexture(GL_TEXTURE_2D, texturaCristal);

      glBegin(GL_TRIANGLE_STRIP);

    glTexCoord2f(0, 0);glVertex3f(objeto1.posicao.x1, objeto1.posicao.y1, 0.0);
    glTexCoord2f(1, 0);glVertex3f(objeto1.posicao.x2, objeto1.posicao.y1, 0.0);
    glTexCoord2f(0, 1);glVertex3f(objeto1.posicao.x1, objeto1.posicao.y2, 0.0);
    glTexCoord2f(1, 1);glVertex3f(objeto1.posicao.x2, objeto1.posicao.y2, 0.0);




   glEnd();
    glDisable(GL_TEXTURE_2D);
      glColor3f(1.0, 1.0, 1.0);

    //Obstaculo 1
/*
    glColor3f(0.8, 0.0, 0.0);

    glBegin(GL_TRIANGLE_STRIP);

    glVertex3f(objeto1.posicao.x1, objeto1.posicao.y1, 0.0);
    glVertex3f(objeto1.posicao.x2, objeto1.posicao.y1, 0.0);
    glVertex3f(objeto1.posicao.x1, objeto1.posicao.y2, 0.0);
    glVertex3f(objeto1.posicao.x2, objeto1.posicao.y2, 0.0);
*/

    objeto1.posicao.x1 = (objeto1.posicao.x1 + (vO*0.3));
    objeto1.posicao.x2 = (objeto1.posicao.x2 + (vO*0.3));

    if (objeto1.posicao.x2 <= 0) {
        srand( (unsigned)time(NULL) );
        naleatorio = (160 + rand()%seed1);

        if ((naleatorio - objeto2.posicao.x2) > DISTANCIA_MINIMA_OBSTACULO  && (naleatorio - objeto3.posicao.x2) > DISTANCIA_MINIMA_OBSTACULO & (naleatorio - objeto4.posicao.x2) > DISTANCIA_MINIMA_OBSTACULO) {
            objeto1.posicao.x1 = naleatorio;
            objeto1.posicao.x2 = naleatorio + 10;
        }

    }
    glEnd();

    //Obstaculo 2
    // glClear(GL_COLOR_BUFFER_BIT);
     glColor4f(1, 1, 1,1);
     glEnable(GL_TEXTURE_2D);
     glBindTexture(GL_TEXTURE_2D, texturaPlataforma);
    //  glBegin(GL_TRIANGLE_FAN);
    //desenha o chao
      glBegin(GL_TRIANGLE_STRIP);

    glTexCoord2f(0, 0);glVertex3f(objeto2.posicao.x1, objeto2.posicao.y1, 0.0);
    glTexCoord2f(1, 0);glVertex3f(objeto2.posicao.x2, objeto2.posicao.y1, 0.0);
    glTexCoord2f(0, 1);glVertex3f(objeto2.posicao.x1, objeto2.posicao.y2, 0.0);
    glTexCoord2f(1, 1);glVertex3f(objeto2.posicao.x2, objeto2.posicao.y2, 0.0);




   glEnd();
    glDisable(GL_TEXTURE_2D);
      glColor3f(1.0, 1.0, 1.0);

    objeto2.posicao.x1 = (objeto2.posicao.x1 + (vO*0.3));
    objeto2.posicao.x2 = (objeto2.posicao.x2 + (vO*0.3));

    if (objeto2.posicao.x2 <= 0) {

        srand( (unsigned)time(NULL) );
        naleatorio = (160 + rand()%seed2);

        if ((naleatorio - objeto1.posicao.x2) > DISTANCIA_MINIMA_OBSTACULO  && (naleatorio - objeto3.posicao.x2) > DISTANCIA_MINIMA_OBSTACULO && (naleatorio - objeto4.posicao.x2) > DISTANCIA_MINIMA_OBSTACULO) {
            objeto2.posicao.x1 = naleatorio;
            objeto2.posicao.x2 = naleatorio + 20;
        }
    }

    glEnd();

    //Obstaculo 3
    glColor4f(1, 1, 1,1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texturaCristal);
   //  glBegin(GL_TRIANGLE_FAN);
   //desenha o chao
     glBegin(GL_TRIANGLE_STRIP);

   glTexCoord2f(0, 0);glVertex3f(objeto3.posicao.x1, objeto3.posicao.y1, 0.0);
   glTexCoord2f(1, 0);glVertex3f(objeto3.posicao.x2, objeto3.posicao.y1, 0.0);
   glTexCoord2f(0, 1);glVertex3f(objeto3.posicao.x1, objeto3.posicao.y2, 0.0);
   glTexCoord2f(1, 1);glVertex3f(objeto3.posicao.x2, objeto3.posicao.y2, 0.0);




  glEnd();
   glDisable(GL_TEXTURE_2D);
     glColor3f(1.0, 1.0, 1.0);

    objeto3.posicao.x1 = (objeto3.posicao.x1 + (vO * 0.3));
    objeto3.posicao.x2 = (objeto3.posicao.x2 + (vO * 0.3));

    if (objeto3.posicao.x2 <= 0) {

        srand( (unsigned)time(NULL) );
        naleatorio = (160 + rand()%seed2);

        if ((naleatorio - objeto1.posicao.x2) > DISTANCIA_MINIMA_OBSTACULO && (naleatorio - objeto2.posicao.x2) > DISTANCIA_MINIMA_OBSTACULO && (naleatorio - objeto4.posicao.x2) > DISTANCIA_MINIMA_OBSTACULO) {
            objeto3.posicao.x1 = naleatorio;
            objeto3.posicao.x2 = naleatorio + 10;
        }
    }

    glEnd();

    //Obstaculo 4
    glColor4f(1, 1, 1,1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texturaPlataforma);
   //  glBegin(GL_TRIANGLE_FAN);
   //desenha o chao
     glBegin(GL_TRIANGLE_STRIP);

   glTexCoord2f(0, 0);glVertex3f(objeto4.posicao.x1, objeto4.posicao.y1, 0.0);
   glTexCoord2f(1, 0);glVertex3f(objeto4.posicao.x2, objeto4.posicao.y1, 0.0);
   glTexCoord2f(0, 1);glVertex3f(objeto4.posicao.x1, objeto4.posicao.y2, 0.0);
   glTexCoord2f(1, 1);glVertex3f(objeto4.posicao.x2, objeto4.posicao.y2, 0.0);




  glEnd();
   glDisable(GL_TEXTURE_2D);
     glColor3f(1.0, 1.0, 1.0);

    objeto4.posicao.x1 = (objeto4.posicao.x1 + (vO * 0.3));
    objeto4.posicao.x2 = (objeto4.posicao.x2 + (vO * 0.3));

    if (objeto4.posicao.x2 <= 0) {

        srand( (unsigned)time(NULL) );
        naleatorio = (160 + rand()%seed1);

        if ((naleatorio - objeto1.posicao.x2) > DISTANCIA_MINIMA_OBSTACULO && (naleatorio - objeto2.posicao.x2) > DISTANCIA_MINIMA_OBSTACULO && (naleatorio - objeto3.posicao.x2) > DISTANCIA_MINIMA_OBSTACULO) {
            objeto4.posicao.x1 = naleatorio;
            objeto4.posicao.x2 = naleatorio + 20;
        }
    }

    glEnd();
}

// callback de desenho (display)
void desenhaCena()
{
  glClear(GL_COLOR_BUFFER_BIT);


   glClear(GL_COLOR_BUFFER_BIT);
   glColor4f(1, 1, 1,1);
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, texturaChao);
  glBegin(GL_TRIANGLE_FAN);
  //desenha o chao
  //  glBegin(GL_TRIANGLE_STRIP);

  glTexCoord2f(0, 0);glVertex3f(chao.posicao.x1, chao.posicao.y1, 0.0);
  glTexCoord2f(0, 1);glVertex3f(chao.posicao.x1, chao.posicao.y2, 0.0);
  glTexCoord2f(1, 1);glVertex3f(chao.posicao.x2, chao.posicao.y2, 0.0);
  glTexCoord2f(1, 0);glVertex3f(chao.posicao.x2, chao.posicao.y1, 0.0);




  glEnd();
  glDisable(GL_TEXTURE_2D);
    glColor3f(1.0, 1.0, 1.0);


    glColor4f(1, 1, 1, 1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texturaCeu);
   glBegin(GL_TRIANGLE_FAN);
   //desenha o ceu
   //  glBegin(GL_TRIANGLE_STRIP);

   glTexCoord2f(0, 0);glVertex3f(cenario.posicao.x1, cenario.posicao.y1, 0.0);
   glTexCoord2f(0,1);glVertex3f(cenario.posicao.x1, cenario.posicao.y2, 0.0);
   glTexCoord2f(1,1);glVertex3f(cenario.posicao.x2, cenario.posicao.y2, 0.0);
   glTexCoord2f(1,0);glVertex3f(cenario.posicao.x2, cenario.posicao.y1, 0.0);

   glEnd();

   /*
   glDisable(GL_TEXTURE_2D);
     glColor3f(1.0, 1.0, 1.0);

     glClear(GL_COLOR_BUFFER_BIT);
     glColor4f(1, 1, 1,1);
     glEnable(GL_TEXTURE_2D);
     glBindTexture(GL_TEXTURE_2D, texturaCristal);
      glBegin(GL_TRIANGLE_FAN);
    //desenha o chao
    //  glBegin(GL_TRIANGLE_STRIP);

    glTexCoord2f(0, 0);glVertex3f(objeto1.posicao.x1, objeto1.posicao.y1, 0.0);
    glTexCoord2f(0, 1);glVertex3f(objeto1.posicao.x2, objeto1.posicao.y1, 0.0);
    glTexCoord2f(1, 1);glVertex3f(objeto1.posicao.x1, objeto1.posicao.y2, 0.0);
    glTexCoord2f(1, 0);glVertex3f(objeto1.posicao.x2, objeto1.posicao.y2, 0.0);




   glEnd();
    glDisable(GL_TEXTURE_2D);
      glColor3f(1.0, 1.0, 1.0);
     */



    switch (tela) {
      glutSwapBuffers();
        case JOGO:
            glEnable(GL_TEXTURE_2D);
            //Desenha o personagem
            desenhaPersonagem();

            //Desenha obstaculos
            obstaculo();

            //Aumenta a dificuldade
            dificuldade();

             chao.posicao.x1 = (chao.posicao.x1 + (vO*0.3));
             chao.posicao.x2 = (chao.posicao.x2 + (vO*0.3));


            if(chao.posicao.x2 <= 170)
             chaof();

            cenario.posicao.x1 = (cenario.posicao.x1 + (vO*0.008));
            cenario.posicao.x2 = (cenario.posicao.x2 + (vO*0.008));
            glDisable(GL_TEXTURE_2D);
            break;


        case INICIO:
            glDisable(GL_TEXTURE_2D);
            inicio();
            glEnable(GL_TEXTURE_2D);
            break;

        case PAUSE:
            glDisable(GL_TEXTURE_2D);
            pausaJogo();
            glEnable(GL_TEXTURE_2D);
            break;

        case GAMEOVER:
            glDisable(GL_TEXTURE_2D);
            gameOver();
            glEnable(GL_TEXTURE_2D);
            break;

        case NOME:
            glDisable(GL_TEXTURE_2D);
            nome();
            glEnable(GL_TEXTURE_2D);
            break;

        case PONTUACAO:
            glDisable(GL_TEXTURE_2D);
            pontuacao();
            glEnable(GL_TEXTURE_2D);
            break;

        default:
            break;

    }

   /*printf("xo1 = %f, xo2 = %f, yo1 = %f, yo2 = %f\nx1o1 = %f, x2o1 = %f, y1o1 = %f, y2o1 = %f\nx1o2 = %f, x2o2 = %f, y1o2 = %f, y2o2 = %f\nx1o3 = %f, x2o3 = %f, y1o3 = %f, y2o3 = %f\n", xo1, xo2, yo1, yo2, x1o1, x2o1, y1o1, y2o1, x1o2, x2o2, y1o2, y2o2, x1o3, x2o3, y1o3, y2o3);*/

    glutSwapBuffers();
    glutPostRedisplay(); //redesenha a tela
        //glutTimerFunc(30, desenhaCena, 0);

}

// NOVIDADE: uma função que vamos chamar dentro
//    do "main"
// Inicia algumas variáveis de estado do OpenGL

/*void dificuldade () {
    corCeu *= -1;
    if (corCeu == 1)
        glClearColor(0.0, 0.7, 1.0, 0.5); // azul claro
    else
        glClearColor(0.0, 0.0, 0.5, 0.0); //azul escuro

    //corCeu *= -1;
    vO += -3;
    vo += 5;
    g -= 7;

    glutPostRedisplay(); //redesenha a tela
    //if(tela==JOGO)
    glutTimerFunc(TEMPO_AUMENTA_DIFICULDADE, dificuldade, 0);
}*/

// NOVIDADE: callback para o evento "reshape"
void redimensionada(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 160.0, 0.0, 100.0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}



void teclaLiberada(unsigned char key, int x, int y)
{
    switch (key) {
        case 's':
        case 'S':
            abaixar = -1;
            glutPostRedisplay();
            break;

        default:
            break;
    }
}

// Função principal
int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitContextVersion(1, 1); //Descomentar
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE); //Descomentar

    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(800, 500);
    glutInitWindowPosition(100, 100);

    glutCreateWindow("Game");
    // Registra callbacks para alguns eventos

    glutDisplayFunc(desenhaCena);
    glutTimerFunc(30, desenhaCena, 0);

    glutReshapeFunc(redimensionada);
    glutKeyboardFunc(teclaPressionada);
    glutKeyboardUpFunc(teclaLiberada);
    glutSpecialFunc(specialKeyboardInput);
    inicializa();
    // Configura valor inicial de algumas
    // variáveis de estado do OpenGL

    setup();
    //glutTimerFunc(TEMPO_AUMENTA_DIFICULDADE, dificuldade, 0); //Atualiza a cor do céu.

    glutMainLoop();
    return 0;
}
