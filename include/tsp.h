#ifndef TSP_H
#define TSP_H

#include <cplex.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// Data Structures
typedef struct {
    char input_file[1000];  // Path of the file
    char name[100];         // Identifies the data file
    char type[10];          // Specifies the type of data (TSP or ATSP)
    char comment[1000];     // Comment of the problem
    char weight_type[10];   // Specifies how the edge weights (or distances) are given
    char weight_format[20]; // Specifies how the edge weights (or distances) are formatted
    char data_type[20];     // Specifies how the data are displayed
    int seed;               // Seed given to cplex
    int run;               // Number of current run
} parameter;

typedef struct { // Node

    int id;   // number of the node (e.g. 1, 2, 3, ..., n)
    double x; // x coordinate
    double y; // y coordinate

} node;

typedef struct { // Edge in the circuit

    //    _Bool flag;			        // = TRUE (1) if inside the circuit, = FALSE (0) otherwise
    double dist; // Weight of the edge
    int prev;    // Starting node
    int next;    // Ending node

} edge;

typedef struct {

    // Input data
    int dimension; // Number of nodes of the problem
    node *nodes;   // List of nodes
    edge *edges;   // Refined list of edges (u,v) returned by CPLEX
    int n_edges;   // Total number of edges inside the best solution

    double *weights;     // weights
    _Bool integer_costs; // = TRUE (1) for integer costs (rounded distances), = FALSE (0) otherwise

    parameter param; // Parameters of the instance

    double time_limit; // Specifies the maximum time allowed within the execution
    int model_type;    // Specifies the type of the model
    double z_best;     // Value of the best solution available
    double best_lb;

} instance;

// Enumerations
enum verbose_level {
    QUIET = 0,
    NORMAL = 1,
    VERBOSE = 2,
    NERD = 3,
    DEBUG = 4
};

enum sections {
    PARAMETERS = 0,
    NODE_COORD = 1,
    EDGE_WEIGHT = 2,
    DISPLAY_DATA = 3
};

static const char *verbose_name[] = {"QUIET", "NORMAL", "VERBOSE", "NERD", "DEBUG"};
static const char *model_name[] = {"STD", "MTZ", "MTZMOD", "MTZL", "MTZLS", "GG", "LOOP"};
static int verbose = NORMAL;

// TSP solver
int TSPopt(instance *inst);

// Exact model builder
void build_model(CPXENVptr env, CPXLPptr lp, instance *inst);

// Exact model prototypes

// Basic model (undirected graphs)
// model 0: basic model (no SEC)
void basic_model_no_sec(CPXENVptr env, CPXLPptr lp, instance *inst);

// model 6: benders model (SEC)
void bender(CPXENVptr env, CPXLPptr lp, instance *inst);

// Compact model (directed graphs)
// model 1: TMZ_static
void TMZ_static(CPXENVptr env, CPXLPptr lp, instance *inst);

// model 2: TMZ_static_mod
//void TMZ_static_mod(CPXENVptr env, CPXLPptr lp, instance *inst);

// model 3: TMZ_lazy
void TMZ_lazy(CPXENVptr env, CPXLPptr lp, instance *inst);

// model 4: MTZ_lazy_sec
void TMZ_lazy_sec(CPXENVptr env, CPXLPptr lp, instance *inst);

// model 5: GG
void GG(CPXENVptr env, CPXLPptr lp, instance *inst);


double dist(int i, int j, instance *inst);

int xpos(int i, int j, instance *inst);     // position in the model for undirected graphs
int xpos_dir(int i, int j, instance *inst); // position in the model for directed graphs
int upos(int i, instance *inst);            // position in the model of i-th u-variable
int ypos(int i, int j, instance *inst);     // position in the model of y-variable for the arc (i,j)

void print_time_csv();

#endif //TSP_H
