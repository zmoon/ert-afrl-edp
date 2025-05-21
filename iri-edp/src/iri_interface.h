/**
 * @file
 * @brief C interface for the International Reference Ionosphere (IRI) model
 *
 * This header provides C-callable functions to access the IRI model
 * for calculating ionospheric parameters and saving the output.
 */

#ifndef IRI_INTERFACE_H
#define IRI_INTERFACE_H

/* Number of "JF" option switches */
#define NUM_JF 50

/* Maximum number of height steps, 2nd dimension in the Fortran `outf` */
#define MAX_HEIGHT 1000

/* Number of Fortran `outf` array columns */
#define NUM_OUTF 20

/* Number of Fortran `outf` array columns that have vertical profile data */
#define NUM_OUTF_PROFILE 12

/* Number of Fortran `outf` array columns that have vertical profile data + 1
 * for height */
#define NUM_PROFILE NUM_OUTF_PROFILE + 1

/* Length of the Fortran `oarr` array */
#define NUM_OARR 100

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the IRI model
 *
 * This function must be called before any other IRI function to
 * initialize the model's internal data.
 *
 * @return 0 to indicate success
 */
int iri_init(void);

/**
 * @brief Calculate height array based on start, end, and step, filling
 * `heights`
 *
 * @param height_start    Start height in km
 * @param height_end      End height in km
 * @param height_step     Height step in km
 * @param heights         Output array to store heights
 *
 * @return Number of height steps calculated
 */
int iri_heights(double height_start, double height_end, double height_step,
                double heights[MAX_HEIGHT]);

/**
 * @brief Calculate vertical profile for a specific parameter using the IRI
 * model
 *
 * @param latitude   Latitude in degrees North
 * @param longitude  Longitude in degrees East
 * @param year       Year (4 digits)
 * @param month      Month (1-12)
 * @param day        Day of month (1-31)
 * @param hour       Local time (or Universal time + 25) in decimal hours
 * @param height_start    Start height in km
 * @param height_end      End height in km
 * @param height_step     Height step in km
 * @param values     Output array for the profile data
 *
 * @return 0 on success, non-zero on error
 */
int iri_profiles(double latitude, double longitude, int year, int month,
                 int day, double hour, double height_start, double height_end,
                 double height_step, double values[NUM_PROFILE][MAX_HEIGHT]);

/**
 * @brief Write height and parameter values to a CSV file
 *
 * @param filename  Output filename, or NULL for stdout
 * @param values    Array of profile values (height first)
 *
 * @return 0 on success, non-zero on error
 */
int iri_write_csv(const char *filename,
                  const double values[NUM_PROFILE][MAX_HEIGHT]);

#ifdef __cplusplus
}
#endif

#endif /* IRI_INTERFACE_H */
