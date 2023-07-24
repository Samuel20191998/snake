// By Samuel Alves - E-mail: samuka20191998@gmail.com
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define linhas 20 // altura da tela ( padrão: 20 )
#define colunas 40 // Largura da tela ( padrão: 40 )
#define time 3 // Tempo de duração da animação de carregamento do jogo em segundos ( padrão 3 )
#define cc 219 // Caracter usado na cobra ( padrão: 219 )
#define ca 3 // Caracter usado no alimento ( padrão: 3 )
#define co 'X' // Caracter usado no obstaculo ( padrão: 'X' )
#define status_obs 1 // Caso este macro tenha o valor 0 o obstaculo ficará inativo durante o jogo ( padrão 1 )

void cls(void); // Esta função é usada no lugar do clássico "system("CLS")" por questões de segurança e eficiência
void cursor_poz(int x, int y); // Função para posicionar o cursor na tela
void write_main(void); // Função que desenha o retângulo em volta da tela
void progress(void); // Função que mostra a barra de progresso no carregamento do jogo ( serve basicamente para nada )
void placar(void); // Função que mostra os pontos e o nivel do usuario na parte de baixo da tela
void menu(void);  // função que direciona o usuario para o menu principal
int menu_pause(void);  // função que redireciona o usuario para o menu de pausa
void config(void); // função que exibe as opções de configuração da cor e do nivel
void perdeu(int arg); // Função responsavel basicamente por exibir na tela que o jogador perdeu o jogo, e também o motivo
void eat(void); // Função responsavel por criar alimento
void obst(void); // Função responsavel por criar obstaculo
void sair(void); // função responsavel pela confirmação e fechamento do programa
void jogar(void); // função que é chamada quando o usuario começa a jogar
void sobre(void); //  função que exibe na tela as informações sobre o jogo
void instrucoes(void); // função que exibe na tela as instruções do jogo

char cmd[30]; // variavel responsavel por armazenar o comando que vai ser executado no sistema operacional
int nivel = 2; // Variavel que contem o valor do nivel [ 1 - 3 ]
short pontos; // Variavel que contem o valor do pontuação do jogador
char cor[] = "02"; // Variavel que contem o valor da cor da tela
short color = 1; // Variavel que contem o numero da cor que sera exibido para o usuario
short tecla; // variavel que sera usada durante o programa para gravar a tecla que o usuario pressionar
short x,y,z,k; // Variaveis auxiliares de contagem
short direcao; // Variavel que irá guardar o número inteiro correspondente a direcao da cobra
float poz[2][250]; // Variavel que contem a pozição dos caracteres que compõem a cobra
short tamanho; // Variavel que irá guardar o número inteiro correspondente ao tamanho da cobra
float velocidade; // Variavel que irá guardar o número inteiro correspondente a velocidade da cobra
short alimento[2]; // Variavel que contem a pozição dos caracteres que compõem da cobra
short write_eat; // Variavel usada para verificar se o alimento já foi desenhado na tela

#if (status_obs != 0)
    short obstaculo[2][25]; // Variavel que contem a pozição dos caracteres que compõem os obstaculos
    short m,g; // Variveis de auxiliares contadoras
    short write_obs; // Variavel usada para verificar se o obstáculo já foi desenhado na tela
#endif

int main(void) {
    sprintf(cmd,"mode con:lines=%i cols=%i",linhas+2,colunas+3);
    system(cmd);
    sprintf(cmd,"color %s",cor);
    system(cmd);
    system("title Jogo Snake v 1.0 - Carregando...");
    progress();
    write_main();
    menu();
    getch();
    return 0;
}

void cursor_poz(int x, int y) {
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),(COORD){y,x});
}

