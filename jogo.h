#include "inimigos.h"
#include "personagem.h"
#ifndef JOGO_H
#define JOGO_H




struct Jogo {
    int ondaAtual;
    int inimigos;
    int inimigosRestantes;
    bool entreOndas;
};

Jogo jogo;

void iniciarJogo(){
    jogo.ondaAtual = 1;
    jogo.inimigos = 14;
    jogo.inimigosRestantes = 14;
    jogo.entreOndas = false;
}

void exibirMenu() {
    system("cls");
    cout << "================== MENU PRINCIPAL ==================\n";
    cout << "1. Iniciar Jogo\n";
    cout << "2. Como Jogar\n";
    cout << "3. Power-ups\n";
    cout << "0. Sair\n";
    cout << "====================================================\n";
    cout << "Escolha uma opcao: ";
}

void mostrarComoJogar() {
    system("cls");
    cout << "=============== COMO JOGAR ===============\n";
    cout << "WASD - Mover o personagem\n";
    cout << "ESPACO - Atirar\n";
    cout << "Derrote os inimigos e sobreviva as ondas!\n";
    cout << "Evite ser atingido pelos projeteis inimigos.\n";
    cout << "\nPressione qualquer tecla para voltar...";
    _getch();
}

void mostrarPowerUps() {
    system("cls");
    cout << "=============== POWER-UPS ===============\n";
    cout << "Visao Noturna\n";
    cout << "Vida\n";
    cout << "Multiplicador";
    cout << "\nPressione qualquer tecla para voltar...";
    _getch();
}

#endif