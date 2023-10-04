#include <stdio.h>
#include <stdlib.h>


typedef struct NO{
    int code;//A maioria dos documentos tem um codigo que o identifica, rg, cpf, titulo eleitoral,cartões de credito e debito, etc.
    int rg;//diferenciar duas pessoas com mesmo nome
    char Nome[50]; //nome do dono
    char doc[20]; //nome do documento
    int vencimento[3];
    int titularidade;//0- nao pode ser alterada  1-pode ser alterada, contas e escrituras podem ter a titularidade alterada
    struct NO * esq;
    struct NO * dir;

}NO;



NO * raiz=NULL;

//As 3 a seguir são funções recursivas que fazem a maior parte do trabalho
NO * adicionar(NO*aux,NO*novo);
NO * delete(NO* aux,int code);
NO * search(int code, NO*aux);

//Chamadas no começo e no fim do código, as funções a seguir respectivamente recuperam e guardam os dados da arvore
void recuperar_dados(FILE * reg);//cria structs, atribui os dados armazenados no arquivo, e utiliza a função de adicionar para reconstruir a arvore
void arquivar(FILE * reg,NO * aux);//começando pela raiz, segue pela esquerda ate encontrar NULL, e entao explora a direita

//Funções de "Atendimento ao cliente", com input e output de informações e fazendo uso das funções recursivas para eficiencia
void cadastrar();//pede e le as informações, e entao adiciona a raiz se for nula, ou redireciona a struct para a função adicionar
void remover(int code);//verifica se a raiz e nula, verifica se o NO existe, se so existe a raiz, e entao chama a função delete
void alterar(int code);// usa search para achar o NO, mostra os dados e pergunta qual deve ser alterado.
void buscar(int code);//usa search para conseguir o NO e mostra as informações

//Se for NULL retorna 0 e printa uma mensagem, se não for NULL retorna 1
int check ();//verifica se a raiz e NULL
int check_aux(NO*aux);//verifica se o NO e NULL

//Funções necessarias para o funcionamento da funçõe delete
NO * copiar_data (NO* aux,NO * aux2);//copia os dados da struct
NO * sucessorinor (NO* aux2);//reorganiza a Arvore antes do NO ser deletado

int main()
{
    
    FILE * registro;//recebe o fopen e e chamado pelas funções
    int code;//guarda as respostas
 
    if(fopen("registro.txt","r"))//Como NULL e o mesmo que falso ou 0 no contexto de condições, o fopen identifica se a uma sessao anterior antes de perguntar
    {
        printf("Bem vindo ao sistema de gerenciamento de arquivos!\nVoce quer\n[1] Acessar os dados da sessao anterior \n[2] Iniciar a sessao do zero(os dados da sessao anterior serao apagados)\n\n");
        scanf("%d",&code);
        
        if(code==1)
        {
            registro = fopen("registro.txt","r");
            recuperar_dados(registro);//escreve os dados do arquivo numa arvore
            fclose(registro);
        }      
    
    }
    for(int a = 0; a != 5;){

        printf("\n\nO que deseja fazer?\n[1] Cadastrar um novo documento \n[2] Excluir um documento \n[3] Alterar os dados em um documento\n[4] Buscar documento \n[5] Sair e arquivar dados\n");
        scanf("%d", &a);

       
        if(a == 1){

            cadastrar();           
        }
       
       
        if(a == 2)
        {
            if(check())//O retorno de 1 ou 0 responde o if(), evita pedir dados desnecessariamente, e envia uma mensagem de erro caso seja nulo
            {
                printf("Informe o codigo do documento a ser removido: \n");
                scanf("%d", &code);
                remover(code);
            }
                 
        }


        if(a == 3)
        {
            if(check())
            {
                printf("Informe o codigo do documento a ter dados alterados:\n");
                scanf("%d", &code);
                alterar(code);
            }

            
        }
        
    
        if(a == 4)
        {
            if(check())
            {
                 printf("Informe a code a ser buscada:\n");
                 scanf("%d", &code);
                 buscar(code);
            }          
                
        }
    
    
        if(a == 5)
        {
            registro=fopen("registro.txt","w");
            arquivar(registro,raiz);
            printf("Dados registrados no arquivo de nome <registro.txt>\nObrigado por usar o Sistema de Gerenciamento de Documentos!\n");
        }
    }
    
    
    
    return 0;
}

