!        注意数组维数和类型
integer,allocatable,dimension(:        ) :: nbe,icma,isp
integer,allocatable,dimension(:,:      ) :: ibande,ibandbe,ibandfe,isphre,ibante,ibantbe,ibantfe,nu
integer,allocatable,dimension(:,:,:    ) :: inuma,isqure
integer,allocatable,dimension(:,:,:,:  ) :: nc
integer,allocatable,dimension(:,:,:,:,:) :: inumbantb,inumbantf,inumcircl
character,allocatable,dimension(:,:,:  ) :: indx*3
real*4 ,allocatable,dimension(:        ) :: edm,sumban,sumfan,sumnu
real*4 ,allocatable,dimension(:,:,:    ) :: sumnc
real*4 ,allocatable,dimension(:,:,:,:  ) :: sumbant,sumfant
real*8 ,allocatable,dimension(:        ) :: solid_angle,theta,phi
real*8 pai,rad,solid,a0,a1,b0,b1,t0,t1,p0,p1
character fname*3

!===========================================================输入数据文件
open(5,file='n(e).dat',status='old',form='binary')
read(5) estart,estopm,poine,nmax,theta0
read(5) idive,eunit,idiva,aunit,idivb,bunit
!        求出能量增部分的格点数
estopm=estopm-poine; e_offset=idive*eunit-(estart-estopm); igain=int(e_offset/eunit)
!        分配数组：能量格点数idivx大于（无能量增时）idive以便求能量上端的卷积
idivx=idive-igain+max(200,igain)
allocate (nbe(idivx),icma(idivx),ibande(idiva,idivx),ibandbe(idiva,idivx),ibandfe(idiva,idivx), &
	  isqure(idiva,4*idiva,idivx),isphre(4*idiva,idivx),inuma(4*idiva,idivx,5), &
	  ibante(idivb,idivx),ibantbe(idivb,idivx),ibantfe(idivb,idivx))
allocate (inumbantb(idivb,idivx,0:2,0:2,0:2),inumbantf(idivb,idivx,0:2,0:2,0:2),inumcircl(4*idiva,idivx,0:2,0:2,0:2))
!        开始读取
read(5) (nbe  (i),i=1,idive)
read(5) (icma (i),i=1,idive)
read(5) ((ibande (i,j),j=1,idive),i=1,idiva)
read(5) ((ibandbe(i,j),j=1,idive),i=1,idiva)
read(5) ((ibandfe(i,j),j=1,idive),i=1,idiva)
read(5) (((isqure(i,j,k),k=1,idive),j=1,4*idiva),i=1,idiva)
read(5) ((isphre (i,j),j=1,idive),i=1,4*idiva)
read(5) (((inuma (i,j,k),k=1,5),j=1,idive),i=1,4*idiva)
read(5) ((ibante (i,j),j=1,idive),i=1,idivb)
read(5) ((ibantbe(i,j),j=1,idive),i=1,idivb)
read(5) ((ibantfe(i,j),j=1,idive),i=1,idivb)
read(5) (((((inumbantb(i,j,k1,k2,k3),k3=0,2),k2=0,2),k1=0,2),j=1,idive),i=1,idivb)
read(5) (((((inumbantf(i,j,k1,k2,k3),k3=0,2),k2=0,2),k1=0,2),j=1,idive),i=1,idivb)
read(5) (((((inumcircl(i,j,k1,k2,k3),k3=0,2),k2=0,2),k1=0,2),j=1,idive),i=1,4*idiva)
close(5)
write(*,*) nmax

deallocate(nbe,icma,isqure)

!===========================================================
allocate (edm(idivx),sumban(idivb),sumfan(idivb))
allocate (solid_angle(idivb),theta(0:idivb),phi(0:idivb))
pai=dacos(-1.d0); rad=pai/180.d0

my_angle=6; thet=my_angle*aunit/4; circl=2*pai*(1-cos(thet)); alph=acos(29./30.)
print *, '探测极角: 实验=',alph/rad,'    模拟=',thet/rad
my_event=3 !  环探测极角下总非弹性散射次数范围
my_count=2 !环带探测极角下分非弹性散射次数范围，数组分配依次为：真空中入射，真空中出射，固体

allocate(nu(idivx,my_event),isp(idivx),sumnu(my_event))
allocate(nc(idivx,0:my_count,0:my_count,0:my_count))
allocate(sumbant(idivb,0:my_count,0:my_count,0:my_count))
allocate(sumfant(idivb,0:my_count,0:my_count,0:my_count))
allocate(indx (0:my_count,0:my_count,0:my_count))
allocate(sumnc(0:my_count,0:my_count,0:my_count))
!===========================================================
do i=0,my_count
	do j=0,my_count
		do k=0,my_count
			write(indx(i,j,k),'(3i1)') i,j,k
		end do
	end do
end do
isp=0; nu=0; nc=0
do i=1,idive
	do j=1,my_angle
		isp(i)=isp(i)+isphre(j,i)
		forall(k=1:my_event) nu(i,k)=nu(i,k)+inuma(j,i,k)
		forall(k1=0:my_count,k2=0:my_count,k3=0:my_count)
			nc(i,k1,k2,k3)=nc(i,k1,k2,k3)+inumcircl(j,i,k1,k2,k3)
		end forall
	end do
