oop-big-homework-RSMT
=====================
用法可以参考 `widget.cpp : void Widget::on_runTest_clicked()` 
因为当时懒得考虑鲁棒性，实现的很懒惰...现在修复了几个bug
主要程序在 `fly.h`里

* 修复了内存泄露的bug
* 修复<=1个点是时会出现的bug
* 优化了效率(n个点挤在一起的时候会退化成 `O(n!)` ...),不过还是很慢，因为求mst的算法是 `O(N^2logN)` 的，近期应该会吧它优化成 `O(NlogN)` 的

用法大概如下：

    GetTree getTree;
    getTree.work(n,x,y);
    //x,y是两个指针，下标范围是1~n
    getTree.tree.dp(lineX,lineY);
    //lineX，lineY是两个vector引用传参，表示{ (lineX[0],lineY[0]) -> (lineX[1],lineY[1]) , (lineX[2],lineY[2]) -> (lineX[3],lineY[3]) ... }

##update

* 已优化为nlogn

