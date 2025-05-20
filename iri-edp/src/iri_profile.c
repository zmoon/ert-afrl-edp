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
    printf("  -lat <value>      Latitude in degrees North (default: 37.8)\n");
    printf("  -lon <value>      Longitude in degrees East (default: -75.4)\n");
    printf("  -year <value>     Year (default: 2021)\n");
    printf("  -month <value>    Month (1-12) (default: 3)\n");
    printf("  -day <value>      Day of month (default: 3)\n");
    printf("  -hour <value>     Local hour (0-24) (default: 11.0)\n");
    printf("  -hstart <value>   Start height in km (default: 65)\n");
    printf("  -hend <value>     End height in km (default: 600)\n");
    printf("  -hstep <value>    Height step in km (default: 5)\n");
    printf("  -o <filename>     Output file (default: stdout)\n");
    printf("  -param <type>     Parameter to calculate (default: 1)\n");
    printf("                    1 = Electron density (m^-3)\n");
    printf("                    2 = Electron temperature (K)\n");
    printf("                    3 = Ion temperature (K)\n");
    printf("                    4 = Neutral temperature (K)\n");
    printf("  -geom             Use geomagnetic coordinates (default: geographic)\n");
    printf("  -h                Show this help message\n");
}

int main(int argc, char* argv[]) {
    /* Default parameters */
    double latitude = 37.8;
    double longitude = -75.4;
    int year = 2021;
    int month = 3;
    int day = 3;
    double hour = 11.0;
    double h_start = 65.0;
    double h_end = 600.0;
    double h_step = 5.0;
    char* output_file = NULL;
    int jmag = 0;  /* 0 = geographic, 1 = geomagnetic */
    
    /* Default parameter: electron density */
    iri_param_t param_type = IRI_PARAM_ELECTRON_DENSITY;
    
    /* Parse command line arguments */
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-lat") == 0 && i+1 < argc) {
            latitude = atof(argv[++i]);
        } else if (strcmp(argv[i], "-lon") == 0 && i+1 < argc) {
            longitude = atof(argv[++i]);
        } else if (strcmp(argv[i], "-year") == 0 && i+1 < argc) {
            year = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-month") == 0 && i+1 < argc) {
            month = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-day") == 0 && i+1 < argc) {
            day = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-hour") == 0 && i+1 < argc) {
            hour = atof(argv[++i]);
        } else if (strcmp(argv[i], "-hstart") == 0 && i+1 < argc) {
            h_start = atof(argv[++i]);
        } else if (strcmp(argv[i], "-hend") == 0 && i+1 < argc) {
            h_end = atof(argv[++i]);
        } else if (strcmp(argv[i], "-hstep") == 0 && i+1 < argc) {
            h_step = atof(argv[++i]);
        } else if (strcmp(argv[i], "-o") == 0 && i+1 < argc) {
            output_file = argv[++i];
        } else if (strcmp(argv[i], "-param") == 0 && i+1 < argc) {
            int param_val = atoi(argv[++i]);
            if (param_val >= 1 && param_val <= NUM_OUTF) {
                param_type = (iri_param_t)param_val;
            } else {
                fprintf(stderr, "Error: Invalid parameter type %d (must be 1-4)\n", param_val);
                return 1;
            }
        } else if (strcmp(argv[i], "-geom") == 0) {
            jmag = 1;
        } else if (strcmp(argv[i], "-h") == 0) {
            print_usage(argv[0]);
            return 0;
        } else {
            printf("Unknown option: %s\n", argv[i]);
            print_usage(argv[0]);
            return 1;
        }
    }

    /* Calculate MMDD parameter for IRI */
    int mmdd = month * 100 + day;

    /* Initialize the IRI model */
    if (iri_init() != 0) {
        fprintf(stderr, "Failed to initialize IRI model\n");
        return 1;
    }

    /* Calculate height array */
    double heights[MAX_HEIGHT];
    int num_heights = iri_heights(h_start, h_end, h_step, heights);

    /* Array to hold parameter values */
    double values[MAX_HEIGHT];
    double oarr[NUM_OARR];

    /* Run the IRI model */
    if (iri_profile(
        jmag, latitude, longitude, year, mmdd, hour,
        h_start, h_end, h_step, param_type, values, oarr) != 0) {
        fprintf(stderr, "IRI model calculation failed\n");
        return 1;
    }

    /* Write results to CSV */
    if (iri_write_csv(output_file, heights, values, num_heights, param_type) != 0) {
        fprintf(stderr, "Failed to write CSV output\n");
        return 1;
    }

    return 0;
}