end do

deallocate(isphre,inuma,inumcircl)

theta(0)=0.d0; phi(0)=0.d0
do i=1,idivb
	theta(i)=i*pai/2/idivb;	t0=theta(i-1); t1=theta(i)
	phi(i)=dacos(dsin(theta(1))/dsin(t1));	p0=phi(i-1); p1=phi(i)
	a0=dsin(p0)/dcos(theta(1)); if(dabs(a0)>1.d0) a0=dsign(1.d0,a0)
	a1=dsin(p1)/dcos(theta(1)); if(dabs(a1)>1.d0) a1=dsign(1.d0,a1)
	b0=dtan(theta(1))*dtan(p1); if(dabs(b0)>1.d0) b0=dsign(1.d0,b0)
	b1=dtan(theta(1))*dtan(p0); if(dabs(b1)>1.d0) b1=dsign(1.d0,b1)
	solid=(p1-pai/2)*(dcos(t1)-dcos(t0))+dcos(t0)*(p1-p0)+dasin(a0)-dasin(a1)+dsin(theta(1))*(dasin(b0)-dasin(b1))
	solid_angle(i)=2*solid
end do

edm=(/(estopm+i*eunit,i=1,idivx)/)
!      用高斯函数作为卷积的权重: 能量分辨率(FWHM)为0.75eV
de=0.75/1000; sigma=dble(de)/2.d0/dsqrt(2.d0*dlog(2.d0))

open(1,file='Angular(E)_band.dat')
open(2,file='Angular(E)_cirl.dat')
open(3,file='Frequen(E)_cirl.dat')

bunit=bunit/rad; no_use=(my_count+1)**3
do i=1,idivb,4
	write(fname,'(f3.0)') 90-(i-1)*bunit
	open(10+i/2,file='(E-n)_band-'//fname//'dat')
	open(50+i/2,file='(E-n)_fand-'//fname//'dat')
	write(10+i/2,'(a,<no_use>(4x,a3,4x))') ' E-loss',indx
	write(50+i/2,'(a,<no_use>(4x,a3,4x))') ' E-loss',indx
end do
	write(3     ,'(a,<no_use>(4x,a3,4x))') ' E-loss',indx

do i=1,idivx
	ei=edm(i)
	sumarea=0.; sumsp=0.; sumban=0.; sunfan=0.; sumbant=0.; sumfant=0.; sumnu=0.; sumnc=0.
	do j=1,idivx
		ej=edm(j)
		weight=dexp(-(dble(ei-ej)/sigma)**2/2.d0)
		sumarea=sumarea+weight
		cw=weight/circl
		sumsp=sumsp+cw*isp(j)
		forall(k=1:my_event) sumnu(k)=sumnu(k)+cw*nu(j,k)
		forall(k1=0:my_count,k2=0:my_count,k3=0:my_count)
			sumnc(k1,k2,k3)=sumnc(k1,k2,k3)+cw*nc(j,k1,k2,k3)
		end forall
		do k=1,idivb
			cw=weight/solid_angle(k)
			sumban(k)=sumban(k)+cw*ibantbe(k,j)
			sumfan(k)=sumfan(k)+cw*ibantfe(k,j)
		end do
		do k=1,idivb,4
			cw=weight/solid_angle(k)
			forall(k1=0:my_count,k2=0:my_count,k3=0:my_count)
				sumbant(k,k1,k2,k3)=sumbant(k,k1,k2,k3)+cw*inumbantb(k,j,k1,k2,k3)
				sumfant(k,k1,k2,k3)=sumfant(k,k1,k2,k3)+cw*inumbantf(k,j,k1,k2,k3)
			end forall
		end do
	end do
	ce=10000/sumarea/nmax
	e_loss=(edm(idive-igain)-ei)*1000
	sumban=ce*sumban; sumbant=ce*sumbant; sumnu=ce*sumnu
	sumfan=ce*sumfan; sumfant=ce*sumfant; sumnc=ce*sumnc; sumsp=ce*sumsp
	if(-1.<=e_loss .and. e_loss<=1.) then
		sumban=sumban/20.; sumfan=sumfan/20.
	end if
	write(1,'(f7.3,<2*idivb>es11.4)') e_loss,sumban(idivb:1:-1),sumfan(1:idivb)
	write(2,'(f7.3,<1+my_event>es11.4)') e_loss,sumsp,sumnu(1:my_event)
	write(3,'(f7.3,<no_use>es11.4)') e_loss,sumnc
	do k=1,idivb,4
		write(10+k/2,'(f7.3,<no_use>es11.4)') e_loss,sumbant(k,:,:,:)
		write(50+k/2,'(f7.3,<no_use>es11.4)') e_loss,sumfant(k,:,:,:)
	end do
end do
close(1);close(2);close(3)
do i=1,idivb,4
	close(10+i/2);close(50+i/2)
end do
end