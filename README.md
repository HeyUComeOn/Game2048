Game2048
========
Created by myself on 2014/9/8

when connect to the remote, first pull, then push.

《2048》的游戏逻辑：

在屏幕上显示4X4的游戏区域，16个元素；

在游戏开始时在随机位置产生一个元素块（数值2）；

用户通过触摸屏幕可以移动该块向 上下左右移动；

在屏幕上的所有块将按滑动的方向移动，直到顶点或被其他块挡住；

在移动的过程中，相邻相同数字的块，相加产生一个新的块，旧块消失；

如果有3块数字相同则消除最前方的2个块；

如果有4块数字相同的块，则前方2个块相加，后面2个块相加；

如果有4块数字两两相邻的相同，则两两相加，产生新的2个块；

每移动一次，在屏幕上将会在空白区域产生一个新的块(2或4)；

如果屏幕上没有了空白区域；

判断4X4范围内是否有相邻的数字相同的块，不产生新块等待用户移动；

如果在4X4范围内没有相邻的数字相同的块，GAME OVER；
