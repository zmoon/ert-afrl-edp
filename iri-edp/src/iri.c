/**
 * @file
 * @brief Program to generate a vertical profile from the IRI model
 * 
 * This program calls the IRI model to generate a vertical profile of
 * electron density or temperature and outputs the results in CSV format.
 */

#include "iri_interface.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_usage(const char* progname) {
    printf("Usage: %s [options]\n", progname);
    printf("Options:\n");
    printf("  -c|--case <case>     Case number (default: 1)\n");
    printf("                       - 1: 2021-03-03 11:00 UTC\n");
    printf("                       - 2: 2021-03-04 23:00 UTC\n");
    printf("  -o|--out <filename>  Output file (default: stdout)\n");
    printf("  -h|--help            Show this help message\n");
}

int main(int argc, char* argv[]) {
    /* Default parameters */
    double latitude = 37.8;
    double longitude = -75.4;
    int year = 2021;
    int month = 3;
    int day = 3;
    double hour = 11.0 + 25.0;
    double height_start = 80.0;
    double height_end = 600.0;
    double height_step = 10.0;
    char* output_file = NULL;
    int case_num = 1;

    /* Parse command line arguments */
    for (int i = 1; i < argc; i++) {
        if (((strcmp(argv[i], "-c") == 0) || (strcmp(argv[i], "--case") == 0)) && i+1 < argc) {
            case_num = atof(argv[++i]);
        } else if (((strcmp(argv[i], "-o") == 0) || (strcmp(argv[i], "--out") == 0)) && i+1 < argc) {
            output_file = argv[++i];
        } else if ((strcmp(argv[i], "-h") == 0) || (strcmp(argv[i], "--help") == 0)) {
            print_usage(argv[0]);
            return 0;
        } else {
            printf("Unknown option: %s\n", argv[i]);
            print_usage(argv[0]);
            return 1;
        }
    }

    /* Set parameters based on case number */
    if (case_num == 1) {
        /* Already set */
    } else if (case_num == 2) {
        day = 4;
        hour = 23.0 + 25.0;
    } else {
        fprintf(stderr, "Error: Invalid case number %d (must be 1 or 2)\n", case_num);
        return 1;
    }

    /* Initialize the IRI model */
    if (iri_init() != 0) {
        fprintf(stderr, "Failed to initialize IRI model\n");
        return 1;
    }

    /* Vertical profile outputs */
    double profiles[NUM_PROFILE][MAX_HEIGHT];

    /* Run the IRI model */
    if (iri_profiles(
        latitude, longitude,
        year, month, day, hour,
        height_start, height_end, height_step,
        profiles) != 0) {
        fprintf(stderr, "IRI model calculation failed\n");
        return 1;
    }

    /* Write results to CSV */
    if (iri_write_csv(output_file, profiles) != 0) {
        fprintf(stderr, "Failed to write CSV output\n");
        return 1;
    }

    return 0;
}
