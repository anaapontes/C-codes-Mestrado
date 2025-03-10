#include<stdio.h>    // Inclui a biblioteca padrão de entrada e saída, usada para funções como printf e scanf.
#include<stdlib.h>   // Inclui a biblioteca padrão de funções utilitárias, como alocação de memória e controle de processos.
#include<math.h>     // Inclui a biblioteca de funções matemáticas, como sqrt e pow.

// Definindo constantes para parâmetros do modelo
#define phiU (2.23)             // Taxa de oviposição para indivíduos não infectados. Em média 60-103 ovos por ciclo; a fêmea vive entre 32 a 37 dias com uma viabilidade de ovos entre 92-97%.
#define phiI 0.              // Taxa de oviposição para indivíduos infectados. Valor médio estimado em 2.07 ovos por ciclo.
#define muaU 0.02               // Taxa de mortalidade para indivíduos imaturos não infectados. Varia entre 0.01 e 0.47 por dia.
#define muaI (muaU)             // Taxa de mortalidade para indivíduos imaturos infectados, igual à dos não infectados.
#define muU 0.07                // Taxa de mortalidade de adultos não infectados, em dias^-1. Média de 0.07 por dia.
#define muI (muU*1.1)           // Taxa de mortalidade para adultos infectados, calculada como 10% maior que a dos não infectados (10-56% maior).
#define tauU 0.1                // Taxa de desenvolvimento para indivíduos não infectados. Indica a velocidade de desenvolvimento dos imaturos.
#define tauI (tauU)             // Taxa de desenvolvimento para indivíduos infectados, igual à dos não infectados.
#define q 1.                    // Incompatibilidade citoplasmática, a taxa de incompatibilidade entre indivíduos.
#define xi 0.                   // Probabilidade de transmissão vertical, com referência entre 0.93 e 0.95.
#define R 0.5                   // Razão de nascimentos entre fêmeas e machos, indicando a proporção de cada sexo nascimentos.
#define LR 250                  // Tamanho da rede de simulação, representando uma malha de 250x250 células.
#define LR2 (LR*LR)             // Área total da rede de simulação, calculada como o quadrado do tamanho da rede (250*250).
#define DR 7                    // Tamanho da vizinhança de saída para os alados, número de células ao redor de um ponto onde os mosquitos podem se mover.
#define BORDAS (DR+1)           // Borda da rede, deve ser maior que DR para garantir que a vizinhança de saída esteja totalmente dentro da rede.
#define DR2 ((2*DR+1)*(2*DR+1)) // Área da vizinhança de saída, calculada como o quadrado da dimensão da vizinhança (baseado em DR).
#define DO 5                    // Tamanho da vizinhança de acasalamento, número de células ao redor de um ponto onde os mosquitos podem acasalar.
#define DO2 ((2*DO+1)*(2*DO+1)) // Área da vizinhança de acasalamento, calculada como o quadrado da dimensão da vizinhança (baseado em DO).
#define MEDIA 150               // Número de simulações para calcular a média dos resultados.
#define TEQ (5*365)             // Tempo para o equilíbrio, número de dias para o sistema atingir o estado de equilíbrio (5 anos).
#define TFIM (TEQ+1)            // Tempo de liberação dos mosquitos infectados com Wolbachia
#define TFIM2 (TFIM+30)         // Tempo final, após 30 dias sem liberação
#define TDIF 7                  // Número de passos de difusão, número de passos de tempo para simular a difusão dos mosquitos.
#define CC 30                   // Tamanho dos containers, representando o tamanho das caixas onde os mosquitos são simulados.
#define QU 0.5                  // Proporção de indivíduos não infectados no início da simulação, a fração inicial da população que não está infectada.

void vizinhanca(int i, int V[8]);   // Função para determinar a vizinhança de saída de um mosquito em uma posição dada
void figura(double p, int NCC);        // Função para gerar uma figura
void dif(int resto);                // Função para simular a difusão dos mosquitos
int lugar(int kk, int LUG[DR2]);    // Função para encontrar um local para um mosquito em uma posição dada
int ImpCaixa(int i);                // Função para imprimir informações sobre uma caixa
void MortAdulto();                  // Função para lidar com a mortalidade de adultos
void Oviposicao(int NCC);           // Função para lidar com a oviposição
void TransImaturo(int NCC);         // Função para simular o desenvolvimento dos imaturos
int ContagemAd();                   // Função para realizar a contagem de adultos
int Equilibrio(int i, int NC);      // Função para obter o estado de equilíbrio do sistema
void TIPO(int i, int VV[2]);        // Função para definir o tipo de um determinado indivíduo
void Release(int i);
double Eficacia(double dinU[TFIM2]);


