#include "header.h"

#define CAMERA1 1
#define CAMERA2 2

//Texturas
GLuint texturaFrente, texturaDireita, texturaEsquerda, texturaTras, texturaChao;
//
static GLint fogMode;

//Structs
struct camera {
    float posicaoX, posicaoY, posicaoZ;
    float olharX, olharY, olharZ;
    float cimaX, cimaY, cimaZ;
    int cameraSelecionada;
};

struct nave {
    int frente, lado, pousar, vertical, abducao;
    float rotacaoInicial, rotacao;
};

struct posicaoCenario {
    float x, y, z;
};

struct camera camera;
struct nave nave;
struct posicaoCenario posicaoCenario;

enum cameraModo {
    a,
    b,
    c,
    d
}modoCamera;

//Variaveis globais
int iluminacao;
int neblina;

void tocar_musica(char const nome[40], int loop){
        Mix_Chunk *som = NULL;
        int canal;
        int canal_audio=2;
        int taxa_audio = 22050;
        Uint16 formato_audio = AUDIO_S16SYS;
        int audio_buffers = 4096;
        if(Mix_OpenAudio(taxa_audio, formato_audio, canal_audio, audio_buffers) != 0) {
                printf("Não pode inicializar audio: %s\n", Mix_GetError());
        }
       som = Mix_LoadWAV(nome);
        if(som == NULL) {
                printf("Não pode inicializar audio: %s\n", Mix_GetError());
        }
       Mix_HaltChannel(-1);
       canal = Mix_PlayChannel( -1, som, loop);
        if(canal == -1) {
                printf("Não pode inicializar audio: %s\n", Mix_GetError());
        }
}

void parar_musica(){
    Mix_HaltChannel(-1);
}

void desenhaNave() {
    //Anel de rotacao
    glColor3f(0.1, 0.2, 0.5); //Cinza Azulado
    glPushMatrix();
    
    glTranslatef(0, 1.9, 0);
    glRotatef(-90,1,0,0);
    glRotatef(nave.rotacaoInicial,0,0,1);
    glutWireTorus(0.1, 0.5, 10, 10);
    
    glPopMatrix();
    //Chassi
    glColor3f(0.5, 0.5, 0.5); //Cinza
    glPushMatrix();
    
    glTranslatef(0, 2, 0);
    glRotatef(-90,1,0,0);
    glutSolidTorus(0.1, 0.5, 10, 10);
    glPopMatrix();
    //Cabine
    glColor3f(0.1, 0.5, 1);//Azul
    glPushMatrix();
    
    glTranslatef(0, 2.1, 0);
    glRotatef(-90,1,0,0);
    glutSolidTorus(0.21, 0.21, 10, 10);
    
    glPopMatrix();
    
    if (nave.rotacaoInicial == 360) {
        nave.rotacaoInicial = 0;
    }
    else {
        nave.rotacaoInicial += nave.rotacao;
    }
}

void pousar () {
    if (posicaoCenario.x != -5.0 && posicaoCenario.z != 5.02) {
        if (posicaoCenario.x > 5) {
            posicaoCenario.x -= 0.05;
        }
        if (posicaoCenario.x < 5) {
            posicaoCenario.x += 0.05;
        }
        if (posicaoCenario.z > -5) {
            posicaoCenario.z -= 0.05;
        }
        if (posicaoCenario.z < -5) {
            posicaoCenario.z += 0.05;
        }

        //if ((posicaoCenario.x > -5.02 || posicaoCenario.x < -5.0) && (posicaoCenario.z < 5.02 || posicaoCenario.z > 5.0)) {
        if (posicaoCenario.y <= 2) {
            posicaoCenario.y += 0.05;
        }
        //printf ("%f - %f\n", posicaoCenario.x, posicaoCenario.z);
    }
}

void abducao() {
    if (posicaoCenario.y > -0.09 && posicaoCenario.y < 0.01) {
        if (nave.abducao == 1) {
            glColor3f(0.7, 0.9, 1.0);//branco
            glPushMatrix();

            //glTranslatef(5, 1, -5);
            glRotatef(-90,1,0,0);
            glutSolidCone (1, 2, 10, 10);

            glPopMatrix();
        }
    }
}

