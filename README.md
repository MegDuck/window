# window
Window - is a simple native linux console graphics.



It gives very simple api to devs. 
Api headers locate in src/fbapi/draw.h.

There is in file:
drawrect(int x, int y, int x1, int y1, const char *rgb, int border);

this function draw a rectangle from x, y, to x1, y1 with color rgb(HEX)
and if border == 1, draw border.

example:
```c
#include <stdio.h>
#include "/path/to/fbapi/headers/draw.h"

int main(){
    //draw rect from pixel 100(x) and from pixel 100(y)
    //to pixel 300(x) and pixel 300(y)
    //with blue-purple color and no border.
    drawrect(100, 100, 300, 300, "#4D18CA", 0);
    return 0;
}
```

Now there is no good ways to delete rect. Just type "clear"
to clear screen.

And function drawpixel(x, y, rgb);
Just draw a pixel on x,y with color rgb
```c
#include <stdio.h>

int main(void){
    for(int x=0; x<100;x++){
        for (int y=0; y<100; y++){
            drawpixel(x, y, "#4D18CA");
        }
    }
}
```
