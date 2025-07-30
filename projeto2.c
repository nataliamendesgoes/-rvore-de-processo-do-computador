 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #define M 97
int colisaoL=0, colisaoQ=0; 

typedef struct lista{
    char frase[100];
    struct lista *prox; 
}T_Lista;

typedef struct sistema{
    int codigo;
    char nome[30];
    int prioridade;
    char estado[15];
    struct sistema *esq;
    struct sistema *dir;
    int bal;
}T_Sistema;

typedef struct {
    int codigo;
    char nome[15];
    int prioridade;
} Item;

typedef struct {
    Item* vetor;
    int capacidade;
    int tamanho;
} T_Heap;
/*typedef struct Hash {
    int chave;
    struct Hash* prox;
} T_Hash;*/

typedef struct Processo {
    int codigo;
    char estado[15]; 
    struct Processo* prox;
} Processo;

//LISTA SIMPLES
T_Lista* inicializa_lista();
T_Lista* cria_no(const char frase[]);
void insere_no_final(T_Lista *cabeca, const char *frase);

//AVL
int insere_T_Sistema(T_Sistema **p, int codigo, char nome[], int prioridade, char estado[], T_Lista *lista); 
T_Sistema *del(T_Sistema **p);
int delete(T_Sistema **p, int x, T_Lista *lista);
int bal_esq(T_Sistema **p);
int bal_dir(T_Sistema **p);
T_Sistema *busca_T_Sistema(T_Sistema **p, int x, T_Lista *lista) ;
void em_ordem(T_Sistema *, T_Lista *lista); 
int rot_dir(T_Sistema **p); 
int rot_esq(T_Sistema **p); 


//HEAP
T_Heap* criarT_Heap(int capacidade);
void inserir(T_Heap* heap, int codigo, char nome[], int prioridade, T_Lista *lista);
Item remover(T_Heap* heap, T_Lista *lista);
void imprimirHeap(T_Heap* heap, T_Lista *lista);
void destruirHeap(T_Heap* heap);
void troca(Item* a, Item* b);
void heapify(T_Heap* heap, int i); 
void alterarPrioridade(T_Heap* heap, int codigo, int novaPrioridade, T_Lista *lista);
void removerEspecifico(T_Heap* heap, int codigo, T_Lista *lista);

//HASH
Processo* criarProcesso(int codigo, const char estado[]);
void inicializa(Processo* vh[]);
int sondLinear(Processo* vh[], int codigo, const char estado[], T_Lista *lista);
int sondQuad(Processo* vh[], int codigo, const char estado[], T_Lista *lista);
void imprime(Processo* vh[]);
void limpar(Processo* vh[]);
void mudarEstado(Processo* vh[], int codigo,  char novoEstado, T_Lista *lista);
void listarPorEstado(Processo* vh[], const char estado[], T_Lista *lista);
void removerProcessoLinear(Processo* vh[], int codigo, T_Lista *lista);
void removerProcessoQuadratico(Processo* vh[], int codigo, T_Lista *lista);

void final(T_Lista *listaAvl, T_Lista *listaHeap, T_Lista *listaHashLin, T_Lista *listaHashQuad);

