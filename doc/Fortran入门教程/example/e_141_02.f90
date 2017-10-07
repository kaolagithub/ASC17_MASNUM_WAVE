real*8 a, pai, pi_
a=1.d-307; print *, a, 1./a; print *, tiny(a), huge(a)
b=1.e-39 ; print *, b, 1./b; print *, tiny(b), huge(b)

pi =3.14159265358979_8 ; print *, pi    ! pi 无双精度定义，仍为单精度。
a  =3.14159265358979_8 ; print *, a     ! _8 的作用相当于 d0, 正确的方法
pai=3.14159265358979   ; print *, pai   ! 尽管定义为双精度，但无d0，仍为单精度。
pi_=3.14159265358979d0 ; print *, pi_   ! 正确的方法
						 print *, 3.14159265358979_8   ! 直接打印双精度数
						 print *, 3.14159265358979_4   ! 直接打印单精度数

END