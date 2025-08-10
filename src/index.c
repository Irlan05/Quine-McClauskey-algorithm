#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct node {
    int data[257], bin[26], noofones, isimplicant, minarr[1000];
    char term[26];
    struct node* right;
}node;

node *root, *head, *improot, *save, *fin;
int var, min, number = 1, columns = 2, check = 1, limit, imptable[100][100], counter = 0, essential[1000], t = 0, no = 0, minterms[1000];
char a[26], b[26];

void group1();          
void arrange();
void swap(struct node*,struct node*);           
void disp();
void further_groupings();           
void end_loop(struct node*);
void display_implicants();   
void implicants(struct node*);
void collect();
void variables();
void convert();
void implicants_table();        
void func();
void other_implicants();
void final_terms();
void store_minterms();

void ler_arquivo_PLA(const char* nome_arquivo){
    FILE* arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL){
        printf("Erro ao abrir o arquivo %s\n", nome_arquivo);
        exit(1);
    }

    char linha[256];
    int entrada, nminterms = 0;

    while (fgets(linha, sizeof(linha), arquivo)){
        if (linha[0] == '.' && linha[1] == 'i'){
            sscanf(linha, ".i %d", &var);
            break;
        }
    }

    while (fgets(linha, sizeof(linha), arquivo)){
        if (linha[0] == '.' && linha[1] == 'p'){
            sscanf(linha, ".p %d", &min);
            break;
        }
    }

    node* temp = root = (node*)malloc(sizeof(node));

    nminterms = 0;
    while (fgets(linha, sizeof(linha), arquivo)){
        if (linha[0] == '.') 
            break;

        int dec = 0;
        int valid = 1;
        for (int i = 0; i < var; i++){
            if (linha[i] == '0' || linha[i] == '1'){
                dec = (dec << 1) | (linha[i] - '0');
            }
            else{
                valid = 0;
                break;
            }
        }

        if (!valid)
            continue; 

        temp->data[0] = dec;
        temp->noofones = 0;

        int j = dec;
        for (int k = 0; k < var; k++){
            temp->bin[k] = (j & 1);
            if (temp->bin[k] == 1)
                temp->noofones++;
            j >>= 1;
        }

        if (nminterms < min - 1){
            temp = temp->right = (node*)malloc(sizeof(node));
        }
        else{
            temp->right = NULL;
        }
        nminterms++;
    }

    fclose(arquivo);
}

int main(int argc, char* argv[]) {
    const char* nome_arquivo = argv[1];
    if(argc != 3){
        perror("Modo de uso: ./quine <nome_arquivo_PLA> <nome_arquivo_saida>");
        return 1;
    }

    ler_arquivo_PLA(nome_arquivo);

    arrange();
    store_minterms();
    group1();
    disp();
    end_loop(root);
    head = (node*)malloc(sizeof(node));
    while (check > 0){
        further_groupings();
    }
    save->right = NULL;
    printf("No pairs formed hence no further calculation required\n\n");
    end_loop(improot);
    collect();
    display_implicants();
    variables();
    implicants_table();
    other_implicants();
    final_terms();
    end_loop(fin);
    convert();
    FILE* saida = fopen("saida.txt", "w");
    if(saida == NULL){
        printf("Erro ao abrir o arquivo para escrita.\n");
        return 1;
    }

    func(saida);
    fclose(saida); 

    return 0;
}

void arrange(){
    node *temp1, *temp2;
    temp1 = temp2 = root;
    while(temp1 != NULL){
        temp2 = root;
        while(temp2 != NULL){
            if(temp1->data[0] < temp2->data[0]){
                swap(temp1,temp2);
            }
            temp2 = temp2->right;
        }
        if(temp1->right == NULL){
            limit = temp1->data[0];
        }
        temp1 = temp1->right;
    }
}

void store_minterms(){
    int i = 0;
    node* temp;
    temp = root;
    while(temp != NULL){
        minterms[i] = temp->data[0];
        i++;
        temp = temp->right;
    }
}

void swap(node* temp1, node* temp2){
    int x, y, i = 0;
    i = var;
    for(i = 0; i < var; i++){
        y = temp1->bin[i];
        temp1->bin[i] = temp2->bin[i];
        temp2->bin[i] = y;
    }
    y = temp1->noofones;
    temp1->noofones = temp2->noofones;
    temp2->noofones = y;
    x = temp1->data[0];
    temp1->data[0] = temp2->data[0];
    temp2->data[0] = x;
}

