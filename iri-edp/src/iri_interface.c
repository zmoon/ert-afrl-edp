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
 * Using Fortran logical values: -1 = .TRUE., 0 = .FALSE.
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
static const char* param_names[] = {
    "Height(km)", 
    "Ne(m^-3)",
    "Temperature_electron(K)",
    "Temperature_ion(K)",
    "Temperature_neutral(K)",
    "O+(m^-3)",
    "H+(m^-3)",
    "He+(m^-3)",
    "O2+(m^-3)",
    "NO+(m^-3)",
    "Cluster_ions(m^-3)",
    "Ion_density_N+(m^-3)",
    "TEC(10^16m^-2)",
    "NmF2(m^-3)",
    "hmF2(km)",
    "B0(km)",
    "NmF1(m^-3)",
    "hmF1(km)",
    "NmE(m^-3)",
    "hmE(km)"
};

int iri_init(void) {
    /* "Programs using subroutine IRI_SUB need to include" */
    read_ig_rz_();
    readapf107_();
    /* Note: Currently no easy way to know if the routines succeeded or not */
    return 0;
}

int iri_heights(
    double h_start,
    double h_end,
    double h_step,
    double heights[MAX_HEIGHT]
) {
    int num_heights = (int)((h_end - h_start) / h_step) + 1;
    if (num_heights > MAX_HEIGHT) {
        num_heights = MAX_HEIGHT;
    }

    for (int i = 0; i < num_heights; i++) {
        heights[i] = h_start + i * h_step;
    }

    return num_heights;
}

int iri_profile(
    int jmag,
    double latitude,
    double longitude,
    int year,
    int mmdd,
    double hour,
    double h_start,
    double h_end, 
    double h_step,
    iri_param_t param_type,
    double values[MAX_HEIGHT],
    double oarr[NUM_OARR]
) {
    /* Use single-precision float arrays for Fortran function outputs */
    float f_outf[MAX_HEIGHT][NUM_OUTF];
    float f_oarr[NUM_OARR];
    
    /* Convert double-precision parameters to single for Fortran */
    float f_latitude = (float)latitude;
    float f_longitude = (float)longitude;
    float f_hour = (float)hour;
    float f_h_start = (float)h_start;
    float f_h_end = (float)h_end;
    float f_h_step = (float)h_step;
    
    /* Initialize oarr and f_oarr */
    for (int i = 0; i < NUM_OARR; i++) {
        f_oarr[i] = -1.0f;
        if (oarr != NULL) {
            oarr[i] = -1.0;
        }
    }

    /* Convert longitude from [-180, 180) to [0, 360) if necessary */
    if (longitude < 0) {
        longitude += 360.0;
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
        &f_h_start,
        &f_h_end,
        &f_h_step,
        f_outf,
        f_oarr
    );

    /* Calculate number of height steps */
    int num_heights = (int)((h_end - h_start) / h_step) + 1;
    if (num_heights > MAX_HEIGHT) {
        num_heights = MAX_HEIGHT;
    }

    /* Examine the first few rows */
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 50; j++) {
            printf("f_outf[%d][%d] = %f\n", i, j, f_outf[j][i]);
        }
    }

    /* Copy requested parameter to output array */
    if (param_type >= IRI_PARAM_FIRST && param_type <= IRI_PARAM_LAST) {
        for (int i = 0; i < num_heights; i++) {
            values[i] = (double)f_outf[i][param_type - 1];
        }
        
        /* Copy oarr values if not NULL */
        if (oarr != NULL) {
            for (int i = 0; i < NUM_OARR; i++) {
                oarr[i] = (double)f_oarr[i];
            }
        }
        
        return 0; /* Success */
    } else {
        return 1; /* Invalid parameter type */
    }
}

int iri_write_csv(
    const char* filename,
    const double heights[MAX_HEIGHT],
    const double values[MAX_HEIGHT],
    int num_heights,
    iri_param_t param_type
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

    /* Validate parameter type */
    if (param_type < IRI_PARAM_FIRST || param_type > IRI_PARAM_LAST) {
        fprintf(stderr, "Error: Invalid parameter type %d\n", param_type);
        if (filename != NULL) fclose(fp);
        return 1;
    }

    /* Write CSV header */
    fprintf(fp, "%s,%s\n", param_names[0], param_names[param_type]);

    /* Write data rows */
    for (int i = 0; i < num_heights; i++) {
        fprintf(fp, "%.6e,%.6e\n", heights[i], values[i]);
    }

    /* Close file if not stdout */
    if (filename != NULL) {
        fclose(fp);
    }

    return 0;
}
