with Ada.Text_IO, Ada.Integer_Text_IO;
use  Ada.Text_IO, Ada.Integer_Text_IO;
-- hacer un arreglo de tareas? es el ejercicio 1 de la guia, en tasks
procedure Principal is 
    arr : array (1..10) of Integer := (others => 0);
begin 
    for i in Integer range 1..10 loop
        put(arr(i));
    end loop;
end Principal; 