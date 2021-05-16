/**
 * Ficheiro: Projeto1.cpp
 * Realizado por: David Belchior (95550) e Diogo Santos (95562)
 * Instituto Superior Tecnico - LEIC-A - Analise e Sintese de Algoritmos
 * Para compilar: g++ -std=c++11 -O3 -Wall Projeto2.cpp -lm
*/

#include <iostream>
#include <vector>
#include <queue>
#include <stack>

using namespace std;

/*BFS CONST*/
#define WHITE -1
#define GREY 0
#define BLACK 1
#define INF 10000

// Each edge has a flux and an id (position in original graph)
typedef struct{
    int flux;
    int id;
} edge;

// Each vertex has an array of edges and necessary params for BFS
typedef struct{
    vector<edge> edges;
    int parent;
    int parentEdge;
    int colour;
} vertex;

//Residual Graph
vector<vertex> residual;

void printGraph(vector<vertex> g);
void printStack(stack<int> s);

void readInput(){
    /**
     * Reads input from stdin and initialises the correspondent graph
    */
    int n, k;
    if (scanf("%d %d", &n, &k) < 0){
        cout << "Input invalido. Tente de novo!" << endl;
        exit(EXIT_FAILURE);
    }
    if (n < 2 || k < 0){
        cout << "Input invalido. Tente de novo!" << endl;
        exit(EXIT_FAILURE);
    }

    residual = vector<vertex>(n+2);

    int x, y;
    for (int i = 1; i <= n; ++i){
        if (scanf("%d %d", &x, &y) < 0){
            cout << "Vertice invalido. Tente de novo!" << endl;
            exit(EXIT_FAILURE);
        }

        edge rX1, rX2, rY1, rY2;

        rX1.flux = x;   rX2.flux = x;
        rX1.id = 0;     rX2.id = i;
        
        rY1.flux = y;   rY2.flux = y;
        rY1.id = n+1;   rY2.id = i;

        residual[i].edges.push_back(rX1);   //Aresta do processo i para processador X
        residual[0].edges.push_back(rX2);   //Aresta do processador X para processo i
        residual[i].edges.push_back(rY1);   //Aresta do processo i para processador Y
        residual[n+1].edges.push_back(rY2); //Aresta do processador Y para processado i
    }
    
    int u, v, c;
    for (int i = 0; i < k; ++i){
        if (scanf("%d %d %d", &u, &v, &c) < 0){
            cout << "Vertice invalido. Tente de novo!" << endl;
            exit(EXIT_FAILURE);
        }
        if (u > n || v > n){
            cout << "Vertice invalido. Tente de novo!" << endl;
            exit(EXIT_FAILURE);
        }

        edge r1, r2;

        r1.flux = c; r2.flux = c;
        r1.id = v;   r2.id = u;

        residual[u].edges.push_back(r1);  //Aresta do processo u para o processo v
        residual[v].edges.push_back(r2);  //Aresta do processo v para o processo u
    }
}

int findIndex(vector<edge> v, int x){
    /**
     * Finds the index of x in the vector v
     */
    for (unsigned int i = 0; i < v.size(); ++i){
        if (v[i].id == x){
            return i;
        }
    }
    return -1;
}

void BFS(int s){
    /**
     * Breadth-First Search
     */
    for(unsigned int i = 0; i < residual.size(); ++i){
        residual[i].parent = -1;
        residual[i].parentEdge = -1;
        residual[i].colour = WHITE;
    }
    residual[s].colour = GREY;

    queue<int> Q;
    Q.push(s);
    
    while(!Q.empty()){
        int w = Q.front();    
        Q.pop();
        for(unsigned int i = 0; i < residual[w].edges.size(); ++i){
            int id = residual[w].edges[i].id;
            if(residual[w].edges[i].flux > 0 && residual[id].colour == WHITE){
                Q.push(id);
                residual[id].colour = GREY;
                residual[id].parent = w;
                residual[id].parentEdge = i;
            }
        }
        residual[w].colour = BLACK;
    }
}

pair<stack<int>,int> findAugPath(int s){
    /**
     * Finds shortest path from X to Y
     */
    BFS(s);

    stack<int> path = stack<int>();
    int child = residual.size()-1;
    int parent = residual[child].parent;

    int min = INF;
    while (parent >= 0){
        if (min > residual[parent].edges[]){
            min = fluxes[child][parent];
        }
        path.push(child);
        child = parent;
        parent = residual[child].parent;
    }
    path.push(0);
    return make_pair(path,min);
}

int EdmondsKarp(){

    pair<stack<int>,int> aug = findAugPath(0);
    stack<int> path;
    int min, cur, next, cur_index, next_index,total=0;
    while((path = aug.first).size() > 1){
        min = aug.second;
        total += min;
        
        while(path.size() > 1){
            cur = path.top();
            path.pop();
            next = path.top();
            next_index = residual[next].parentEdge;
            cur_index = findIndex(residual[next].edges, cur);
            if (cur_index >= 0 && next_index >= 0){
                residual[cur].edges[next_index].flux -= min;
                residual[next].edges[cur_index].flux -= min;
            }
        }
        aug = findAugPath(0);
    }
    return total;
}

/*************************************************************************************************************************************/
/*                                                             MAIN                                                                  */
/*************************************************************************************************************************************/

int main(){
    readInput();
    //printGraph(graph);
    //printGraph(residual);
    cout << EdmondsKarp() << endl;
    exit(EXIT_SUCCESS);  
}

/*************************************************************************************************************************************/
/*                                                             PRINTS                                                                */
/*************************************************************************************************************************************/
void printGraph(vector<vertex> g){
    /**
     * Prints graph
    */
    cout << "Graph:" << endl;
    for(unsigned int i = 0; i < g.size(); ++i){
        cout << i << " _> ";
        for(unsigned int j = 0; j < g[i].edges.size(); ++j){
            cout << "(" << g[i].edges[j].id << ", " << g[i].edges[j].flux << ") ";
        }
        cout << endl;
    }
}

void printStack(stack<int> st){
    /**
     * Prints stack
     */
    stack<int> s = st;
    while(!s.empty()){
        cout << s.top() << ",";
        s.pop();
    }
    cout << endl;
}