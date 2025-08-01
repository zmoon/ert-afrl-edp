/**
 * @file
 * @brief Coordinate transformation library: radar <-> geodetic,
 * using a spherical Earth model for simplification
 */

#include "lib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Convert degrees to radians
 */
static double deg2rad(double degrees) { return degrees * (M_PI / 180.0); }

/**
 * Convert radians to degrees
 */
static double rad2deg(double radians) { return radians * (180.0 / M_PI); }

/**
 * Geodetic to Radar conversion (Haversine formula)
 * Calculates range and bearing from initial to final coordinates
 *
 * @param range Pointer to store the calculated range (km)
 * @param bearing Pointer to store the calculated initial bearing (deg)
 * @param lonInitial Initial longitude (deg)
 * @param latInitial Initial latitude (deg)
 * @param lonFinal Final longitude (deg)
 * @param latFinal Final latitude (deg)
 * @return 0 on success, non-zero on failure
 */
int g2r(double *range, double *bearing, double lonInitial, double latInitial,
        double lonFinal, double latFinal) {

  if (range == NULL || bearing == NULL) {
    return -1; // Invalid pointers
  }

  // If the points are the same, range is zero and bearing is undetermined
  if (latInitial == latFinal && lonInitial == lonFinal) {
    *range = 0.0;
    *bearing = UNDETERMINED_BEARING;
    return 0;
  }

  // Convert degrees to radians
  double lat1 = deg2rad(latInitial);
  double lon1 = deg2rad(lonInitial);
  double lat2 = deg2rad(latFinal);
  double lon2 = deg2rad(lonFinal);

  // Calculate differences
  double dlon = lon2 - lon1;
  double dlat = lat2 - lat1;

  // Haversine formula
  double a = sin(dlat / 2) * sin(dlat / 2) +
             cos(lat1) * cos(lat2) * sin(dlon / 2) * sin(dlon / 2);
  double c = 2 * atan2(sqrt(a), sqrt(1 - a));

  // Calculate range (distance)
  *range = EARTH_RADIUS * c;

  // If the points are at the poles (same or opposite pole), bearing is
  // undetermined
  if (fabs(latInitial) == 90.0 && fabs(latFinal) == 90.0) {
    *bearing = UNDETERMINED_BEARING;
    return 0;
  }

  // Calculate bearing
  double y = sin(dlon) * cos(lat2);
  double x = cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(dlon);
  double bearing_rad = atan2(y, x);

  // Convert bearing to degrees, normalized to [0, 360)
  *bearing = fmod(rad2deg(bearing_rad) + 360.0, 360.0);

  return 0;
}

/**
 * Radar to Geodetic conversion
 * Calculates final coordinates given initial point, range, and bearing
 *
 * @param range Distance from initial point (km)
 * @param bearing Initial bearing (deg)
 * @param lonInitial Initial longitude (deg)
 * @param latInitial Initial latitude (deg)
 * @param lonFinal Pointer to store calculated final longitude (deg)
 * @param latFinal Pointer to store calculated final latitude (deg)
 * @return 0 on success, non-zero on failure
 */
int r2g(double range, double bearing, double lonInitial, double latInitial,
        double *lonFinal, double *latFinal) {

  if (lonFinal == NULL || latFinal == NULL) {
    return -1; // Invalid pointers
  }

  // If range is zero, stay where we are
  if (range == 0.0) {
    *lonFinal = lonInitial;
    *latFinal = latInitial;
    return 0;
  }

  // If we are at a pole, and range is pi r_e, we go to the opposite pole
  // and keep the same longitude
  if (fabs(latInitial) == 90.0 && range == EARTH_RADIUS * M_PI) {
    *latFinal = -latInitial;
    *lonFinal = lonInitial;
    return 0;
  }

  // Convert to radians
  double lat1 = deg2rad(latInitial);
  double lon1 = deg2rad(lonInitial);
  double brng = deg2rad(bearing);

  // Angular distance in radians
  double angular_distance = range / EARTH_RADIUS;

  // Calculate final latitude
  double lat2 = asin(sin(lat1) * cos(angular_distance) +
                     cos(lat1) * sin(angular_distance) * cos(brng));

  // Calculate final longitude
  // If we are at a pole, the final longitude is determined by the bearing
  double lon2;
  if (latInitial == 90.0) {
    lon2 = M_PI - brng;
  } else if (latInitial == -90.0) {
    lon2 = brng;
  } else {
    lon2 = lon1 + atan2(sin(brng) * sin(angular_distance) * cos(lat1),
                        cos(angular_distance) - sin(lat1) * sin(lat2));
  }

  // Convert back to degrees
  *latFinal = rad2deg(lat2);
  *lonFinal = rad2deg(lon2);

  // Normalize longitude to [-180, 180)
  *lonFinal = fmod(*lonFinal + 540.0, 360.0) - 180.0;

  return 0;
}