void menu(void) {
    system("title Jogo Snake v 1.0 - by [S]amuka");
    cursor_poz((linhas/2-6),(colunas/2-7));
    printf("Jogo Snake v 1.0");
    cursor_poz((linhas/2-5),(colunas/2-6));
    printf("**************");
    cursor_poz((linhas/2-4),(colunas/2-3));
    printf("[S]amuka");

    cursor_poz((linhas/2),(colunas/2)-6);
    printf("Comece a Jogar");
    cursor_poz((linhas/2)+2,(colunas/2)-6);
    printf(" Sobre o Jogo");
    cursor_poz((linhas/2)+4,(colunas/2)-6);
    printf(" Instru%c%ces",135,228);
    cursor_poz((linhas/2)+6,(colunas/2)-6);
    printf("Configura%c%ces",135,228);
    cursor_poz((linhas/2)+8,(colunas/2)-6);
    printf("Sair do jogo");
    cursor_poz((linhas+1),(colunas+1));

    // seleção
    x = 0;
    while (1) {
        cursor_poz((linhas/2+x),(colunas/2)-8);
        printf("[");
        cursor_poz((linhas/2+x),(colunas/2)+9);
        printf("]");
        fflush(stdin);
        tecla = getch();

        if (tecla == 72 || tecla == 75 || tecla == 119 || tecla == 97) {  // apertou para cima, pra esquerda, W ou A
            cursor_poz((linhas/2+x),(colunas/2)-8);
            printf(" ");
            cursor_poz((linhas/2+x),(colunas/2)+9);
            printf(" ");
            if (x == 0) {
                x = 8;
            } else {
                x = x-2;
            }
        }

        if (tecla == 80 || tecla == 77|| tecla == 115 || tecla == 100) {  // apertou para baixo, pra direita, apertou S ou D
            cursor_poz((linhas/2+x),(colunas/2)-8);
            printf(" ");
            cursor_poz((linhas/2+x),(colunas/2)+9);
            printf(" ");
            if (x == 8) {
                x = 0;
            } else {
                x = x+2;
            }
        }

        if (tecla == 13) {
            switch (x) {
            case 0: { write_main(); jogar(); break;}
            case 2: { sobre(); break;}
            case 4: { instrucoes(); break;}
            case 6: { config(); break;}
            case 8: { sair(); write_main(); menu(); break;}
            }
        }
    }
}

void write_main(void) {
    // parte de cima
    cls();
    cursor_poz(0,1);
    printf("%c",201);
    for (y = 1; y < colunas; y++) {
        cursor_poz(0,y+1);
        printf("%c",205);
    }

    printf("%c",187);
    // lado direito
    for (y = 0; y < linhas; y++) {
        cursor_poz(y+1,colunas+1);
        printf("%c",186);
    }

    cursor_poz(y+1,colunas+1);
    printf("%c",188);
    // parte de baixo
    for (y = 0; y < colunas; y++) {
        cursor_poz(linhas+1,y+1);
        printf("%c",205);
    }

    cursor_poz(linhas+1,1);
    printf("%c",200);
    // lado esquerdo
    for (y = 0; y < linhas; y++) {
        cursor_poz(y+1,1);
        printf("%c",186);
    }
}