int main(){

    T_Sistema *raiz= NULL;
    T_Heap *heap = criarT_Heap(100);
    Processo* vlinear[M];
    Processo* vquad[M];
    inicializa(vlinear);
    inicializa(vquad);
    T_Lista *lista_Avl = inicializa_lista();
    T_Lista *lista_Heap = inicializa_lista();
    T_Lista *lista_HashLin = inicializa_lista();
    T_Lista *lista_HashQuad = inicializa_lista();
    
    FILE *arq;
    char comando[20];
    arq = fopen("natalia.txt", "r");

    if (arq == NULL) {
        printf("Impossivel abrir arquivo!\n");
        return 1;
    } else {
        while (fscanf(arq, "%19s", comando) == 1)  {
            printf("Comando lido: %s\n", comando);

            if (strcmp(comando, "InserirAVL") == 0) {
                int codigo, prioridade;
                char nome[30], estado[15];
                fscanf(arq, "%d %s %d %s", &codigo, nome, &prioridade, estado);
                insere_T_Sistema(&raiz, codigo, nome, prioridade, estado, lista_Avl);
                inserir(heap, codigo, nome, prioridade, lista_Heap);
                sondLinear(vlinear, codigo, estado, lista_HashLin);
                sondQuad(vquad, codigo, estado, lista_HashQuad);

            } else if (strcmp(comando, "ListarAVL") == 0) {
                if(raiz){
                    insere_no_final(lista_Avl, "Listando:\n");
                    em_ordem(raiz, lista_Avl);
                }else{
                    insere_no_final(lista_Avl, "Não há nenhum processo\n");
                }

            } else if (strcmp(comando, "TerminarAVL") == 0) {
                int rem=0;
                fscanf(arq, "%d", &rem);
                delete(&raiz, rem, lista_Avl);

            } else if (strcmp(comando, "AlterarHeap") == 0) {
                int codigo, troca;
                fscanf(arq, "%d %d", &codigo, &troca);
                alterarPrioridade(heap, codigo, troca, lista_Avl);
            
            } else if (strcmp(comando, "ListarHeap") == 0) {
                if(raiz){
                    insere_no_final(lista_Heap, "Listando:\n");
                    imprimirHeap(heap, lista_Heap);
                }else{
                    insere_no_final(lista_Heap, "Não há nenhum processo\n");
                }
                

            } else if (strcmp(comando, "RemoverHeap") == 0) {
                remover(heap, lista_Heap);
            
            }else if (strcmp(comando, "BloquearHash") == 0) {
                int codigo;
                fscanf(arq, "%d", &codigo);
                mudarEstado(vlinear, codigo, 'B', lista_HashLin);
                mudarEstado(vquad, codigo, 'B', lista_HashQuad);

            }else if (strcmp(comando, "ListarHash") == 0) {
                char estado[15];
                printf("Informe o estado: ");
                fscanf(arq, " %s", estado);
                listarPorEstado(vlinear, estado, lista_HashLin);
                listarPorEstado(vquad, estado, lista_HashQuad);
                
            
            }
            else if (strcmp(comando, "DesbloquearHash") == 0) {
                int codigo;
                fscanf(arq, "%d", &codigo);
                mudarEstado(vlinear, codigo, 'P', lista_HashLin);
                mudarEstado(vquad, codigo, 'P', lista_HashQuad);
            
            
            }else if (strcmp(comando, "RemoverHash") == 0) {
                int codigo;
                fscanf(arq, "%d", &codigo);
                removerProcessoLinear(vlinear, codigo, lista_HashLin);
                removerProcessoQuadratico(vquad, codigo, lista_HashQuad);
            
            }else if (strcmp(comando, "Executar") == 0) {
                int codigo;
                fscanf(arq, "%d", &codigo);
                mudarEstado(vlinear, codigo, 'E', lista_HashLin);
                mudarEstado(vquad, codigo, 'E', lista_HashQuad);
                
            }else if (strcmp(comando, "Terminar") == 0) {
                int codigo;
                char nome[30];
                fscanf(arq, "%d %s", &codigo, nome);
                delete(&raiz, codigo, lista_Avl);
                removerEspecifico(heap, codigo, lista_Heap);
                removerProcessoLinear(vlinear, codigo, lista_HashLin);
                removerProcessoQuadratico(vquad, codigo, lista_HashQuad);

        } else if (strcmp(comando, "Encerrar") != 0){
                insere_no_final(lista_Heap, "Foi encerrado todos os processos");
                insere_no_final(lista_Avl, "Foi encerrado todos os processos");
                insere_no_final(lista_HashQuad, "Foi encerrado todos os processos");
                insere_no_final(lista_HashLin, "Foi encerrado todos os processos");
                limpar(vlinear);
                limpar(vquad);
                destruirHeap(heap);
                while (raiz != NULL) {
                    delete(&raiz, raiz->codigo, lista_Avl);
                }

            break;
        }
        }
        fclose(arq);
    }
    final(lista_Avl, lista_Heap, lista_HashLin, lista_HashQuad);
    return 0;
}
T_Lista* cria_no(const char frase[]) {
    T_Lista *novoNo = (T_Lista*)malloc(sizeof(T_Lista));
    if (novoNo == NULL) {
        printf("Erro ao alocar memória para novo nó.\n");
        return NULL;
    }

    strcpy(novoNo->frase, frase);
    novoNo->prox = NULL;
    return novoNo;
}
T_Lista* inicializa_lista() {
    T_Lista *cabeca = (T_Lista *)malloc(sizeof(T_Lista));
    if (cabeca == NULL) {
        printf("Erro ao alocar memória.\n");
        exit(1);
    }
    cabeca->prox = NULL;
    return cabeca;
}
void insere_no_final(T_Lista *cabeca, const char *frase) {
    T_Lista *novo = cria_no(frase);
    T_Lista *atual = cabeca;
    while (atual->prox != NULL) {
        atual = atual->prox;
    }
    atual->prox = novo;
}
void final(T_Lista *listaAvl, T_Lista *listaHeap, T_Lista *listaHashLin, T_Lista *listaHashQuad){
    
	FILE *arquivoLog1;
    FILE *arquivoLog2;
    FILE *arquivoLog3;
    FILE *arquivoLog4;
    FILE *arquivoLog5;
    arquivoLog1 = fopen("AVL.txt", "w");
    arquivoLog2 = fopen("Heap.txt", "w");
    arquivoLog3 = fopen("HashLin.txt", "w");
    arquivoLog4 = fopen("HashQuad.txt", "w");
    arquivoLog5 = fopen("Saida.txt", "w");

    if (arquivoLog5 != NULL) {
        fprintf(arquivoLog5, "Relatorio de saida:\n");
        fprintf(arquivoLog5, "----------------------------\n");
        fprintf(arquivoLog5, "Avl:\n");
    if (arquivoLog1 != NULL) {
        fprintf(arquivoLog1, "Relatorio de AVL:\n");
        fprintf(arquivoLog1, "----------------------------\n");

        while (listaAvl != NULL) {
            
            fprintf(arquivoLog1, "%s", listaAvl->frase);
            fprintf(arquivoLog5, "%s", listaAvl->frase);
            
            listaAvl = listaAvl->prox;
        }
        
        
        fprintf(arquivoLog1, "-------------------------------------------\n\n");
        fclose(arquivoLog1);
        printf("\nRelatorio de log gerado com sucesso!\n");
    } else {
        printf("\nErro ao abrir o arquivo de log.\n");
    }
    fprintf(arquivoLog5, "Heap:\n");
    if (arquivoLog2 != NULL) {
        fprintf(arquivoLog2, "Relatorio de Heap:\n");
        fprintf(arquivoLog2, "----------------------------\n");

        while (listaHeap != NULL) {
            
            fprintf(arquivoLog2, "%s", listaHeap->frase);
            fprintf(arquivoLog5, "%s", listaHeap->frase);
            
            listaHeap = listaHeap->prox;
        }
        
        
        fprintf(arquivoLog2, "-------------------------------------------\n\n");
        fclose(arquivoLog2);
        printf("\nRelatorio de log gerado com sucesso!\n");
    } else {
        printf("\nErro ao abrir o arquivo de log.\n");
    }
    fprintf(arquivoLog5, "Hash Linear:\n");
    if (arquivoLog3 != NULL) {
        fprintf(arquivoLog3, "Relatorio de Hash Linear:\n");
        fprintf(arquivoLog3, "----------------------------\n");

        while (listaHashLin != NULL) {
            
            fprintf(arquivoLog3, "%s", listaHashLin->frase);
            fprintf(arquivoLog5, "%s", listaHashLin->frase);
            
            listaHashLin = listaHashLin->prox;
        }
        
        
        fprintf(arquivoLog3, "-------------------------------------------\n\n");
        fclose(arquivoLog3);
        printf("\nRelatorio de log gerado com sucesso!\n");
    } else {
        printf("\nErro ao abrir o arquivo de log.\n");
    }
    fprintf(arquivoLog5, "Hash Quadratica:\n");
    if (arquivoLog4 != NULL) {
        fprintf(arquivoLog4, "Relatorio de Hash Quadratica:\n");
        fprintf(arquivoLog4, "----------------------------\n");

        while (listaHashQuad != NULL) {
            
            fprintf(arquivoLog4, "%s", listaHashQuad->frase);
            fprintf(arquivoLog5, "%s", listaHashQuad->frase);
            
            listaHashQuad = listaHashQuad->prox;
        }
        
        
        fprintf(arquivoLog4, "-------------------------------------------\n\n");
        fclose(arquivoLog4);
        printf("\nRelatorio de log gerado com sucesso!\n");
    } else {
        printf("\nErro ao abrir o arquivo de log.\n");
    }
    

        fprintf(arquivoLog5, "-------------------------------------------\n\n");
        fclose(arquivoLog5);
        printf("\nRelatorio de log gerado com sucesso!\n");
    } else {
        printf("\nErro ao abrir o arquivo de log.\n");
    }
}
void removerProcessoLinear(Processo* vh[], int codigo, T_Lista* lista) {
    int hx;
    Processo* atual;
    Processo* anterior;
    
    for (int i = 0; i < M; i++) {
        hx = (codigo + i) % M;
        atual = vh[hx];
        anterior = NULL;

        while (atual != NULL && atual->codigo != codigo) {
            anterior = atual;
            atual = atual->prox;
        }

        if (atual != NULL) {
            if (anterior == NULL) {
                vh[hx] = atual->prox;
            } else {
                anterior->prox = atual->prox;
            }

            char mensagem[100];
            sprintf(mensagem, "O processo %d foi removido\n", codigo);
            insere_no_final(lista, mensagem);
            free(atual);
            return;
        }
    }

    char mensagem[100];
    sprintf(mensagem, "O processo %d não foi removido\n", codigo);
    insere_no_final(lista, mensagem);
}