// Estrutura que representa uma caixa com capacidade, estado dos indivíduos e posição.
struct caixa {
    int CAP;            // Capacidade da caixa.
    int TAQ[LR];        // Array que armazena o estado dos indivíduos: 0 (vazio), 1 (infectado) ou 2 (não infectado).
    int POS;            // Posição da caixa no grid.
};

// Array de estruturas do tipo caixa, representando todas as caixas na simulação.
struct caixa PA[LR2];  // PA[i] representa uma caixa;

// Array para armazenar a população de adultos em cada célula do grid
int PT[LR2];

int main() {
    // Declaração de variáveis
    int i, j, k, l, n, nU, nI, NC,NCC,jrelease;
    double QI,xe,J,dinU[TFIM2], dinI[TFIM2], somaEficacia;
    FILE *ar;

    // Abre o arquivo para escrita
    ar = fopen("eficacia_area_m_libvirgem_virgem.dat", "w");

    QI=0.;//proporcao de infectados com relacao a nao infectados
    NC=315;// Alterar para o número de criadouros desejado
    while(QI<=20){
        somaEficacia = 0.0; // Variável para acumular a eficácia

        // Loop para calcular médias em várias simulações
        for (n = 0; n < MEDIA; ) {
            for(l=0;l<TFIM2;l++) dinU[l]=dinI[l]=0.;

            // Obtém o equilíbrio do sistema para o número atual de caixas (NC)
            NCC = Equilibrio(0, NC); // 0 = aleatório e 1 = uniforme

            // Simula a dinâmica dos mosquitos
            for (i = 0, j = 0; i < TEQ; i++) {
                MortAdulto();      // Atualiza a mortalidade dos adultos
                Oviposicao(NCC);   // Simula a oviposição
                TransImaturo(NCC); // Simula o desenvolvimento dos imaturos

                // Simula a difusão dos mosquitos
                for (k = 0; k < TDIF; k++) {  // Número de passos de difusão
                    dif(j); // Atualiza a difusão
                    j = (j) ? 0 : 1;  // Alterna entre 0 e 1 para a próxima iteração
                }
                
                for (k = 0,nU = 0,nI = 0; k < LR2; k++) {
	                if (PT[k] == 3) nU++; // só VIRGEM nao infectado
	                else if ( PT[k] == 4) nI++;
                }
                
                dinU[i]+=nU;
                dinI[i]+=nI;
            }// Fim TEQ

            jrelease = (int) (QI*nU);
            Release(jrelease);  //numero de nao infetados adultos na rede
            // Liberacaoes periodicas
            for(i=TEQ;i<TFIM;i++){
                MortAdulto();       // mortalidade adulto
	            Oviposicao(NCC);    // oviposicao
	            TransImaturo(NCC);  // desenvolvimento

	            for (k = 0; k < TDIF; k++) {  // Difusao
	                dif(j);
	                j = (j) ? 0 : 1;
	            }

	            for (k = 0,nU = 0,nI = 0; k < LR2; k++) {
	                if ( PT[k] == 3 ) nU++; // só VIRGEM nao infectado
	                else if ( PT[k] == 4) nI++;
                }
         
                dinU[i]+=nU;
                dinI[i]+=nI;

                
            } // fim do TFIM

            for(i=TFIM;i<TFIM2;i++){  // final das liberacoes
	            MortAdulto();       // mortalidade adulto
	            Oviposicao(NCC);    // oviposicao
	            TransImaturo(NCC);  // desenvolvimento

	            for (k = 0; k < TDIF; k++) {  // Difusao
	                dif(j);
	                j = (j) ? 0 : 1;
	            }

	            for (k = 0,nU = 0,nI = 0; k < LR2; k++) {
	                if ( PT[k] == 3) nU++; // só VIRGEM nao infectado
	                else if ( PT[k] == 4) nI++;
                }
         
                dinU[i]+=nU;
                dinI[i]+=nI;
            } // fim do TFIM2

            // Verifica condição para incrementar 'n'
            if (nU > 0) {
                somaEficacia += Eficacia(dinU);
                n++; // Incrementa o contador de médias somente se nU > 0
            } else {
                printf("Repetindo simulação: nU <= 0\n");
            }
        } // Fim do loop de médias

        figura(QI,NCC);

        J = somaEficacia / MEDIA;

        fprintf(ar,"%lf %lf\n",QI,J);
        printf("QI:%lf J:%lf\n",QI,J);
    
        QI+=1.0;

  } // fim do while
  fclose(ar);
}// fim do main

