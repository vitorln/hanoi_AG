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
} moviments;

moviments  **population;

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

int fitness(int num_disks, moviments *individuo_index, int num_mov)
{
    int size_towers[3];
    size_towers[0] = num_disks;
    size_towers[1] = 0;
    size_towers[2] = 0;
    int state[3][num_disks];
    for(int i  = num_disks - 1; i >= 0; i--)
    {
        state[0][i] = num_disks - i;
        state[1][i] = 0;
        state[2][i] = 0;
    }
    int fitness_value = 0;
    
    for(int i = 0; i < num_mov; i++)
    {
        int origin_tower = individuo_index[i].torre_origem;
        int destination_tower = individuo_index[i].torre_destino;
        
        if(size_towers[origin_tower] == 0)
        {
            fitness_value -= 10;
            continue;
        }
        int disk_getted = state[origin_tower][size_towers[origin_tower]-1];
        if(size_towers[destination_tower] != 0)
        {
            if(size_towers[destination_tower] > 0 && state[destination_tower][size_towers[destination_tower] - 1] < disk_getted)
            {
                fitness_value -= 10;
                continue;
            }
        }
        state[origin_tower][size_towers[origin_tower]-1] = 0;
        size_towers[origin_tower] -= 1;
        state[destination_tower][size_towers[destination_tower]] = disk_getted;
        size_towers[destination_tower] += 1;

        if(size_towers[2] == num_disks)
        {
            fitness_value += 10000;
                break;
        }   
    }
    int sum_pow_t2 = 0;
    for(int i = 0; i < size_towers[2]; i++)
    {
        sum_pow_t2 += pow(3, state[2][i]);
    }
    fitness_value += sum_pow_t2 * 15;
    fitness_value -= size_towers[0] * 15;
    fitness_value -= size_towers[1] * 15;

    return fitness_value;
}

int roleta(int population_size, int fitness[])
{
    int less_fitness = fitness[0], sum_fitness = 0;
    for(int i=0; i<population_size; i++)
    {     
        if(less_fitness > fitness[i])
        {
            less_fitness = fitness[i];
        }
    }
    for(int i=0; i<population_size; i++)
    {
        fitness[i] -= less_fitness;
        fitness[i] += 1;
        sum_fitness += fitness[i];
    } 
    
    int rand_fitness = rand() % sum_fitness;

    for(int i=0; i<population_size; i++)
    {
        rand_fitness -= fitness[i];
        if(rand_fitness <= 0)
        {
            return i;
        }  
    }
    return -1; 
}

void crossover(int population_size, int fitness[], int num_mov)
{
    moviments **new_generation;
    new_generation = new moviments*[population_size];
    for(int i = 0; i < population_size; i++)
    {
        new_generation[i] = new moviments[num_mov];
    }
    
    for(int i=0; i<population_size/2; i++)
    {
        int father1 = roleta(population_size, fitness);
        int father2 = roleta(population_size, fitness);
        int corte = rand() % num_mov;
        for(int j=0; j<num_mov; j++){
            if(j<=corte)
            {
                new_generation[i][j] = population[father1][j];
                new_generation[i*2][j] = population[father2][j];
            }
            else
            {
                new_generation[i*2][j] = population[father1][j];
                new_generation[i][j] = population[father2][j];
            }
        }
    }
    population = new_generation;
}

void mutate(int population_size, int num_mov, double rate_mutate)
{
    for(int i=0; i<population_size; i++)
    {
        double sorter = (double)rand() / RAND_MAX;
        if(sorter < rate_mutate)
        {
            int rand_cromossome = rand() % num_mov;

            int torre_destino;
            int torre_origem = rand() % 3;
            do
            {
                torre_destino = rand() % 3;
            }while(torre_origem == torre_destino);
        
            population[i][rand_cromossome].torre_origem = torre_origem;
            population[i][rand_cromossome].torre_destino = torre_destino;
        }
    }
}

moviments *findTheBest(int population_size, int fit[], moviments the_best[], int num_mov, int num_disks){
    int the_best_fitness = fitness(num_disks, the_best, num_mov);
    for(int i=0; i<population_size; i++)
    {
        if(fit[i] > the_best_fitness)
        {
            the_best_fitness = fit[i];
            the_best = population[i];
        }
    }
    return the_best;
}

void printTowers(moviments the_best[], int num_disks, int num_mov)
{
    int size_towers[3];
    size_towers[0] = num_disks;
    size_towers[1] = 0;
    size_towers[2] = 0;
    int state[3][num_disks];
    for(int i  = num_disks - 1; i >= 0; i--)
    {
        state[0][i] = num_disks - i;
        state[1][i] = 0;
        state[2][i] = 0;
    }
    for(int i = 0; i < num_mov; i++)
    {
        int origin_tower = the_best[i].torre_origem;
        int destination_tower = the_best[i].torre_destino;
        
        if(size_towers[origin_tower] == 0)
        {
            continue;
        }
        int disk_getted = state[origin_tower][size_towers[origin_tower]-1];
        if(size_towers[destination_tower] != 0)
        {
            if(size_towers[destination_tower] > 0 && state[destination_tower][size_towers[destination_tower] - 1] < disk_getted)
            {
                continue;
            }
        }
        state[origin_tower][size_towers[origin_tower]-1] = 0;
        size_towers[origin_tower] -= 1;
        state[destination_tower][size_towers[destination_tower]] = disk_getted;
        size_towers[destination_tower] += 1;

        if(size_towers[2] == num_disks)
        {
            break;
        }   
        
    }
    for(int i=0; i<3; i++){//towers
        cout<<"torre "<<i<<": |";
        for(int j=0; j<size_towers[i]; j++){//discs
            cout<<state[i][j]<<" ";
        }
        cout<<endl;
    }
}

int main()
{
    srand (time(NULL));
    int num_disks = 5;
    int population_size = 2000;
    int num_generations = 1000;
    int num_mov = (pow(2,num_disks) -1) * 3;
    int population_fitness[population_size];
    moviments *the_best;
    the_best = new moviments[num_mov];
    moviments *the_best_aux;
    the_best_aux = new moviments[num_mov];
    double mutation_rate = 0.2;
    int cont_generation = 0;


    population = new moviments*[population_size];
    for(int i = 0; i < population_size; i++)
    {
        population[i] = new moviments[num_mov];
    }
    generatePopulation(population_size, num_mov);

    the_best = population[0];
    while(num_generations > cont_generation)
    {
        for(int i = 0; i < population_size; i++)
        {
            population_fitness[i] = fitness(num_disks, population[i], num_mov);
        }
        the_best_aux = findTheBest(population_size, population_fitness, the_best, num_mov, num_disks);
        int fitness_best = fitness(num_disks, the_best, num_mov);
        int fitness_aux = fitness(num_disks, the_best_aux, num_mov);
        if(fitness_best == fitness_aux)
        {
            cont_generation++;
            
        }
        else 
        {
            cont_generation = 0;
        }
        the_best = the_best_aux;

        crossover(population_size, population_fitness, num_mov);
        mutate(population_size, num_mov, mutation_rate);
    }
    cout << "fitness: |" << fitness(num_disks, the_best, num_mov) << endl;
    printTowers(the_best, num_disks, num_mov);
    //printPopulation(population_size, num_mov);
}