      PROGRAM vasp2xyz

!     Convert VASP-style files (POSCAR or CONTCAR) to xyz format
!     with option for creating periodic images
!
!     ----------------------------------------------------------
!
!     Jeffery M. Hawkins
!     Department of Chemical Engineering
!     University of Florida
!     Email: jhawkins@che.ufl.edu
!
!     ----------------------------------------------------------
!
!     Version 1.0: 27 September 2007
!     Version 1.1: 13 December 2007
!                  Fix for non-orthogonal basis vectors
!     Version 1.2: 13 July 2009
!                  Fix for basis vectors where c-axis is not along z-axis
!
!     ----------------------------------------------------------

      IMPLICIT NONE

      REAL(KIND=8) :: Scale, x0, y0, z0
      REAL(KIND=8) :: Abasis(1:3), Bbasis(1:3), Cbasis(1:3)
      REAL(KIND=8), ALLOCATABLE :: x(:), y(:), z(:)

      INTEGER :: Foo, PerImgs, Ntypes, Tatoms1, Tatoms2
      INTEGER :: i, j, k, q
      INTEGER, ALLOCATABLE :: Natoms(:)

      CHARACTER(80) :: File
      CHARACTER(5) :: Flag
      CHARACTER(2), ALLOCATABLE :: AtomType(:), Label(:)
      CHARACTER(20) :: Bar

      CALL GET_COMMAND_ARGUMENT(1, File, q, Foo)
      IF (Foo /= 0) STOP 'No file specified.'

      PerImgs = 2

      OPEN (11, File=File, Status="old")

      CALL SYSTEM('head -6 ' // File // ' | tail -1 | wc -w > temp')
      OPEN (12, File="temp", Status="old")
      READ (12,*) Ntypes
      CLOSE (12)
      CALL SYSTEM('rm temp')

      ALLOCATE (AtomType(Ntypes))
      READ (11,*) (AtomType(i), i=1,Ntypes)

      READ (11,*) Scale
      READ (11,*) Abasis(1), Abasis(2), Abasis(3)
      READ (11,*) Bbasis(1), Bbasis(2), Bbasis(3)
      READ (11,*) Cbasis(1), Cbasis(2), Cbasis(3)

      DO i=1,3
         Abasis(i) = Abasis(i) * Scale
         Bbasis(i) = Bbasis(i) * Scale
         Cbasis(i) = Cbasis(i) * Scale
      ENDDO

      ALLOCATE (Natoms(Ntypes))
      READ (11,*) (Natoms(i), i=1,Ntypes)

      READ (11,*) Bar
      READ (11,*) Bar

      Tatoms1 = SUM(Natoms)
      Tatoms2 = Tatoms1 * PerImgs * PerImgs

      ALLOCATE (x(Tatoms2))
      ALLOCATE (y(Tatoms2))
      ALLOCATE (z(Tatoms2))
      ALLOCATE (Label(Tatoms2))

      q = 1
      DO i=1,Tatoms1
         READ (11,*) x(i), y(i), z(i)         
         IF (z(i) < 0.60)  z(i) = z(i) + 1.d0
         x0 = x(i)*Abasis(1) + y(i)*Bbasis(1) + z(i)*Cbasis(1)
         y0 = x(i)*Abasis(2) + y(i)*Bbasis(2) + z(i)*Cbasis(2)
         z0 = x(i)*Abasis(3) + y(i)*Bbasis(3) + z(i)*Cbasis(3)
         x(i) = x0
         y(i) = y0
         z(i) = z0
         IF (i == SUM(Natoms(1:q))+1)  q = q + 1
         Label(i) = AtomType(q)
      ENDDO

      CLOSE (11)

      OPEN (12, File=TRIM(File)//".xyz", Status="replace") 
      WRITE(12,*) Tatoms2
      WRITE(12,*) " "

      q = 0
      DO j=0,PerImgs-1
         DO k=0,PerImgs-1
            DO i=1,Tatoms1
               q = q + 1
               x(q) = x(i) + FLOAT(j)*Abasis(1) + FLOAT(k)*Bbasis(1)
               y(q) = y(i) + FLOAT(j)*Abasis(2) + FLOAT(k)*Bbasis(2)
               z(q) = z(i) + FLOAT(j)*Abasis(3) + FLOAT(k)*Bbasis(3)
               Label(q) = Label(i)
               WRITE(12, FMT="(A5,3F14.8)") Label(q), x(q), y(q), z(q)
            ENDDO
         ENDDO
      ENDDO

      STOP 'Successful.'

      END PROGRAM vasp2xyz
