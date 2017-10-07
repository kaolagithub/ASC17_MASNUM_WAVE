      abcdef=1.                                                         这个年头
      ghijkl=2.                                                         谁还用此
      mnopqr=3                                                          注释区？
      stuvwx=4.                                                         吃饱撑了
c
      write (*,*) abcdef, ghijkl, mnopqr, stuvwx                        这句可以
c	write (*,*) abcdef, ghijkl, mnopqr, stuvwx                        这句不行
                ! 为啥不行？看起来不都一样吗?
      end