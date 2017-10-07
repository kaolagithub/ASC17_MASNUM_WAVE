      subroutine frepath(i,j)
      common/angl/ux,uy,uz,vx,vy,vz
      common/cood/x,y,z,xd,yd,zd
      common/cute/estop1,estop2,estop,estopm
      common/divi/div,idiv,ediv(200),rediv(199)
      common/ener/energy,energ2,denerg,valens
      common/icon/theta0,estart,eprima,x0,y0,z0,zs,rs
      common/llii/il
      common/path/ss
      common/pote/workf(4),poine(4)
      common/tcrs/tcstt(4,200),rela(4,200),race(4,2,200),raci(4,2,200)
     &           ,racs(2,0:1,0:1,200),beta(2,1,200),thct(2,1,200,160)
      l=int(sqrt((energy-estopm)/div))+1
      if(l.gt.idiv) l=idiv
      sigma1=tcstt(1,l)
      sigma2=tcstt(2,l)
      T0=-z/uz
      T1=-1
      T2=-1
   10 call randomz(2,1,urn)
      if(urn.lt.1.e-20) go to 10
      c0=-log(urn)
      b=x*ux+y*uy+(z-zs)*uz
      c=x**2+y**2+(z-zs)**2-rs**2
      if(b*b<=c) then
          k=0
      else
          k=1
          T1=-b-sqrt(b*b-c)
          T2=-b+sqrt(b*b-c)
      end if
      if(uz>=0.) then  !!!!!!真空->固体
          if(zs<=-rs) then               !球在真空中
	      if(c<=0.) then                 !起点在球内或球上
                  if(sigma2*T2>c0) then                  !线1-S
	              ss=c0/sigma2
	              j=2
                  else
                      ss=T0+(c0-sigma2*T2)/sigma1
                      j=1
                  end if                                 !线1-E
              else                           !起点在球外
                  if(T1>=0.) then                        !线2-S
                      if(sigma2*(T2-T1)>c0) then
                          ss=T1+c0/sigma2
                          j=2
                      else
                          ss=T0+(c0-sigma2*(T2-T1))/sigma1
                          j=1
                      end if                             !线2-E
                  elseif((k==0.or.T2<=0.).and.T0>0.) then!线3-S
                      ss=T0+c0/sigma1
                      j=1                                !线3-E
                  else                                   !线4-S
                      ss=c0/sigma1
                      j=1
                  end if                                 !线4-E
              end if
          elseif(-rs<zs.and.zs<rs) then  !球半嵌在固体中
              if(T1>=0..and.T0>=T2) then                 !线1-S
                  if(sigma2*(T2-T1)>c0) then
                      ss=T1+c0/sigma2
                      j=2
                  else
                      ss=T0+(c0-sigma2*(T2-T1))/sigma1
                      j=1
                  end if                                 !线1-E
              elseif(T1<0..and.T2>0.and.T2<T0) then      !线2-S
                  if(sigma2*T2>c0) then
                      ss=c0/sigma2
                      j=2
                  else
                      ss=T0+(c0-sigma2*T2)/sigma1
                      j=1
                  end if                                 !线2-E
              elseif(T1>=0..and.T1<T0.and.T0<T2) then    !线3-S
                  if(sigma2*(T2-T1)>c0) then
                      ss=T1+c0/sigma2
		      j=2
		  else
		      ss=T2+(c0-sigma2*(T2-T1))/sigma1
	              j=1
	          end if                                 !线3-E
              elseif(T1<=0..and.T2>=T0.and.T2>=0.) then  !线4,5-S
                  if(sigma2*T2>c0) then
                      ss=c0/sigma2
                      j=2
                  else
                      ss=T2+(c0-sigma2*T2)/sigma1
                      j=1
                  end if                                 !线4,5-E
              elseif(T0>=0..and.T1>=T0) then             !线6-S
                  if(sigma1*(T1-T0)>c0) then
                      ss=T0+c0/sigma1
                      j=1
                  elseif(sigma1*(T1-T0)+sigma2*(T2-T1)>c0) then
                      ss=T1+(c0-sigma1*(T1-T0))/sigma2
                      j=2
                  else
	              ss=T2+(c0-sigma1*(T1-T0)-sigma2*(T2-T1))/sigma1
                      j=1
                  end if                                 !线6-E
              elseif(T0<0..and.T1>=0.) then              !线7-S
                  if(sigma1*T1>c0) then
                      ss=c0/sigma1
                      j=1
                  elseif(sigma1*T1+sigma2*(T2-T1)>c0) then
                      ss=T1+(c0-sigma1*T1)/sigma2
                      j=2
                  else
                      ss=T2+(c0-sigma1*T1-sigma2*(T2-T1))/sigma1
                      j=1
                  end if                                 !线7-E
              elseif((k==0.or.T2<=0.).and.T0>0.) then    !线8-S
                  ss=T0+c0/sigma1
                  j=1                                    !线8-E
              else                                       !线9-S
                  ss=c0/sigma1
                  j=1
              end if                                     !线9-E
	  else                           !球在固体中
              if(T0>=0..and.T1>0.) then                  !线1-S
                  if(sigma1*(T1-T0)>c0)then
                      ss=T0+c0/sigma1
                      j=1
                  elseif(sigma1*(T1-T0)+sigma2*(T2-T1)>c0) then
                      ss=T1+(c0-sigma1*(T1-T0))/sigma2
                      j=2
                  else
                      ss=T2+(c0-sigma1*(T1-T0)-sigma2*(T2-T1))/sigma1
                      j=1
                  end if                                 !线1-E
              elseif(c<=0.) then                         !线2-S
                  if(sigma2*T2>c0) then
                      ss=c0/sigma2
                      j=2
                  else
                      ss=T2+(c0-sigma2*T2)/sigma1
                      j=1
                  end if                                 !线2-E
              elseif(T0<0..and.T1>0.) then               !线3-S
                  if(sigma1*T1>c0) then
                      ss=c0/sigma1
                      j=1
                  elseif(sigma1*T1+sigma2*(T2-T1)>c0) then
                      ss=T1+(c0-sigma1*T1)/sigma2
                      j=2
                  else
                      ss=T2+(c0-sigma1*T1-sigma2*(T2-T1))/sigma1
                      j=1
                  end if                                 !线3-E
              elseif((k==0.or.T2<=0.).and.T0>0.) then    !线4-S
                  ss=T0+c0/sigma1
                  j=1                                    !线4-E
              else                                       !线5-S
                  ss=c0/sigma1
                  j=1
              end if                                     !线5-E
          end if
      else             !!!!!!固体->真空
          if(zs<=-rs) then               !球在真空中
	      if(c<=0.) then             !起点在球内或球上  线1-S
                  if(sigma2*T2>c0) then
                      ss=c0/sigma2
                      j=2
                  else
                      j=0
                  end if                                 !线1-E
              elseif(T0>=0..and.T1>=T0) then             !线2-S
                  if(sigma1*T0>c0) then
                      ss=c0/sigma1
                      j=1
                  elseif(sigma1*T0+sigma2*(T2-T1)>c0) then
                      ss=T1+(c0-sigma1*T0)/sigma2
                      j=2
                  else
                      j=0
                  end if                                 !线2-E
              else                                       !线3-S
                  if(sigma1*T0>c0) then
                      ss=c0/sigma1
                      j=1
                  else
                      j=0
                  end if                                 !线3-E
              end if
          elseif(-rs<zs.and.zs<rs) then  !球半嵌在固体中
              if(c<=0..and.T2>=T0) then                  !线1-S
                  if(sigma2*T2>c0) then
                      ss=c0/sigma2
                      j=2
                  else
                      j=0
                  end if                                 !线1-E
              elseif(c<=0..and.T2<T0) then               !线2-S
                  if(sigma2*T2>c0) then
                      ss=c0/sigma2
                      j=2
                  elseif(sigma2*T2+sigma1*(T0-T2)>c0) then
                      ss=T2+(c0-sigma2*T2)/sigma1
                      j=1
                  else
                      j=0
                  end if                                 !线2-E
              elseif(T1>0..and.T0>=T2) then              !线4-S
                  if(sigma1*T1>c0) then
                      ss=c0/sigma1
                      j=1
                  elseif(sigma1*T1+sigma2*(T2-T1)>c0) then
                      ss=T1+(c0-sigma1*T1)/sigma2
                      j=2
                  elseif(sigma1*(T1+T0-T2)+sigma2*(T2-T1)>c0) then
                      ss=T2+(c0-sigma1*T1-sigma2*(T2-T1))/sigma1
                      j=1
                  else
                      j=0
                  end if                                 !线4-E
              elseif(T1>=0..and.T0>=T1.and.T2>=T0) then  !线5-S
                  if(sigma1*T1>c0)then
                      ss=c0/sigma1
                      j=1
                  elseif(sigma1*T1+sigma2*(T2-T1)>c0) then
                      ss=T1+(c0-sigma1*T1)/sigma2
                      j=2
                  else
                      j=0
                  end if                                 !线5-E
              elseif(T1>T0.and.T0>0.) then               !线6-S
                  if(sigma1*T0>c0) then
                      ss=c0/sigma1
                      j=1
                  elseif(sigma1*T0+sigma2*(T2-T1)>c0) then
                      ss=T1+(c0-sigma1*T0)/sigma2
                      j=2
                  else
                      j=0
                  end if                                 !线6-E
              else                                       !线3-S
                  if(sigma1*T0>c0)then
                      ss=c0/sigma1
                      j=1
                  else
                      j=0
                  end if                                 !线3-E
              end if
          else                           !球在固体中
              if(c<=0.) then                             !线1-S
                  if(sigma2*T2>c0) then
                      ss=c0/sigma2
                      j=2
                  elseif(sigma2*T2+sigma1*(T0-T2)>c0) then
                      ss=T2+(c0-sigma2*T2)/sigma1
                      j=1
                  else
                      j=0
                  end if                                 !线1-E
              elseif(T1>=0..and.T0>=T2) then             !线2-S
                  if(sigma1*T1>c0)then
                      ss=c0/sigma1
                      j=1
                  elseif(sigma1*T1+sigma2*(T2-T1)>c0)then
                      ss=T1+(c0-sigma1*T1)/sigma2
                      j=2
                  elseif(sigma1*(T1+T0-T2)+sigma2*(T2-T1)>c0) then
                      ss=T2+(c0-sigma1*T1-sigma2*(T2-T1))/sigma1
                      j=1
                  else
                      j=0
                  end if                                 !线2-E
              else                                       !线3-S
                  if(sigma1*T0>c0) then
                      ss=c0/sigma1
                      j=1
                  else
                      j=0
                  end if                                 !线3-E
              end if
          end if		         !结束球在固体中
      end if       !结束固体->真空
      if(i==0) then
          energy=energy+poine(j)
      elseif(j==0) then
          energy=energy-poine(i)
      else
          energy=energy-poine(i)+poine(j)
      end if
      return
      end
