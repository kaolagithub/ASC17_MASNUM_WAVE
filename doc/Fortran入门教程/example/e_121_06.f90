real :: r(3)
data r /1, 3, 12.5/
do i=1,3
	print *, '°ë¾¶£½', r(i), 'Ô²ÖÜ³¤£½', C(r(i))
end do
end

function c(radius)
	pi=acos(-1.0)
	c=2*pi*radius
	return
end