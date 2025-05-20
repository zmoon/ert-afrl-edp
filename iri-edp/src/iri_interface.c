/**
 * @file
 * @brief Implementation of C interface for the IRI model
 */

#include "iri_interface.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Function prototypes for Fortran functions */
extern void read_ig_rz_();
extern void readapf107_();
extern void iri_sub_(
    int jf[NUM_JF],
    int* jmag,
    float* alati,
    float* along,
    int* iyyyy,
    int* mmdd,
    float* dhour,
    float* heibeg,
    float* heiend,
    float* heistp,
    float outf[MAX_HEIGHT][NUM_OUTF],
    float oarr[NUM_OARR]
);

/* 
 * Default JF switches array for standard IRI operation
 * Recommended default values from iritest.for
 * -1 => .TRUE., 0 => .FALSE.
 */
static int jf[NUM_JF] = {
    -1,  -1,  -1,   0,   0,  /*  1-5  */
     0,  -1,  -1,  -1,  -1,  /*  6-10 */
    -1,  -1,  -1,  -1,  -1,  /* 11-15 */
    -1,  -1,  -1,  -1,  -1,  /* 16-20 */
    -1,  -1,   0,  -1,  -1,  /* 21-25 */
    -1,  -1,  -1,  -1,   0,  /* 26-30 */
    -1,  -1,   0,  -1,   0,  /* 31-35 */
    -1,  -1,  -1,   0,   0,  /* 36-40 */
    -1,  -1,  -1,  -1,  -1,  /* 41-45 */
    -1,   0,  -1,  -1,  -1   /* 46-50 */
};

/* Names of IRI output parameters for CSV headers */
static const char* col_names[NUM_PROFILE] = {
    "height(km)",
    /* 1: electron number density */
    "ne(m-3)",
    /* 2-4: temperatures */
    "Tn(K)",
    "Ti(K)",
    "Te(K)",
    /* 5-11: ion densities */
    "O+(%)",
    "H+(%)",
    "He+(%)",
    "O2+(%)",
    "NO+(%)",
    "CI(%)",
    "N+(%)",
    /* 12-13: unmarked in irisub.for */
    /* 14: misc. column */
    /* 15: plasma frequency divided by gyro frequency */
    "PF/GF(1)"
    /* 16-20: free */
};

/* One-based indices of the `outf` columns we want to take */
static const int take_cols[NUM_OUTF_PROFILE] = {
    1,
    2, 3, 4,
    5, 6, 7, 8, 9, 10, 11,
    15
};

int iri_init(void) {
    /* "Programs using subroutine IRI_SUB need to include" */
    read_ig_rz_();
    readapf107_();
    /* Note: Currently no easy way to know if the routines succeeded or not */
    return 0;
}

int iri_heights(
    double height_start,
    double height_end,
    double height_step,
    double heights[MAX_HEIGHT]
) {
    int num_heights = (int)((height_end - height_start) / height_step) + 1;
    if (num_heights > MAX_HEIGHT) {
        num_heights = MAX_HEIGHT;
    }

    for (int i = 0; i < num_heights; i++) {
        heights[i] = height_start + i * height_step;
    }
    for (int i = num_heights; i < MAX_HEIGHT; i++) {
        heights[i] = -1.0;
    }

    return num_heights;
}

int iri_profiles(
    double latitude,
    double longitude,
    int year,
    int month,
    int day,
    double hour,
    double height_start,
    double height_end,
    double height_step,
    double values[NUM_PROFILE][MAX_HEIGHT]
) {
    /* Use single-precision float arrays for Fortran function outputs */
    float f_outf[MAX_HEIGHT][NUM_OUTF];
    float f_oarr[NUM_OARR];
    
    /* Convert double-precision parameters to single for Fortran */
    float f_latitude = (float)latitude;
    float f_longitude = (float)longitude;
    float f_hour = (float)hour;
    float f_height_start = (float)height_start;
    float f_height_end = (float)height_end;
    float f_height_step = (float)height_step;

    /* Calculate MMDD parameter for IRI */
    int mmdd = month * 100 + day;

    /* Assume geographic (= 0, as opposed to geomagnetic, = 1) */
    int jmag = 0;

    /* Initialize output arrays */
    for (int i = 0; i < MAX_HEIGHT; i++) {
        for (int j = 0; j < NUM_OUTF; j++) {
            f_outf[i][j] = -1.0f;
        }
    }
    for (int i = 0; i < NUM_OARR; i++) {
        f_oarr[i] = -1.0f;
    }

    /* Call the Fortran IRI_SUB routine */
    iri_sub_(
        jf,
        &jmag,
        &f_latitude,
        &f_longitude,
        &year,
        &mmdd,
        &f_hour,
        &f_height_start,
        &f_height_end,
        &f_height_step,
        f_outf,
        f_oarr
    );

    /* Compute heights, which will be the first column in our output */
    int num_heights = iri_heights(height_start, height_end, height_step, values[0]);

    /* Copy profile results */
    int i_f;
    for (int i = 0; i < NUM_OUTF_PROFILE; i++) {
        i_f = take_cols[i] - 1;
        for (int j = 0; j < num_heights; j++) {
            values[i + 1][j] = (float)f_outf[j][i_f];
        }
    }

    return 0;
}

int iri_write_csv(
    const char* filename,
    const double values[NUM_PROFILE][MAX_HEIGHT]
) {
    FILE* fp;
    
    /* Open file for writing or use stdout */
    if (filename != NULL) {
        fp = fopen(filename, "w");
        if (fp == NULL) {
            fprintf(stderr, "Error opening file %s for writing\n", filename);
            return 1;
        }
    } else {
        fp = stdout;
    }

    /* Write CSV header */
    for (int i = 0; i < NUM_PROFILE; i++) {
        fprintf(fp, "%s", col_names[i]);
        if (i < NUM_PROFILE - 1) {
            fprintf(fp, ",");
        }
    }
    fprintf(fp, "\n");

    /* Write data rows */
    for (int i = 0; i < MAX_HEIGHT; i++) {
        if (values[0][i] == -1.0) break;

        for (int j = 0; j < NUM_PROFILE; j++) {
            fprintf(fp, values[j][i] == -1.0 ? "%.1e" : "%.6e", values[j][i]);
            if (j < NUM_PROFILE - 1) {
                fprintf(fp, ",");
            }
        }
        fprintf(fp, "\n");
    }

    /* Close file if not stdout */
    if (filename != NULL) {
        fclose(fp);
    }

    return 0;
}