// Função responsável pela especificação dos parâmetros de iluminação
void defineIluminacao (void)
{
    GLfloat luzAmbiente[4]={0.2,0.2,0.2,1.0}; 
    GLfloat luzDifusa[4]={0.7,0.7,0.7,1.0};          // "cor" 
    GLfloat luzEspecular[4]={1.0, 1.0, 1.0, 1.0};// "brilho" 
    GLfloat posicaoLuz[4]={0.0, 50.0, 50.0, 1.0};
 
    // Capacidade de brilho do material
    GLfloat especularidade[4]={1.0,1.0,1.0,1.0}; 
    GLint especMaterial = 60;
 
    // Define a refletância do material 
    glMaterialfv(GL_FRONT,GL_SPECULAR, especularidade);
    // Define a concentração do brilho
    glMateriali(GL_FRONT,GL_SHININESS,especMaterial);
 
    // Ativa o uso da luz ambiente 
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);
 
    // Define os parâmetros da luz de número 0
    glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente); 
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa );
    glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular );
    glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz );   
}

skybox () {
    glClear(GL_COLOR_BUFFER_BIT);

    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);

    /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

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

    // Enable/Disable features
    glPushAttrib(GL_ENABLE_BIT);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);*/
 
    // Just in case we set all vertices to white.
    glColor4f(1, 1, 1, 1);
 
    // Render the front quad
    glBindTexture(GL_TEXTURE_2D, texturaFrente);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0.1); glVertex3f(  -15.0f, 0.0f, -15.0f );
        glTexCoord2f(1, 0.1); glVertex3f( 15.0f, 0.0f, -15.0f );
        glTexCoord2f(1, 1); glVertex3f( 15.0f,  15.0f, -15.0f );
        glTexCoord2f(0, 1); glVertex3f(  -15.0f,  15.0f, -15.0f );
    glEnd();
 
    // Render the left quad
    glBindTexture(GL_TEXTURE_2D, texturaFrente);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0.1); glVertex3f(  -15.0f, 0.0f,  15.0f );
        glTexCoord2f(1, 0.1); glVertex3f(  -15.0f, 0.0f, -15.0f );
        glTexCoord2f(1, 1); glVertex3f(  -15.0f,  15.0f, -15.0f );
        glTexCoord2f(0, 1); glVertex3f(  -15.0f,  15.0f,  15.0f );
    glEnd();
 
    // Render the back quad
    glBindTexture(GL_TEXTURE_2D, texturaFrente);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0.1); glVertex3f( 15.0f, 0.0f,  15.0f );
        glTexCoord2f(1, 0.1); glVertex3f(  -15.0f, 0.0f,  15.0f );
        glTexCoord2f(1, 1); glVertex3f(  -15.0f,  15.0f,  15.0f );
        glTexCoord2f(0, 1); glVertex3f( 15.0f,  15.0f,  15.0f );
 
    glEnd();
 
    // Render the right quad
    glBindTexture(GL_TEXTURE_2D, texturaFrente);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0.1); glVertex3f( 15.0f, 0.0f, -15.0f );
        glTexCoord2f(1, 0.1); glVertex3f( 15.0f, 0.0f,  15.0f );
        glTexCoord2f(1, 1); glVertex3f( 15.0f,  15.0f,  15.0f );
        glTexCoord2f(0, 1); glVertex3f( 15.0f,  15.0f, -15.0f );
    glEnd();
 
    /*
    // Render the top quad
    //glBindTexture(GL_TEXTURE_2D, _skybox[4]);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 1); glVertex3f( -0.5f,  0.5f, -0.5f );
        glTexCoord2f(0, 0); glVertex3f( -0.5f,  0.5f,  0.5f );
        glTexCoord2f(1, 0); glVertex3f(  0.5f,  0.5f,  0.5f );
        glTexCoord2f(1, 1); glVertex3f(  0.5f,  0.5f, -0.5f );
    glEnd();
 
    // Render the bottom quad
    //glBindTexture(GL_TEXTURE_2D, _skybox[5]);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f( -0.5f, -0.5f, -0.5f );
        glTexCoord2f(0, 1); glVertex3f( -0.5f, -0.5f,  0.5f );
        glTexCoord2f(1, 1); glVertex3f(  0.5f, -0.5f,  0.5f );
        glTexCoord2f(1, 0); glVertex3f(  0.5f, -0.5f, -0.5f );
    glEnd();
 
    // Restore enable bits and matrix
    //glPopAttrib();
    */
    glDisable(GL_TEXTURE_2D);
}


