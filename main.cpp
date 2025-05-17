#include <iostream>
#include <windows.h>
#include <conio.h>
#include <cmath>
#include <chrono>
#include "mapa.h"
#include "personagem.h"
#include "inimigos.h"
#include "powerups.h"
#include "jogo.h"

int vidaInimigoBase = 5; 
int danoInimigoBase = 0; 

using namespace std;

int main(){
    srand(time(0));

    auto inicioJogo = chrono::steady_clock::now();
    int segundosDecorridos = 0;

        ///ALERTA: NAO MODIFICAR O TRECHO DE CODIGO, A SEGUIR.
        //INICIO: COMANDOS PARA QUE O CURSOR NAO FIQUE PISCANDO NA TELA
        HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO     cursorInfo;
        GetConsoleCursorInfo(out, &cursorInfo);
        cursorInfo.bVisible = false; // set the cursor visibility
        SetConsoleCursorInfo(out, &cursorInfo);
        //FIM: COMANDOS PARA QUE O CURSOR NAO FIQUE PISCANDO NA TELA
        //INICIO: COMANDOS PARA REPOSICIONAR O CURSOR NO INICIO DA TELA
        short int CX=0, CY=0;
        COORD coord;
        coord.X = CX;
        coord.Y = CY;
        //FIM: COMANDOS PARA REPOSICIONAR O CURSOR NO INICIO DA TELA
    ///ALERTA: NAO MODIFICAR O TRECHO DE CODIGO, ACIMA.


    Jogador jogador;
    jogador.x = 10;
    jogador.y = 25;
    jogador.raioDeVisao = 8;
    jogador.vida = 5;
    jogador.dano = 1;
    jogador.simbolo = char(64);
    jogador.ultimaDirecao = DIREITA;
    jogador.cooldown = 0;
    jogador.score = 0;

    Inimigo inimigo;
    inimigo.vida;
    inimigo.dano;

    vector<Inimigo> inimigos;
    vector<Projetil> projeteis;
    vector<string> mensagensPowerUps;

    int ondaAtual = 0;
    int maxOndas = 10;
    int inimigosPorOnda = 1;
    bool novaOnda = true;
    bool primeiroPowerUp = true;
    string mensagemPowerUp = "";

    int opcao = -1;
    while (opcao != 1) {
    exibirMenu();
    cin >> opcao;
    switch (opcao) {
        case 1: {
            int dificuldade = exibirDificuldade();
            switch (dificuldade) {
                case 1: // Fácil
                    vidaInimigoBase = 1;
                    danoInimigoBase = 1;
                    break;
                case 2: // Médio
                    vidaInimigoBase = 2;
                    danoInimigoBase = 2;
                    break;
                case 3: // Difícil
                    vidaInimigoBase = 3;
                    danoInimigoBase = 3;
                    break;
                default:
                    cout << "Dificuldade inválida! Usando Fácil como padrão.\n";
                    vidaInimigoBase = 1;
                    danoInimigoBase = 1;
                    _getch();
                    break;
            }
            opcao = 1; // Sai do loop para iniciar o jogo
            break;
        }
        case 2: {
            mostrarComoJogar();
            break;
        }
        case 3: {
            mostrarPowerUps();
            break;
        }
        case 0: {
            cout << "Saindo do jogo...\n";
            return 0;
        }
        default: {
            cout << "Opção inválida! Tente novamente.\n";
            break;
            }
        }
    }

    

    while(true){
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

        if (novaOnda && ondaAtual < maxOndas) {
            jogador.x = 10; // joga o player no meio do mapa
            jogador.y = 25;
            jogador.cooldown = 0; 
            ondaAtual++;
            spawnInimigos(inimigos, inimigosPorOnda, mapa, vidaInimigoBase, danoInimigoBase); 
            inimigosPorOnda += 2;
            novaOnda = false;
            if(ondaAtual == 1){
                jogador.score = 0;
            } else {
                jogador.score += 5;
            }

        }

        moverInimigos(inimigos, jogador, mapa);
        inimigosAtirar(inimigos, projeteis, jogador);
        moverProjeteis(projeteis, mapa);

        // verifica colisão de projéteis com o jogador
        for (auto i = projeteis.begin(); i != projeteis.end();) {
            if (i->x == jogador.x && i->y == jogador.y) {
                jogador.vida -= danoInimigoBase; // dano do projétil inimigo
                jogador.score -= 1; // -1 ponto por dano sofrido
                if (jogador.score < 0) jogador.score = 0; 
                i = projeteis.erase(i);
            } else {
                i++;
            }
        }
        for (int i = 0; i < 20; i++) {
            for (int j = 0; j < 50; j++) {
                double distancia = sqrt((i - jogador.x) * (i - jogador.x) + (j - jogador.y) * (j - jogador.y));
                if (distancia > jogador.raioDeVisao) {
                    cout << " ";
                } else {
                    bool desenhouEntidade = false;

                    // printa o jogador
                    if (i == jogador.x && j == jogador.y) {
                        cout << jogador.simbolo;
                        desenhouEntidade = true;
                    }
                    // printa inimigos
                    for (const auto& inimigo : inimigos) {
                        if (i == inimigo.x && j == inimigo.y && inimigo.ativo) {
                            cout << inimigo.simbolo;
                            desenhouEntidade = true;
                            break;
                        }
                    }
                    for (const auto& projetil : projeteis) {
                        if (projetil.x == i && projetil.y == j) {
                            cout << projetil.simbolo;
                            desenhouEntidade = true;
                            break;
                        }
                    }
                    if (!desenhouEntidade) {
                        switch (mapa[i][j]) {
                            case 0: cout << " "; break;
                            case 1: cout << char(219); break;
                        }
                    }
                }
            }
            cout << "\n";
        }



        for (auto i = projeteis.begin(); i != projeteis.end();) {
            bool colidiu = false;
            if (i->origem) { // verifica o projetil do jogador
                for (auto& inimigo : inimigos) {
                    if (inimigo.ativo && i->x == inimigo.x && i->y == inimigo.y) {
                        inimigo.vida -= jogador.dano;
                        if (inimigo.vida <= 0) {
                            inimigo.ativo = false;
                            if (multiplicadorAtivo) {
                                jogador.score += 2;
                            } else {
                                jogador.score += 1;
                            }                  
                        }
                        colidiu = true;
                        break;
                    }
                }
            }
            if (colidiu) {
                i = projeteis.erase(i);
            } else {
                i++;
            }
        }

        if (todosInimigosMortos(inimigos)) { 
            novaOnda = true;
            if (primeiroPowerUp || (rand() % 100 < 50)) {
                string mensagemRecebida = aplicarPowerUpAleatorio(jogador);
                if (!mensagemRecebida.empty()) {
                    mensagensPowerUps.push_back(mensagemRecebida);
                }
                primeiroPowerUp = false;
            }
        }

        if (jogador.cooldown > 0) {
            jogador.cooldown--;
        }

        if (_kbhit()) {
            char tecla = _getch();
            switch (tecla) {
                case 'w':
                    moverPersonagem(jogador, CIMA, mapa);
                    jogador.ultimaDirecao = CIMA;
                    break;
                case 's':
                    moverPersonagem(jogador, BAIXO, mapa);
                    jogador.ultimaDirecao = BAIXO;
                    break;
                case 'a':
                    moverPersonagem(jogador, ESQUERDA, mapa);
                    jogador.ultimaDirecao = ESQUERDA;
                    break;
                case 'd':
                    moverPersonagem(jogador, DIREITA, mapa);
                    jogador.ultimaDirecao = DIREITA;
                    break;
                case ' ':
                    jogadorAtirar(jogador, projeteis); 
                    break;
            }
            // limpa o input
            while (_kbhit()) _getch();
        }

        if (multiplicadorAtivo) {
            tempoMultiplicador--;
            if (tempoMultiplicador <= 0) {
                multiplicadorAtivo = false;
                mensagensPowerUps.push_back("Multiplicador de Score terminou!");
            }
        }

        auto agora = chrono::steady_clock::now();
        segundosDecorridos = chrono::duration_cast<chrono::seconds>(agora - inicioJogo).count();


        // printa HUD e mensagens
        COORD hudCoord = {0, 22}; // abaixo do mapa
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), hudCoord);
        cout << "Vida: " << jogador.vida 
             << " | Onda: " << ondaAtual 
             << "/" << maxOndas 
             << " | Score: " << jogador.score 
             << " | Tempo: " << segundosDecorridos << "seg   \n";

        for (const auto& msg : mensagensPowerUps) {
            cout << msg << "\n";
        }

        if (jogador.vida <= 0) {
            cout << "Game Over!" << endl;
            break;
        }
        if (ondaAtual >= maxOndas && todosInimigosMortos(inimigos)) {
            cout << "Parabéns! Você venceu todas as ondas!" << endl;
            break;
        }
    }                     
}