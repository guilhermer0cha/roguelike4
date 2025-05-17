#include <vector>
#include <cstdlib>
#include <windows.h>
#include "personagem.h"
#include "mapa.h"


#ifndef INIMIGOS_H
#define INIMIGOS_H


struct Inimigo {
    int x, y; // posicao inimigo
    int vida;
    int dano;
    char simbolo; // como ele aparecerá no mapa
    bool ativo;
    int cooldown;
};

void spawnInimigos(vector<Inimigo>& inimigos, int quantidade, int mapa[20][50], int vidaBase, int danoBase) {
    inimigos.clear();
    for (int i = 0; i < quantidade; i++) {
        Inimigo inimigo;
        bool posicaoValida = false;
        int tentativas = 0;
        const int maxTentativas = 100;

        while (!posicaoValida && tentativas < maxTentativas) {
            int lado = rand() % 3;
            switch (lado) {
                case 0: // topo
                    inimigo.x = 1;
                    inimigo.y = rand() % 48 + 1;
                    break;
                case 1: // esquerda
                    inimigo.x = rand() % 16 + 1;
                    inimigo.y = 1;
                    break;
                case 2: // direita
                    inimigo.x = rand() % 16 + 1;
                    inimigo.y = 48;
                    break;
            }

            if (inimigo.x >= 0 && inimigo.x < 20 && inimigo.y >= 0 && inimigo.y < 50 && podeMover(inimigo.x, inimigo.y, mapa)) {
                posicaoValida = true;
            }
            tentativas++;
        }

        if (!posicaoValida) {
            inimigo.x = 10;
            inimigo.y = 25;
        }

        inimigo.vida = vidaBase;
        inimigo.dano = danoBase;
        inimigo.ativo = true;
        inimigo.simbolo = char(234);
        inimigo.cooldown = 20;
        inimigos.push_back(inimigo);
        cout << "Inimigo criado com vida: " << vidaBase << " e dano: " << danoBase << endl;
    }
}

bool todosInimigosMortos(const vector<Inimigo>& inimigos) {
    for (const auto& inimigo : inimigos) {
        if (inimigo.ativo) return false;
    }
    return true;
}

void moverInimigos(vector<Inimigo>& inimigos, const Jogador& jogador, int mapa[20][50]) {
    static int contador = 0;
    contador++;
    if (contador < 40) return; // velocidade de movimento
    contador = 0;

    for (auto& inimigo : inimigos) {
        if (!inimigo.ativo) continue;

        // escolhe uma direção aleatória
        int direcao = rand() % 4; // 0: cima, 1: baixo, 2: esquerda, 3: direita 0:
        int novoX = inimigo.x;
        int novoY = inimigo.y;

        switch (direcao) {
            case CIMA: novoX--; break;
            case BAIXO: novoX++; break;
            case ESQUERDA: novoY--; break;
            case DIREITA: novoY++; break;
        }


        if (podeMover(novoX, novoY, mapa)) {
            inimigo.x = novoX;
            inimigo.y = novoY;
        }
    }
}

void inimigosAtirar(vector<Inimigo>& inimigos, vector<Projetil>& projeteis, const Jogador& jogador) {
    for (auto& inimigo : inimigos) {
        if (!inimigo.ativo || projeteis.size() >= max_projeteis) continue;

        if (inimigo.cooldown > 0) {
            inimigo.cooldown--;
            continue;
        }

        // verifica se estao nas mesmas linhas e colunas para atirar
        int direcao = -1;
        if (inimigo.x == jogador.x) {
            // mesma linha horizontal
            if (jogador.y > inimigo.y) direcao = DIREITA;
            else if (jogador.y < inimigo.y) direcao = ESQUERDA;
        } else if (inimigo.y == jogador.y) {
            // mesma linha vertical
            if (jogador.x > inimigo.x) direcao = BAIXO;
            else if (jogador.x < inimigo.x) direcao = CIMA;
        }

        if (direcao != -1) {
            Projetil projetil;
            projetil.x = inimigo.x;
            projetil.y = inimigo.y;
            projetil.direcao = direcao;
            projetil.alcance = 20;
            projetil.simbolo = char(250);
            projeteis.push_back(projetil);
            projetil.origem = false;
            inimigo.cooldown = 50; // tempo entre os tiros
        }
    }
}

void moverProjeteis(vector<Projetil>& projeteis, int mapa[20][50]) {
    static int contador = 0;
    contador++;
    if (contador < 5) return; // move o projetil
    contador = 0;

    for (auto i = projeteis.begin(); i != projeteis.end();) {
        Projetil& projetil = *i;
        int novoX = projetil.x;
        int novoY = projetil.y;

        switch (projetil.direcao) {
            case CIMA: novoX -= 1; break;
            case BAIXO: novoX += 1; break;
            case ESQUERDA: novoY -= 1; break;
            case DIREITA: novoY += 1; break;
        }

        if (novoX < 0 || novoX >= 20 || novoY < 0 || novoY >= 50 || !podeMover(novoX, novoY, mapa)) {
            i = projeteis.erase(i);
        } else {
            projetil.x = novoX;
            projetil.y = novoY;
            projetil.alcance--;
            if (projetil.alcance <= 0) {
                i = projeteis.erase(i);
            } else {
                i++;
            }
        }
    }
}




#endif