/* avl - Árvore Binária de Busca com Balanceamento AVL
 *
 * by Joukim, Outubro de 2020 - Estrutura de Dados (GCC216)
 * Caracteristicas de implementação outubro de 2019:
 * -> tratamento de exceção
 * -> usa dado na forma chave/valor
 * -> uso de métodos recursivos
 * -> sem duplo encadeamento
 * -> invés de transplanta, usa removeMenor
 *
 * Changed by Perninha, Janeiro de 2021 - Aluno de Estrutura de Dados (GCC216)
 *  Todas caracteristicas de implementaÃ§Ã£o estao marcadas como "//Implementado"
 */

#include <iostream>
#include <stdexcept>

using namespace std;

struct dado {
    unsigned ano; // ano é a chave para ordenação na AVL
    string cidadeRealizacao;
    unsigned qtdadeFilmesApresentados;
};

ostream& operator<<(ostream& saida, const dado& e) {
    saida << "(" << e.ano << "," << e.cidadeRealizacao << "," << e.qtdadeFilmesApresentados << ")";
    return saida;
}

istream& operator>>(istream& entrada, dado& e) {
    entrada >> e.ano >> e.cidadeRealizacao >> e.qtdadeFilmesApresentados;
    return entrada;
}
typedef unsigned tipoChave;

class noh {
    friend class avl;
    private:
        dado elemento;
        noh* PtEsq;
        noh* PtDir;
        unsigned altura;
    public:
        noh(const dado& umDado):
            elemento(umDado),  PtEsq(NULL), PtDir(NULL), altura(1) { }
        ~noh() { }
        int fatorBalanceamento();
        int informarAltura(noh* umNoh);
        void atualizarAltura();
};

int noh::fatorBalanceamento() {
//Implementado
    return informarAltura(PtEsq) - informarAltura(PtDir);
}

int noh::informarAltura(noh* umNoh){
//Implementado
    if(umNoh == NULL){
        return 0;
    }
    else{
        return umNoh->altura;
    }
}

void noh::atualizarAltura(){
//Implementado
	int altArvEsq = 0, altArvDir = 0;
    altArvEsq = informarAltura(PtEsq);
	altArvDir = informarAltura(PtDir);
	this->altura = 1 + max(altArvEsq, altArvDir);
}

class avl {
    friend ostream& operator<<(ostream& output, avl& arvore);
    private:
        noh* raiz;
        // percorrimento em ordem da árvore
        void percorreEmOrdemAux(noh* atual, int nivel);
        int Nivel(int chave);

        // funções auxiliares para remoção
        noh* encontraMenor(noh* raizSub);
        noh* removeMenor(noh* raizSub);

        // funções auxiliares para inserção e remoção usando método recursivo
        // retorna o nó para ajustar o pai ou o raiz
        noh* insereAux(noh* umNoh, const dado& umDado);
        noh* removeAux(noh* umNoh, tipoChave chave);

        // métodos para manutenção do balanceamento
        noh* rotacaoEsquerda(noh* umNoh);
        noh* rotacaoDireita(noh* umNoh);
        noh* rotacaoEsqDir(noh* umNoh);
        noh* rotacaoDirEsq(noh* umNoh);
        noh* arrumaBalanceamento(noh* umNoh);

        // busca, método iterativo
        noh* buscaAux(tipoChave chave);

        // função auxiliar do destrutor, usa percorrimento pós-ordem
        void destruirRecursivamente(noh* umNoh);

        void imprimirDir(const std::string& prefixo, const noh* meuNoh);
        void imprimirEsq(const std::string& prefixo, const noh* meuNoh, bool temIrmao);

        // função auxiliar do levantamento, usa percorrimento pré-ordem
        void levantamentoAux(noh* umNoh, unsigned* ptrAnoProcurado, unsigned* ptrMaiorNumFilmesApres);
    public:
        avl() { raiz = NULL; }
        ~avl();
        void imprimir();
        // inserção e remoção são recursivos
        void insere(const dado& umDado);
        void remove(tipoChave chave);
        // inserção e remoção, métodos recursivos