void removerProcessoQuadratico(Processo* vh[], int codigo, T_Lista* lista) {
    int incr = 0;
    int hx;
    Processo* atual;
    Processo* anterior;

    while (1) {
        hx = (codigo + incr * incr) % M;
        atual = vh[hx];
        anterior = NULL;

        while (atual != NULL && atual->codigo != codigo) {
            anterior = atual;
            atual = atual->prox;
        }

        if (atual == NULL) {
            if (incr == 0) {  // Significa que já fizemos uma passagem completa
                char mensagem[100];
                sprintf(mensagem, "O processo %d não foi removido\n", codigo);
                insere_no_final(lista, mensagem);
                return;
            }
            incr++;
        } else {
            if (anterior == NULL) {
                vh[hx] = atual->prox;
            } else {
                anterior->prox = atual->prox;
            }

            char mensagem[100];
            sprintf(mensagem, "O processo %d foi removido\n", codigo);
            insere_no_final(lista, mensagem);
            free(atual);
            return;
        }
    }
}

Processo *criarProcesso(int codigo, const char estado[]) {
    Processo* novo = (Processo*)malloc(sizeof(Processo));
    novo->codigo = codigo;
    strcpy(novo->estado, estado);
    novo->prox = NULL;
    return novo;
}

void inicializa(Processo* vh[]) {
    for (int i = 0; i < M; i++) {
        vh[i] = NULL;
    }
}
int sondLinear(Processo* vh[], int codigo, const char estado[], T_Lista *lista) {
    int incr = 0; 
    int hx, cl = 0;

    for (incr = 0; incr < M; incr++) {
        hx = (codigo + incr) % M;

        if (vh[hx] == NULL) {
            vh[hx] = criarProcesso(codigo, estado);
            char mensagem[100];
            sprintf(mensagem, "O processo %d foi inserido. Total de colisões na sondagem linear: %d \n", codigo, cl);
            insere_no_final(lista, mensagem);
            return cl;
        }

        else {
            cl++;
        }
    }

    // Hash table is full
    char mensagem[100];
    sprintf(mensagem, "Hash cheia\n");
    insere_no_final(lista, mensagem);
    return -1;
}

