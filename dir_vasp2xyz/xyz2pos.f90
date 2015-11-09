      PROGRAM xyz2pos

      IMPLICIT none

      CHARACTER(80)             :: File
      CHARACTER(2), ALLOCATABLE :: AtomName(:)
      INTEGER                   :: Natoms, i
      INTEGER                   :: Dummy
      REAL                      :: Scale
      REAL, ALLOCATABLE         :: X(:), Y(:), Z(:)

      CALL GET_COMMAND_ARGUMENT(1, File, Dummy)

      IF (Dummy == -1) STOP 'No file specified.'

      WRITE(*,*) 'Enter desired box size: '
      READ(*,*) Scale

      OPEN(12, file=File, status="old")
      READ(12,*) Natoms
      ALLOCATE(AtomName(Natoms))
      ALLOCATE(X(Natoms))
      ALLOCATE(Y(Natoms))
      ALLOCATE(Z(Natoms))
      DO i = 1, Natoms
         READ(12,*) AtomName(i), X(i), Y(i), Z(i)
      ENDDO
      CLOSE(12)

      OPEN(21, file="poscar", status="replace")

      WRITE(21, *) 'Si H'
      WRITE(21, fmt="(F8.2)") Scale
      WRITE(21, *) '1.000  0.000  0.000'
      WRITE(21, *) '0.000  1.000  0.000'
      WRITE(21, *) '0.000  0.000  1.000'
      IF (Natoms == 41) WRITE(21, *) '21  20'
      IF (Natoms == 21) WRITE(21, *) '9  12'
      WRITE(21, *) 'Selective dynamics'
      WRITE(21, *) 'Direct'
      DO i = 1, Natoms
         X(i) = (X(i) / Scale)
         Y(i) = (Y(i) / Scale)
         Z(i) = (Z(i) / Scale)
         WRITE(21, fmt="(3F14.8,A10,A4)") X(i), Y(i), Z(i), 'T  T  T', AtomName(i)
      ENDDO
      WRITE(21, *) ''

      STOP

      END PROGRAM xyz2pos