void desenhaCena(void)
{   
    float cor[] = { .5f, .5f, .5f };//neblina

    // Limpa a tela (com a cor definida por glClearColor(r,g,b)) para que
    // possamos desenhar
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    if (iluminacao == 1) {
        // Habilita a definição da cor do material a partir da cor corrente
        glEnable(GL_COLOR_MATERIAL);

        //Habilita o uso de iluminação
        glEnable(GL_LIGHTING);

        // Habilita a luz de número 0
        glEnable(GL_LIGHT0);

        // Habilita o depth-buffering
        //glEnable(GL_DEPTH_TEST);

        // Habilita o modelo de colorização de Gouraud
        glShadeModel(GL_SMOOTH);

        defineIluminacao();
    }

     if(neblina == 1){
            glEnable(GL_FOG);
    {
       GLfloat fogColor[4] = {0.5, 0.5, 0.5, 1.0};

       fogMode = GL_EXP;
       glFogi (GL_FOG_MODE, fogMode);
       glFogfv (GL_FOG_COLOR, fogColor);
       glFogf (GL_FOG_DENSITY, 0.15);
       glHint (GL_FOG_HINT, GL_DONT_CARE);
       glFogf (GL_FOG_START, 1.0);
       glFogf (GL_FOG_END, 5.0);
    }
    glClearColor(0.5, 0.5, 0.5, 1.0);  /* fog color */
    }
    else  glDisable(GL_FOG);

    /*
    if (neblina == 1) {
        glClearColor(cor[0], cor[1], cor[2], 1.0f);

        glFogi(GL_FOG_MODE, GL_EXP);        // Linear, exp. ou exp²
        glFogfv(GL_FOG_COLOR, cor);         // Cor
        glFogf(GL_FOG_DENSITY, 0.05f);      // Densidade
        glHint(GL_FOG_HINT, GL_DONT_CARE);  // Não aplicar se não puder
        glFogf(GL_FOG_START, 20.0f);         // Profundidade inicial
        glFogf(GL_FOG_END, 30.0f);           // Profundidade final
        glEnable(GL_FOG);
    }
    else {
        glDisable(GL_FOG);
    }   
    */
    
    // Começa a usar a cor vermelha
    glColor3f(1, 0, 0);

    glPushMatrix();

    //Posiciona a camera
    gluLookAt(camera.posicaoX, camera.posicaoY, camera.posicaoZ, //posicao do olho
    camera.olharX, camera.olharY, camera.olharZ, //para onde olhamos
    camera.cimaX, camera.cimaY, camera.cimaZ); //onde fica "para cima"

    //Skybox
    skybox();

    //glColor3f(0.0, 0.5, 1.0);
    //glutSolidCube (30.0);
    
    glPushMatrix();
    if (nave.pousar == 1) {
        pousar();
    }
    else {
        if (posicaoCenario.y > 0) {
            if (nave.vertical == -1) {
                nave.vertical = 0;
            }
            else {
                posicaoCenario.y -= 0.05;
            }
        }
        else {
            if (nave.frente == 1) {
                posicaoCenario.z += 0.05;
            }
            else if (nave.frente == -1) {
                posicaoCenario.z -= 0.05;
            }
            
            if (nave.lado == 1) {
                posicaoCenario.x -= 0.05;
            }
            else if (nave.lado == -1) {
                posicaoCenario.x += 0.05;
            }
            if (nave.vertical == 1) {
                posicaoCenario.y -= 0.01;
            }
            else if (nave.vertical == -1) {
                posicaoCenario.y += 0.01;
        }
        }
    }
    
    glTranslatef(posicaoCenario.x, posicaoCenario.y, posicaoCenario.z);//Translacao do cenario
    
    desenhaCenario();

    glPopMatrix();
    abducao();
    //Desenha a nave
    desenhaNave();

    glPopMatrix();

    // Começa a desenhar um polígono com os vértices especificados
    /*glBegin(GL_POLYGON);
        glVertex3f(20, 20, 0);
        glVertex3f(80, 20, 0);
        glVertex3f(80, 80, 0);
        glVertex3f(20, 80, 0);
    glEnd();*/

    // Diz ao OpenGL para colocar o que desenhamos na tela
    
    glutSwapBuffers();
    glDisable(GL_LIGHTING);
    glutPostRedisplay(); //redesenha a tela
}

