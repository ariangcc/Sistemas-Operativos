with Ada.Text_IO; use Ada.Text_IO;
with Ada.Integer_Text_IO; use Ada.Integer_Text_IO;


procedure prueba is 

    protected mutex is 
        entry ent1; 
    private 
        n : Integer := 0;
    end mutex; 

    protected body mutex is 
        
    end mutex;
    
    task type tsk;

    tasks : array (1..5) of tsk;
begin 
    null;
end prueba;