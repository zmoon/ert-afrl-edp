/**
 * g2r - Geodetic to Radar Coordinate Transformation Tool
 *
 * Calculates range and bearing between two geographic points
 */

#include "lib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_description() {
  printf("Convert geodetic coordinates to radar range and bearing.\n\n");
}

void print_usage() {
  printf("Usage: g2r <lon1> <lat1> <lon2> <lat2>\n");
  printf("  All coordinates should be in decimal degrees.\n");
  printf("  Output is <range> <bearing>\n");
  printf("  with range in kilometers and bearing in decimal degrees.\n\n");
  printf("Options:\n");
  printf("  -h, --help  Display this help message (must be the only arg).\n\n");
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
  double lonInitial = atof(argv[1]);
  double latInitial = atof(argv[2]);
  double lonFinal = atof(argv[3]);
  double latFinal = atof(argv[4]);

  // Validate inputs
  if (lonInitial < -180.0 || lonInitial >= 180.0 || lonFinal < -180.0 ||
      lonFinal >= 180.0 || latInitial < -90.0 || latInitial > 90.0 ||
      latFinal < -90.0 || latFinal > 90.0) {
    fprintf(stderr, "Error: Invalid coordinates. ");
    fprintf(stderr, "Longitude must be [-180, 180), latitude [-90, 90].\n");
    return 1;
  }

  // Call the g2r function
  double range, bearing;
  if (g2r(&range, &bearing, lonInitial, latInitial, lonFinal, latFinal) != 0) {
    fprintf(stderr, "Error: Calculation failed.\n");
    return 1;
  }

  // Output results
  printf("%.3f %.4f\n", range, bearing);

  return 0;
}
