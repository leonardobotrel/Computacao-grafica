//Desenhar um curral
#include "header.h"

//structs
typedef struct posicao {
    float x1, x2, z1, z2;
} tipo_posicao;

struct chao {
    tipo_posicao posicao;
};

struct rua {
    tipo_posicao posicao;
};

struct chao chao;
struct rua rua;

//variaveis globais
float rotacaoInicialMoinho = 0, rotacaoMoinho = 360/96;

void inicializaCenario () {
    chao.posicao.x1 = -1000.0;
    chao.posicao.z1 = -1000.0;
    chao.posicao.x2 = 1000.0;
    chao.posicao.z2 = 1000.0;
}

void desenhaRua (float x1, float x2, float z1, float z2) {
    glColor3f(0.5, 0.5, 0.5);
    
    glBegin(GL_TRIANGLE_FAN);

    glVertex3f(rua.posicao.x1, 0.0, rua.posicao.z1);
    glVertex3f(rua.posicao.x1, 0.0, rua.posicao.z2);
    glVertex3f(rua.posicao.x2, 0.0, rua.posicao.z2);
    glVertex3f(rua.posicao.x2, 0.0, rua.posicao.z1);

    glEnd();
}

void desenhaArvore (float x, float z) {
     //Desenha tronco
    glColor3f(0.7, 0.3, 0.1);//marron
    glPushMatrix();

    glTranslatef(x + chao.posicao.x1, 0.0, z + chao.posicao.z1);
    glRotatef(-90,1,0,0);
    glutSolidCylinder(0.1,1,30,30);

    glPopMatrix();
    //Desenha as folhas
    glColor3f(0.0, 0.45, 0.0);//verde escuro
    glPushMatrix();

    glTranslatef(x + chao.posicao.x1, 0.6, z + chao.posicao.z1);
    glRotatef(-90, 1, 0, 0);
    glutSolidCone(0.4,1,10,10);

    glPopMatrix();
    
}

void desenhaCasa (float x, float z) {
    glColor3f(0.65, 0.4, 0.2);//Marron

    //Desenha estrutura
    glPushMatrix();

    glTranslatef(x + chao.posicao.x1, 0.0, z + chao.posicao.z1);
    glRotatef(40, 0, 0.1, 0.0);
    glutSolidCube (1.0);

    glPopMatrix();
    glColor3f(0.0, 0.0, 1.0);//Azul
    //Desenha janela
    glPushMatrix();

    glBegin(GL_TRIANGLE_FAN);
    
    glVertex3f(x + chao.posicao.x1 + 0.3, -0.3, z + chao.posicao.z1 + 0.45);
    glVertex3f(x + chao.posicao.x1 - 0.0, -0.3, z + chao.posicao.z1 + 0.7);
    glVertex3f(x + chao.posicao.x1 - 0.0, 0.2, z + chao.posicao.z1 + 0.7);
    glVertex3f(x + chao.posicao.x1 + 0.3, 0.2, z + chao.posicao.z1 + 0.45);
    
    glEnd();

    glPopMatrix();
    glColor3f(0.4, 0.2, 0.0);//marrom escuro
    //Desenha Porta
    glPushMatrix();

    glBegin(GL_TRIANGLE_FAN);
    
    glVertex3f(x + chao.posicao.x1 + 0.7, -0.4, z + chao.posicao.z1 + 0.15);
    glVertex3f(x + chao.posicao.x1 + 0.5, -0.4, z + chao.posicao.z1 + 0.35);
    glVertex3f(x + chao.posicao.x1 + 0.5, 0.4, z + chao.posicao.z1 + 0.35);
    glVertex3f(x + chao.posicao.x1 + 0.7, 0.4, z + chao.posicao.z1 + 0.15);
    
    glEnd();

    glPopMatrix();
    glColor3f(0.7, 0.3, 0.2);//Marrom alaranjado
    //Desenha teto
    glPushMatrix();

    glTranslatef(x + chao.posicao.x1, 0.5, z + chao.posicao.z1);
    glRotatef(-90, 1, 0, 0);
    glutSolidCone (1.0, 0.65, 4, 10);
    glColor3f(0.4, 0.1, 0.05);//marrom escuro
    glutWireCone (1.0, 0.65, 4, 10);

    glPopMatrix();
}

