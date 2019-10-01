#pragma once

#define __CL_ENABLE_EXCEPTIONS
#include "../headers/util.hpp"

#include "../headers/err_code.hpp"
#include "CL/cl.hpp"
#include <vector>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include <chrono>
#include <cmath>

using namespace std;

void devReduce(cl::Context *context, cl::CommandQueue *queue,cl::Program *program,vector<int> &data,int* devResults,double *devTime,int globalSize,int localSize);
void hostReduce(vector<int> &data,int* hostResults,double* hostTime);
