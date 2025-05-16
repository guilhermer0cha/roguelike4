#ifndef POWERUPS_H
#define POWERUPS_H

#include <iostream>
#include <string>

using namespace std;



void aplicarPowerUpVida(Jogador& jogador) {
    jogador.vida += 5;
}


bool multiplicadorAtivo = false;
int tempoMultiplicador = 0;


void aplicarPowerUpMultiplicador() {
    multiplicadorAtivo = true;
    tempoMultiplicador = 300; // 5 segundos +-
}


void aplicarPowerUpVisao(Jogador& jogador) {
    jogador.raioDeVisao = 16;
}


string aplicarPowerUpAleatorio(Jogador& jogador) {
    int tipo = rand() % 3; 
    switch (tipo) {
        case 0:
            aplicarPowerUpVida(jogador);
            return "PowerUp: +5 de vida!";
        case 1:
            aplicarPowerUpMultiplicador();
            return "PowerUp: Multiplicador de Score Ativado!";
        case 2:
            aplicarPowerUpVisao(jogador);
            return "PowerUp: Visao aumentada!";
    }
    return "";
}







#endif