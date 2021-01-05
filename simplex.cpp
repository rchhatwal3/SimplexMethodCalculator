/*
    Ramneek Chhatwal, Anthony Alber, & Aman Garlapati
    MATH 4400
    Final Project, Topic 4, Parts (1) & (2)
*/

#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

// Function prototypes

int optimalCheck(double* &red_cost, int totalCols);
int unboundedCheck(double* &red_cost, double** &matrix, int totalRows, int totalCols);
void determineEntering(double* &red_cost, int& column, int totalCols);
void determineDeparting(double* &const_b, double** &matrix, int& row, int column, int totalRows);
void bland(double* &red_cost, double* &const_b, double** &matrix, int& row, int& column, int totalRows, int totalCols);
void pivot(double* &red_cost, double* &const_b, double** &matrix, double& solution, int row, int column, int totalRows, int totalCols);
void printTableau(double* &red_cost, double* &const_b, double** &matrix, double& solution, int totalRows, int totalCols);
void printX(double* &red_cost, double* &const_b, double** &matrix, int totalRows, int totalCols);

// Main

int main() {
    // Optimal value variable
    double val = 0.0;

    // Matrix A dimensions
    int m, n;
    cout << endl << "# of rows in A: ";
    cin >> m;
    cout << endl << "# of columns in A: ";
    cin >> n;
    cout << endl;

    // Initialize matrix A
    double** A = new double*[m];
    for (int i = 0; i < m; ++i) {
        A[i] = new double[n];
    }

    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << "Enter matrix element A[" << i + 1 << "][" << j + 1 << "]: ";
            cin >> A[i][j];
        }
    }

    // Initialize RHS vector b
    double* b = new double[m];
    cout << endl;
    for (int i = 0; i < m; ++i) {
        cout << "Enter RHS b[" << i + 1 << "]: ";
        cin >> b[i];
    }

    // Initialize cost vector c
    double* c = new double[n];
    cout << endl;
    for (int i = 0; i < n; ++i) {
        cout << "Enter cost coefficient c[" << i + 1 << "]: ";
        cin >> c[i];
    }

    // Copy cost vector c into reduced cost vector red_c
    double* red_c = new double[n];
    for (int i = 0; i < n; ++i) {
        if ((c[i] > -0.0001) && (c[i] < 0.0001)) {
            red_c[i] = c[i];
        } else {
            red_c[i] = -1.0 * c[i];
        }
    }

    // Initialize Initial BFS
    double* bfs = new double[n];
    cout << endl;
    for (int i = 0; i < n; ++i) {
        cout << "Enter BFS element B[" << i + 1 << "]: ";
        cin >> bfs[i];
    }

    // Bland's Method prompt
    int blandCheck;
    cout << endl << "Enter 1 to use Bland's rule, Enter 0 if not: ";
    cin >> blandCheck;
    cout << endl;
    

    // Entering column / departing row indexes
    int enter, depart;

    // Print initial tableau
    printTableau(red_c, b, A, val, m, n);

    // ** ALGORITHM **

    while ((optimalCheck(red_c, n) != 1) && (unboundedCheck(red_c, A, m, n) != -3)) {
        if (blandCheck == 1) {
            bland(red_c, b, A, depart, enter, m, n);
        } else {
            determineEntering(red_c, enter, n);
            determineDeparting(b, A, depart, enter, m);
        }
        pivot(red_c, b, A, val, depart, enter, m, n);
        printTableau(red_c, b, A, val, m, n);
    }

    if (optimalCheck(red_c, n) == 1) {
        printX(red_c, b, A, m, n);
        cout << endl << "Optimal Value: " << val << endl;
    } else {
        cout << "Error -3: problem unbounded" << endl;
    }

    // Free memory
    // Free matrix A
    for (int i = 0; i < m; i++){
        delete[] A[i];
    }
    delete[] A;

    // Free RHS vector b
    delete[] b;

    // Free initial cost vector c
    delete[] c;

    // Free reduced cost vector red_c
    delete[] red_c;

    // Free initial bfs
    delete[] bfs;

    return 0;
}

