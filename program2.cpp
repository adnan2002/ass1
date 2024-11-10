#include <mpi.h>
#include <iostream>

using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    // Check if there are exactly 2 processes
    if (size != 2) {
        if (rank == 0) {
            cout << "This program requires exactly 2 processes. Current number of processes: " << size << endl;
        }
        MPI_Finalize();
        return 1;
    }
    
    const int ARRAY_SIZE = 1000;
    int x[ARRAY_SIZE];
    double t1, t2, tp0, tp1;
    int element;
    
    // Only P0 populates the array
    if (rank == 0) {
        for (int i = 0; i < ARRAY_SIZE; i++) {
            x[i] = i;
        }
    }
    
    // Barrier to ensure both processes are ready
    MPI_Barrier(MPI_COMM_WORLD);
    
    if (rank == 0) {  // Process P0
        t1 = MPI_Wtime();
        for (int i = 0; i < ARRAY_SIZE; i++) {
            element = x[i];
            MPI_Send(&element, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        }
        t2 = MPI_Wtime();
        tp0 = t2 - t1;
        
        // Receive timing from P1
        MPI_Recv(&tp1, 1, MPI_DOUBLE, 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        cout << "Program 2 Results:" << endl;
        cout << "Time for sending (TP0): " << tp0 << " seconds" << endl;
        cout << "Time for receiving (TP1): " << tp1 << " seconds" << endl;
    }
    else if (rank == 1) {  // Process P1
        t1 = MPI_Wtime();
        for (int i = 0; i < ARRAY_SIZE; i++) {
            MPI_Recv(&element, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            x[i] = element;
        }
        t2 = MPI_Wtime();
        tp1 = t2 - t1;
        
        // Send timing back to P0
        MPI_Send(&tp1, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
    }
    
    MPI_Finalize();
    return 0;
}