//---------------------------------------------------------------------------------------------
// Função para distribuir caixas e indivíduos na rede
int Equilibrio(int icond, int NC) {
    int j, i, n, k, NCC, TC[3] = {CC, CC, CC};
    int sum, px, l = 3, m = 3;
    double xe;

    // DISTRIBUICAO DA POPULACAO ALADA
    for (n = 0; n < LR2; n++) {
        xe = (double)rand() / (RAND_MAX + 1.);
        if (xe < QU) 
            PT[n] = 2; // Se o número é menor que QU, NAO INFECTADO (SELVAGEM)
        else 
            PT[n] = 0; // Caso contrário, VAZIO
    }

    // Inicializa a posição de todas as caixas (PA[n].POS) como -1, indicando que nenhuma caixa está posicionada na rede.
    for (n = 0; n < LR2; n++) PA[n].POS = -1; 

    // DISTRIBUICAO ALEATORIA DAS CAIXAS NA REDE
    if (icond == 0) { // Se o argumento icond for igual a 0, as caixas são distribuídas aleatoriamente na rede.
        n = 0; // Inicializa a variável n como zero, será usada para rastrear quantas caixas foram posicionadas.
        do {
            // POSICAO I,J CAIXA
            j = (int)rand() % LR;  // Gera posição j aleatoriamente, % é usado para garantir que os valores estejam dentro dos limites de LR.
            i = (int)rand() % LR;  // Gera posição i aleatoriamente

            if ((i >= BORDAS) && (i < LR - BORDAS) && (j >= BORDAS) && (j < LR - BORDAS)) {
                // Verifica se a posição está dentro de um raio para evitar influência das condições de contorno
                sum = 1; // Inicializa a variável sum como verdadeira (1)
                for (k = 0; k < n; k++) {
                    px = j + (int)i * LR; // Calcula uma representação da posição (i, j) como um inteiro. Multiplicar i*LR garante que as posições sejam únicas na rede.
                    if (px == PA[k].POS) 
                        sum = 0;          // A posição já foi sorteada, então não deve ser repetida
                    break;
                }
                if (sum) {
                    px = j + (int)i * LR;
                    PA[n].POS = px;         // Atribui a posição da caixa na rede
                    n++;                    // A variável n é incrementada para rastrear quantas caixas já foram posicionadas
                }
            }
        } while (n < NC); // Repete até que todas as caixas estejam posicionadas
        NCC = n;
    } 
    else { // Se o argumento icond não for igual a 0, as caixas são distribuídas uniformemente na rede
        n = 0;  // Inicializa a variável n como zero, será usada para rastrear quantas caixas foram posicionadas.
        for (i = BORDAS; i < LR - BORDAS; i += m) { // A cada iteração, i é incrementado em m.
            // Verifica se a posição está dentro de um raio para evitar influência das condições de contorno
            for (j = BORDAS; j < LR - BORDAS; j += l) { // A cada iteração, j é incrementado em l.
                px = j + (int)i * LR;   // Calcula uma representação única da posição (i, j) como um valor inteiro.
                PA[n].POS = px;         // Atribui a posição da caixa na rede uniformemente
                n++;                    // Incrementa a variável n para rastrear quantas caixas já foram posicionadas.
            }
        }
        NCC = n;
    }

    // COLOCANDO INDIVIDUOS IMATUROS NA CAIXA
    k = 0;
    for (n = 0; n < NCC; n++) {  // Percorre todas as caixas
        PA[n].CAP = TC[k % 3];   // Capacidade da caixa
        k++;                     // Incrementa a variável k para definir a próxima capacidade da caixa na próxima iteração do loop.
        for (i = 0; i < PA[n].CAP; i++) {
            xe = rand() / (RAND_MAX + 1.);
            if (xe < 0.4){
                PA[n].TAQ[i] = 2;           // População na caixa é não infectada
            } else{
                PA[n].TAQ[i] = 0;           // População na caixa é vazia
            }
        }
    }

    return NCC;
}

//---------------------------------------------------------------------------------------------
// Função que simula a liberação dos mosquitos
void Release(int j)	
{	
  int i,jk,ik,m,k,ni;		
  double xe;

  //DISTRIBUICAO DA POPULACAO ALADA
  k=m=0;

  while(k<j){
    jk = (int) rand()%LR; //POSICAO I,J CAIXA0
    ik = (int) rand()%LR;
    i =  jk + (int) ik*LR;

    if(PT[i]==0){
	        PT[i] = 4;        // IN virgem
	        k++;
    }
    m++;
    if(m==LR2) break; // Garante que a busca pare após verificar todos os lugares
  }
}

