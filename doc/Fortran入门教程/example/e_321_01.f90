CALL RANDOM_SEED      ! Processor initializes the seed randomly from the date and time
call random_number(R)
Number=int(10*R+1)
do
	write(*,'("Your Guess:")'); read *, MyGuess
	if(MyGuess==Number) exit
	if(MyGuess> Number) then
		print *, 'Too high. Try again.'
	else
		print *, 'Too low.  Try again.'
	end if
end do
print *,'You are lucky. It is',Number
end