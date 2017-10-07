!---------------------------------------------------
!     I/O列表中的变量，数组元素，派生类型及元素
real b(99)
type your_data       
	integer age
	character(30) name
end type your_data

type(your_data) wang,zhang
wang .age =23
wang .name='Xiao Li'
zhang.age =30
zhang.name='Lao Wang'

write(*,"('hi')")
write(*,100) wang, zhang.age    !I/O列表在(*,100)之后
100 format(I3,'   ',A,'(age=',I3,')') !说明输出数据的形式

read(*,300) n, b(n)             ! n和b(n)是I/O列表
300 format(I2,F10.5)            ! 说明输入数据的形式
!---------------------------------------------------
end

