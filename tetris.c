#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

// Estrutura da peça
typedef struct {
    char nome;  // 'I', 'O', 'T', 'L'
    int id;     // ID único da peça
} Peca;

// Fila circular
typedef struct {
    Peca pecas[TAM_FILA];
    int frente;
    int tras;
    int qtd;
} Fila;

// Pilha
typedef struct {
    Peca pecas[TAM_PILHA];
    int topo;
} Pilha;

// Variáveis globais
char tipos[] = {'I', 'O', 'T', 'L'};
int contadorID = 0;

// gerar peças aleatórias
Peca gerarPeca() {
    Peca p;
    p.nome = tipos[rand() % 4];
    p.id = contadorID++;
    return p;
}

// -- Funções da fila --
void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = -1;
    f->qtd = 0;
    for (int i = 0; i < TAM_FILA; i++) {
        enfileirar(f, gerarPeca());  
    }
}

int filaVazia(Fila *f) {
    return f->qtd == 0;
}

int filaCheia(Fila *f) {
    return f->qtd == TAM_FILA;
}

void enfileirar(Fila *f, Peca p) {
    if (filaCheia(f)) return;
    f->tras = (f->tras + 1) % TAM_FILA;
    f->pecas[f->tras] = p;
    f->qtd++;
}

Peca desenfileirar(Fila *f) {
    Peca p = f->pecas[f->frente];
    f->frente = (f->frente + 1) % TAM_FILA;
    f->qtd--;
    return p;
}

// -- Funções da pilha --
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

int pilhaCheia(Pilha *p) {
    return p->topo == TAM_PILHA - 1;
}

void empilhar(Pilha *p, Peca pe) {
    if (pilhaCheia(p)) return;
    p->topo++;
    p->pecas[p->topo] = pe;
}

Peca desempilhar(Pilha *p) {
    Peca pe = p->pecas[p->topo];
    p->topo--;
    return pe;
}

// -- Ações do jogo --
void jogarPeca(Fila *f) {
    if (filaVazia(f)) {
        printf("Fila vazia!\n");
        return;
    }
    Peca p = desenfileirar(f);
    printf("Jogou a peça [%c %d]\n", p.nome, p.id);
    enfileirar(f, gerarPeca()); 
}

void reservarPeca(Fila *f, Pilha *p) {
    if (filaVazia(f)) {
        printf("Fila vazia!\n");
        return;
    }
    if (pilhaCheia(p)) {
        printf("Pilha cheia!\n");
        return;
    }
    Peca pe = desenfileirar(f);
    empilhar(p, pe);
    printf("Peça [%c %d] reservada.\n", pe.nome, pe.id);
    enfileirar(f, gerarPeca());
}

void usarPeca(Pilha *p) {
    if (pilhaVazia(p)) {
        printf("Pilha vazia!\n");
        return;
    }
    Peca pe = desempilhar(p);
    printf("Usou a peça [%c %d]\n", pe.nome, pe.id);
}

void trocarPeca(Fila *f, Pilha *p) {
    if (filaVazia(f) || pilhaVazia(p)) {
        printf("Troca impossível!\n");
        return;
    }
    Peca temp = f->pecas[f->frente];
    f->pecas[f->frente] = p->pecas[p->topo];
    p->pecas[p->topo] = temp;
    printf("Troca realizada entre frente da fila e topo da pilha.\n");
}

void trocaMultipla(Fila *f, Pilha *p) {
    if (f->qtd < 3 || p->topo < 2) {
        printf("Troca múltipla impossível!\n");
        return;
    }
    for (int i = 0; i < 3; i++) {
        Peca temp = f->pecas[(f->frente + i) % TAM_FILA];
        f->pecas[(f->frente + i) % TAM_FILA] = p->pecas[p->topo - i];
        p->pecas[p->topo - i] = temp;
    }
    printf("Troca múltipla realizada!\n");
}

// -- Visualização --
void mostrarEstadoVisual(Fila *f, Pilha *p) {
    printf("\n=== Estado Atual ===\n");

    // pilha verticalmente (Topo -> Base)
    printf("Pilha de reserva (Topo -> Base):\n");
    for (int i = TAM_PILHA - 1; i >= 0; i--) {
        if (i <= p->topo)
            printf("   [%c %d]\n", p->pecas[i].nome, p->pecas[i].id);
        else
            printf("   [    ]\n");
    }

    // Fila horizontalmente (Frente -> Tras)
    printf("\nFila de peças (Frente -> Tras):\n   ");
    for (int i = 0; i < f->qtd; i++) {
        int idx = (f->frente + i) % TAM_FILA;
        printf("[%c %d] ", f->pecas[idx].nome, f->pecas[idx].id);
    }
    printf("\n====================\n");
}

// -- Menu principal --
int main() {
    srand(time(NULL));
    Fila fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    int opcao;
    do {
        mostrarEstadoVisual(&fila, &pilha);
        printf("\nOpções:\n");
        printf("1 - Jogar peça\n");
        printf("2 - Reservar peça\n");
        printf("3 - Usar peça\n");
        printf("4 - Trocar frente da fila com topo da pilha\n");
        printf("5 - Troca múltipla (3 peças)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        while(getchar() != '\n'); 

        switch(opcao) {
            case 1: jogarPeca(&fila); break;
            case 2: reservarPeca(&fila, &pilha); break;
            case 3: usarPeca(&pilha); break;
            case 4: trocarPeca(&fila, &pilha); break;
            case 5: trocaMultipla(&fila, &pilha); break;
            case 0: printf("Encerrando...\n"); break;
            default: printf("Opção inválida!\n");
        }
    } while (opcao != 0);

    return 0;
}
