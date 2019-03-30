# Wave Function Collapse

My personal project on WaveFunctionCollapse algorithm
## 1) Sudoku Solver
Backtracking combined with Wave Function Collapse

How to solve sudoku problem with Wave Function Collapse algo?  
Step 1. Set an empty 9x9 board with max superposed wave states 0x1ff  
Step 2. Read the initial board from .csv and do wave collapse on known cells  
Step 3. Use backtracking and WFC to solve all unkown cells, prioritize cells with the lowest entropy  

How to do wave collapse on a cell  
Step 1. Collapse - Reduce the superposed states of the cell to one certain state  
Step 2. Propagate - Remove that state from all other cells affected by the cell (colomn, row, square)  
Step 3. Re-propagate - If any of these cells is reduced to one certain state, re-propagate from that cell until no propagation is needed  
If any cell is reduced to 0 state, then the board is invalid and algo returns false (need backtracking)  

Sudoku  
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
x64 release
board initialized in 0.000243 sec
solution found in 0.003439 sec

## 2) Tilemap Generation
## 3) 3D Game Scene
will do if I have time...
