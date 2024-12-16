#include <iostream>
#include <vector>
#include <cmath>
#include <omp.h>
using namespace std;

void SieveOfEratosthenes(int n, int num_threads) {
    // Create a boolean array "prime[0..n]" and initialize all entries as true
    int count = 0;
    vector<bool> prime(n + 1, true);
    prime[0] = prime[1] = false; // 0 and 1 are not prime numbers

    int sqrt_n = sqrt(n);

    // Parallel region to mark non-prime numbers
    #pragma omp parallel for num_threads(num_threads) schedule(dynamic)
    for (int p = 2; p <= sqrt_n; p++) {
        if (prime[p]) {
            // Critical section to ensure no race conditions while marking multiples
            for (int i = p * p; i <= n; i += p) {
                #pragma omp critical
                {
                    prime[i] = false;
                }
            }
        }
    }

    // Print all prime numbers
    for (int p = 2; p <= n; p++) {
        if (prime[p]) {
            count++;
        }
    }
    cout << "From the calculation, the number of prime numbers from 2 to 100,000 is: " << count <<endl;
}

int main() {
    int n=100000, num_threads=4;

    if (n < 2) {
        cout << "No prime numbers exist below 2.\n";
        return 0;
    }
    
    // Measure execution time
    double start_time = omp_get_wtime();
    SieveOfEratosthenes(n, num_threads);
    double end_time = omp_get_wtime();

    cout << "Execution Time: " << (end_time - start_time) << " seconds\n";

    return 0;
}
