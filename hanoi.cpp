#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <vector>
#include <cmath>

using namespace std;

typedef struct 
{
    int torre_origem;
    int torre_destino;
} movimento;

movimento  **population;
void generatePopulation(int population_size, int num_mov)
{
    for(int i = 0; i < population_size; i++)
    {
        int j = 0;
        while (j < num_mov)
        {
            int torre_origem = rand() % 3;

            if(j == 0)
            {
                torre_origem = 0;
            }
            
            int torre_destino = rand() % 3;

            if(torre_origem != torre_destino)
            {
                population[i][j].torre_origem = torre_origem;
                population[i][j].torre_destino = torre_destino;
                j++;
            }
        }
        
    }
}

void printPopulation(int population_size, int num_mov)
{
    for(int i = 0; i < population_size; i++)
    {
        cout << "individuo " << i << endl << endl;
        for(int j = 0; j < num_mov; j++)
        {
            cout << "de " << population[i][j].torre_origem << " para " << population[i][j].torre_destino << endl;
        }
        cout << endl << endl << endl;
    }
}

int fitness(int num_disks, int individuo_index, int num_mov)
{
    int size_tower_0 = num_disks;
    int size_tower_1 = 0;
    int size_tower_2 = 0;
    int state[3][num_disks];
    for(int i  = num_disks; i > 0; i--)
    {
        state[0][i-1] = i;
    }
    int fitness_value = 0;
    
    for(int i = 0; i < num_mov; i++)
    {
        int origin_tower = population[individuo_index][0].torre_origem;
        int destination_tower = population[individuo_index][1].torre_destino;

    }
    
    return 0;
}
int main()
{
    srand (time(NULL));
    int num_disks = 3;
    int population_size = 10;
    int num_generations = 50;
    int num_mov = (pow(2,num_disks) -1) + 50;
    int population_fitness[population_size];

    population = new movimento*[population_size];
    for(int i = 0; i < population_size; i++)
    {
        population[i] = new movimento[num_mov];
    }
    generatePopulation(population_size, num_mov);
    for(int i = 0; i < population_size; i++)
    {
        population_fitness[i] = fitness(num_disks, i, num_mov);
    }
    
    //printPopulation(population_size, num_mov);
}