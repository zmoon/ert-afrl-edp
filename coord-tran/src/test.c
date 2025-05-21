/**
 * Test program for coordinate transformation library
 */

#include "lib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define EPSILON 1.0e-10

int test_g2r_r2g_roundtrip() {
  printf("Testing g2r -> r2g roundtrip...\n");

  // Initial: Wallops Islands
  double lon1 = -75.0;
  double lat1 = 37.0;
  ;

  // Target: Puerto Rico
  double lon2 = -66.0;
  double lat2 = 18.0;

  double range, bearing;

  // Convert geodetic to radar
  if (g2r(&range, &bearing, lon1, lat1, lon2, lat2) != 0) {
    printf("Failed to convert geodetic to radar\n");
    return 1;
  }

  printf("Range = %.2f km, Bearing = %.2f degrees\n", range, bearing);

  // Convert back using radar to geodetic
  double lon_result, lat_result;
  if (r2g(range, bearing, lon1, lat1, &lon_result, &lat_result) != 0) {
    printf("Failed to convert radar to geodetic\n");
    return 1;
  }

  printf("Roundtrip result: %.15f, %.15f\n", lon_result, lat_result);
  printf("Original target:  %.15f, %.15f\n", lon2, lat2);

  // Check if the result is close to the original target
  double lon_diff = fabs(lon_result - lon2);
  double lat_diff = fabs(lat_result - lat2);

  // Handle longitude wrap-around
  if (lon_diff > 180.0)
    lon_diff = 360.0 - lon_diff;

  printf("Difference: %.6f, %.6f\n", lon_diff, lat_diff);

  if (lon_diff < EPSILON && lat_diff < EPSILON) {
    printf("Coordinates match within acceptable tolerance\n");
    return 0;
  } else {
    printf("Coordinates don't match within tolerance\n");
    return 1;
  }
}

int main() {
  int result = test_g2r_r2g_roundtrip();
  printf("Test %s\n", result == 0 ? "PASSED" : "FAILED");

  return result;
}
