**Introduction:**

The simplex method is an algorithm used to solve systems of linear equations involving updated
iterations of a tableau that contains objective values, constraints, and a current solution. The
simplex version explored is the simplest form of the
method, with added Bland’s rule functionality to avoid cycling. The linear program to be solved
is of the form:
```
max cT x
s.t. Ax = b
x ≥ 0
```
Entry of the linear program into the solver requires that it be in:
  * Standard form
  * Objective function value being maximized   
  * Slack variables entered into the constraint values of 0 for their respective cost coefficients in the objective function

The solver will display the
iterations of the tableau being solved, and return the optimal value and optimal solution. 


**Deployment:**
In order to compile, the following command must be entered into the terminal window:
```
g++ -std=c++11 -Wall simplex.cpp -o simplex
```

There are different ways to execute this program:
  * If on Windows (with g++ compiler installed), enter ```simplex.exe``` into the cmd window
  * If on Mac/Linux, enter ```./simplex``` into the terminal shell to run the program.
  
When the program is run, the first thing the user will be asked is to enter the dimensions of the A
matrix (first rows and then columns). The program will individually ask for each element of the
A matrix starting with the upper-left entry and working to the right. The user will then be asked
to enter the entries for the b vector one at a time. Then, the user will be asked to enter the cost
coefficients (objective function coefficients) for the c vector one at a time. Next, the user will be
asked to enter an initial basic feasible solution in the same fashion that the b and c vectors were
initialized. Finally, the user will be asked to enter a 1 or a 0 to represent if they would like to use
Bland’s rule for anti-cycling in their solution.

The program will then print out the tableau at each step before finally outputting the optimal
solution and optimal value if they exist, or an error message if the solution is unbounded.
