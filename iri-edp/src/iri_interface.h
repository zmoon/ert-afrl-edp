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

/* Number of Fortran `outf` columns */
#define NUM_OUTF 20

/* Length of the Fortarn `oarr` array */
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
 * @brief Calculate height array based on start, end, and step, filling `heights`
 * 
 * @param h_start    Start height in km
 * @param h_end      End height in km
 * @param h_step     Height step in km
 * @param heights    Output array to store heights
 * 
 * @return Number of height steps calculated
 */
int iri_heights(
    double h_start,
    double h_end,
    double h_step,
    double heights[MAX_HEIGHT]
);

/**
 * IRI output parameter types for use with iri_profile
 */
typedef enum {
    IRI_PARAM_ELECTRON_DENSITY = 1,  /* Electron density in m^-3 */
    IRI_PARAM_NEUTRAL_TEMP = 2,      /* Neutral temperature in K */
    IRI_PARAM_ION_TEMP = 3,          /* Ion temperature in K */
    IRI_PARAM_ELECTRON_TEMP = 4,     /* Electron temperature in K */
    IRI_PARAM_PF_GF = 15,            /* Plasma frequency / gyro frequency */
    IRI_PARAM_FIRST = IRI_PARAM_ELECTRON_DENSITY,
    IRI_PARAM_LAST = IRI_PARAM_PF_GF
} iri_param_t;

/**
 * @brief Calculate vertical profile for a specific parameter using the IRI model
 * 
 * @param latitude   Latitude in degrees North
 * @param longitude  Longitude in degrees East
 * @param year       Year (4 digits)
 * @param month      Month (1-12)
 * @param day        Day of month (1-31)
 * @param hour       Local time (or Universal time + 25) in decimal hours
 * @param h_start    Start height in km
 * @param h_end      End height in km
 * @param h_step     Height step in km
 * @param param_type Parameter type to calculate (see iri_param_t)
 * @param outf       Output array for the profile data
 * @param oarr       Additional output parameters (can be NULL if not needed)
 * 
 * @return 0 on success, non-zero on error
 */
int iri_profile(
    double latitude,
    double longitude,
    int year,
    int month,
    int day,
    double hour,
    double h_start,
    double h_end, 
    double h_step,
    iri_param_t param_type,
    double values[MAX_HEIGHT],
    double oarr[NUM_OARR]
);

/**
 * @brief Write height and parameter values to a CSV file
 * 
 * @param filename    Output filename, or NULL for stdout
 * @param heights     Array of height values
 * @param values      Array of parameter values
 * @param num_heights Number of height steps
 * @param param_type  Parameter type that was calculated
 * 
 * @return 0 on success, non-zero on error
 */
int iri_write_csv(
    const char* filename,
    const double heights[MAX_HEIGHT],
    const double values[MAX_HEIGHT],
    int num_heights,
    iri_param_t param_type
);

#ifdef __cplusplus
}
#endif

#endif /* IRI_INTERFACE_H */
