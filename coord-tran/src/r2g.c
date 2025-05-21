/**
 * r2g - Radar to Geodetic Coordinate Transformation Tool
 * 
 * Calculates final coordinates given initial point, range, and bearing
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib.h"

void print_description() {
    printf("Convert radar range and bearing to final geodetic coordinates.\n\n");
}

void print_usage() {
    printf("Usage: r2g <lon1> <lat1> <range> <bearing>\n");
    printf("  Coordinates are in decimal degrees, range in kilometers, bearing in degrees.\n\n");
    printf("Options:\n");
    printf("  -h, --help  Display this help message\n\n");
}

int main(int argc, char *argv[]) {
    if (argc == 2) {
        if ((strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)) {
            print_description();
            print_usage();
            return 0;
        } else {
            fprintf(stderr, "Error: Invalid argument '%s'.\n", argv[1]);
            print_usage();
            return 1;
        }
    }
    if (argc != 5) {
        fprintf(stderr, "Error: Incorrect number of arguments.\n");
        print_usage();
        return 1;
    }
    
    // Parse command line arguments
    double lonInitial = atof(argv[1]);
    double latInitial = atof(argv[2]);
    double range = atof(argv[3]);
    double bearing = atof(argv[4]);
    
    // Validate input (basic sanity check)
    if (lonInitial < -180.0 || lonInitial > 180.0 ||
        latInitial < -90.0 || latInitial > 90.0 ||
        range < 0.0 || bearing < 0.0 || bearing >= 360.0) {
        fprintf(stderr, "Error: Invalid inputs. Longitude must be [-180, 180], latitude [-90, 90],\n");
        fprintf(stderr, "range > 0, bearing [0, 360).\n");
        return 1;
    }
    
    // Call the r2g function
    double lonFinal, latFinal;
    if (r2g(range, bearing, lonInitial, latInitial, &lonFinal, &latFinal) != 0) {
        fprintf(stderr, "Error: Calculation failed.\n");
        return 1;
    }
    
    // Output results
    printf("Final coordinates (lon, lat): %.6f, %.6f\n", lonFinal, latFinal);
    
    return 0;
}