// Function definitions

// Check if current solution is optimal
int optimalCheck(double* &red_cost, int totalCols) {
    // Loop through reduced cost vector red_cost
    for (int i = 0; i < totalCols; ++i) {
        // Check if reduced cost element is negative
        if (red_cost[i] < -0.0001) {
            // Return non-optimal
            return 0;
        }
    }
    // Return optimal
    return 1;
}

// Check if problem is unbounded
int unboundedCheck(double* &red_cost, double** &matrix, int totalRows, int totalCols) {
    // Loop through reduced cost vector red_cost
    for (int i = 0; i < totalCols; ++i) {
        // Negative element counter
        int counter = 0;
        // Check if reduced cost element is negative
        if (red_cost[i] < -0.0001) {
            // Loop through matrix column
            for (int j = 0; j < totalRows; ++j) {
                // Check if matrix element is non-positive
                if (matrix[j][i] <= 0.0001) {
                    // Increment negative element counter
                    ++counter;
                }
            }
            // Check if entire column is non-positive
            if (counter == totalRows) {
                // Return unbounded
                return -3;
            }
        }
    }
    // Return bounded
    return 0;
}

// Determine variable entering the basis
void determineEntering(double* &red_cost, int& column, int totalCols) {
    // Most negative reduced cost tracker
    double min = 0.0;
    // Loop through reduced cost vector red_cost
    for (int i = 0; i < totalCols; ++i) {
        // Check if reduced cost element is less than minimum
        if ((red_cost[i] - min) < 0.0001) {
            // Update minimum
            min = red_cost[i];
            // Update column index
            column = i;
        }
    }
    return;
}

// Determine variable departing the basis
void determineDeparting(double* &const_b, double** &matrix, int& row, int column, int totalRows) {
    // Most negative ratio tracker
    double min = 1000000.0;
    // Loop through RHS vector const_b
    for (int i = 0; i < totalRows; ++i) {
        // Calculate ratio
        double ratio = const_b[i] / matrix[i][column];
        // Check if matrix element is positive and ratio is less than minimum
        if ((matrix[i][column] > 0.0001) && ((ratio - min) < 0.0001)) {
            // Update minimum
            min = ratio;
            // Update row index
            row = i;
        }
    }
    return;
}

// Determine variables entering and departing the basis with Bland's rule
void bland(double* &red_cost, double* &const_b, double** &matrix, int& row, int& column, int totalRows, int totalCols) {
    // Find smallest index with negative reduced cost
    for (int i = totalCols - 1; i >= 0; --i) {
        if (red_cost[i] < -0.0001) {
            column = i;
        }
    }
    // Find smallest index of variables in the basis tied for minimum ratio test
    int minIndex;
    int minRatio = 1000000.0;
    // Loop through reduced cost vector
    for (int i = totalCols - 1; i >= 0; --i) {
        int basicCheck = 0;
        // Check if reduced cost is 0
        if ((red_cost[i] > -0.0001) && (red_cost[i] < 0.0001)) {
            // Loop through matrix column
            for (int j = 0; j < totalRows; ++j) {
                // Check if matrix element is 0 or 1
                if (((matrix[j][i] > -0.0001) && (matrix[j][i] < 0.0001)) || ((matrix[j][i] - 1 > -0.0001) && (matrix[j][i] - 1 < 0.0001))) {
                    basicCheck = 1;
                    // Check if matrix element is 1
                    if ((matrix[j][i] - 1 > -0.0001) && (matrix[j][i] - 1 < 0.0001)) {
                        // Check if pivot element is positive
                        if ((matrix[j][column] > 0.0001)) {
                            // Calculate ratio
                            double ratio = const_b[i] / matrix[j][column];
                            // Check if ratio is less than or equal to minimum ratio
                            if (((ratio - minRatio) < 0.0001) || (((ratio - minRatio) < 0.0001) && ((ratio - minRatio) > -0.0001))) {
                                minIndex = j;
                            }
                        }
                    }
                } else {
                    basicCheck = 0;
                }
            }
            // Check if column is basic
            if (basicCheck == 1) {
                row = minIndex;
            }
        }
    }
    return;
}