//---------------------------------------------------------------------------------------------
// Função que calcula as posições das caixas vizinhas em uma grade bidimensional, levando em conta uma topologia toroidal.
void vizinhanca(int k,int V[8])  
{
  int i, j, m, n;
  // i: representa a linha
  // j: representa a coluna
  // m: É usado para calcular a posição do vizinho na direção vertical (norte e sul) em relação à caixa k
  // n: É usado para calcular a posição do vizinho na direção horizontal (leste e oeste) em relação à caixa k.
  
  // Calcula a posição da caixa k na grade bidimensional
  i = (int)k / LR; 
  j = (int)k % LR; 
  
  // Calcula as posições dos vizinhos em relação à caixa k
  if (i == 0)         // Se a caixa k estiver na primeira linha (índice 0)
    m = LR - 1;       // m receberá o valor LR - 1, que é o índice da última linha 
  else
    m = (i - 1) % LR; // m será calculado como (i - 1) % LR, que é o índice da linha imediatamente acima da caixa k
  
  if (j == 0) {       // Se a caixa k estiver na primeira coluna (índice 0)
    n = LR - 1;       // n receberá o valor LR - 1, que é o índice da última coluna
  } else {
    n = (j - 1) % LR; // n será calculado como (j - 1) % LR, que é o índice da coluna imediatamente à esquerda da caixa k
  }

  V[0] = LR * m + j % LR;                       // NORTE
  V[1] = LR * ((i + 1) % LR) + j % LR;          // SUL
  V[2] = LR * (i % LR) + (j + 1) % LR;          // LESTE
  V[3] = LR * (i % LR) + (n) % LR;              // OESTE
  V[4] = LR * m + n % LR;                       // NOROESTE
  V[5] = LR * ((i + 1) % LR) + n % LR;          // SUDOESTE
  V[6] = LR * m + (j + 1) % LR;                 // NORDESTE
  V[7] = LR * ((i + 1) % LR) + (j + 1) % LR;    // SUDESTE
}

//---------------------------------------------------------------------------------------------
// Função que calcula quantos lugares vazios estão nas proximidades de uma caixa identificada por sua posição kk na grade e retorna esse número.
int lugar(int kk,int LUG[DR2])
{
  int i,j,n1,k,l,m;
  // n1: contador de lugares vazios na vizinhança da caixa.
  // k: a posição de um vizinho na grade bidimensional 'PT'.

  i = (int) kk / LR;                    // Divide kk pelo tamanho da grade LR para obter a linha.
  j = (int) kk % LR;                    // Calcula o resto da divisão de kk por LR para obter a coluna.

  n1 = 0;                               // Inicializa o contador de lugares vazios como zero.
  for (l = i - DR; l <= i + DR; l++) {  // Loop para percorrer as linhas vizinhas da caixa.
    for (m = j - DR; m <= j + DR; m++) {// Loop para percorrer as colunas vizinhas da caixa.
      k = m + (int) l * LR;             // Calcula a posição do vizinho na grade.

      if (PT[k] == 0) {                 // Verifica se o vizinho está vazio (valor igual a 0).
        LUG[n1] = k;                    // Armazena a posição do vizinho no vetor LUG.
        n1++;                           // Incrementa o contador de lugares vazios.
      }
    }
  }

  return n1;                            // Retorna o número de lugares vazios na vizinhança da caixa.
}

//---------------------------------------------------------------------------------------------
// Função para contar o número de indivíduos selvagens (não infectados) e infectados em uma área ao redor de um ponto específico
void TIPO(int kk,int nUI[2])  // nUI[0] = nU (não infectado), nUI[1] = nI (infectado)
{
  int i,j,n1,k,l,m,nl0, nl1, nc0, nc1;
  
  // Inicializa os contadores
  nUI[0] = nUI[1] = 0;
  
  // Converte o índice linear kk para coordenadas 2D
  i = (int) kk / LR;  // Coordenada linha
  j = (int) kk % LR;  // Coordenada coluna

  // Define os limites da área de análise, considerando a borda
  nl0 = i - DO;                 // Limite inferior da linha
  if (nl0 < 0) nl0 = 0;         // Corrige se o limite inferior for negativo
  nl1 = i + DO;                 // Limite superior da linha
  if (nl1 >= LR) nl1 = LR - 1;  // Corrige se o limite superior ultrapassar a borda
  nc0 = j - DO;                 // Limite inferior da coluna
  if (nc0 < 0) nc0 = 0;         // Corrige se o limite inferior for negativo
  nc1 = j + DO;                 // Limite superior da coluna
  if (nc1 >= LR) nc1 = LR - 1;  // Corrige se o limite superior ultrapassar a borda
  
  // Percorre a área ao redor do ponto especificado
  for (l = nl0; l <= nl1; l++) { // Itera sobre as linhas da área
    for (m = nc0; m <= nc1; m++) { // Itera sobre as colunas da área
      k = m + (int) l * LR; // Calcula o índice linear para a posição (l, m)

      // Conta o número de indivíduos não infectados e infectados
      if (PT[k] == 2) nUI[0]++;      // Se o indivíduo é não infectado não virgem
      else if (PT[k] == 3) nUI[0]++; // Se o indivíduo é não infectado virgem
      else if (PT[k] == 1) nUI[1]++; // Se o indivíduo é infectado não virgem
      else if (PT[k] == 4) nUI[1]++; // Se o indivíduo é infectado virgem
    }
  }
}