void cadastrar()
{
    NO * Novo=malloc(sizeof(NO));
    printf("Informe o codigo do documento:\n");
    scanf("%d",&Novo->code);

    if (search(Novo->code,raiz))
    {/*//verifica se o codigo esta em uso, search() retorna NULL se o codigo nao estiver registrado, o que serve de "falso", e se retornar um 
        ponteiro se estiver em uso, o que serve de "verdadeiro"*/
        printf("Ja existe um documento com esse codigo.\n\n");
    }
    
    else
    {
        
        printf("Informe o nome do dono do documento:\n");
        scanf("%s",&Novo->Nome);

        printf("Informe o RG do de %s\n",Novo->Nome);
        scanf("%d",&Novo->rg);

        printf("Informe qual o documento:\n");
        scanf("%s",&Novo->doc);

        printf("Informe a data de vencimento na forma dd/mm/aa:\n");
        int x=0;
        for(char temp='/';temp=='/';x++)
        {// permite armazenar os 3 numeros na mesma linha
            scanf("%d%c",&Novo->vencimento[x],&temp);
        }

        printf("A titularidade pode ser alterada?\n[0]Nao\n[1]Sim\n");
        scanf("%d",&Novo->titularidade);

        Novo->dir=NULL;
        Novo->esq=NULL;

    }
    if (raiz==NULL)
    {
        raiz=Novo;
    }
    else
    {
        adicionar(raiz,Novo);
        printf("Documento cadastrado com sucesso\n\n");
    }

} 

void remover(int code)
{
    if(check())//Verifica se a raiz e Nula, se for Nula envia uma mensagem e retorna 0 ("falso"), se nao for Nula retorna 1("verdadeiro")
    {
        if(check_aux(search(code,raiz)))//verifica se existe um NO com o codigo, se nao existir envia uma mensagem
        {
             if((raiz->dir==NULL) && (raiz->esq==NULL))
          {//se raiz for o ultimo membro, simplesmente e anulada
            raiz=NULL;
          }       
           else
           {//depois de verificar os casos menos relevantes, chama a função recursiva
             delete(raiz,code);
             printf("Remocao realizada com sucesso\n\n");
           }
        }
    }
}

void alterar(int code)
{
    NO* aux= search(code,raiz);//Recebe o ponteiro do NO, ou NULL
    char temp;
    if(check_aux(aux))//se for NULL, vai dar falso e exibir uma mensagem, do contrario da verdadeiro;
    {
        printf("\nDocumento:%s \nDono:%s \nData de Vencimento:%d/%d/%d\n\n",aux->doc,aux->Nome,*(aux->vencimento),*(aux->vencimento+1),*(aux->vencimento+2));
        for(int R=0;R!=3;)
        {
            printf("Quais dados deseja alterar?\n[1]Data de Vencimento\n[2]Titularidade\n[3]Finalizar alteracoes\n");
            scanf("%d",&R);
            
           if(R==1)
           { 
            
            printf("Informe a data de vencimento atualizada:\n");
            int x=0;
            for(temp='/';temp=='/';x++)
            {
                scanf("%d%c",&aux->vencimento[x],&temp);
            
            }
           }

           if(R==2)
           {
            if(aux->titularidade)
            {
                printf("Informe o nome do novo dono:\n");
                scanf("%s",aux->Nome);
                printf("Informe o RG de %s",aux->Nome);
                scanf("%d",&aux->rg);
            }
            else
            {
                printf("O documento em questao nao pode ter titularidade alterada\n");
            }
           }


            if(R==3)
            {
                printf("Funcionalidade de alteracoes finalizada");       
            }
        }


    }
}

void buscar(int code)
{
    NO*aux=search(code,raiz);
    if(check_aux(aux))//retorna 1 se existir, e se for nulo retorna 0 e envia mensagem
    {
        printf("\nDono:%s \nDocumento:%s   Vencimento:%d/%d/%d",aux->Nome,aux->doc,*aux->vencimento,*(aux->vencimento+1),*(aux->vencimento+2));
    }
}

void recuperar_dados(FILE * reg)
{
    NO * aux= malloc(sizeof(NO));
    aux->code = getw(reg);
    do
    {       
        
        aux->rg=getw(reg);
        aux->titularidade=getw(reg);
        *(aux->vencimento)=getw(reg);
        *(aux->vencimento+1)=getw(reg);
        *(aux->vencimento+2)=getw(reg);
        fscanf(reg,"%s",aux->Nome);
        fscanf(reg,"%s",aux->doc);

        aux->esq=aux->dir=NULL;
        if(raiz==NULL)
        {
            raiz=aux;
        }
        else
        {
            adicionar(raiz,aux);
        }

        //essa parte esta no final apesar de ser o "começo", ja que e preciso o aux->code do proximo NO para decidir se o while continua ou nao

        aux= malloc(sizeof(NO));
        fgetc(reg);// pula uma linha que foi adicionada no arquivar() para evitar um erro de leitura envolvendo as strings Nome e doc
        aux->code = getw(reg);

    } while (aux->code>0);//quando le uma linha vazia, getw() retorna -1, logo quando encontar uma linha vazia vai finalizar o loop
    
}

