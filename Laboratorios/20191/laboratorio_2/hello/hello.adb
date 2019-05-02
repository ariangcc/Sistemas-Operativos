with Ada.Text_IO; use Ada.Text_IO;

procedure Hello is
    a,b,c : INTEGER;
begin 
    a := Integer'Value(Get_Line);
    b := Integer'Value(Get_Line);
    c := a + b;
    Put_Line("The value of c is: " & Integer'image(c)); 

end Hello; 