void config(void){
    write_main();
    cursor_poz((linhas/2-4),(colunas/2)-12);
    printf("Pressione <ENTER> para alterar");
    cursor_poz((linhas/2-3),(colunas/2)-7);
    printf("o valor desejado");
    cursor_poz((linhas/2),(colunas/2)-3);
    printf("Nivel: %i",nivel);
    cursor_poz((linhas/2+2),(colunas/2)-6);
    printf("Cor da tela: %i",color);
    cursor_poz((linhas/2+4),(colunas/2)-5);
    printf("Voltar <ESC>");
    x = 0;
    while (1) {
        cursor_poz((linhas/2+x),(colunas/2)-8);
        printf("[");
        cursor_poz((linhas/2+x),(colunas/2)+9);
        printf("]");
        fflush(stdin);
        tecla = getch();

        if (tecla == 72 || tecla == 75 || tecla == 119 || tecla == 97) {  // apertou para cima, pra esquerda, W ou A
            cursor_poz((linhas/2+x),(colunas/2)-8);
            printf(" ");
            cursor_poz((linhas/2+x),(colunas/2)+9);
            printf(" ");
            if (x == 0) {
                x = 4;
            } else {
                x = x-2;
            }
        }

        if (tecla == 80 || tecla == 77|| tecla == 115 || tecla == 100) {  // apertou para baixo, pra direita, apertou S ou D
            cursor_poz((linhas/2+x),(colunas/2)-8);
            printf(" ");
            cursor_poz((linhas/2+x),(colunas/2)+9);
            printf(" ");

            if (x == 4) {
                x = 0;
            } else {
                x = x+2;
            }
        }

        // Enter
        if (tecla == 13) {
            switch (x) {
                case 0: {
                    if (nivel == 1) {
                        nivel = 2;
                        cursor_poz((linhas/2),(colunas/2)-3);
                        printf("Nivel: %i",nivel);
                        break;
                    }
                    if (nivel == 2) {
                        nivel = 3;
                        cursor_poz((linhas/2),(colunas/2)-3);
                        printf("Nivel: %i",nivel);
                        break;
                    }
                    if (nivel == 3) {
                        nivel = 1;
                        cursor_poz((linhas/2),(colunas/2)-3);
                        printf("Nivel: %i",nivel);
                        break;
                    }
                }
                case 2: {
                    if (color == 1) {
                        color = 2;
                        cor[0] = '0';
                        cor[1] = '1';
                        sprintf(cmd,"COLOR %s",cor);
                        system(cmd);
                        cursor_poz((linhas/2+2),(colunas/2)-6);
                        printf("Cor do tela: %i",color);
                        break;
                    }
                    if (color == 2) {
                        color = 3;
                        cor[0] = '1';
                        cor[1] = '2';
                        sprintf(cmd,"COLOR %s",cor);
                        system(cmd);
                        cursor_poz((linhas/2+2),(colunas/2)-6);
                        printf("Cor do tela: %i",color);

                        break;
                    }
                    if (color == 3) {
                        color = 4;
                        cor[0] = '0';
                        cor[1] = '9';
                        cursor_poz((linhas/2+2),(colunas/2)-6);
                        printf("Cor do tela: %i",color);
                        sprintf(cmd,"COLOR %s",cor);
                        system(cmd);
                        break;
                    }
                    if (color == 4) {
                        color = 5;
                        cor[0] = '0';
                        cor[1] = 'D';
                        sprintf(cmd,"COLOR %s",cor);
                        system(cmd);
                        cursor_poz((linhas/2+2),(colunas/2)-6);
                        printf("Cor do tela: %i",color);
                        break;
                    }
                    if (color == 5) {
                        color = 6;
                        cor[0] = '0';
                        cor[1] = '4';
                        sprintf(cmd,"COLOR %s",cor);
                        system(cmd);
                        cursor_poz((linhas/2+2),(colunas/2)-6);
                        printf("Cor do tela: %i",color);
                        break;
                    }
                    if (color == 6) {
                        color = 7;
                        cor[0] = '0';
                        cor[1] = '7';
                        sprintf(cmd,"COLOR %s",cor);
                        system(cmd);
                        cursor_poz((linhas/2+2),(colunas/2)-6);
                        printf("Cor do tela: %i",color);
                        break;
                    }
                    if (color == 7) {
                        color = 8;
                        cor[0] = 'F';
                        cor[1] = '0';
                        sprintf(cmd,"COLOR %s",cor);
                        system(cmd);
                        cursor_poz((linhas/2+2),(colunas/2)-6);
                        printf("Cor do tela: %i",color);
                        break;
                    }
                    if (color == 8) {
                        color = 9;
                        cor[0] = 'C';
                        cor[1] = '1';
                        sprintf(cmd,"COLOR %s",cor);
                        system(cmd);
                        cursor_poz((linhas/2+2),(colunas/2)-6);
                        printf("Cor do tela: %i",color);
                        break;
                    }
                    if (color == 9) {
                        color = 1;
                        cor[0] = '0';
                        cor[1] = '2';
                        sprintf(cmd,"COLOR %s",cor);
                        system(cmd);
                        cursor_poz((linhas/2+2),(colunas/2)-6);
                        printf("Cor do tela: %i",color);
                        break;
                    }
                }
                case 4: {
                    write_main();
                    menu();
                    break;
                }
            }
        }

        // ESC
        if (tecla == 27) {
            write_main();
            menu();
        }
    }
}

void cls(void) {
    COORD coordScreen = {0, 0};
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConSize;
    HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
     GetConsoleScreenBufferInfo(hConsole, &csbi);
    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

    FillConsoleOutputCharacter(hConsole, TEXT(' '),dwConSize, coordScreen, &cCharsWritten);

    GetConsoleScreenBufferInfo(hConsole, &csbi);

    FillConsoleOutputAttribute(hConsole, csbi.wAttributes,dwConSize, coordScreen, &cCharsWritten);

    SetConsoleCursorPosition(hConsole, coordScreen);
}

void progress(void) {
    write_main();
    if (time != 0) {
        cursor_poz(linhas/2-6,colunas/2-5);
        printf("Carregando");
        cursor_poz(linhas/2-4,colunas/2-11);
        printf("[____________________]");
        x = 0;
        y = 0;
        while (x != 20) {
            cursor_poz(linhas/2-4,colunas/2-10+x);
            printf("X");
            cursor_poz(linhas/2-2,colunas/2-2);
            y = y+5;
            printf("%i%%",y);
            x++;
            if (kbhit()) { getch(); };
            Sleep(time*50);
        }
    }
}

