//
// Created by felix on 17.06.24.
//

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <limits.h>

/*
*     Convert string to double.
*     Errno set to ERANGE if conversion failed.
* */
double validate_d_argument(char* str) {
    errno = 0;
    char* endp;

    double res = strtod(str, &endp);
    if (errno == ERANGE) {
        fprintf(stderr, "Failed double argument validation.\n");
        exit(EXIT_FAILURE);
    } else if (endp == str || endp != (void *)0) {
        fprintf(stderr, "Failed double argument validation.\n");
        exit(EXIT_FAILURE);
    }
    return res;
}

/*
*     Convert string to float.
*     Errno set to ERANGE if conversion failed.
* */
float validate_f_argument(char* str) {
    errno = 0;
    char* endp;

    float res = strtof(str, &endp);
    if (errno == ERANGE) {
        fprintf(stderr, "Failed double argument validation.\n");
        exit(EXIT_FAILURE);
    } else if (endp == str || endp != (void *)0) {
        fprintf(stderr, "Failed double argument validation.\n");
        exit(EXIT_FAILURE);
    }
    return res;
}

/*
*     Convert string to long.
*     Errno set to ERANGE if conversion failed.
* */
long int validate_l_argument(char* str) {
    errno = 0;
    char* endp;
    int base = 10;

    long int res = strtol(str, &endp, base);
    if (errno == ERANGE) {
        fprintf(stderr, "Failed double argument validation.\n");
        exit(EXIT_FAILURE);
    } else if (endp == str || endp != (void *)0) {
        fprintf(stderr, "Failed double argument validation.\n");
        exit(EXIT_FAILURE);
    }
    return res;
}

/*
*     Convert string to long double.
*     Errno set to ERANGE if conversion failed.
* */
long double validate_ld_argument(char* str) {
    errno = 0;
    char* endp;

    long double res = strtold(str, &endp);
    if (errno == ERANGE) {
        fprintf(stderr, "Failed double argument validation.\n");
        exit(EXIT_FAILURE);
    } else if (endp == str || endp != (void *)0) {
        fprintf(stderr, "Failed double argument validation.\n");
        exit(EXIT_FAILURE);
    }
    return res;
}

/*
*     Convert string to long long.
*     Errno set to ERANGE if conversion failed.
* */
long long int validate_ll_argument(char* str) {
    errno = 0;
    char* endp;
    int base = 10;

    long long int res = strtoll(str, &endp, base);
    if (errno == ERANGE) {
        fprintf(stderr, "Failed double argument validation.\n");
        exit(EXIT_FAILURE);
    } else if (endp == str || endp != (void *)0) {
        fprintf(stderr, "Failed double argument validation.\n");
        exit(EXIT_FAILURE);
    }
    return res;
}

/*
*     Convert string to unsigned long.
*     Errno set to ERANGE if conversion failed.
* */
unsigned long int validate_ul_argument(char* str) {
    errno = 0;
    char* endp;
    int base = 10;

    unsigned long int res = strtoul(str, &endp, base);
    if (errno == ERANGE) {
        fprintf(stderr, "Failed double argument validation.\n");
        exit(EXIT_FAILURE);
    } else if (endp == str || endp != (void *)0) {
        fprintf(stderr, "Failed double argument validation.\n");
        exit(EXIT_FAILURE);
    }
    return res;
}

/*
*     Convert string to unsigned long long.
*     Errno set to ERANGE if conversion failed.
* */
unsigned long long int validate_ull_argument(char *str) {
    errno = 0;
    char* endp;
    int base = 10;

    unsigned long long int res = strtoull(str, &endp, base);
    if (errno == ERANGE) {
        fprintf(stderr, "Failed double argument validation.\n");
        exit(EXIT_FAILURE);
    } else if (endp == str || endp != (void *)0) {
        fprintf(stderr, "Failed double argument validation.\n");
        exit(EXIT_FAILURE);
    }
    return res;
}