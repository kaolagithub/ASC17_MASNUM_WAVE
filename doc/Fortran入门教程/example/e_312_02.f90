CHARACTER*24 filename

PAUSE
PAUSE 'Enter DIR to see available files or press RETURN if you already know filename.'

READ(*,'(A\)') filename
OPEN(1, FILE=filename)
CLOSE(1)

END