desenhaCurral (float x, float z) {
    //Coluna1
    glColor3f(0.5, 0.3, 0.1);//marron
    glPushMatrix();

    glTranslatef(x + chao.posicao.x1 - 1.6, 0.0, z + chao.posicao.z1);
    glRotatef(-90,1,0,0);
    glutSolidCylinder(0.1,1,30,30);

    glPopMatrix();
    //Coluna2
    glPushMatrix();

    glTranslatef(x + chao.posicao.x1 + 1.6, 0.0, z + chao.posicao.z1);
    glRotatef(-90,1,0,0);
    glutSolidCylinder(0.1,1,30,30);

    glPopMatrix();
    //Coluna3
    glPushMatrix();

    glTranslatef(x + chao.posicao.x1, 0.0, z + chao.posicao.z1 - 1.6);
    glRotatef(-90,1,0,0);
    glutSolidCylinder(0.1,1,30,30);

    glPopMatrix();
    //Coluna4
    glPushMatrix();

    glTranslatef(x + chao.posicao.x1, 0.0, z + chao.posicao.z1 + 1.6);
    glRotatef(-90,1,0,0);
    glutSolidCylinder(0.1,1,30,30);

    glPopMatrix();
    //Desenha teto
    glPushMatrix();

    glTranslatef(x + chao.posicao.x1, 0.9, z + chao.posicao.z1);
    glRotatef(-90, 1, 0, 0.0);
    glutSolidCone (2.0, 0.65, 4, 10);
    glColor3f(0.4, 0.1, 0.05);//marron
    glutWireCone (2.0, 0.65, 4, 10);

    glPopMatrix();
}

desenhaMoinho (float x, float z) {
    glColor3f(0.5, 0.3, 0.1);//marron
    //Estrutura
    glPushMatrix();
    glTranslatef(x + chao.posicao.x1, 0.0, z + chao.posicao.z1);
    glRotatef(-90,1,0,0);
    glutSolidCone (0.5, 1.5, 6, 5);
    glPopMatrix();

    //Eixo
    glPushMatrix();

    glTranslatef(x + chao.posicao.x1, 1.0, z + chao.posicao.z1);
    glutSolidCylinder(0.04,0.3,30,30);

    glPopMatrix();
    //Elices
    glPushMatrix();

    glTranslatef(x + chao.posicao.x1, 1.0, z + chao.posicao.z1 + 0.2);
    glRotatef(-90,1,0,0);
    glRotatef(rotacaoInicialMoinho,0,1,0);
    glutSolidCylinder(0.08,0.7,30,30);

    glPopMatrix();

    glPushMatrix();

    glTranslatef(x + chao.posicao.x1, 1.0, z + chao.posicao.z1 + 0.2);
    glRotatef(90,1,0,0);
    glRotatef(-rotacaoInicialMoinho,0,1,0);
    glutSolidCylinder(0.08,0.7,30,30);

    glPopMatrix();

    glPushMatrix();

    glTranslatef(x + chao.posicao.x1, 1.0, z + chao.posicao.z1 + 0.2);
    glRotatef(-90,0,1,0);
    glRotatef(-rotacaoInicialMoinho,1,0,0);
    glutSolidCylinder(0.08,0.7,30,30);

    glPopMatrix();

    glPushMatrix();

    glTranslatef(x + chao.posicao.x1, 1.0, z + chao.posicao.z1 + 0.2);
    glRotatef(90,0,1,0);
    glRotatef(rotacaoInicialMoinho,1,0,0);
    glutSolidCylinder(0.08,0.7,30,30);

    glPopMatrix();

    if (rotacaoInicialMoinho == 360) {
        rotacaoInicialMoinho = 0;
    }
    else {
        rotacaoInicialMoinho += rotacaoMoinho;
    }
}

desenhaBalanco (float x, float z) {
    //Coluna1
    glColor3f(0.5, 0.3, 0.1);//marron
    glPushMatrix();

    glTranslatef(x + chao.posicao.x1 + 1.5, 0.0, z + chao.posicao.z1 + 0.3);
    glRotatef(-115,1,0,0);
    glutSolidCylinder(0.07,1,30,30);

    glPopMatrix();
    //Coluna2
    glPushMatrix();

    glTranslatef(x + chao.posicao.x1 + 1.5, 0.0, z + chao.posicao.z1 - 0.3);
    glRotatef(-65,1,0,0);
    glutSolidCylinder(0.07,1,30,30);

    glPopMatrix();
    //Coluna3
    glPushMatrix();

    glTranslatef(x + chao.posicao.x1, 0.0, z + chao.posicao.z1 - 0.3);
    glRotatef(-65,1,0,0);
    glutSolidCylinder(0.07,1,30,30);

    glPopMatrix();
    //Coluna4
    glPushMatrix();

    glTranslatef(x + chao.posicao.x1, 0.0, z + chao.posicao.z1 + 0.3);
    glRotatef(-115,1,0,0);
    glutSolidCylinder(0.07,1,30,30);

    glPopMatrix();

    //eixo
    glPushMatrix();

    glTranslatef(x + chao.posicao.x1, 1.0, z + chao.posicao.z1);
    glRotatef(90,0,1,0);
    glutSolidCylinder(0.07,1.6,30,30);

    glPopMatrix();

    //Corda1
    glPushMatrix();

    glTranslatef(x + chao.posicao.x1 + 0.9, 0.3, z + chao.posicao.z1);
    glRotatef(-90,1,0,0);
    glutSolidCylinder(0.02,0.7,30,30);

    glPopMatrix();

    //Corda2
    glPushMatrix();

    glTranslatef(x + chao.posicao.x1 + 0.6, 0.3, z + chao.posicao.z1);
    glRotatef(-90,1,0,0);
    glutSolidCylinder(0.02,0.7,30,30);

    glPopMatrix();

    //Pneu
    glColor3f(0.2, 0.2, 0.2);//cinza
    glPushMatrix();

    glTranslatef (x + chao.posicao.x1 + 0.73, 0.3, z + chao.posicao.z1);
    glutSolidTorus (0.03, 0.18, 10, 10);

    glPopMatrix();
}

