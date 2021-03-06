# bmkvp
Modal keyboard-driven virtual pointer.

 ![](preview.gif)
 > bmkvp running on a 1920x1080 screen, navigating archlinux.org 

```
Usage: bmkvp OPTION

Options are:
-h or --help: shows this help menu
-v or --version: shows bmkvp's version information
Default controls(Numpad):
- the arrows will move the grid
- `7` will shrink the grid in the top left corner
- `9` will shrink the grid in the top right corner
- `1` will shrink the grid in the bottom left corner
- `3` will shrink the grid in the bottom right corner
- `5` will simulate a pointer click
- `0` will simulate a right click
- `q` will exit bmkvp

```

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
 Also, in order for `bmkvp` to run properly, you need a desktop compositor(e.g. `compton`). Without one, bmkvp's background will be a solid black color, as there is no way to display RGBA color.
 To install it, you want to move `bmkvp` to `/bin` and set a key combination in order to launch it on the fly(this can be easily done through your desktop environment's settings).
 
 ## Controls
 The default controls are based on your keyboard's numpad:
 
 ![](https://www.jegsworks.com/lessons/computerbasics/lesson3/keys-numericpad.png)
 > courtesy of www.jegsworks.com
 
 - the arrows will move the grid
 - `7` will shrink the grid in the top left corner
 - `9` will shrink the grid in the top right corner
 - `1` will shrink the grid in the bottom left corner
 - `3` will shrink the grid in the bottom right corner
 - `5` will simulate a pointer click
 - `0` will simulate a right click
 - `q` will exit bmkvp

 ## Customization
 If you want to customize bmkvp, you can do so by accessing the source code.
 The user configurable space is located @ [line 41](https://github.com/datcuandrei/bmkvp/blob/bc32ac69084c8753be9cfa4a318411be0eae0dbe/bmkvp.c#L41) and you can change the controls, precision of the pointer grid, transparency and color. The code is easily editable, so you can go even further with customizing it.
 You need to recompile bmkvp in order for the modifications to apply.
 
 ## Thanks
 This project was inspired by: 
 - [warpd](https://github.com/rvaiya/warpd)
 
 Also, this project was done with the help of the following resources:
 - [Xlib Online Manual](https://tronche.com/gui/x/xlib/)
 - [Xlib Tutorial](http://xopendisplay.hilltopia.ca/2009/Jan/Xlib-tutorial-part-1----Beginnings.html)
 - [XTest Extension](https://www.x.org/releases/X11R7.7/doc/libXtst/xtestlib.html#References)
 - [How to set Xlib window background transparent?](https://stackoverflow.com/questions/23051594/how-to-set-xlib-window-background-transparent)
 
 > I wanted to publish my resources, as Xlib is pretty poorly documented online and I know that it will help the people interested in learning Xlib.
 
 ## To-do
 - [ ] override window manager's attributes(in order to remove window decorations)
 - [x] simulate left-click
 - [x] focus on `bmkvp` once launched (related to wm's overriding)
 - [x] warp in quadrants, not based on *precision* 
 - [ ] multi-monitor support 
 - [x] added help screen
 - [ ] add "hold" click
 - [ ] add normal mode

 ## License
 `bmkvp` is licensed under the MIT License. To learn more about it, see [LICENSE](https://github.com/datcuandrei/bmkvp/blob/main/LICENSE).