//---------------------------------------------------------------------------------------------
// Função para realizar a difusão de Morgulous, trocando valores de posições vizinhas em uma matriz
void dif(int a) // difusao de morgulous
{
  int i, j;
  int temp, S, E, SE, kk;
  double xe;
  
  // Itera sobre a matriz em passos de 2, começando pelo índice 'a'
  for (i = a; i < LR; i += 2) {   
    for (j = a; j < LR; j += 2) {
      
      kk = j + (int) i * LR; // Calcula o índice linear do ponto atual
        
      // Calcula os índices lineares para as posições ao redor do ponto atual
      S = LR * ((i + 1) % LR) + j % LR;         // Sul 
      E = LR * (i % LR) + (j + 1) % LR;         // Leste
      SE = LR * ((i + 1) % LR) + (j + 1) % LR;  // Sudeste
      
      xe = (double) rand()/(RAND_MAX+1.); // Gera um número aleatório para decidir a direção da rotação

      if (xe < 0.5) { // Se o número aleatório é menor que 0.5, gira para a direita
	    temp = PT[E];
	    PT[E] = PT[kk];
	    PT[kk] = PT[S];
	    PT[S] = PT[SE];
	    PT[SE] = temp;
      } else {       // Caso contrário, gira para a esquerda
	    temp = PT[S];
	    PT[S] = PT[kk];
	    PT[kk] = PT[E];
	    PT[E] = PT[SE];
	    PT[SE] = temp;
      }
    }
  }
}

//---------------------------------------------------------------------------------------------
// Função para contar o número total de mosquitos imaturos em uma caixa
int ImpCaixa(int i)
{
  int j,n1,n2;
  
  // Inicializa os contadores
  n1 = n2 = 0;

  // Itera sobre cada posição na caixa
  for (j = 0; j < PA[i].CAP; j++) {
        if (PA[i].TAQ[j] == 1) {
            // Incrementa o contador de imaturos infectados
            n1++;
        } else if (PA[i].TAQ[j] == 2) {
            // Incrementa o contador de imaturos não infectados
            n2++;
        }
    }

  return (n1+n2); // Retorna o número total de mosquitos imaturos (infectados e não infectados)
}

