PROGRAM Example_1_1       ! 求两种平均値
	REAL :: a, b, av1, av2
	READ *, a, b
		av1 = (a + b)/2; av2 = (a*b)**0.5
	PRINT *, av1, av2
END