// Pivot simplex tableau
void pivot(double* &red_cost, double* &const_b, double** &matrix, double& solution, int row, int column, int totalRows, int totalCols) {
    // Adjust matrix A pivot row
    double p_factor = 1.0 / matrix[row][column];
    for (int i = 0; i < totalCols; ++i) {
        matrix[row][i] *= p_factor;
    }
    // Adjust RHS pivot row
    const_b[row] *= p_factor;
    // Adjust matrix A and RHS non-pivot rows
    for (int i = 0; i < totalRows; ++i) {
        if (i != row) {
            double factor = matrix[i][column];
            for (int j = 0; j < totalCols; ++j) {
                matrix[i][j] = -1.0 * matrix[row][j] * factor + matrix[i][j];
            }
            const_b[i] = -1.0 * const_b[row] * factor + const_b[i];
        }
    }
    // Adjust reduced cost vector and solution
    double r_factor = red_cost[column];
    for (int i = 0; i < totalCols; ++i) {
        red_cost[i] = -1.0 * matrix[row][i] * r_factor + red_cost[i];
    }
    solution = -1.0 * const_b[row] * r_factor + solution;
    return;
}

// Print simplex tableau
void printTableau(double* &red_cost, double* &const_b, double** &matrix, double& solution, int totalRows, int totalCols) {
    for (int i = 0; i < totalCols; ++i) {
        cout << setw(9) << fixed << setprecision(4) << red_cost[i];
    }
    cout << " | " << solution << endl;
    for (int i = 0; i < totalCols; ++i) {
        cout << "---------";
    }
    cout << "-----------" << endl;
    for (int i = 0; i < totalRows; ++i) {
        for (int j = 0; j < totalCols; ++j) {
            cout << setw(9) << fixed << setprecision(4) << matrix[i][j];
        }
        cout << " | " << const_b[i] << endl;
    }
    cout << endl << endl;
}

// Print optimal solution
void printX(double* &red_cost, double* &const_b, double** &matrix, int totalRows, int totalCols) {
    int* indices = new int[totalCols];

    // Loop through reduced cost vector
    for (int i = totalCols - 1; i >= 0; --i) {
        int basicCheck = 0;
        int b_index;
        // Check if reduced cost is 0
        if ((red_cost[i] > -0.0001) && (red_cost[i] < 0.0001)) {
            // Loop through matrix column
            for (int j = 0; j < totalRows; ++j) {
                // Check if matrix element is 0 or 1
                if (((matrix[j][i] > -0.0001) && (matrix[j][i] < 0.0001)) || ((matrix[j][i] - 1 > -0.0001) && (matrix[j][i] - 1 < 0.0001))) {
                    basicCheck = 1;
                    // Check if matrix row corresponds to solution
                    if ((matrix[j][i] - 1 > -0.0001) && (matrix[j][i] - 1 < 0.0001)) {
                        b_index = j;
                    }
                } else {
                    basicCheck = 0;
                }
            }
        }
        // Check if column is basic
        if (basicCheck == 1) {
            // Store RHS index
            indices[i] = b_index + 1;
        }
        else {
            indices[i] = 0;
        }
    }
    cout << "Optimal Solution: (";
    for (int i = 0; i < totalCols; ++i) {
        if (i == totalCols - 1) {
            if (indices[i] == 0) {
                cout << 0.0;
            } else {
                cout << setprecision(4) << const_b[indices[i] - 1];
            }
        } else {
            if (indices[i] == 0) {
                cout << 0.0 << ",";
            } else {
                cout << setprecision(4) << const_b[indices[i] - 1] << ",";
            }
        }
    }
    cout << ")";

    // Free basic variable indices
    delete[] indices;
    
    return;
}
