#include <vector>
#include <iostream>
#ifndef PERSONAGEM_H
#define PERSONAGEM_H

#define CIMA 0
#define BAIXO 1
#define ESQUERDA 2
#define DIREITA 3

using namespace std;

int max_projeteis = 50;
int max_inimigos = 35;


struct Jogador {
    int x, y;   //  posicao do jogador
    int vida;
    int dano;
    int raioDeVisao;
    char simbolo;
    int ultimaDirecao;
    int cooldown; // controla o tempo entre tiros
    int score;
};

struct Projetil {
    int x, y;
    int direcao; // 0: cima 1: baixo 2: esquerda 3: direita
    int alcance;
    char simbolo;
    bool origem; // true = jogador, false = inimigo
};

void jogadorAtirar(Jogador& jogador, vector<Projetil>& projeteis) {
    if (jogador.cooldown > 0) {
        return;
    }
    if (projeteis.size() >= max_projeteis) {
        return;
    }

    Projetil projetil;
    switch (jogador.ultimaDirecao) {
        case CIMA:    projetil.x = jogador.x - 1; projetil.y = jogador.y; break;
        case BAIXO:   projetil.x = jogador.x + 1; projetil.y = jogador.y; break;
        case ESQUERDA:projetil.x = jogador.x; projetil.y = jogador.y - 1; break;
        case DIREITA: projetil.x = jogador.x; projetil.y = jogador.y + 1; break;
    }
    projetil.direcao = jogador.ultimaDirecao;
    projetil.alcance = 15; 
    projetil.simbolo = char(45);
    projetil.origem = true; // Origem é o jogador
    projeteis.push_back(projetil);

    jogador.cooldown = 20; // tempo entre tiros jogador
}

bool podeMover(int novoX, int novoY, int mapa[20][50]){
    if(novoX >= 0 && novoX < 20 && novoY >= 0 && novoY < 50){
        if(mapa[novoX][novoY] == 0){
            return true;
        }
    }
    return false;
}

void moverPersonagem(Jogador &jogador, int direcao, int mapa[20][50]){
    int novoX = jogador.x;
    int novoY = jogador.y;

    switch (direcao){
        case 0: novoX = jogador.x - 1; break;  //cima
        case 1: novoX = jogador.x + 1; break;  //baixo
        case 2: novoY = jogador.y - 1; break;  //esquerda
        case 3: novoY = jogador.y + 1; break;  //direita
    }

    if(podeMover(novoX, novoY, mapa)){
        jogador.x = novoX;
        jogador.y = novoY; 
    }
}

#endif