int sondQuad(Processo* vh[], int codigo, const char estado[], T_Lista *lista) {
    int incr = 0; // Initialize incr to 0
    int hx, cq = 0;

    for (incr = 0; incr < M; incr++) {
        hx = (codigo + incr * incr) % M;

        // Check if the hash table entry is empty
        if (vh[hx] == NULL) {
            vh[hx] = criarProcesso(codigo, estado);
            char mensagem[100];
            sprintf(mensagem, "O processo %d foi inserido. Total de colisões na sondagem quadratica: %d\n", codigo, cq);
            insere_no_final(lista, mensagem);
            return cq;
        }

        // Collision occurred, try next slot using quadratic probing
        else {
            cq++;
        }
    }

    // Hash table is full
    char mensagem[100];
    sprintf(mensagem, "Hash cheia\n");
    insere_no_final(lista, mensagem);
    return -1;
}

void imprime(Processo* vh[]) {
    for (int i = 0; i < M; i++) {
        if (vh[i] != NULL) {
            printf("Código: %d, Estado: %s\n", vh[i]->codigo, vh[i]->estado);
        }
    }
}

void limpar(Processo* vh[]) {
    for (int i = 0; i < M; i++) {
        Processo* atual = vh[i];
        while (atual != NULL) {
            Processo* temp = atual;
            atual = atual->prox;
            free(temp);
        }
        vh[i] = NULL;
    }
}