void sair(void) {
    cls();
    write_main();
    cursor_poz(linhas/2,colunas/2-10);
    printf("Deseja sair? [S ou N]");
    if (kbhit()) { getch(); }
    while (1) {
        tecla = getch();
        if (tecla == 'S' || tecla == 's') {
            exit(0);
        }
        if (tecla == 'N' || tecla == 'n') {
            break;
        }
    }
}

void jogar(void) {
    pontos = 0;
    //write_main();
    placar();
    poz[0][0] = linhas/2; // pozição x inicial da cobra
    poz[1][0] = colunas/4; // pozição y inicial da cobra
    tamanho = 3; // Tamanho inicial da cobra
    direcao = 1; // Direção inicial da cobra
    velocidade = 200/nivel; // Define a velocidade da cobra com base no nivel
    eat();
    #if (status_obs != 0)
        m = 10;
        g = 0;
    #endif

    for (k = tamanho; k != 0; k--) {
        poz[0][k] = poz[0][k-1];
    }
    for (k = tamanho; k != 0; k--) {
        poz[1][k] = poz[1][k-1];
    }
    poz[0][0] = linhas/2;
    poz[1][0] = colunas/4+1;

    system("title Bom jogo!");

    // Exibe a cobra
    for (z=0; z<tamanho; z++) {
        cursor_poz(poz[0][z],poz[1][z]);
        printf("%c",cc);
    }

    while (1) {

        // Atualiza a posição da cobra na tela
        cursor_poz(poz[0][0], poz[1][0]);
        printf("%c",cc);

        // Exibe o alimento na tela
        if ( write_eat == 0 ) {
            cursor_poz(alimento[0],alimento[1]);
            printf("%c",ca);
            write_eat = 1;
        }

        // Controla a velocidade da cobra
        Sleep(velocidade);

        // Verfica se foi pressionada alguama tecla
        if (kbhit()) {
            // Verfica qual tecla foi pressionada
            tecla = getch();
            if ((tecla == 72 || toupper(tecla) == 'W') && ( direcao != 4 )) { direcao = 3; velocidade = 200/nivel+(40); } // apertou para cima ou W
            if ((tecla == 80 || toupper(tecla) == 'S') && ( direcao != 3 )) { direcao = 4; velocidade = 200/nivel+(40); } // apertou para baixo ou S
            if ((tecla == 75 || toupper(tecla) == 'A') && ( direcao != 1 )) { direcao = 2; velocidade = 200/nivel; } // apertou para a esquerda ou ou A
            if ((tecla == 77 || toupper(tecla) == 'D') && ( direcao != 2 )) { direcao = 1; velocidade = 200/nivel; } // apertou para a direita ou D
            if (tecla == 13 || tecla == 27 ){ menu_pause(); } // apertou ESC ou ENTER
        }

        // Detecta a captura do alimento
        if (poz[0][0] == alimento[0] && poz[1][0] == alimento[1]) {
            printf("\a");
            pontos++;
            tamanho++;
            eat();
            #if (status_obs != 0)
                m++;
            #endif
            placar();
        }

        #if (status_obs != 0)
            // Cria os obstaculos
            if (m == 10) {
                m = 0;
                obst();
            }
            // Exibe os obstaculos
            if ( write_obs == 0 ) {
                for(k=0;k<g;k++) {
                    cursor_poz(obstaculo[0][k],obstaculo[1][k]);
                    printf("%c",co);
                }
                write_obs = 1;
            }
            // Detecta caso haja colisão com obstaculos
            for (z = 0; z < g; z++) {
                if (poz[0][0] == obstaculo[0][z] && poz[1][0] == obstaculo[1][z]) {
                    perdeu(3);
                }
            }
        #endif

        // Detecta caso haja colisão com a parede
        if (poz[1][0] == colunas+1 || poz[1][0] == 1 || poz[0][0] == linhas-1 || poz[0][0] == 0) {
            perdeu(1);
        }

        // Detecta caso haja colisão com a cauda
        for (z = 1; z <= tamanho; z++) {
            if (poz[0][0] == poz[0][z] && poz[1][0] == poz[1][z]) {
                perdeu(2);
            }
        }

        // Avança a poziçao da cauda da cobra
        for (k = tamanho; k != 0; k--) {
            poz[0][k] = poz[0][k-1];
        }
        for (k = tamanho; k != 0; k--) {
            poz[1][k] = poz[1][k-1];
        }

        // Apaga o rastro
        cursor_poz(poz[0][tamanho],poz[1][tamanho]);
        printf(" ");

        // Avança a pozição cabeça da cobra
        if (direcao == 1) { poz[1][0]++;} // para o lado direito
        if (direcao == 2) { poz[1][0]--;} // para o lado esquerdo
        if (direcao == 3) { poz[0][0]--;} // para cima
        if (direcao == 4) { poz[0][0]++;} // para baixo
    }
}

