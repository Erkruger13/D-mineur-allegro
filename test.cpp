#include <iostream>
#include <string>
#include <ctime>
#include <vector>  

struct Point {
    short int x;
    short int y;
    Point() : x(0), y(0) {}  
    Point(short int xi, short int yi) {         // Constructeur trivial avec valeurs par défaut
        x = xi;
        y = yi;
    }
};

int facto(int n){
    if(n == 0){return 1;}
    return n * facto(n-1);
}


int main(){
    int a = 20;
    printf("Yann à %d ans aujourd'hui \n", a);
    std::cout<< "factoriel de 4 = " << facto(4) <<std::endl;
}
// crée une fonction récursive pour le premier clic 
// cette fonction prend en paramètre des coordonée et 
// générer des nombres entre 0 et 99 puis faire des opérations de sorte a séparer le chiffre des dizaines et des unité