// Inicia algumas variáveis de estado
void inicializa(void)
{
    tocar_musica("nome.wav",1);
    //Textura
    texturaFrente = SOIL_load_OGL_texture(
        "frente.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );
    if (texturaFrente == 0) {
        printf("Erro do SOIL: '%s'\n", SOIL_last_result());
    }

    //Camera
    camera.posicaoX = 0.0;
    camera.posicaoY = 4;
    camera.posicaoZ = 5.0;
    camera.olharX = 0.0;
    camera.olharY = 2.0;
    camera.olharZ = 0.0;
    camera.cimaX = 0.0;
    camera.cimaY = 1.0;
    camera.cimaZ = -1.0;
    camera.cameraSelecionada = CAMERA2;
    modoCamera = b;
    
    //Nave
    nave.frente = 0;
    nave.lado = 0;
    nave.rotacaoInicial = 0;
    nave.rotacao = 360/12;
    nave.pousar = 0;
    nave.abducao = 0;
    
    //cenario
    posicaoCenario.x = 0;
    posicaoCenario.y = 0;
    posicaoCenario.z = 0;

    //Sistema de iluminacao
    iluminacao = 1;

    //Neblina
    neblina = 1;

    glClearColor(1, 1, 1, 1);
    /*
    //Texturas
    // habilita mesclagem de cores, para termos suporte a texturas (semi-)transparentes
    glEnable(GL_BLEND );
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    texturaMario = SOIL_load_OGL_texture(
        "texceu.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
	);

    if (texturaMario == 0) {
        printf("Erro do SOIL: '%s'\n", SOIL_last_result());
    }
    */

    // cor para limpar a tela
    glClearColor(1, 1, 1, 0);      // branco
}

// Callback de redimensionamento
void redimensiona(int w, int h)
{
   glViewport(0, 0, w, h);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   //glFrustum(0, 160, 0, 90, -1, 50);
   gluPerspective(60.0f, 16.0f/9.0f, 0, 7);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

}

// Callback de evento de teclado
void teclado(unsigned char key, int x, int y)
{
   switch(key)
   {
      // Tecla ESC
      case 27:
         exit(0);
         break;

      case 49: //1
         camera.posicaoX = 0.0;
         camera.posicaoY = 5.0;
         camera.posicaoZ = 0.0;
         camera.olharX = 0.0;
         camera.olharY = 0.0;
         camera.olharZ = 0.0;
         camera.cimaX = 0.0;
         camera.cimaY = 0.0;
         camera.cimaZ = -1.0;
         camera.cameraSelecionada = CAMERA1;
         glutPostRedisplay();
         break;

      case 50: //2
         camera.cameraSelecionada = CAMERA2;

         if (modoCamera == b) {
            camera.posicaoX = 0.0;
            camera.posicaoY = 4.0;
            camera.posicaoZ = 5.0;
            camera.olharX = 0.0;
            camera.olharY = 2.0;
            camera.olharZ = 0.0;
            camera.cimaX = 0.0;
            camera.cimaY = 1.0;
            camera.cimaZ = -1.0;
         }

         if (modoCamera == a) {
            camera.posicaoX = 0.0;
            camera.posicaoY = 4.0;
            camera.posicaoZ = -5.0;
            camera.olharX = 0.0;
            camera.olharY = 2.0;
            camera.olharZ = 0.0;
            camera.cimaX = 0.0;
            camera.cimaY = 1.0;
            camera.cimaZ = 1.0;
         }

         if (modoCamera == c) {
            camera.posicaoX = 5.0;
            camera.posicaoY = 4.0;
            camera.posicaoZ = 0.0;
            camera.olharX = 0.0;
            camera.olharY = 2.0;
            camera.olharZ = 0.0;
            camera.cimaX = -1.0;
            camera.cimaY = 1.0;
            camera.cimaZ = 0.0;
         }

         if (modoCamera == d) {
            camera.posicaoX = -5.0;
            camera.posicaoY = 4.0;
            camera.posicaoZ = 0.0;
            camera.olharX = 0.0;
            camera.olharY = 2.0;
            camera.olharZ = 0.0;
            camera.cimaX = 1.0;
            camera.cimaY = 1.0;
            camera.cimaZ = 0.0;
         }

         glutPostRedisplay();
         break;

    case 'a':
       case 'A':
         if (camera.cameraSelecionada == CAMERA2) {
            modoCamera = a;
            teclado(50, 0, 0);
         }
         break;

    case 'b':
       case 'B':
        if (camera.cameraSelecionada == CAMERA2) {
            modoCamera = b;
            teclado(50, 0, 0);
        }
        break;

    case 'c':
       case 'C':
        if (camera.cameraSelecionada == CAMERA2) {
            modoCamera = c;
            teclado(50, 0, 0);
        }
        break;

    case 'd':
       case 'D':
        if (camera.cameraSelecionada == CAMERA2) {
            modoCamera = d;
            teclado(50, 0, 0);
        }
        break;

    case 'l':
    case 'L':
        if (iluminacao == 1) {
            iluminacao = 0;
        }
        else if (iluminacao == 0) {
            iluminacao = 1;
        }
        break;

    case 'r':
    case 'R':
        if (posicaoCenario.x > 4.0 && posicaoCenario.x < 6.0 && posicaoCenario.z > -5.5 && posicaoCenario.z < -4.0)
            nave.pousar = 1;
        break;

    case 't':
    case 'T':
        nave.pousar = 0;
        break;

    case 'n':
    case 'N':
        if (neblina == 1) {
            neblina = 0;
        }
        else if (neblina == 0) {
            neblina = 1;
        }
        break;

    case 'f':
    case 'F':
        if (nave.abducao == 1) {
            nave.abducao = 0;
        }
        else if (nave.abducao == 0) {
            nave.abducao = 1;
        }
        break;

      default:
         break;
   }
}

void specialKeyboardInput(int key, int x, int y){
    switch (key) {
        case GLUT_KEY_UP:
            if (nave.frente == 0) {
                nave.frente = 1;
            }
            else if (nave.frente == -1) {
                nave.frente = 0;
            }
            break;
            
        case GLUT_KEY_DOWN:
            if (nave.frente == 0) {
                nave.frente = -1;
            }
            else if (nave.frente == 1) {
                nave.frente = 0;
            }
            break;
            
        case GLUT_KEY_RIGHT:
            if (nave.lado == 0) {
                nave.lado = 1;
            }
            else if (nave.lado == -1) {
                nave.lado = 0;
            }
            break;
            
        case GLUT_KEY_LEFT:
            if (nave.lado == 0) {
                nave.lado = -1;
            }
            else if (nave.lado == 1) {
                nave.lado = 0;
            }
            break;

        case GLUT_KEY_PAGE_UP:
            if (nave.vertical == 0) {
                nave.vertical = 1;
            }
            else if (nave.vertical == -1) {
               nave.vertical = 0;
           }
           break;

       case GLUT_KEY_PAGE_DOWN:
           if (nave.vertical == 0) {
               if (posicaoCenario.y < 0) {
                    nave.vertical = -1;
               }
           }
           else if (nave.vertical == 1) {
               nave.vertical = 0;
           }
           break;
    }
}

// Rotina principal
int main(int argc, char **argv)
{
    // Acordando o GLUT
    glutInit(&argc, argv);

    // Definindo a versão do OpenGL que vamos usar
    glutInitContextVersion(1, 1);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    // Configuração inicial da janela do GLUT
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(800, 450);
    glutInitWindowPosition(100, 100);

    // Abre a janela
    glutCreateWindow("TP2");

    // Registra callbacks para alguns eventos
    glutDisplayFunc(desenhaCena);
    glutTimerFunc(30, desenhaCena, 0);
    glutReshapeFunc(redimensiona);
    glutKeyboardFunc(teclado);
    glutSpecialFunc(specialKeyboardInput);
    inicializa();

    // Entra em loop e nunca sai
    glutMainLoop();
    return 0;
}