desenhaPiscina(float x, float z) {
    //Cimento
    glColor3f(0.8, 0.8, 0.8);//cinza

    glBegin(GL_TRIANGLE_FAN);
    
    glVertex3f(x + chao.posicao.x1 - 1, 0.1, z + chao.posicao.z1);
    glVertex3f(x + chao.posicao.x1 + 1, 0.1, z + chao.posicao.z1);
    glVertex3f(x + chao.posicao.x1 + 1, 0.1, z + chao.posicao.z1 + 1);
    glVertex3f(x + chao.posicao.x1 - 1, 0.1, z + chao.posicao.z1 + 1);
    
    glEnd();

    //Piscina
    glColor3f(0.0, 0.2, 0.8);//azul
    glBegin(GL_TRIANGLE_FAN);
    
    glVertex3f(x + chao.posicao.x1 - 0.8, 0.0, z + chao.posicao.z1);
    glVertex3f(x + chao.posicao.x1 + 0.8, 0.0, z + chao.posicao.z1);
    glVertex3f(x + chao.posicao.x1 + 0.8, 0.0, z + chao.posicao.z1 + 0.8);
    glVertex3f(x + chao.posicao.x1 - 0.8, 0.0, z + chao.posicao.z1 + 0.8);
    
    glEnd();
}

desenhaPoste (float x, float z) {
    GLfloat luzAmbiente[4]={0.2,0.2,0.2,1.0}; 
    GLfloat luzDifusa[4]={1.0, 1.0, 0.7, 1.0};          // "cor" 
    GLfloat luzEspecular[4]={1.0, 1.0, 1.0, 1.0};// "brilho" 
    GLfloat posicaoLuz[4]={x + chao.posicao.x1, 1.4, z + chao.posicao.z1, 1.0};
 
    // Capacidade de brilho do material
    GLfloat especularidade[4]={1.0, 1.0, 1.0, 1.0}; 
    GLint especMaterial = 60;

    //haste
    glColor3f(0.0, 0.0, 0.0);//preto
    glPushMatrix();

    glTranslatef(x + chao.posicao.x1, 0.0, z + chao.posicao.z1);
    glRotatef(-90,1,0,0);
    glutSolidCylinder(0.03,1,30,30);

    glPopMatrix();
    //Iluminacao
    // Define a refletância do material 
    glMaterialfv(GL_FRONT,GL_SPECULAR, especularidade);
    // Define a concentração do brilho
    glMateriali(GL_FRONT,GL_SHININESS,especMaterial);
 
    // Ativa o uso da luz local 
    glLightModelfv(GL_EMISSION, luzAmbiente);
 
    // Define os parâmetros da luz de número 0
    glLightfv(GL_LIGHT0, GL_EMISSION, luzAmbiente); 
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa );
    glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular );
    glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz ); 

    //Lampada
    glColor3f(1.0, 1.0, 0.7);//Amarelo
    glPushMatrix();

    glTranslatef(x + chao.posicao.x1, 1.0, z + chao.posicao.z1);
    glutSolidSphere (0.1, 10, 10);

    glPopMatrix();
}

