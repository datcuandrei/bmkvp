# bmkvp
Modal keyboard-driven virtual pointer.

 ![](preview.gif)
 
 ## NOTE :
 This is by no means the finished product. bmkvp is still in active development, and the current published version is still in it's alpha stage.
 
 ## Build
 To build bmkvp, you need:
 - `gcc`
 - `libx11`
 - `libxtst`
 
 You may want to compile it using the following command:
 ```
 gcc -o bmkvp -I. bmkvp.c -lX11 -lXtst
 ```
 
 ## Controls
 The default controls are based on your keyboard's numpad:
 
 ![](https://www.jegsworks.com/lessons/computerbasics/lesson3/keys-numericpad.png)
 
 - the arrows will move the grid
 - `7` will shrink the grid in the top left corner
 - `9` will shrink the grid in the top right corner
 - `1` will shrink the grid in the bottom left corner
 - `3` will shrink the grid in the bottom right corner
 - `5` will simulate the pointer click
 - `q` will exit bmkvp

 ## Customization
 If you want to customize bmkvp, you can do so by accessing the source code.
 The user configurable space is located @ ![line 11](https://github.com/datcuandrei/bmkvp/blob/89ec269a62d84d2c537d73d15923d7db9e558232/bmkvp.c#L11) and you can change the controls, precision of the pointer grid, transparency and color. The code is easily editable, so you can go even further with customizing it.
 You need to recompile bmkvp in order for the modifications to apply.
 
 ## Inspiration
 This project was inspired by: 
 - ![warpd](https://github.com/rvaiya/warpd)
 
 ## License
 `bmkvp` is licensed under the MIT License. To learn more about it, see ![LICENSE](https://github.com/datcuandrei/bmkvp/blob/main/LICENSE).
