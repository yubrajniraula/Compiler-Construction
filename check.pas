PROGRAM CHECK1;
VAR
  A : INTEGER;
  B : INTEGER;
  C : INTEGER;
  D : INTEGER;
  
BEGIN
  WRITE('THE FIRST PART OF THE PROGRAM PERFORMS SWAPPING OF VARIABLES A AND B');
  WRITE('PLEASE ENTER THE FIRST NUMBER FOR A(4)');
  READ(A);
  WRITE('PLEASE ENTER THE SECOND NUMBER FOR B(6)');
  READ(B);
  C := A;
  A := B;
  B := C;
  D := 100;
  WRITE('THE VALUE IS SWAPPED: A IS ');
  WRITE(A);
  WRITE('B IS');
  WRITE(B);
  WRITE('
  
  THE SECOND PART OF THE PROGRAM PERFORMS OPERATIONS +, -, * AND /');
  WRITE('ENTER THE BASE INTEGER');
  C := ((A * B) + (B - A)) / D;
  WRITE('THE RESULT IS ');
  WRITE(C);  

  WRITE('
  THE THIRD PART PERFORMS THE < OR > OPERATIONS');
  IF A < B THEN
    BEGIN
        WRITE(A);
  	    WRITE(' IS SMALLER')
    END
  ELSE
    BEGIN
      WRITE(A);
      WRITE (' IS GREATER')
    END;
  IF B < A THEN
    BEGIN
        WRITE(B);
  	    WRITE(' IS SMALLER')
    END;
    WRITE('
    
    THE FOURTH AND LAST PROGRAM PERFORMS WHILE LOOP CHECKING ');
  WRITE('ENTER VALUE 1');
  READ(A);
  WRITE('ENTER VALUE 100');
  READ(B);
  WHILE A < B
  BEGIN
    WRITE(A);
    A := A+1
  END

END