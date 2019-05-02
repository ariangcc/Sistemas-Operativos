with Ada.Text_IO, Ada.Integer_Text_IO;
use  Ada.Text_IO, Ada.Integer_Text_IO;

procedure imprime_primos is 
    num : Integer; 

    function es_primo(N : Integer) return Boolean is 
        R : Boolean := TRUE;
        I : Integer := 2; 
    begin 
        while(I * I <= N) and R loop 
            if(N mod I) = 0 then 
                R := FALSE;
            end if;
            I := I + 1; 
        end loop; 
        return R; 
    end es_primo;

begin
    put("Ingrese un numero: "); 
    get(num);
    -- put(num);
    for I in Integer range 2..num loop 
        if es_primo(I) then put(I,3); 
        end if;
    end loop;

end imprime_primos;