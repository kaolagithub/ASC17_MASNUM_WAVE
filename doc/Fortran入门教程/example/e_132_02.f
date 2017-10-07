      abcdef=1. 
      ghijkl=2. 
	mnopqr=3
	stuvwx=4.
	write (*,*) '这是正确的F77格式：',abcdef, ghijkl, mnopqr, stuvwx      
	write (*,*) '这一行尽管溢出72列，当编译时是正确的：',                 abcdef, ghijkl, mnopqr, stuvwx
	write (*,*) '这一行也是溢出72列，当编译时是正确的？',                  abcdef, ghijkl, mnopqr, stuvwx
c
      !空格的有效性：
	print *,'我想续行！   我想续行！   我想续行！   我想续行！                 
     _                      我想考试！   我想考试！   '
      end