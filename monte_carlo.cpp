{\rtf1\ansi\ansicpg1252\cocoartf2761
\cocoatextscaling0\cocoaplatform0{\fonttbl\f0\fswiss\fcharset0 Helvetica;}
{\colortbl;\red255\green255\blue255;}
{\*\expandedcolortbl;;}
\paperw11900\paperh16840\margl1440\margr1440\vieww11520\viewh8400\viewkind0
\pard\tx720\tx1440\tx2160\tx2880\tx3600\tx4320\tx5040\tx5760\tx6480\tx7200\tx7920\tx8640\pardirnatural\partightenfactor0

\f0\fs24 \cf0 #include <iostream>\
#include <cmath>\
#include <cstdlib>\
#include <ctime>\
#include <omp.h>\
\
using namespace std;\
\
double f(double x) \{\
    return x * x;\
\}\
\
int main(int argc, char **argv)\
\{\
    double x_lower = 1;             // Lower limit of x\
    double x_upper = 3;             // Upper limit of x\
    int iterations = 1000000;       // Number of iterations\
\
    double y_lower = 0;             // Lower limit of y (derived from x^2)\
    double y_upper = pow(x_upper, 2); // Upper limit of y (derived from x_upper^2)\
    double area_rect = y_upper * (x_upper - x_lower); // Area of the rectangle\
\
    int total_area = 0;             // Total points generated\
    int monte_area = 0;             // Points under the curve\
    double result = 0;              // Result of integration\
\
    srand(time(NULL));\
\
    // Sequential version\
    clock_t start_time = clock();\
\
    for (int i = 0; i < iterations; i++)\
    \{\
        double x, y;\
        x = rand() % 1000;           // Generate random integer upto 1000\
        y = rand() % 1000;\
\
        x = x / 1000;                // Scale the numbers to [0, 1]\
        y = y / 1000;\
\
        // Map scaled numbers to the actual range [x_lower, x_upper] and [y_lower, y_upper]\
        x = x_lower + x * (x_upper - x_lower);\
        y = y_lower + y * (y_upper - y_lower);\
\
        // Check if the point (x, y) is under the curve y = x^2\
        if (y <= f(x))\
        \{\
            monte_area += 1;         // Point is under the curve\
        \}\
    \}\
\
    // Calculate the result based on Monte Carlo simulation\
    result = area_rect * monte_area / (double)iterations;\
\
    // Output the result and the analytical result for comparison\
    cout << "Sequential execution time: " << (double)(clock() - start_time) / CLOCKS_PER_SEC << " seconds" << endl;\
\
    // Parallel versions\
    for (int num_threads : \{2, 4, 8\})\
    \{\
        start_time = clock();\
\
        #pragma omp parallel for reduction(+: monte_area) num_threads(num_threads)\
        for (int i = 0; i < iterations; i++)\
        \{\
            double x, y;\
            x = rand() % 1000;           // Generate random integer upto 1000\
            y = rand() % 1000;\
\
            x = x / 1000;                // Scale the numbers to [0, 1]\
            y = y / 1000;\
\
            // Map scaled numbers to the actual range [x_lower, x_upper] and [y_lower, y_upper]\
            x = x_lower + x * (x_upper - x_lower);\
            y = y_lower + y * (y_upper - y_lower);\
\
            // Check if the point (x, y) is under the curve y = x^2\
            if (y <= f(x))\
            \{\
            #pragma omp atomic\
                monte_area += 1;         // Point is under the curve\
            \}\
        \}\
\
        // Calculate the result based on Monte Carlo simulation\
        result = area_rect * monte_area / (double)iterations;\
\
        // Output the result and the analytical result for comparison\
        cout << "Parallel execution time with " << num_threads << " threads: " << (double)(clock() - start_time) / CLOCKS_PER_SEC << " seconds" << endl;\
        \
         // Output the result and the analytical result for comparison\
    cout << "Area = " << result << endl;\
    cout << "Analytical result = " << (27.0 - 1) / 3 << endl;\
\
    \}\
\
    return 0;\
\}\
}