void mudarEstado(Processo* vh[], int codigo, char novoEstado, T_Lista *lista) {
    const char* estadoStr;
    switch (novoEstado) {
        case 'P': estadoStr = "PRONTO"; break;
        case 'E': estadoStr = "EXECUTANDO"; break;
        case 'B': estadoStr = "BLOQUEADO"; break;
    }
    for (int i = 0; i < M; i++) {
        Processo* atual = vh[i];
        while (atual != NULL) {
            if (atual->codigo == codigo) {
                strcpy(atual->estado, estadoStr);
                char mensagem[100];
                sprintf(mensagem, "Estado do processo %d mudado para %s\n", codigo, estadoStr);
                insere_no_final(lista, mensagem);
                return;
            }
            atual = atual->prox;
        }
    }
    char mensagem[100];
    sprintf(mensagem, "Processo %d não encontrado para mudar estado\n", codigo);
    insere_no_final(lista, mensagem);
}

void listarPorEstado(Processo* vh[], const char estado[], T_Lista *lista) {
    char mensagem[100];
    sprintf(mensagem, "Foi listado processos no estado %s\n", estado);
    insere_no_final(lista, mensagem);
    for (int i = 0; i < M; i++) {
        Processo* atual = vh[i];
        while (atual != NULL) {
            if (strcmp(atual->estado, estado) == 0) {
                printf("Processo %d\n", atual->codigo);
                
                sprintf(mensagem, "Processo %d\n", atual->codigo);
                insere_no_final(lista, mensagem);
            }
            atual = atual->prox;
        }
    }
    
}
void troca(Item* a, Item* b) {
    Item temp = *a;
    *a = *b;
    *b = temp;
}

void heapify(T_Heap* heap, int i) {
    int maior = i;
    int filhoEsquerda = 2 * i;
    int filhoDireita = 2 * i + 1;

    if (filhoEsquerda <= heap->tamanho && heap->vetor[filhoEsquerda].prioridade > heap->vetor[maior].prioridade) {
        maior = filhoEsquerda;
    }

    if (filhoDireita <= heap->tamanho && heap->vetor[filhoDireita].prioridade > heap->vetor[maior].prioridade) {
        maior = filhoDireita;
    }

    if (maior != i) {
        troca(&heap->vetor[i], &heap->vetor[maior]);
        heapify(heap, maior);
    }
}

T_Heap* criarT_Heap(int capacidade) {
    T_Heap* heap = (T_Heap*) malloc(sizeof(T_Heap));
    heap->vetor = (Item*) malloc((capacidade + 1) * sizeof(Item)); 
    heap->capacidade = capacidade;
    heap->tamanho = 0;
    return heap;
}
void removerEspecifico(T_Heap* heap, int codigo, T_Lista *lista) {
    char mensagem[100];
    if (heap == NULL || heap->vetor == NULL || heap->tamanho == 0) {
        return;
    }

    int i;
    for (i = 1; i <= heap->tamanho; i++) {
        if (heap->vetor[i].codigo == codigo) {
            sprintf(mensagem, "Processo %d  encontrado e removido\n", codigo);
            insere_no_final(lista, mensagem);
            break;
        }
    }

    if (i > heap->tamanho) {
            sprintf(mensagem, "Processo %d nao encontrado, nao foi removido\n", codigo);
            insere_no_final(lista, mensagem);
        return;
    }

    heap->vetor[i] = heap->vetor[heap->tamanho];
    heap->tamanho--;
    heapify(heap, i);
}


void inserir(T_Heap* heap, int codigo, char nome[], int prioridade, T_Lista *lista) {
    char mensagem[100];

    if (heap == NULL || heap->vetor == NULL) {
        sprintf(mensagem, "Erro ao inserir o processo %d\n", codigo);
        insere_no_final(lista, mensagem);
        return;
    }

    if (heap->tamanho == heap->capacidade) {
        heap->capacidade *= 2;
        heap->vetor = realloc(heap->vetor, (heap->capacidade + 1) * sizeof(Item));
        if (heap->vetor == NULL) {
            sprintf(mensagem, "Erro ao inserir o processo %d\n", codigo);
            insere_no_final(lista, mensagem);
            return;
        }
    }

    heap->tamanho++;
    int i = heap->tamanho;
    heap->vetor[i].codigo = codigo;
    strcpy(heap->vetor[i].nome, nome);
    heap->vetor[i].prioridade = prioridade;

    int pai = i / 2;
    while (i > 1 && heap->vetor[i].prioridade > heap->vetor[pai].prioridade) {
        troca(&heap->vetor[i], &heap->vetor[pai]);
        i = pai;
        pai = i / 2;
    }
    sprintf(mensagem, "O processo %d foi inserido\n", codigo);
    insere_no_final(lista, mensagem);
}
Item remover(T_Heap* heap, T_Lista *lista) {
    Item removido = {0, "", 0};
    char mensagem[100];

    if (heap == NULL || heap->vetor == NULL || heap->tamanho == 0) {
        insere_no_final(lista, "Erro ao remover o processo: Heap vazio ou não inicializado.\n");
        return removido;
    }

    removido.codigo = heap->vetor[1].codigo;
    strcpy(removido.nome, heap->vetor[1].nome);
    removido.prioridade = heap->vetor[1].prioridade;

    heap->vetor[1] = heap->vetor[heap->tamanho];
    heap->tamanho--;
    heapify(heap, 1);

    sprintf(mensagem, "O processo %d foi removido com sucesso\n", removido.codigo);
    insere_no_final(lista, mensagem);
    return removido;
}