desenhaFoguete (float x, float z) {
    //base
    glColor3f(0.8, 0.8, 0.8);//cinza

    glBegin(GL_TRIANGLE_FAN);
    
    glVertex3f(x + chao.posicao.x1 - 1, 0.1, z + chao.posicao.z1 - 1);
    glVertex3f(x + chao.posicao.x1 + 1, 0.1, z + chao.posicao.z1 - 1);
    glVertex3f(x + chao.posicao.x1 + 1, 0.1, z + chao.posicao.z1 + 1);
    glVertex3f(x + chao.posicao.x1 - 1, 0.1, z + chao.posicao.z1 + 1);
    
    glEnd();

    //pes
    glColor3f(1.0, 1.0, 1.0);//branco

    glBegin(GL_TRIANGLE_FAN);
    
    glVertex3f(x + chao.posicao.x1, 0.1, z + chao.posicao.z1 - 0.4);
    glVertex3f(x + chao.posicao.x1, 0.1, z + chao.posicao.z1 - 0.2);
    glVertex3f(x + chao.posicao.x1, 0.4, z + chao.posicao.z1 - 0.2);
    glVertex3f(x + chao.posicao.x1, 0.4, z + chao.posicao.z1 - 0.4);
    
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    
    glVertex3f(x + chao.posicao.x1, 0.1, z + chao.posicao.z1 + 0.4);
    glVertex3f(x + chao.posicao.x1, 0.1, z + chao.posicao.z1 + 0.2);
    glVertex3f(x + chao.posicao.x1, 0.4, z + chao.posicao.z1 + 0.2);
    glVertex3f(x + chao.posicao.x1, 0.4, z + chao.posicao.z1 + 0.4);
    
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    
    glVertex3f(x + chao.posicao.x1 - 0.4, 0.1, z + chao.posicao.z1);
    glVertex3f(x + chao.posicao.x1 - 0.2, 0.1, z + chao.posicao.z1);
    glVertex3f(x + chao.posicao.x1 - 0.2, 0.4, z + chao.posicao.z1);
    glVertex3f(x + chao.posicao.x1 - 0.4, 0.4, z + chao.posicao.z1);
    
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    
    glVertex3f(x + chao.posicao.x1 + 0.4, 0.1, z + chao.posicao.z1);
    glVertex3f(x + chao.posicao.x1 + 0.2, 0.1, z + chao.posicao.z1);
    glVertex3f(x + chao.posicao.x1 + 0.2, 0.4, z + chao.posicao.z1);
    glVertex3f(x + chao.posicao.x1 + 0.4, 0.4, z + chao.posicao.z1);
    
    glEnd();

    //Cilindro
    glPushMatrix();

    glTranslatef(x + chao.posicao.x1, 0.4, z + chao.posicao.z1);
    glRotatef(-90,1,0,0);
    glutSolidCylinder(0.2,1.4,30,30);

    glPopMatrix();

    //Parte de cima
    glPushMatrix();

    glTranslatef(x + chao.posicao.x1, 1.8, z + chao.posicao.z1);
    glRotatef(-90,1,0,0);
    glutSolidCone (0.2, 0.2, 10, 10);

    glPopMatrix();
}

baseDePouso (float x, float z) {
    glColor3f(0.7, 0.7, 0.4);//cinza
    glPushMatrix();

    glTranslatef(x + chao.posicao.x1, 0.0, z + chao.posicao.z1);
    glRotatef(90,1,0,0);
    glutSolidCylinder(1.5,0.7,30,30);

    glPopMatrix();
}

void desenhaCenario () {
    float i, j;
    inicializaCenario();
    
    glColor3f(0.45, 0.6, 0.12);
    
    //Desenho do plano
    glBegin(GL_TRIANGLE_FAN);
    
    glVertex3f(chao.posicao.x1, -1.0, chao.posicao.z1);
    glVertex3f(chao.posicao.x1, -1.0, chao.posicao.z2);
    glVertex3f(chao.posicao.x2, -1.0, chao.posicao.z2);
    glVertex3f(chao.posicao.x2, -1.0, chao.posicao.z1);
    
    glEnd();

    desenhaCasa (995, 995);
    desenhaCurral (1005, 1005);
    desenhaMoinho (1005, 995);
    baseDePouso (995, 1005);
    desenhaBalanco (1000, 995);
    desenhaPiscina (997, 994);
    desenhaFoguete (999, 1005);
    //desenhaPoste(1000, 998);

    //Desenha as arvores
    for (i = 950; i <= 1050; i+=10) {
        for (j = 950; j<= 1050; j+=10) {
            desenhaArvore (i, j);
        }
    }
    /*
    glColor3f(0, 0, 1);
    
    glPushMatrix();
    glTranslatef(-5, 1, -5);
    glutWireSphere (0.5, 10, 10);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-5, 1, 5);
    glutWireCone (1, 1, 10, 10);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(5, 1, -5);
    glutWireTorus (0.5, 1.5, 10, 10);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(5, 1, 5);
    glutWireCube (1.0);
    glPopMatrix();
    */
    glColor3f(1, 0, 0);//cor final
}
