     DIMENSION A(5)
     DATA A/1.,2.,3.,4.,5./

     DO 10 I=1,10
     WRITE (6,100) I
100  FORMAT (I<MAX(I,5)>)
10   CONTINUE

     DO 20 I=1,5
     WRITE (6,101) (A(I), J=1,I)
101  FORMAT (<I>F10.<I-1>)
20   CONTINUE
     END