/*Item remover(T_Heap* heap, T_Lista *lista) {
    Item removido = {0, "", 0};
    char mensagem[100];

    if (heap == NULL || heap->vetor == NULL || heap->vetor[0] == 0) {
        insere_no_final(lista, "Erro ao remover o processo: Heap vazio ou não inicializado.\n");
        return removido;
    }

    removido.codigo = heap->vetor[1].codigo;
    strcpy(removido.nome, heap->vetor[1].nome);
    removido.prioridade = heap->vetor[1].prioridade;

    heap->vetor[1] = heap->vetor[heap->vetor[0]];
    heap->vetor[0]--;
    heapify(heap, 1);

    sprintf(mensagem, "O processo %d foi removido com sucesso\n", removido.codigo);
    insere_no_final(lista, mensagem);
    return removido;
}

Item remover(T_Heap* heap, T_Lista *lista) {
    Item removido = {0, "", 0};
    char mensagem[100];

    if (heap == NULL || heap->vetor == NULL || heap->tamanho == 0) {
        
        insere_no_final(lista, "Erro ao remover o processo: Heap vazio ou não inicializado.\n");
        return removido;
    }

    removido = heap->vetor[1];
    heap->vetor[1] = heap->vetor[heap->tamanho];
    heap->tamanho--;
    heapify(heap, 1);
    sprintf(mensagem, "O processo %d removido com sucesso\n", removido.codigo);
    insere_no_final(lista, mensagem);
    return removido;
}*/

void imprimirHeap(T_Heap* heap, T_Lista *lista) {
    char mensagem[100];
    if (heap == NULL || heap->vetor == NULL || heap->tamanho == 0) {
        return;
    }

    printf("Heap:\n");
    for (int i = 1; i <= heap->tamanho; i++) {
        sprintf(mensagem, "Código: %d, Nome: %s, Prioridade: %d\n", heap->vetor[i].codigo, heap->vetor[i].nome, heap->vetor[i].prioridade);
        insere_no_final(lista, mensagem);
        printf("Código: %d, Nome: %s, Prioridade: %d\n", heap->vetor[i].codigo, heap->vetor[i].nome, heap->vetor[i].prioridade);
    }
    printf("\n");
}

void destruirHeap(T_Heap* heap) {
    if (heap != NULL) {
        free(heap->vetor);
        free(heap);
    }
}

