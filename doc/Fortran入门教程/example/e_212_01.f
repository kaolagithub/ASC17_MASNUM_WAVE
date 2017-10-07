      read *, n
	i=2
    1 continue
	if(mod(n,i).eq.0) then
	    print *, '不是素数，可被',i,'整除'
      end if
	if(mod(n,i).ne.0) then
          i=i+1
          if(i.le.int(sqrt(n*1.))) go to 1
          print *, '是素数'
      end if
      end