program main
    implicit none
    real*8 :: arr(64000)
	
    call c_doge(arr)
    
	print *, arr(1), arr(1001), arr(63001)
end