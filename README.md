# Wave Function Collapse

My personal project on WaveFunctionCollapse algorithm
## 1) Sudoku Solver
Backtracking combined with Wave Function Collapse

How to solve sudoku problem with Wave Function Collapse algo?  
1. Set an empty 9x9 board with max superposed wave states 0x1ff  
2. Read the initial board from .csv and apply wave collapse to known cells  
3. Use backtracking and WFC to solve all unkown cells, prioritize cells with the lowest entropy as search targets  

How to apply wave collapse to a cell?  
1. Collapse - Reduce the superposed states of the cell to one certain state  
2. Propagate - Remove that state from all other cells affected by the cell (colomn, row, square)  
3. Re-propagate - If any of these cells is reduced to one certain state, re-propagate from that cell until no propagation is needed  

If any cell is reduced to 0 state, then the board is invalid and algo returns false (need backtracking)  

Memory usage  
+ Wave states array (bit flags) x 81  
+ Entropy lookup table (vector of idx) x 7

Demo Sudoku  
<table>
      <tr><td>8</td><td> </td><td> </td><td> </td><td> </td><td> </td><td> </td><td> </td><td> </td></tr>
      <tr><td> </td><td> </td><td>3</td><td>6</td><td> </td><td> </td><td> </td><td> </td><td> </td></tr>
      <tr><td> </td><td>7</td><td> </td><td> </td><td>9</td><td> </td><td>2</td><td> </td><td> </td></tr>
      <tr><td> </td><td>5</td><td> </td><td> </td><td> </td><td>7</td><td> </td><td> </td><td> </td></tr>
      <tr><td> </td><td> </td><td> </td><td> </td><td>4</td><td>5</td><td>7</td><td> </td><td> </td></tr>
      <tr><td> </td><td> </td><td> </td><td>1</td><td> </td><td> </td><td> </td><td>3</td><td> </td></tr>
      <tr><td> </td><td> </td><td>1</td><td> </td><td> </td><td> </td><td> </td><td>6</td><td>8</td></tr>
      <tr><td> </td><td> </td><td>8</td><td>5</td><td> </td><td> </td><td> </td><td>1</td><td> </td></tr>
      <tr><td> </td><td>9</td><td> </td><td> </td><td> </td><td> </td><td>4</td><td> </td><td> </td></tr>
</table>

Result  
on x64 release  
board initialized in 0.000243 sec  
solution found in 0.003439 sec

## 2) Tilemap Generation
## 3) 3D Game Scene
will do if I have time...