        // busca retorna uma cópia do objeto armazenado
        dado busca(tipoChave chave);

        // efetua levantamento do ano com maior número de filmes apresentados
        unsigned levantamento();
        // função para depuração
        void percorreEmOrdem() { percorreEmOrdemAux(raiz, raiz->altura); }
};

// destrutor
avl::~avl() {
    destruirRecursivamente(raiz);
}

// destrutor é recursivo, fazendo percorrimento pós-ordem
void avl::destruirRecursivamente(noh* umNoh) {
//Implementado
    if(umNoh != NULL){
        destruirRecursivamente(umNoh->PtEsq);
        destruirRecursivamente(umNoh->PtDir);
        delete umNoh;
    }
}

void avl::insere(const dado& umDado) {
    raiz = insereAux(raiz, umDado);
}

// inserção recursiva, devolve nó para atribuição de pai ou raiz
noh* avl::insereAux(noh* umNoh, const dado& umDado) {
//Implementado
    if(umNoh == NULL){
        noh* NovoNoh = new noh(umDado);
        return NovoNoh;
    }
    else{
        if(umDado.ano < umNoh->elemento.ano){
            umNoh->PtEsq = insereAux(umNoh->PtEsq, umDado);
        }
        else{
            umNoh->PtDir = insereAux(umNoh->PtDir, umDado);
        }
    }
    return arrumaBalanceamento(umNoh);
}

// checa e arruma, se necessário, o balanceamento em umNoh,
// fazendo as rotações e ajustes necessários
noh* avl::arrumaBalanceamento(noh* umNoh) {
//Implementado
    if(umNoh == NULL){
        return umNoh;
    }
    umNoh->atualizarAltura();
    int fatorBal = umNoh->fatorBalanceamento();
    if((fatorBal >= -1) and (fatorBal <= 1)){
        return umNoh;
    }
    if((fatorBal > 1) and (umNoh->PtEsq->fatorBalanceamento() >= 0)){
        return rotacaoDireita(umNoh);
    }
    if((fatorBal > 1) and (umNoh->PtEsq->fatorBalanceamento() < 0)){
        return rotacaoEsqDir(umNoh);
    }
    if((fatorBal < -1) and (umNoh->PtDir->fatorBalanceamento() <= 0)){
        return rotacaoEsquerda(umNoh);
    }
    if((fatorBal < -1) and (umNoh->PtDir->fatorBalanceamento() > 0)){
        return rotacaoDirEsq(umNoh);
    }
    return umNoh;
}


// rotação à esquerda na subárvore com raiz em umNoh
// retorna o novo pai da subárvore
noh* avl::rotacaoEsquerda(noh* umNoh) {
//Implementado
    noh* aux =  umNoh->PtDir;
    umNoh->PtDir = aux->PtEsq;
    aux->PtEsq = umNoh;
    umNoh->atualizarAltura();
    aux->atualizarAltura();
    return aux;
}


// rotação à direita na subárvore com raiz em umNoh
// retorna o novo pai da subárvore
noh* avl::rotacaoDireita(noh* umNoh) {
//Implementado
    noh* aux = umNoh->PtEsq;
    umNoh->PtEsq = aux->PtDir;
    aux->PtDir = umNoh;
    umNoh->atualizarAltura();
    aux->atualizarAltura();
    return aux;
}

noh* avl::rotacaoEsqDir(noh* umNoh){
//Implementado
    umNoh->PtEsq = rotacaoEsquerda(umNoh->PtEsq);
    return rotacaoDireita(umNoh);
}

noh* avl::rotacaoDirEsq(noh* umNoh){
//Implementado
    umNoh->PtDir = rotacaoDireita(umNoh->PtDir);
    return rotacaoEsquerda(umNoh);
}


