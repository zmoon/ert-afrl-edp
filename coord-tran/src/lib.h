/**
 * Coordinate transformation library header
 * Functions for converting between geodetic coordinates and radar measurements
 */

#ifndef COORD_TRAN_LIB_H
#define COORD_TRAN_LIB_H

/* Define M_PI if it's not already defined */
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

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
int g2r(
    double* range,
    double* bearing,
    double lonInitial,
    double latInitial,
    double lonFinal,
    double latFinal
);

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
int r2g(
    double range,
    double bearing,
    double lonInitial,
    double latInitial,
    double* lonFinal,
    double* latFinal
);

#endif /* COORD_TRAN_LIB_H */