void group1(){
    int i, count = 0 , j, k = 0;
    node *temp, *next;
    temp = save = root;
    root = next = (node*)malloc(sizeof(node));
    for(i = 0; i <= var; i++){
        temp = save;
        while(temp != NULL){
            if(temp->noofones == i){
                next->data[0] = temp->data[0];
                k++;
                for(j=0;j<var;j++){
                    next->bin[j] = temp->bin[j];
                }
                next->noofones = temp->noofones;
                next = next->right = (node*)malloc(sizeof(node));
            }
            temp = temp->right;
        }
    }
    minterms[k] = -1;
    next->right = NULL;
}

void disp(){
    int i, j = min;
    node* temp;
    temp = root;
    printf("\n\nColumn #%d\n\n\n", number);
    while(temp->right != NULL){
        printf("%d\t", temp->data[0]);
        for(i = var - 1; i >= 0; i--){
            printf("%d", temp->bin[i]);
        }
        temp = temp->right;
        printf("\n");
    }
    temp->right = NULL;
    number++;
}

void end_loop(node* ptr){
    node* temp;
    temp = ptr;
    while(temp->right->right != NULL){
        temp = temp->right;
    }
    temp->right = NULL;
}

void further_groupings(){
    int i, count, k, j, x;
    node *temp, *next, *p, *imp;
    check = 0;
    if(columns == 2){
        imp = improot = (node*)malloc(sizeof(node));
        p = head;
    }
    else{
        imp = save;
        root = head;
        p = head = (node*)malloc(sizeof(node));
    }
    temp = root;
    implicants(root);
    printf("\n\nColumn #%d\n\n\n", number);
    while(temp != NULL){
        next = temp->right;
        while(next != NULL){
            count = 0;
            if(next->noofones - temp->noofones == 1){
                for(i=0;i<var;i++){
                    if(temp->bin[i]!=next->bin[i]){
                        k = i;
                        count++;
                    }
                }
            }
            if(count == 1){
                temp->isimplicant = 0;
                next->isimplicant = 0;
                check++;
                for(i = 0; i < var; i++){
                    p->bin[i] = temp->bin[i];
                }
                p->bin[k] = -1;
                x = 0; 
                for(j = 0; j < columns / 2; j++){
                    p->data[x] = temp->data[j];
                    x++;
                }
                for(j = 0; j < columns / 2; j++){
                    p->data[x] = next->data[j];
                    x++;
                }
                p->noofones = temp->noofones;
                for(j = 0; j < columns; j++){
                    printf("%d,",p->data[j]);
                }
                printf("\b ");
                printf("\t");
                for(i = var - 1; i >= 0; i--){
                    if(p->bin[i] == -1)
                        printf("-");
                    else
                        printf("%d", p->bin[i]);
                }
                printf("\n");
                p = p->right = (node*)malloc(sizeof(node)); 
            }
            next = next->right;
        }
        temp = temp->right;
    }
    p->right = NULL;
    if(check != 0){
        end_loop(head);
    }
    temp = root;
    while(temp != NULL){
        if(temp->isimplicant == 1){
            i = 0;
            for(i = 0; i < columns / 2; i++){
                imp->data[i] = temp->data[i];
            }
            imp->data[i] = -1;
            for(i=0;i<var;i++){
                imp->bin[i] = temp->bin[i];
            }
            imp = imp->right = (node*)malloc(sizeof(node));
        }
        temp = temp->right;
    }
    save = imp;
    columns = columns * 2;
    number++;
}

void display_implicants(){
    int i = 0;
    node* temp;
    temp = improot;
    printf("\n\nThe prime implicants are:- \n\n");
    while(temp != NULL){
        i = 0;
        i = var - 1;
        while(i >= 0){
            if(temp->bin[i] == -1){
                printf("-");
            }
            else{
                printf("%d", temp->bin[i]);
            }
            i--;
        }
        printf("\t\t");
        i = 0;
        while(temp->data[i] != -1){
            printf("%d,", temp->data[i]);
            i++;
        }
        printf("\b ");
        temp = temp->right;
        printf("\n\n");
        counter++;
    }
}

void implicants(node* ptr){
    node* temp;
    temp = ptr;
    while(temp != NULL){
        temp->isimplicant = 1;
        temp = temp->right;
    }
}

void collect(){
    int common = 0, i;
    node *temp1, *temp2, *temp3;
    temp1 = temp2 = improot;
    while(temp1 != NULL){
        temp2 = temp1->right;
        while(temp2 != NULL){
            common = 0;
            for(i = 0; i < var; i++){
                if(temp2->bin[i] == temp1->bin[i]){
                    common++;
                }
            }
            if(common == var){
                temp3 = improot;
                while(temp3->right != temp2){
                    temp3 = temp3->right;
                }
                temp3->right = temp2->right;
                temp2 = temp3;
            }
            temp2 = temp2->right;
        }
        temp1 = temp1->right;
    }
}