//---------------------------------------------------------------------------------------------
// Função para simular a mortalidade dos adultos e o acasalamento
void MortAdulto()
{
  int i,j,k,px,nUI[2];
  double xe,xe1,ENI,ENV,ENU,TE;
  
  // Itera sobre todas as posições na rede
  for(k = 0; k < LR2; k++){
    // Seleciona uma posição aleatória na rede
    j = (int) rand()%LR;  
    i = (int) rand()%LR;
    px =  j + (int) i*LR;   // Calcula o índice da posição
    
    switch(PT[px]){
      
    case 1: // Caso 1: Infectado não virgem
      xe = rand()/(RAND_MAX+1.);
      if(xe < muI) {
        PT[px] = 0; // Se xe for menor que muI, o indivíduo morre
      }
      break;

    case 2: // Caso 2: Não infectado não virgem
      xe = rand()/(RAND_MAX+1.);
      if(xe < muU){
        PT[px] = 0; // Se xe for menor que muU, o indivíduo morre
      }
      break;

    case 3: // Caso 3: Não infectado virgem, pode acasalar
      xe = rand()/(RAND_MAX+1.);
      if(xe < muU){
        PT[px] = 0; // Se xe for menor que muU, o indivíduo morre
      }
      else { // Caso contrário, o indivíduo pode acasalar
	    TIPO(px,nUI);                // Conta o número de selvagens e infectados na vizinhança do mosquito em questão
	    ENV = DO2 - (nUI[1]+nUI[0]); // Calcula o número de espaços vazios na vizinhança
	    ENI = ((double) nUI[1])/DO2; // Calcula a probabilidade de encontrar um adulto infectado
	    ENU = ((double) nUI[0])/DO2; // Calcula a probabilidade de encontrar um adulto nao infectado
	    TE = ENI+ENU;                // Calcula a probabilidade de ter encontro com um adulto

	    xe = rand()/(RAND_MAX+1.); 
	    if(xe<ENI) { // Se xe for menor que ENI, calcula as populações de não infectados e infectados
	        xe1 = rand()/(RAND_MAX+1.); // Incompatibilidade citoplasmática
	        if(xe1 < q){
                PT[px] = 0; // Se xe é menor que q, o indivíduo não gera descendentes, então retiramos ele
            }
	        else{
                PT[px] = 2;  // Caso contrário, o indivíduo se torna um não infectado que acasala
            }
	    }
	    else if(xe < TE){ // Se xe está entre a probabilidade de encontrar um adulto infectado e a probabilidade total
            PT[px] = 2;   // O indivíduo se torna um não infectado que acasala
        }
      }
      break;

    case 4: // Caso 4: Infectado virgem pode acasalar
      xe = rand()/(RAND_MAX+1.);
      if(xe < muI){
        PT[px] = 0; // Se xe for menor que muI, o indivíduo morre
      }
      else { // Caso contrário, o indivíduo pode acasalar
	    TIPO(px,nUI);                // Conta o número de selvagens e infectados na vizinhança do mosquito em questão
	    ENV = DO2 - (nUI[0]+nUI[1]); // Calcula o número de espaços vazios na vizinhança
	    ENI = ((double) nUI[1])/DO2; // Calcula a probabilidade de encontrar um adulto infectado
	    ENU = ((double) nUI[0])/DO2; // Calcula a probabilidade de encontrar um adulto nao infectado
	    TE = ENI + ENU;              // Calcula a probabilidade de ter encontro com um adulto
        
	    xe = rand()/(RAND_MAX+1.); 
	    if(xe<TE){ // Se xe é menor que a probabilidade total de encontrar um adulto na vizinhança
            PT[px] = 1; // O indivíduo se torna um infectado não virgem
        }
      }
      break;
    }
  }
}
//---------------------------------------------------------------------------------------------
// Função para simular a oviposição de mosquitos nas caixas
void Oviposicao(int NCC) //vizinhanca de moore
{
  int i,j,k,nU,nI,kU,kI,kT,m,menor,V[8],vazio;
  int TAM,MnU,MnI;
  double xe,xe1;

  // Itera sobre todas as caixas
  for(i = 0; i < NCC;i++){
    vazio = ImpCaixa(i);        // Conta os lugares ocupados na caixa
    vazio = PA[i].CAP-vazio;    // Número de espaços vazios na caixa
    vizinhanca(PA[i].POS,V);    // Obtém as posições das vizinhanças da caixa usando a vizinhança de Moore de raio 1
    TAM = 2*pow(PA[i].CAP,2);   // Calcula o máximo para busca, considerando o tamanho da caixa
 
    // Contagem de mosquitos não infectados e infectados na vizinhança
    nU = nI = 0;
    for(k = 0;k < 8;k++){
      if(PT[V[k]] == 1) nI++;
      else if (PT[V[k]] == 2) nU++;
    }

    // Conta o mosquito na própria caixa
    if(PT[PA[i].POS] == 1) nI++;
    else if (PT[PA[i].POS] == 2) nU++;

    // Calcula o número de ovos a serem colocados
    MnU = ceil(phiU*nU);  // oviposicao nao infectado
    MnI = ceil(phiI*nI);  // oviposicao infectado
    k = MnU+MnI; // Total de ovos a serem colocados
    
    // Determina o menor valor entre o número total de ovos e o número de espaços vazios, garantindo que não coloque mais ovos na caixa do que ela acomoda.
    menor = k < vazio ? k : vazio;
    
    kT=kU=kI=m=0;

    // Simula a colocação dos ovos
    while(kT < menor && m < TAM) {
      for(j = 0;j < PA[i].CAP;j++){
	    if(PA[i].TAQ[j] == 0){ // Lugar vazio na caixa
	        xe = k * (rand() / (RAND_MAX+1.));
	        if(xe < MnI && kI < MnI) {
	            xe1 = rand() / (RAND_MAX+1.);
	            if (xe1 < xi) { // Transmissao materna
	                PA[i].TAQ[j] = 1; // Imaturo infectado
	                kI++;
	            }
	            else{
	                PA[i].TAQ[j] = 2; // Imaturo nao infectado
	                kU++;
	            }
	            kT++;
	        } else if(kU < MnU){
	            PA[i].TAQ[j] = 2;     // Imaturo nao infectado
	            kT++;
	            kU++;
	        }
	    } else m++; // Incrementa o contador de espaços ocupados
      }//fim do for PA[i].CAP
    }//fim do while
  }//fim do for NCC
}
//---------------------------------------------------------------------------------------------
// Função que simula a emergência de mosquitos imaturos para adulto
void TransImaturo(int NCC)
{
  int i,j,k,nU,nI,kI,kU,menor,soma,vazio,l,LUG[DR2], xe1;
  double xe;
  
  // Itera sobre todas as caixas (representando diferentes áreas ou células)
  for(i = 0;i < NCC;i++){ 
    
    vazio = lugar(PA[i].POS,LUG); // Verifica a quantidade de lugares vazios ao redor da caixa

    // Inicializa os contadores para os imaturos infectados e não infectados
    nU = nI = 0;
    
    // Itera sobre todos os imaturos na caixa atual
    for(j = 0;j < PA[i].CAP;j++){ 

      // Verifica o tipo de imaturo (infectado ou não infectado) na posição j
      switch(PA[i].TAQ[j]){
        case 1: // Imaturo infectado
            xe=rand()/(RAND_MAX+1.);  // Gera um valor aleatório entre 0 e 1
            if(xe < muaI) { // Verifica a mortalidade de imaturos infectados
                PA[i].TAQ[j] = 0; // Remove o imaturo da caixa
            } else { // Caso o imaturo sobreviva, verifica a possibilidade de emergência
                xe=rand()/(RAND_MAX+1.);  // Novo valor aleatório
                if(xe < tauI) { 
                    PA[i].TAQ[j] = 0; // Remove o imaturo da caixa
                    nI++; // Conta um imaturo infectado emergido
                }
            }
            break;

        case 2: // Imaturo não infectado
            xe=rand()/(RAND_MAX+1.);  // Gera um valor aleatório entre 0 e 1
            if(xe < muaU) { // Verifica a mortalidade de imaturos não infectados
                PA[i].TAQ[j] = 0; // Remove o imaturo da caixa
            } else { // Caso o imaturo sobreviva, verifica a possibilidade de emergência
                xe=rand()/(RAND_MAX+1.);  // Novo valor aleatório
                if(xe < tauU) { 
                    PA[i].TAQ[j] = 0; // Remove o imaturo da caixa
                    nU++; // Conta um imaturo não infectado emergido
                }
            }
            break;        
      } // fim do switch
    } // fim do for sobre os imaturos
    
    // Calcula o número total de imaturos a serem emergidos, com base no parâmetro R
    soma=ceil(R*(nI+nU)); 

    // Determina o menor valor entre o número total de imaturos a emergir e a quantidade de lugares vazios
    menor = soma < vazio ? soma : vazio;

    // Inicializa os contadores para os imaturos infectados e não infectados a serem emergidos
    k=kI=kU=0;

    // Distribui os imaturos emergidos nos lugares vazios disponíveis
    while(k < menor) {
      j = rand()%vazio; // Seleciona um lugar vazio aleatoriamente
      l = soma*(rand()/(RAND_MAX+1.)); // Determina quantos imaturos serão emergidos no lugar selecionado

      xe1=rand() % 2; // Gera um valor aleatório 0 ou 1 para determinar se o imaturo será infectado ou não
      
      if(xe1==0){ // Se o valor aleatório for 0, tenta emergir um imaturo infectado
        if(l < nI && kI < nI){ // Verifica se há imaturos infectados para emergir
            // Verifica se o lugar está vazio
            if(PT[ LUG[j] ] == 0) {
                PT[ LUG[j] ] = 4; // Coloca o imaturo infectado no lugar
                k++;  // Incrementa o contador total de imaturos emergidos
                kI++; // Incrementa o contador de imaturos infectados emergidos
            }
        }
        // Se não houver mais imaturos infectados, tenta emergir imaturo não infectado
        else if(l < nU && kU < nU){
            // Verifica se o lugar está vazio
            if(PT[ LUG[j] ] == 0){
                PT[ LUG[j] ] = 3; // Coloca o imaturo não infectado no lugar
                k++; // Incrementa o contador total de imaturos emergidos
                kU++; // Incrementa o contador de imaturos não infectados emergidos
            }
        }
      }
      else{ // Se o valor aleatório for 1, tenta emergir um imaturo não infectado
        if(l < nU && kU < nU){ // Verifica se há imaturos não infectados para emergir
            // Verifica se o lugar está vazio
            if(PT[ LUG[j] ] == 0) {
                PT[ LUG[j] ] = 3; // Coloca o imaturo não infectado no lugar
                k++;  // Incrementa o contador total de imaturos emergidos
                kU++; // Incrementa o contador de imaturos não infectados emergidos
            }
        }
        // Se não houver mais imaturos não infectados, tenta emergir imaturo infectado
        else if(l < nI && kI < nI){
            // Verifica se o lugar está vazio
            if(PT[ LUG[j] ] == 0){
                PT[ LUG[j] ] = 4; // Coloca o imaturo infectado no lugar
                k++; // Incrementa o contador total de imaturos emergidos
                kI++; // Incrementa o contador de imaturos infectados emergidos
            }
        }
      }
    } // fim do while
  } // fim do for NCC
}


