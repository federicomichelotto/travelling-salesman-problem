#include "../include/utils.h"

int main(int argc, char **argv)
{

    time_t start, end;
    instance inst; // current instance of the problem

    initialize_instance(&inst);
    parse_command_line(argc, argv, &inst);

    parse_instance(&inst);
    print_instance(&inst);

    start = time(NULL);

    TSPopt(&inst) ? print_error("TSPopt() ") : print_message("All went good inside TSPopt()");

    end = time(NULL);

    if (verbose >= QUIET)
        printf("... TSP solved in %ld sec\n", end - start);

    plot_solution(&inst) ? print_error("plot_solution() error") : printf("... gnuplot ok\n");
    //plot_solution_edges(inst.n_edges, inst.nodes, inst.edges) ? print_error("plot_solution_edges() error") : printf("... gnuplot ok\n");

    // Free the memory used by the instance
    free_instance(&inst);

    return 0;
}
