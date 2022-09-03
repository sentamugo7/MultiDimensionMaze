# MultiDimensionalMaze
This is a (up to) six dimensional maze. There are three physical dimensions - x (cyan), y (magenta), z (yellow), and three virtual dimensions - a (red), b (green), c (blue).
You start out at the Begin Cell (1 in each dimension), and your goal is to reach the End Cell (max of each dimension).  
To turn left <img src="icons/left_no.svg" width="20" height="20">, right <img src="icons/right_no.svg" width="20" height="20">, up <img src="icons/up_no.svg" width="20" height="20">, down <img src="icons/down_no.svg" width="20" height="20">, reverse <img src="icons/reverse_no.svg" width="20" height="20">, or go forward <img src="icons/forward_no.svg" width="20" height="20">, click on that icon on the right. The icon is white if there is a path in that direction, otherwise black. 
If there is a path in a virtual dimension, there will be a plus and/or minus sign in front of you in the color of that dimension. To move in that dimension, move to the +/- sign in front of you, then click on the +/- sign in the upper right.  
To get a hint, click on the <img src="icons/hint_blank_neg.svg" width="20" height="20"> icon, the hint will appear in the same place inside a circle. Click the icon again to turn off hints.  
The dimensions icon in the upper right <img src="icons/dimensions_neg.svg" width="20" height="20"> will show the colored orientation, you can click on this icon to change the walls.  
Your current position in the maze is displayed in the lower right.  
You can see the score in the bottom right - the number of moves you have made so far out of the minimum possible moves.  
To change Settings, click on the Settings button on the main menu.  
On the Settings page, you can change the size of each dimension of the maze, drag the slider for that dimension to the new size. You can also change the difficulty, and turn sound on and off.  
To remove a dimension, set the size of that dimension to 1.  
When you’ve chosen the final maze size, click **`Apply`**.  
Click **`Cancel`** to reset to the default settings and return to the menu page.
- A minimum of two of dimensions must contain at least two cells each.
- If the total size of the maze is too big, an error message is displayed.
You can turn the sound on or off in the lower left-hand corner of the screen.
---  
Maze generator - James Clark - https://github.com/sbj42/maze-generator - backtrack generator  
Sounds - https://freesound.org  
