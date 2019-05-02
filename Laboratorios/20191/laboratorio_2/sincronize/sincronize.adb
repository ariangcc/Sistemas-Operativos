with Ada.Text_IO, Ada.Integer_Text_IO;
use  Ada.Text_IO, Ada.Integer_Text_IO;

procedure dac is 
    CONT : Integer := 0;
    task tarea1 is 
        entry uno_dos;
        entry uno_tres;
    end tarea1;
    
    task tarea2 is
        entry dos_cuatro;
    end tarea2;

    task tarea3;
    task tarea4;
    
    task tarea5 is 
        entry cinco_dos;
        entry cinco_tres;
    end tarea5;
    
    task body tarea1 is 
        K : Integer := 0;
    begin
        for K in Integer range 1..10 loop 
            put(K);
            Put_Line("->Tarea1");
        end loop;
        accept uno_dos;
        accept uno_tres;         
    end tarea1; 
    
    task body tarea2 is 
        K : Integer := 0;
    begin
        tarea1.uno_dos;        
        for K in Integer range 1..10 loop 
            put(K);
            Put_Line("->Tarea2");
        end loop; 
        accept dos_cuatro;        
        tarea5.cinco_dos;
    end tarea2; 

    task body tarea3 is 
        K : Integer := 0;
    begin
        tarea1.uno_tres;
        for K in Integer range 1..10 loop 
            put(K);
            Put_Line("->Tarea3");
        end loop; 
        tarea5.cinco_tres;
    end tarea3; 

    task body tarea4 is 
        K : Integer := 0;
    begin
        tarea2.dos_cuatro;
        for K in Integer range 1..10 loop 
            put(K);
            Put_Line("->Tarea4");
        end loop; 
    end tarea4; 

    task body tarea5 is 
        K : Integer := 0;
    begin
        accept cinco_dos;
        accept cinco_tres;
        for K in Integer range 1..10 loop 
            put(K);
            Put_Line("->Tarea5");
        end loop; 
    end tarea5; 
begin 
    null;
end dac; 