//---------------------------------------------------------------------------------------------
// Função para realizar a contagem de mosquitos adultos (nesse caso, só não infectados)
int ContagemAd()
{
  int i,n2;
  
  n2=0;
  // Loop para percorrer todos os elementos do vetor PT
  for(i = 0; i < LR2; i++){
    if(PT[i]==2 || PT[i]==3) n2++; // NAO INFECTADO OU NAO INFECTADO VIRGEM
  }
  
  return n2;
}
//---------------------------------------------------------------------------------------------
//Função que calcula a eficácia usando integral
 double Eficacia(double dinU[TFIM2])
 {
   double A0,Ac,J;
   int i;

   A0=(TFIM2-TEQ)*dinU[TEQ-1];
   Ac=0;
   for(i=TEQ;i<TFIM2;i++) Ac+=dinU[i];
   J=1-Ac/A0;
   J=J*100;
   
   return J;
 }
//---------------------------------------------------------------------------------------------
// Função para gerar figura
void figura(double l,int NCC)
{
  FILE *ar1;
  int m1;
  int px,k,j,i,V[8],BA[LR2];
  char arq1[20];
  
  static char branco[] = "255 255 255";
  static char preto[] = "000 000 000";
  static char vermelho[] = "255 000 000";
  static char verde[] = "000 255 000";
  static char azul[] = "000 000 255";
  static char amarelo[] = "255 255 000";
  static char cinza[] = "10 250 110";
  static char marrom[] = "137 61 27";
  static char roxa[] = "172 51 233";
  static char magenta[] = "200 200 000";
  static char rosa[] = "100 100 100";
  static char pessego[] = "110 150 50";
  
  // Cria o nome do arquivo de saída
  sprintf(arq1,"QI-%lf.dat",l);
  ar1=fopen(arq1,"w");
  
  // Imprime o cabeçalho do arquivo
  fprintf(ar1,"P3\n");
  fprintf(ar1,"%d %d\n",LR,LR);
  fprintf(ar1,"255\n");
  
  // Cria uma cópia da matriz PT para representar o estado das caixas
  for(m1 = 0; m1 < LR2; m1++) {
    BA[m1] = PT[m1];
  }
 
  // Itera sobre as caixas no vetor PA
  for(m1 = 0; m1 < NCC; m1++){
    vizinhanca(PA[m1].POS, V);   // Calcula a vizinhança da caixa
    j = PA[m1].POS;
    BA[j] = 5;                   // Define a caixa atual como preta
    j = V[0];
    BA[j] = 5;                   // Define a caixa vizinha como preta
    j = V[1];
    BA[j] = 5;                   // Define a caixa vizinha como preta
    j = V[2];
    BA[j] = 5;                   // Define a caixa vizinha como preta
    j = V[3]; 
    BA[j] = 5;                   // Define a caixa vizinha como preta
  }
  

  // Preenche as bordas da imagem com a cor preta
  for(i = 0; i < BORDAS; i++) {
    for(k = 0; k < LR; k++) {
      px =  k + (int) i*LR;
      BA[px] = 5;
    }
  }

  for(i = LR-1; i > LR-BORDAS-1; i--) {
    for(k = 0; k < LR; k++) {
      px =  k + (int) i*LR;
      BA[px] = 5;
    }
  }
  
  for(i = 0; i < BORDAS; i++) {
    for(k = 0; k < LR; k++) {
      px =  i + (int) k*LR;
      BA[px] = 5;
    }
  }
  
  for(i = LR-1; i > LR-BORDAS-1; i--) {
    for(k = 0; k < LR; k++) {
      px =  i + (int) k*LR;
      BA[px] = 5;
    }
  }

  k = 0;
  // Preenche o arquivo de saída com as cores correspondentes aos estados das caixas
  for(m1 = 0; m1 < LR2; m1++) {
    switch (BA[m1]) {
    case 0: fprintf(ar1,"%s ",branco); break;   // vazio
    case 1: fprintf(ar1,"%s ",verde); break;    // infectado
    case 2: fprintf(ar1,"%s ",vermelho); break; // nao infectado 
    case 3: fprintf(ar1,"%s ",vermelho); break; // nao infectado virgem
    case 4: fprintf(ar1,"%s ",verde); break;    // infectado virgem
    case 5: fprintf(ar1,"%s ",preto); break;    
    }
    k++;
    if(k == LR){ fprintf(ar1,"\n"); k = 0;}
    
  }
  fclose(ar1);
}
