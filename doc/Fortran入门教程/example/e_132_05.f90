x = 1. ; y = 2. ; z = 3. ; zz=z ; zzz = 3.
 
xyz = x*y*z; if(xyz >= x .or. y <z) print *, x,y,z, xyz
if(zz  == z) print *, zz
if(zzz.eq.z) print *, zzz
end