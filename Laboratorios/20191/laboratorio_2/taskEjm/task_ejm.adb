with Ada.Text_IO; use Ada.Text_IO;

procedure Principal is 
    task Tarea1; 
    task Tarea2; 

    task body Tarea1 is 
        final : Positive := 10; 
    begin
        for count in 1..final loop
            Put_Line("Tarea1");
        end loop;
    end Tarea1;

    task body Tarea2 is 
        final : Positive := 10;
    begin 
        for count in 1..final loop
            Put_Line("Tarea2");
        end loop; 
    end Tarea2; 

begin 
    for count in 1..10 loop 
        Put_Line("Tarea Principal");
    end loop;

end Principal;