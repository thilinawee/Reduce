#include "CL/cl.hpp"

const char *err_code (cl_int err_in);
void check_error(cl_int err, const char *operation, char *filename, int line);

#define checkError(E, S) check_error(E,S,__FILE__,__LINE__)