void alterarPrioridade(T_Heap* heap, int codigo, int novaPrioridade, T_Lista *lista) {
    char mensagem[100];
    if (heap == NULL || heap->vetor == NULL || heap->tamanho == 0) {
        insere_no_final(lista, "Heap vazio ou inválido!\n");
        return;
    }

    int i;
    for (i = 1; i <= heap->tamanho; i++) {
        if (heap->vetor[i].codigo == codigo) {
            heap->vetor[i].prioridade = novaPrioridade;
            break;
        }
    }

    if (i > heap->tamanho) {
        sprintf(mensagem, "Elemento com código %d não encontrado.\n", codigo);
        insere_no_final(lista, mensagem);
        return;
    }

    int pai = i / 2;
    while (i > 1 && heap->vetor[i].prioridade > heap->vetor[pai].prioridade) {
        troca(&heap->vetor[i], &heap->vetor[pai]);
        i = pai;
        pai = i / 2;
    }

    heapify(heap, i);
    sprintf(mensagem, "Alterado a prioridade, processo com código %dencontrado.\n", codigo);
    insere_no_final(lista, mensagem);
}
int insere_T_Sistema(T_Sistema **p, int codigo, char nome[], int prioridade, char estado[], T_Lista *lista) {
    int cresceu;
    char mensagem[100];

    if (*p == NULL) {
        *p = (T_Sistema *)malloc(sizeof(T_Sistema));
        if (*p == NULL) {
            printf("Erro ao alocar memória\n");
            exit(1);
        }
        (*p)->codigo = codigo;
        (*p)->prioridade = prioridade;
        strcpy((*p)->nome, nome);
        strcpy((*p)->estado, estado);
        (*p)->dir = (*p)->esq = NULL;
        (*p)->bal = 0;
        cresceu = 1;
        sprintf(mensagem, "O do processo %d foi inserido\n", codigo);
        insere_no_final(lista, mensagem);
    } else if ((*p)->codigo > codigo) {
        cresceu = insere_T_Sistema(&(*p)->esq, codigo, nome, prioridade, estado, lista);

        if (cresceu) {
            switch ((*p)->bal) {
                case 1:
                    (*p)->bal = 0;
                    cresceu = 0;
                    break;
                case 0:
                    (*p)->bal = -1;
                    cresceu = 1;
                    break;
                case -1:
                    if ((*p)->esq->bal == -1) {
                        rot_dir(p);
                        (*p)->bal = (*p)->dir->bal = 0;
                    } else {
                        rot_esq(&(*p)->esq);
                        rot_dir(p);
                        if ((*p)->bal == -1) {
                            (*p)->esq->bal = 0;
                            (*p)->dir->bal = 1;
                        } else {
                            (*p)->dir->bal = 0;
                            (*p)->esq->bal = -(*p)->bal;
                        }
                        (*p)->bal = 0;
                    }
                    cresceu = 0;
                    break;
            }
        }
    } else if ((*p)->codigo < codigo) {
        cresceu = insere_T_Sistema(&(*p)->dir, codigo, nome, prioridade, estado, lista);

        if (cresceu) {
            switch ((*p)->bal) {
                case -1:
                    (*p)->bal = 0;
                    cresceu = 0;
                    break;
                case 0:
                    (*p)->bal = 1;
                    cresceu = 1;
                    break;
                case 1:
                    if ((*p)->dir->bal == 1) {
                        rot_esq(p);
                        (*p)->bal = (*p)->esq->bal = 0;
                    } else {
                        rot_dir(&(*p)->dir);
                        rot_esq(p);
                        if ((*p)->bal == -1) {
                            (*p)->esq->bal = 0;
                            (*p)->dir->bal = 1;
                        } else {
                            (*p)->dir->bal = 0;
                            (*p)->esq->bal = -(*p)->bal;
                        }
                        (*p)->bal = 0;
                    }
                    cresceu = 0;
                    break;
            }
        }
    } else {
        sprintf(mensagem, "O do processo %d nao foi inserido - já existe na árvore\n", codigo);
        insere_no_final(lista, mensagem);
        cresceu = 0;
    }

    return cresceu;
}

T_Sistema *del(T_Sistema **p) {
    T_Sistema *aux = (*p), *pai = (*p);

    while(aux->dir != NULL) {
        pai = aux;
        aux = aux->dir;
    }

    pai->dir = NULL;
    bal_dir(&pai);
    return aux;
}

int delete(T_Sistema **p, int x, T_Lista *lista) {
    T_Sistema *q, *aux = NULL;
    int flag = 0;
    char mensagem[100];

    if((*p) == NULL) {
        sprintf(mensagem, "Codigo %d nao foi encontrado. Processo nao foi removido\n", x);
        insere_no_final(lista, mensagem);
        return 0;
    }
    else if((*p)->codigo > x) {
        flag = delete(&(*p)->esq, x, lista);

        if(flag){
            bal_esq(&(*p));
        }
    }
    else if((*p)->codigo < x) {
        flag = delete(&(*p)->dir, x, lista);

        if(flag){
            bal_dir(&(*p));
        }
    }
    else {
        q = (*p);

        if(q->dir == NULL) {
            (*p) = q->esq;
            sprintf(mensagem, "Codigo %d encontrado. Processo removido\n", x);
            insere_no_final(lista, mensagem);
            return 1;
        }
        else if(q->esq == NULL) {
            (*p) = q->dir;
            sprintf(mensagem, "Codigo %d encontrado. Processo removido\n", x);
            insere_no_final(lista, mensagem);
            return 1;
        }
        else {
            aux = del(&q->esq);

            (*p)->codigo = aux->codigo;
            (*p)->bal = aux->bal;
            strcpy((*p)->nome, aux->nome);
            strcpy((*p)->estado, aux->estado);
            (*p)->prioridade = aux->prioridade;
            free(aux);

        }
    }
}