#if (status_obs != 0)
void obst(void) {
    // srand((time(NULL)); Não é necessária, pois este jogo não é de adivinhação
    write_obs = 0;
    while (1) {
        k = 0;
        // Determina a pozição do obstaculo na tela
        obstaculo[0][g] = rand() % (linhas-4);
        obstaculo[0][g] = obstaculo[0][g] + 1;
        obstaculo[1][g] = rand() % (colunas-4);
        obstaculo[1][g] = obstaculo[1][g] + 2;

        if (alimento[0] != obstaculo[0][g] && alimento[1] != obstaculo[1][g]) {
            for (z = 0; z <= g; z++) {
                if (z != g && obstaculo[0][g] == obstaculo[0][z] && obstaculo[1][g] == obstaculo[1][z] ) {
                    k = -1;
                    break;
                }
            }
        }
        for (z=0; z<=tamanho; z++) {
            if (obstaculo[0][g] == poz[0][z] && obstaculo[1][g] == poz[1][z]) {
                k = -1;
                break;
            }
        }
        if (k != -1) {
            break;
        }
    }
    g++;
}
#endif

void eat(void) {
    // srand((time(NULL)); Não é necessária, pois este jogo não é de adivinhação e portanto os valores podem ser sempre os mesmos
    while (1) {
        write_eat = 0;
        k = 0;
        alimento[0] = 0+rand() % (linhas-4);
        alimento[0] = alimento[0] + 1;
        alimento[1] = 0+rand() % (colunas-4);
        alimento[1] = alimento[1] + 2;

        #if (status_obs != 0)
            for (z=0; z<=g; z++) {
                if (alimento[0] == obstaculo[0][z] && alimento[1] == obstaculo[1][z]) {
                    k = -1;
                    break;
                }
            }
        #endif

        for (z=0; z<tamanho; z++) {
            if (alimento[0] == poz[0][z] && alimento[1] == poz[1][z]) {
                k = -1;
                break;
            }
        }
        if (k != -1 ) {
            break;
        }
    }
}

int menu_pause(void) {
    while (1) {
        write_main();
        placar();
        cursor_poz(linhas/2,colunas/2-7);
        printf("Continuar Jogando");
        cursor_poz(linhas/2+2,colunas/2-7);
        printf("Menu principal");
        cursor_poz(linhas/2+4,colunas/2-7);
        printf("Sair do jogo");
        z = 0;
        k = 1;
        Sleep(300);

        if (kbhit()) {
            getch();
        }

        while (k) {
            cursor_poz(linhas/2+z,colunas/2-9);
            printf("%c",254);
            fflush(stdin);
            tecla = getch();
            cursor_poz(linhas/2+z,colunas/2-9);
            printf("%c",32);

            if (tecla == 72 || tecla == 75 || tecla == 119 || tecla == 97) {  // apertou para cima, pra esquerda, W ou A
                if (z == 0) {
                    z = 4;
                } else {
                    if (z == 2) {
                        z = 0;
                    }
                    if (z == 4) {
                        z = 2;
                    }
                }
            }
            if (tecla == 80 || tecla == 77 || tecla == 115 || tecla == 100) {  // apertou para baixo, pra direita, apertou S ou D
                if (z == 0) {
                    z = 2;
                } else {
                    if (z == 2) {
                        z = 4;
                    } else {
                        if (z == 4) {
                            z = 0;
                        }
                    }
                }
            }
            if (tecla == 13) {
                if (z == 0) {
                    write_eat = 0;
                    write_obs = 0;
                    write_main();
                    placar();

                    // Exibe a cobra
                    for (z=0; z<tamanho; z++) {
                        cursor_poz(poz[0][z],poz[1][z]);
                        printf("%c",cc);
                    }

                    return 0;
                }
                if (z == 2) {
                    write_main();
                    placar();
                    cursor_poz(linhas/2-4,colunas/2-16);
                    printf("Todo o seu progresso sera perdido!");
                    cursor_poz(linhas/2-2,colunas/2-16);
                    printf("Pressione <ENTER> para continuar");
                    cursor_poz(linhas/2-1,colunas/2-15);
                    printf("ou pressione <ESC> para voltar");
                    while (1) {
                        tecla = getch();
                        if (tecla == 13) {
                            write_main();
                            menu();
                        }
                        if (tecla == 27) {
                            write_main();
                            placar();
                            break;
                        }
                    }
                    break;
                }
                if (z == 4) {
                    sair();
                    break;
                }
            }
        }
    }
}

