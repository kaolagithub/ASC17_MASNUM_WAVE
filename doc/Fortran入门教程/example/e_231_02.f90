! 求解二次方程 a*x**2+b*x+c=0
print *, '输入3个系数'; read *, a,b,c

if(abs(a).le.tiny(a)) then
	print *, 'a的值小于允许的最小实数，认为是零'
	if(abs(b).le.tiny(b)) then
		print *, 'b的值小于允许的最小实数，认为是零'
		if(abs(c).le.tiny(c)) then
			print *, 'c的值小于允许的最小实数，认为是零'
			print *, '恒等式0=0，无需解'
		else
			print *, '无解'
		end if
	else
		print *, '一个解：x=', -c/b
	end if
else
	p1=-b/(2*a); s=b**2-4*a*c; p2=sqrt(abs(s))/(2*a)
	if(s<0.) then
		print *, '两个复数解：x=', p1, ' +-', p2, 'i'
	else if (abs(s).le.tiny(s)) then
		print *, '重根      ：x=', p1
	else
		print *, '两个实数解：x=', p1, ' +-', p2
	end if
end if
end

