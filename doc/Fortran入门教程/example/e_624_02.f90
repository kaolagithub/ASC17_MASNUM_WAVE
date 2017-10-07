INTEGER :: n=0
CHARACTER(LEN=1) :: a,b,c 
a='A';b='B';c ='C'
DO WHILE(n<=0)
	PRINT*, "输入盘的个数"
	READ*, n
END DO
CALL hanoi(n,a,b,c)
END

RECURSIVE SUBROUTINE hanoi(i,x,y,z)
	INTEGER,INTENT(IN) :: i
	CHARACTER(LEN=1),INTENT(IN) :: x,y,z
	IF(i>0) THEN
		CALL hanoi(i-1,x,z,y)
		PRINT '("第",I2,"个盘子：从",A1,"柱移向",A1,"柱")',i,x,y
		CALL hanoi(i-1,z,y,x)
	END IF
END
