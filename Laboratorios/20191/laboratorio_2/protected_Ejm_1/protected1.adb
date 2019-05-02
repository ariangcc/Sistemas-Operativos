with Ada.Text_IO; use Ada.Text_IO;
-- las llamadas a los subprogramas son mutuamente exclusivos

procedure principal is 

    -- el objeto protegido variable brinda acceso protegido a la variable privada V
    protected Variable is 
        procedure Read(X : out Integer);
        procedure Write(X : in Integer);   
    private 
        
        V : Integer := 10;
    end Variable;

    protected body Variable is 
        procedure Read(X: out Integer) is 
        begin
            X := V; 
        end; 

        procedure Write(X: in Integer) is 
        begin 
            V := X;
        end;
    end Variable;

    -- Tareas 
    task tarea1



begin 


end principal; 