T_Sistema *busca_T_Sistema(T_Sistema **p, int x, T_Lista *lista) {
    char mensagem[100];
    if(*p == NULL) {
        return NULL;
    }

    if((*p)->codigo == x) {
        sprintf(mensagem, "Busca na arvore, codigo %d encontrado\n", x);
        insere_no_final(lista, mensagem);
        return *p;
    }
    else if((*p)->codigo > x) {
        return busca_T_Sistema(&(*p)->esq, x, lista);
    }
    else {
        return busca_T_Sistema(&(*p)->dir, x, lista);
    }
    sprintf(mensagem, "Busca na arvore, codigo %d  nao encontrado\n", x);
    insere_no_final(lista, mensagem);
}

int bal_esq(T_Sistema **p) {
    T_Sistema *p1, *p2;

    if((*p) == NULL) {
        return 0;
    }

    printf("Esq: Codigo [%d] \n Bal [%d]\n\n", (*p)->codigo, (*p)->bal);

    if((*p)->bal == -1) {
        (*p)->bal = 0;
        return 1;
    }
    else if((*p)->bal == 0) {
        (*p)->bal = 1;
        return 0;
    }
    else {
        p1 = (*p)->dir;

        if(p1->bal >= 0) {
            (*p)->dir = p1->esq;
            p1->esq = (*p);

            if(p1->bal == 0) {
                (*p)->bal = 1;
                p1->bal = -1;
            }
            else {
                (*p)->bal = 0;
                p1->bal = 0;
            }

			(*p) = p1;

			return 1;
        }
        else {
            p2 = p1->esq;
            p1->esq = p2->dir;
            p2->dir = p1;
            (*p)->dir = p2->esq;
            p2->esq = (*p);

            if(p2->bal == 1) {
                (*p)->bal = -1;
            }
            else {
                (*p)->bal = 0;
            }

            if(p2->bal == -1) {
                p1->bal = 1;
            }
            else {
                p1->bal = 0;
            }

            (*p) = p2;
            p2->bal = 0;

            return 1;
        }
    }
}

int bal_dir(T_Sistema **p) {
    T_Sistema *p1, *p2;
    int b2;

    if((*p) == NULL) {
        return 0;
    }

    printf("Dir: Codigo [%d] -- Bal [%d]\n\n", (*p)->codigo, (*p)->bal);

    if((*p)->bal == 1) {
        (*p)->bal = 0;
        return 1;
    }
    else if((*p)->bal == 0) {
        (*p)->bal = -1;
        return 0;
    }
    else {
        p1 = (*p)->esq;

        if(p1->bal <= 0) {
            (*p)->esq = p1->dir;
            p1->dir = (*p);

            if(p1->bal == 0) {
                (*p)->bal = -1;
                p1->bal = 1;
                (*p) = p1;
                return 0;
            }
            else {
                (*p)->bal = 0;
                p1->bal = 0;
                (*p) = p1;
                return 1;
            }
        }
        else {
            p2 = p1->dir;
            b2 = p2->bal;
            p1->dir = p2->esq;
            p2->esq = (*p);

            if(p2->bal == -1) {
                (*p)->bal = 1;
            }
            else {
                (*p)->bal = 0;
            }

            if(p2->bal == 1) {
                p1->bal = -1;
            }
            else {
                p1->bal = 0;
            }

            (*p) = p2;
            p2->bal = 0;

            return 1;
        }
    }
}



int rot_dir(T_Sistema **p) {
    T_Sistema *q, *aux;

    if((*p) == NULL) return 1;

    q = (*p)->esq;
    aux = q->dir;
    q->dir = (*p);
    (*p)->esq = aux;
    (*p) = q;

    return 0;
}

int rot_esq(T_Sistema **p) {
    T_Sistema *q, *aux;

    if((*p) == NULL) return 1;

    q = (*p)->dir;
    aux = q->esq;
    q->esq = (*p);
    (*p)->dir = aux;
    (*p) = q;

    return 0;
}


void em_ordem(T_Sistema *p, T_Lista *lista) {
    char mensagem[100];
    if (p != NULL) {
        em_ordem(p->esq, lista);
        sprintf(mensagem, "codigo [%d] --- Bal [%d]\n", p->codigo, p->bal);
        insere_no_final(lista, mensagem);
        printf("codigo [%d] --- Bal [%d]\n", p->codigo, p->bal);
        em_ordem(p->dir, lista);
    }
}