// método de busca auxiliar (retorna o nó), iterativo
noh* avl::buscaAux(tipoChave chave) {
//Implementado
    noh* atual = raiz;
    while(atual != NULL){
        if(atual->elemento.ano == chave){
            return atual;
        }
        else if(atual->elemento.ano < chave){
            atual = atual->PtDir;
        }
        else{
            atual = atual->PtEsq;
        }
    }
    return atual;
}

// busca elemento com uma dada chave na árvore e retorna o registro completo
dado avl::busca(tipoChave chave) {
    noh* resultado = buscaAux(chave);
    if (resultado != NULL)
        return resultado->elemento;
    else
        throw runtime_error("Erro na busca: elemento não encontrado!");
}


// nó mínimo (sucessor) de subárvore com raiz em raizSub (folha mais à esquerda)
noh* avl::encontraMenor(noh* raizSub) {
//Implementado
    while(raizSub->PtEsq != NULL){
        raizSub = raizSub->PtEsq;
    }
    return raizSub;
}

// procedimento auxiliar para remover o sucessor substituíndo-o pelo
// seu filho à direita
noh* avl::removeMenor(noh* raizSub) {
//Implementado
    if(raizSub->PtEsq == NULL){
        return raizSub->PtDir;
    }
    else{
        raizSub->PtEsq = removeMenor(raizSub->PtEsq);
        return arrumaBalanceamento(raizSub);
    }
}

// remoção recursiva
void avl::remove(tipoChave chave) {
    raiz = removeAux(raiz, chave);
}

noh* avl::removeAux(noh* umNoh, tipoChave chave) {
//Implementado
    if(umNoh == NULL){
        throw runtime_error("ERRO");
    }
    noh* NovaRaiz = umNoh;
    if(chave < umNoh->elemento.ano){
        umNoh->PtEsq = removeAux(umNoh->PtEsq, chave);
    }
    else if(chave > umNoh->elemento.ano){
        umNoh->PtDir = removeAux(umNoh->PtDir, chave);
    }
    else{
        if(umNoh->PtEsq == NULL){
            NovaRaiz = umNoh->PtDir;
        }
        else if(umNoh->PtDir == NULL){
            NovaRaiz = umNoh->PtEsq;
        }
        else{
            NovaRaiz = encontraMenor(umNoh->PtDir);
            NovaRaiz->PtDir = removeMenor(umNoh->PtDir);
            NovaRaiz->PtEsq = umNoh->PtEsq;
        }
        delete umNoh;
    }
    return arrumaBalanceamento(NovaRaiz);
}

// encontra o nivel do noh
int avl::Nivel(int chave){
//Implementado
    noh* atual = raiz;
    int cont = 0;
    while(atual->elemento.ano != chave){
        if(atual->elemento.ano < chave){
            atual = atual->PtDir;
        }
        else{
            atual = atual->PtEsq;
        }
        cont++;
    }
    return cont;
}

// utiliza o nó atual e seu nível na árvore (para facilitar visualização)
void avl::percorreEmOrdemAux(noh* atual, int nivel) {
//Implementado
    if(atual != NULL){
        percorreEmOrdemAux(atual->PtEsq, atual->altura);
        cout << atual->elemento.ano << "/" << Nivel(atual->elemento.ano) << " ";
        percorreEmOrdemAux(atual->PtDir, atual->altura);
    }
}

ostream& operator<<(ostream& output, avl& arvore) {
    arvore.imprimir();
    return output;
}

// imprime formatado seguindo o padrao tree as subarvores direitas de uma avl
void avl::imprimirDir(const std::string& prefixo, const noh* meuNoh)
{
    if( meuNoh != nullptr )
    {
        std::cout << prefixo
                  << "└d─"
                  << "(" << meuNoh->elemento.ano << "," << meuNoh->elemento.cidadeRealizacao << ")"
                  << std::endl;

        // Repassa o prefixo para manter o historico de como deve ser a formatacao e chama no filho direito e esquerdo
        imprimirEsq( prefixo + "    " , meuNoh->PtEsq , meuNoh->PtDir==nullptr );
        imprimirDir( prefixo + "    " , meuNoh->PtDir );
    }
}

