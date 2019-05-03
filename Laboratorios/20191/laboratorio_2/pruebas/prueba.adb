with Ada.Text_IO; use Ada.Text_IO;
with Ada.Integer_Text_IO; use Ada.Integer_Text_IO;


procedure prueba is 

    start : Integer := 0;

    task type Server is 
        entry S1(paramIn : in Integer); 
        entry S2(paramOut : out Integer);
        entry S3(paramInOut : in out Integer);
    end Server;

    task type Client; 

    task body Server is 
        value : Integer := 10;
        aux : Integer;
    begin 
        select 
            accept S1(paramIn : in Integer) do 
                value := paramIn;
            end S1; 
            or 
            accept S2(paramOut : out Integer) do 
                paramOut := value; 
            end S2; 
            or 
            accept S3(paramInOut : in out Integer) do 
                aux := value; 
                value := paramInOut; 
                paramInOut := aux;
            end S3;
        end select;
    end Server;-- PAGE 110

    task body Client is 
    begin 
        if start = 0 then 
        elsif start > 0 then
        end if; 
    end Client; 

    arrayCli : Client; 
begin 
    null;
end prueba;