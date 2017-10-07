 ! --- 三角脉冲函数 ---

i_max=10; x0=27; x1=33; div=(x1-x0)/i_max

open(1,file='pulse.dat',form='formatted',status='unknown')
do i=1, i_max
    x=x0+div*i
	if( x < 28.0 ) then
		  f = 0.0
	else if( x < 30.0 ) then    ! 注意不能写成 (28.<= x).AND.(x<30.) 
		  f = 0.5*x - 14.0
	else if( x < 32.0 ) then
		  f = -0.5*x +16.0
	else
		  f = 0.0
	end if
    write(1,*) x, f
end do
close(1)

END
