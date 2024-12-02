#include <stdio.h>
#include <stdlib.h>

#define MAX 100

typedef struct {
    int parente;  // Representante do conjunto
    int rank;     // Rank do conjunto
} Subconjunto;

// Estruturas globais
Subconjunto conjuntos[MAX];
int *subconjuntos[MAX];
int tamanho[MAX];

// Função para inicializar conjuntos disjuntos
void criaConjuntos(int n) {
    for (int i = 0; i < n; i++) {
        conjuntos[i].parente = i;
        conjuntos[i].rank = 0;
        subconjuntos[i] = (int *)malloc(sizeof(int) * MAX);
        subconjuntos[i][0] = i + 1;  // Ajuste para exibir elementos como base 1
        tamanho[i] = 1;
    }
}

// Encontrar representante com compressão de caminho
int find(int i) {
    if (conjuntos[i].parente != i) {
        conjuntos[i].parente = find(conjuntos[i].parente);
    }
    return conjuntos[i].parente;
}

// União de dois subconjuntos com otimização por rank
void unir(int x, int y) {
    int raizX = find(x);
    int raizY = find(y);

    if (raizX != raizY) {
        if (conjuntos[raizX].rank < conjuntos[raizY].rank) {
            conjuntos[raizX].parente = raizY;
            for (int i = 0; i < tamanho[raizX]; i++) {
                subconjuntos[raizY][tamanho[raizY]++] = subconjuntos[raizX][i];
            }
            tamanho[raizX] = 0;
        } else if (conjuntos[raizX].rank > conjuntos[raizY].rank) {
            conjuntos[raizY].parente = raizX;
            for (int i = 0; i < tamanho[raizY]; i++) {
                subconjuntos[raizX][tamanho[raizX]++] = subconjuntos[raizY][i];
            }
            tamanho[raizY] = 0;
        } else {
            conjuntos[raizY].parente = raizX;
            conjuntos[raizX].rank++;
            for (int i = 0; i < tamanho[raizY]; i++) {
                subconjuntos[raizX][tamanho[raizX]++] = subconjuntos[raizY][i];
            }
            tamanho[raizY] = 0;
        }
    }
}

// Função para exibir os conjuntos
void verConjuntos(int n) {
    for (int i = 0; i < n; i++) {
        if (find(i) == i) {  // Verifica se i é a raiz de um conjunto
            printf("Conjunto representado por %d: ", i + 1);
            for (int j = 0; j < tamanho[i]; j++) {
                printf("%d ", subconjuntos[i][j]);
            }
            printf("\n");
        }
    }
}

// Funções de ordenação
void bubbleSort(int *arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void insertionSort(int *arr, int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void merge(int *arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    int L[n1], R[n2];
    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int i = 0; i < n2; i++)
        R[i] = arr[m + 1 + i];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void mergeSort(int *arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

int partition(int *arr, int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    int temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;
    return (i + 1);
}

void quickSort(int *arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// Função para ordenar subconjuntos com escolha de algoritmo
void ordenarConjunto(int n) {
    int metodo;
    printf("Escolha o m0todo de ordenacao:\n");
    printf("1. Bubble Sort\n");
    printf("2. Insertion Sort\n");
    printf("3. Merge Sort\n");
    printf("4. Quick Sort\n");
    printf("Escolha: ");
    scanf("%d", &metodo);

    for (int i = 0; i < n; i++) {
        if (find(i) == i && tamanho[i] > 1) {
            switch (metodo) {
                case 1:
                    bubbleSort(subconjuntos[i], tamanho[i]);
                    break;
                case 2:
                    insertionSort(subconjuntos[i], tamanho[i]);
                    break;
                case 3:
                    mergeSort(subconjuntos[i], 0, tamanho[i] - 1);
                    break;
                case 4:
                    quickSort(subconjuntos[i], 0, tamanho[i] - 1);
                    break;
                default:
                    printf("Método inválido.\n");
                    return;
            }
        }
    }
    printf("Conjuntos ordenados com sucesso.\n");
}

int lerEntrada(const char *arquivo) {
    FILE *fp = fopen(arquivo, "r");
    if (!fp) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    int n, x, y;
    fscanf(fp, "%d", &n);  // Ler o número total de elementos
    criaConjuntos(n);      // Inicializar os conjuntos

    while (fscanf(fp, "%d %d", &x, &y) != EOF) {
        unir(x - 1, y - 1);
    }

    fclose(fp);
    return n;  // Retorna o número de elementos
}


// Menu interativo
void menu(int n) {
    int opcao, x, y;
    do {
        printf("\nMenu:\n");
        printf("1. Ver conjuntos\n");
        printf("2. Unir subconjuntos\n");
        printf("3. Ordenar subconjuntos\n");
        printf("4. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                verConjuntos(n);
                break;
            case 2:
                printf("Elementos para unir: ");
                scanf("%d %d", &x, &y);
                unir(x - 1, y - 1);
                break;
            case 3:
                ordenarConjunto(n);
                break;
            case 4:
                printf("Encerrando...\n");
                break;
            default:
                printf("Opcaoo invalida.\n");
        }
    } while (opcao != 4);
}

int main() {
    const char *arquivo = "conjunto.txt";

    // Obter o número de elementos a partir do arquivo
    int n = lerEntrada(arquivo);

    menu(n);

    // Liberar memória
    for (int i = 0; i < n; i++) {
        free(subconjuntos[i]);
    }

    return 0;
}