void perdeu(int arg) {

    if (kbhit()) { getch(); };

    printf("\a\a\a\a");
    write_main();
    cursor_poz(linhas/2-4,colunas/2-8);
    printf("Voc%c perdeu o jogo!",136);
    cursor_poz(linhas/2-2,colunas/2-8);
    if (arg == 1) {
        printf("Causa: Parede");
    }
    if (arg == 2) {
        printf("Causa: Cauda");
    }
    if (arg == 3) {
        printf("Causa: Obst%cculo",160);
    }
    cursor_poz(linhas/2,colunas/2-8);
    printf("Pontos: %i",pontos);
    cursor_poz(linhas/2+2,colunas/2-8);
    printf("Nivel: %i",nivel);
    cursor_poz(linhas/2+4,colunas/2-8);
    printf("Pressione <ENTER>");
    Sleep(100);

    while (1) {
        fflush(stdin);
        tecla = getch();
        if (tecla == 13) {
            write_main();
            menu();
        }
    }
}

void placar(void) {
    cursor_poz(linhas-1,1);
    printf("%c",204);
    for (y = 1; y < colunas; y++) {
        cursor_poz(linhas-1,y+1);
        printf("%c",205);
    }
    printf("%c",185);
    if (colunas >= 60) {
        cursor_poz(linhas,colunas/2-19);
        printf("Pontos: %i                        Nivel: %i",pontos,nivel);
    } else {
        cursor_poz(linhas,colunas/2-14);
        printf("Pontos: %i            Nivel: %i",pontos,nivel);
    }
}

void sobre(void) {
    cls();
    write_main();
    cursor_poz((linhas/2-4),(colunas/2-11));
    printf("Programador: Samuel Alves");
    cursor_poz((linhas/2-2),(colunas/2-10));
    printf("Jogo inteiramente feito");
    cursor_poz((linhas/2-1),(colunas/2-9));
    printf("   na linguagem C");
    cursor_poz((linhas/2+2),(colunas/2-10));
    printf("Press <ENTER> para voltar");

    while (1) {
        fflush(stdin);
        tecla = getch();
        if (tecla == 27 || tecla == 13) {
            write_main();
            menu();
        }
    }
}

void instrucoes(void) {
    cls();
    write_main();
    cursor_poz((linhas/2-8),(colunas/2-16));
    printf("Evite colidir com obst%cculos ou com",160);
    cursor_poz((linhas/2-7),(colunas/2-16));
    printf("a parede sen%co a cobra morrer%c.",198,160);
    cursor_poz((linhas/2-5),(colunas/2-16));
    printf("Use as setas para direcionar a cobra");
    cursor_poz((linhas/2-4),(colunas/2-16));
    printf("ou se preferir use W,S,A e D.");
    cursor_poz((linhas/2-2),(colunas/2-16));
    printf("Durante a partida pressione a tecla ");
    cursor_poz((linhas/2-1),(colunas/2-16));
    printf("<esc> ou <enter> para pausar");
    cursor_poz((linhas/2+1),(colunas/2-16));
    printf("A cada 10 alimentos um novo");
    cursor_poz((linhas/2+2),(colunas/2-16));
    printf("obst%cculo ir%c surgir.",160,160);
    cursor_poz((linhas/2+4),(colunas/2-16));
    printf("Alimente a cobra para elevar");
    cursor_poz((linhas/2+5),(colunas/2-16));
    printf("a sua pontua%c%co.",135,198);
    cursor_poz((linhas/2+7),(colunas/2-16));
    printf("Press <ENTER> para voltar");
    while (1) {
        fflush(stdin);
        tecla = getch();
        if (tecla == 27 || tecla == 13) {
            write_main();
            menu();
        }
    }
}

// by [S]amuka - samuka20191998@gmail.com
