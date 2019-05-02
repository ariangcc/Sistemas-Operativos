with Ada.Text_IO; use Ada.Text_IO; 
with Ada.Integer_Text_IO; use Ada.Integer_Text_IO;

procedure contador is 
    CONT : Integer := 0;
    NO_TERMINA1, NO_TERMINA2 : Boolean := True;


    protected Contador is 
        function Lee Return Integer;
        procedure Incrementa;
    private
        Cont : Integer := 0;
    end Contador; 

    protected body Contador is 
        procedure Incrementa is 
        begin 
            Cont := Cont + 1; 
        end Incrementa; 

        function Lee return Integer is 
        begin 
            return Cont; 
        end Lee; 

    end Contador; 
    -- tareas
    task tarea1 is end tarea1;
    task tarea2 is end tarea2; 

    task body tarea1 is 
        K : Integer := 0;
    begin 
        while K < 1_000_000 loop
            -- CONT := CONT + 1;
            Contador.Incrementa;
            K := K + 1;
        end loop;
        NO_TERMINA1 := False;
    end tarea1; 

    task body tarea2 is 
        K : Integer := 0;
    begin 
        while K < 1_000_000 loop 
            -- CONT := CONT + 1;
            Contador.Incrementa; 
            K := K + 1;
        end loop;
        NO_TERMINA2 := False;
    end tarea2;

begin 
    While NO_TERMINA1 or NO_TERMINA2 loop
        null;
    end loop; 

    Put_Line("La suma es: "); 
    --put(CONT);
    put(Contador.Lee);
end contador;
