#include <chrono>
#include <random>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEAN_VALUE 80.0
#define STANDARD_DEVIATION 15.0
#define LIFT_MAX_LOAD 500

#define NUM_TRIALS 10000000

double generate_random_weight();

int main(int argc, char *argv[])
{
    int total;
    int overweight;
    int max_people_in_lift;
    double this_weight;
    double weight;
    FILE *output_file;

    total = 0;
    overweight = 0;

    if (argc == 3) {
        max_people_in_lift = strtol(argv[1], NULL, 10);
    } else {
        fputs("usage: generate <max people in lift> [output filename]\n", stderr);
        return 1;
    }
    output_file = fopen(argv[2], "w");
    if(!output_file) {
        fputs("failed to open output file\n", stderr);
        return 1;
    }

    /* print table headers. */
    for(int i = 0;i < max_people_in_lift;i++) {
        fprintf(output_file, "guy%d\t", i + 1);
    }
    fputs("total\toverweight?\n", output_file);

    for(int i = 0;i < NUM_TRIALS;i++) {
        weight = 0.0;
        for(int j = 0;j < max_people_in_lift;j++) {
            this_weight = generate_random_weight();
            weight += this_weight;
            fprintf(output_file, "%.2f\t", this_weight);
        }
        total++;
        fprintf(output_file, "%.2f\t", weight);
        if(weight > LIFT_MAX_LOAD) {
            fprintf(output_file, "Y\n");
            overweight++;
        } else {
            fprintf(output_file, "N\n");
        }
    }
    printf("%d out of %d trials were overweight (%.2lf%%)\n", overweight, total, (double)overweight / (double)total * 100.0);

    fclose(output_file);
    return 0;
}

double generate_random_weight()
{
    std::default_random_engine generator;
    generator.seed(std::chrono::system_clock::now().time_since_epoch().count());
    std::normal_distribution<double> distribution(MEAN_VALUE, STANDARD_DEVIATION);
    return distribution(generator);
}
