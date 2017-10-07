INTEGER bells(4)
CHARACTER(4) wys(3),cvar
DATA bells/4*7/,cvar/' is'/,wys/'What',' you',' see'/
OPEN(3,FILE='BSEQ',FORM='BINARY')
WRITE(3) wys,cvar
WRITE(3) 'what', ' you get!'
WRITE(3) bells
CLOSE(3)
END