// imprime formatado seguindo o padrao tree as subarvores direitas de uma avl
void avl::imprimirEsq(const std::string& prefixo, const noh* meuNoh, bool temIrmao)
{
    if( meuNoh != nullptr )
    {
        std::cout << prefixo ;

        // A impressao da arvore esquerda depende da indicacao se existe o irmao a direita
        if (temIrmao)
            std::cout << "└e─" ;
        else
            std::cout << "├e─";

        std::cout << "(" << meuNoh->elemento.ano << "," << meuNoh->elemento.cidadeRealizacao << ")"
                  << std::endl;

        // Repassa o prefixo para manter o historico de como deve ser a formatacao e chama no filho direito e esquerdo
        imprimirEsq( prefixo + "│   " , meuNoh->PtEsq, meuNoh->PtDir==nullptr );
        imprimirDir( prefixo + "│   " , meuNoh->PtDir );
    }
}

void avl::levantamentoAux(noh* umNoh, unsigned* ptrAnoProcurado, unsigned* ptrMaiorNumFilmesApres){
//Implementado
    if(umNoh != NULL){
        levantamentoAux(umNoh->PtEsq, ptrAnoProcurado, ptrMaiorNumFilmesApres);
        if( *ptrMaiorNumFilmesApres < umNoh->elemento.qtdadeFilmesApresentados){
            *ptrMaiorNumFilmesApres = umNoh->elemento.qtdadeFilmesApresentados;
            *ptrAnoProcurado = umNoh->elemento.ano;
        }
        levantamentoAux(umNoh->PtDir, ptrAnoProcurado, ptrMaiorNumFilmesApres);
    }
}

unsigned avl::levantamento(){
//Implementado
    unsigned ano = 0, num = 0;
    levantamentoAux(raiz, &ano, &num);
    return ano;
}


// imprime formatado seguindo o padrao tree uma avl
void avl::imprimir()
{
    if( this->raiz != nullptr )
    {
        std::cout << "(" << this->raiz->elemento.ano << "," << this->raiz->elemento.cidadeRealizacao << ")" << std::endl;
        // apos imprimir a raiz, chama os respectivos metodos de impressao nas subarvore esquerda e direita
        // a chamada para a impressao da subarvore esquerda depende da existencia da subarvore direita
        imprimirEsq( " " , this->raiz->PtEsq, this->raiz->PtDir==nullptr );
        imprimirDir( " " , this->raiz->PtDir );
    } else
        std::cout << "*arvore vazia*" << std::endl;
}

int main() {
    avl arvore;
    tipoChave chave;
    dado umDado;
    unsigned ano;

    char operacao;

    do {
        try {
            cin >> operacao;
            switch (operacao) {
                case 'i': // Inserir recursivamente
                    // objeto recebe nome, cidade e quantidade de filmes apresentados
                    cin >> umDado;
                    arvore.insere(umDado);
                    break;
                case 'r': // Remover recursivamente
                    cin >> chave;
                    arvore.remove(chave);
                    break;
                case 'b': // Buscar
                    cin >> chave; // ler a chave
                    umDado = arvore.busca(chave); // escrever os dados do festival
                    cout << "Elemento buscado: " << umDado << endl;
                    break;
                case 'l': // Levantamento pelo ano com maior número de filmes apresentados
                    ano = arvore.levantamento();
                    cout << "Edição com maior quantidade de filmes apresentados: " << ano << endl;
                    break;
                case 'e': // Escrever tudo (em ordem)
                    cout << arvore;
                    break;
                case 'f': // Finalizar execução
                    break;
                default:
                    cout << "Comando invalido!\n";
            }
        } catch (runtime_error& e) {
            cout << e.what() << endl;
        }
    } while (operacao != 'f');

    return 0;
}

// by Little D. Legs 02/01/2021
