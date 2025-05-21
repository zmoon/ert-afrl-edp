/**
 * g2r - Geodetic to Radar Coordinate Transformation Tool
 * 
 * Calculates range and bearing between two geographic points
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib.h"

void print_usage() {
    printf("Usage: g2r <lon_initial> <lat_initial> <lon_final> <lat_final>\n");
    printf("  Converts geodetic coordinates to radar range and bearing.\n");
    printf("  All coordinates should be in decimal degrees.\n\n");
    printf("Options:\n");
    printf("  -h, --help     Display this help message\n\n");
    printf("Example:\n");
    printf("  g2r -84.39 33.75 -77.04 38.90\n");
    printf("  (Atlanta, GA to Washington, DC)\n");
}

int main(int argc, char *argv[]) {
    // Check for help option
    if (argc == 2 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)) {
        print_usage();
        return 0;
    }
    
    // Check if we have the correct number of arguments
    if (argc != 5) {
        fprintf(stderr, "Error: Incorrect number of arguments.\n");
        print_usage();
        return 1;
    }
    
    // Parse command line arguments
    double lonInitial = atof(argv[1]);
    double latInitial = atof(argv[2]);
    double lonFinal = atof(argv[3]);
    double latFinal = atof(argv[4]);
    
    // Validate input (basic sanity check)
    if (lonInitial < -180.0 || lonInitial > 180.0 ||
        lonFinal < -180.0 || lonFinal > 180.0 ||
        latInitial < -90.0 || latInitial > 90.0 ||
        latFinal < -90.0 || latFinal > 90.0) {
        fprintf(stderr, "Error: Invalid coordinates. Longitude must be [-180, 180], latitude [-90, 90].\n");
        return 1;
    }
    
    double range, bearing;
    
    // Call the g2r function
    if (g2r(&range, &bearing, lonInitial, latInitial, lonFinal, latFinal) != 0) {
        fprintf(stderr, "Error: Calculation failed.\n");
        return 1;
    }
    
    // Output results
    printf("Range: %.2f km\n", range);
    printf("Bearing: %.2f degrees\n", bearing);
    
    return 0;
}
