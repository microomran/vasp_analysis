      Program adjxy
      implicit none

!*************************************************************************
!   The purpose of this program is to take a POSCAR(CONTCAR) file and
!   changing the size of the xy supercell (i.e. 1x1 to 1x2)
!   This will involve reading in the POSCAR file then adding the new atoms 
!   and writing out a new POSCAR file (mod.POSCAR)

!  ***For now assuming an orthogonal basis set***
!*************************************************************************

      integer Nmax,N(4),Natom,Natom_new,xmult,ymult
      parameter (Nmax = 600,xmult=2,ymult=2)

      real*8 atom(3,Nmax),temp,fx,fy,fx0,fy0

      real*8 scale,A(3),B(3),C(3)
      integer i,j,k,iatm

      character*1 ID(3,1:Nmax)
      character*40 dummy
      character*40 comment

!************************************************************************
      open (unit=10, file='POSCAR', status='old')
      open (unit=20, file='mod.POSCAR', status='new')

      iatm = 1   ! set this to the number of species in your POSCAR
!  Read the contents of the POSCAR file

      read(10,'(A25)') comment
      read(10,*) scale
      read(10,*) A(1),A(2),A(3)
      read(10,*) B(1),B(2),B(3)
      read(10,*) C(1),C(2),C(3)
      read(10,*) (N(i),i=1,iatm)
      read(10,*) dummy
      read(10,*) dummy

      Natom = SUM(N(1:iatm))
      Natom_new = xmult*ymult*Natom

      do i=1,Natom
         read(10,*) atom(1,i),atom(2,i),atom(3,i),  &
     &              ID(1,i),ID(2,i),ID(3,i)

!   have to apply PBC to make sure all the x,y frac coor are between [0,1)
         
         if (atom(1,i).lt.0.d0) atom(1,i) = atom(1,i) + 1.d0
         if (atom(2,i).lt.0.d0) atom(2,i) = atom(2,i) + 1.d0
         if (atom(1,i).ge.1.d0) atom(1,i) = atom(1,i) - 1.d0
         if (atom(2,i).ge.1.d0) atom(2,i) = atom(2,i) - 1.d0

      enddo


     
!  We have read the poscar coordinates and now we know that we must write out the
!  new coordinates

      do i=1,iatm
         N(i) = N(i)*xmult*ymult
      enddo
      do i=1,2
         A(i) = A(i)*xmult
         B(i) = B(i)*ymult
      enddo

21    FORMAT (f16.12,3x,f16.12,3x,f16.12)
22    FORMAT (4(I3,2x))
23    FORMAT (f18.16,3x,f18.16,3x,f18.16,2x,'T',2x,'T',2x,'T')
24    FORMAT (f18.16,3x,f18.16,3x,f18.16,2x,'F',2x,'F',2x,'F')

      write(20,'(A25)') comment
      write(20,*) scale
      write(20,21) A(1),A(2),A(3)
      write(20,21) B(1),B(2),B(3)
      write(20,21) C(1),C(2),C(3)
      write(20,22) (N(i),i=1,iatm)
      write(20,*) 'Select Dynamics'
      write(20,*) 'Direct'

      do i=1,Natom
         fx0 = atom(1,i)/(xmult*1.d0)
         fy0 = atom(2,i)/(ymult*1.d0)
         do j=1,xmult
            fx = fx0 + (j-1)*(1.d0/(xmult*1.d0))
            do k=1,ymult
               fy = fy0 + (k-1)*(1.d0/(ymult*1.d0))
                  if (ID(3,i).eq.'T') then
                     write(20,23) fx,fy,atom(3,i)
                  else
                     write(20,24) fx,fy,atom(3,i)
                  endif
            enddo
         enddo
      enddo

      stop
      end PROGRAM adjxy