void variables(){
    int i;
    for(i = 0; i < 26; i++){
        a[i] = 65 + i;  
        b[i] = 97 + i;  
    }
}

void convert(){
    int i, j;
    node* temp;
    temp = fin;
    while(temp!=NULL){
        j = 0;
        for(i = 0; i < var; i++){
            if(temp->bin[i] == 0){
                temp->term[j] = b[i];
                j++;
            }
            if(temp->bin[i] == 1){
                temp->term[j] = a[i];
                j++;
            }
        }
        temp = temp->right;
    }
}

void func(FILE* saida){
    node* temp;
    temp = fin;
    fprintf(saida, "Função minimizada: ");
    while(temp != NULL){
        fprintf(saida, "%s", temp->term);
        if(temp->right != NULL){
            fprintf(saida, " + ");
        }
        temp = temp->right;
    }
    fprintf(saida, "\n");
}


void implicants_table(){
    node* temp;
    int i, j, k, m, n, x, y, count = 0, count2 = 0, a = 0;
    for(i = 0; i < counter; i++){
        for(j = 0; j <= limit; j++){
            imptable[i][j] = 0;
        }
    }
    i = 0;
    j = 0;
    k = 0;
    temp = improot;

    while(temp != NULL){
        k = 0;

        while(temp->data[k] != -1){
            imptable[i][temp->data[k]] = 1;  
            k++;
        }
        i++;
        temp = temp->right;
    }
    printf("\n\n\t\t\tPrime Implicants Table\n\n\n");
    temp = improot;
    i = 0;
    printf(" ");
    while(minterms[i] != -1){
        printf("%d\t",minterms[i]);
        i++;
    }
    printf("\n\n");
    for(i = 0; i < counter; i++){
        printf(" ");
        a = 0;
        for(j = 0; j <= limit; j++){
            if(j == minterms[a]){
                if(imptable[i][j] == 0){
                    printf("-");
                }
                if(imptable[i][j] == 1){
                    printf("X");
                }
                printf("\t");
                a++;
            }
        }
        y = 0;
        while(temp->data[y] != -1){
            printf("%d,", temp->data[y]);
            y++;
        }
        printf("\b ");
        temp = temp->right;
        printf("\n\n");
    }
    printf("\n\n");

    for(i = 0; i < counter; i++){
        for(j = 0; j <= limit; j++){
            count = 0;
            if(imptable[i][j] == 1){
                y = j;
                x = i;
                for(k = 0; k < counter; k++){
                    if(imptable[k][j] == 1){
                        count++;
                    }
                }
                if(count == 1){
                    essential[t] = x;
                    t++;
                    for(n = 0; n <= limit; n++){
                        if(imptable[i][n] == 1)
                        {
                            for(m = 0; m < counter; m++){
                                imptable[m][n] = 0;
                            }
                        }
                    }
                }
            }
        }
    }
    essential[t] = -1;
    i = 0;
}

void other_implicants(){
    no = 0;           
    int count1 = 0, count2 = 0;
    int i, j;
    for(i = 0; i < counter; i++){
        count1 = 0;
        for(j = 0; j <= limit; j++){
            if(imptable[i][j] == 1){
                no++;
                count1++;
            }
        }
        if(count1>count2){
            essential[t] = i;
            count2 = count1;
        }
    }
    for(j=0;j<=limit;j++){
        if(imptable[essential[t]][j]==1){
            for(i=0;i<counter;i++){
                imptable[i][j]=0;
            }
        }
    }
    t++;
    essential[t] = -1;
    if(no > 0){
        other_implicants();
    }
}

void final_terms(){
    int i = 0, j, c = 0,x;
    node *temp, *ptr;
    fin = temp = (node*)malloc(sizeof(node));
    while(essential[i] != -1){
        ptr = improot;
        x = essential[i];
        for(j = 0;j < x;j++){
            ptr = ptr->right;
        }
        j = 0;
        while(ptr->data[j] != -1)    
        {
            temp->data[j] = ptr->data[j];
            j++;
        }
        temp->data[j]=-1;
        for(j = 0; j < var; j++){
            temp->bin[j] = ptr->bin[j];
        }
        temp = temp->right = (node*)malloc(sizeof(node));
        i++;
        c++;
    }
    temp->right = NULL;
}