void arquivar(FILE * reg, NO*aux)
{
     putw(aux->code,reg);         
     putw(aux->rg,reg);
     putw(aux->titularidade,reg);
     putw(*(aux->vencimento),reg);
     putw(*(aux->vencimento+1),reg);
     putw(*(aux->vencimento+2),reg);
     fprintf(reg,aux->Nome);
     fprintf(reg,"\n");
     fprintf(reg,aux->doc);
     fprintf(reg,"\n");

     if(aux->esq)//verifica se a esquerda esta Nula, do contrario chama arquivar(,aux->esq)
     {
        arquivar(reg,aux->esq);
     }
     if(aux->dir)//o mesmo que o if() acima, mas a direita
     {
        arquivar(reg,aux->dir);
     }
    //uma vez que a função recursiva acha as "folhas"(os NOs sem esquerda ou direita) a recursão acaba
}




NO * adicionar(NO*aux,NO*novo)
{
   
     if (aux == NULL)//procura um esquerda ou direita que esteja nulo
     {        
        aux=novo;
     }
     else if (novo->code < aux->code)//se o codigo do novo for menor que o codigo do auxiliar, "envia" o novo para esquerda
     {
        aux->esq = adicionar(aux->esq,novo);
     }
     else if(novo->code> aux->code)//o mesmo do if() acima, mas se for maior "envia" para a direita
     {
        aux->dir = adicionar(aux->dir,novo);
     }
     return aux;
     /*quando a esquerda ou direira do auxiliar nao e nulo, o retorno nao muda nada, quando o adicionar encontra um lugar livre,
     retorna o novo, fazendo-o esquerda ou direita do NO apropriado*/

       
    }

NO * delete(NO* aux,int code)
{
    
    if(aux==NULL)
    {     
        return aux;
    }
    if (code < aux->code)
    {
        
        aux->esq=delete(aux->esq, code);
    }
    else if (code > aux->code)
    {
        aux->dir=delete(aux->dir,code);
    }
    else
    {
        if(aux->esq==NULL)
        {
            return aux->dir;
        }
        else if (aux->dir==NULL)
        {
            return aux->esq;
        }
        
        aux=copiar_data(aux,sucessorinor(aux->dir));
        aux->dir=delete(aux->dir,code);

        
    }
    return aux;

}

NO * search(int code, NO*aux)
{    
        if (aux == NULL){
            
            return NULL;
        }    


        if (aux->code == code)
            return aux;

        if (code < aux->code)
            return search(code,aux->esq);

        else if (code > aux->code)
            return search(code,aux->dir);


        return NULL;
}



int check ()
{//Verificar se ha ao menos uma code cadastrada no sistema para evitar coletas de dados desnecessarias
    if (raiz==NULL)
    {
        printf("Nao ha documentos cadastrados no sistema\n\n");//e junto ja vem a mensagem avisando
        return 0;
    }
    else{
        return 1;//retorna 1 (true) para os if 
    }
}

int check_aux(NO*aux)
{//Pode parecer desnecessario, mas o aviso de code nao cadastrada aparece em 3 das 4 funções essenciais
    if(aux==NULL)
    {//Usado em conjunto com search() para responder rapidamente os If e evitar coleta desnecessaria de dados
        printf("\nNao ha documento registrado no sistema com esse codigo\n");
        return 0;
    }
    else
    {
        return 1;
    }
}

NO* copiar_data (NO* aux,NO * aux2)
{/*para o funcionamento de delete() e necessario copiar todos dados menos esquerda e direita, e achei melhor colocar numa função*/
        
        aux->code = aux2->code;
        *aux->doc = *aux2->doc;
        *aux->Nome = *aux2->Nome;
        aux->rg = aux->rg;
        aux->titularidade = aux2->titularidade;
        aux->vencimento[0]= aux2->vencimento[0];
        aux->vencimento[1]= aux2->vencimento[1];
        aux->vencimento[2]= aux2->vencimento[2];
        return aux;
}

NO* sucessorinor (NO* aux2)
{//sucessor in orden, tambem para a funcao delete funcionar corretamente.
   
    NO * aux =malloc(sizeof(NO));
    aux= copiar_data(aux,aux2);        
        
        while (aux2->esq)//se for NULL da falso
        {
            aux=copiar_data(aux,aux2->esq);
            aux2=aux2->esq;
        }
        return aux;
} 