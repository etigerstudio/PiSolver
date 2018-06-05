//
//  main.cpp
//  PiSolver
//
//  Created by ALuier Bondar on 06/06/2018.
//  Copyright © 2018 E-Tiger Studio. All rights reserved.
//

#include <iostream>
#include "mpi.h"

using namespace std;

double partialPi(long long value, long long length) {
    double sum = 0.0;
    int coe = 1;
    for(long long i = 0; i < length; i++) {
        coe = (value % 2 != 0) ? -1 : 1;
        sum += coe * (1 / (double)( 2 * value + 1));
        value ++;
    }
    return sum;
}

int main(int argc, char *argv[]) {
    int rank, size;
    long long N = 16 * 10000000;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    long long segment = (long long)(N / size);
    long long start = rank * segment;
    
    double value = partialPi(start, segment);
    double result = 0;
    if (rank == 0) {
        result += value;
        for(int i = 1; i < size; i++) {
            MPI_Recv(&value, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            result += value;
        }
        
        cout.precision(17);
        cout << "Pi:" << fixed << result * 4 << endl;
    } else {
        MPI_Send(&value, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}
