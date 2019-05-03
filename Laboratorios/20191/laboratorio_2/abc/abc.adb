with Ada.Text_IO; use Ada.Text_IO; 
with Ada.Integer_Text_IO; use Ada.Integer_Text_IO;

procedure abc is
    numA : Integer := 0; 
    numBC : Integer := 0;
    task Ta is 
        entry mayorA(flg :out Integer); 
    end Ta; 
 
    task Tb is 
        entry terminaB;
    end Tb; 

    task Tc is 
        entry terminaC;
    end Tc; 


    task body Ta is 
    
    begin 
        for i in 1..10 loop
            accept mayorA(flg : out Integer) do 
                flg := numA - numBC;
            end mayorA;
            put("A");
            numA := numA + 1;
        end loop; 
    end Ta; 

    task body Tb is    
        flg : Integer;   
    begin 
        loop 
            Ta.mayorA(flg); 
            if flg >= 0 then
                put("B");
                numBC := numBC + 1;
                accept terminaB; 
                Tc.terminaC;
            end if;
        end loop;
    end Tb;

    task body Tc is 
        flg : Integer;        
    begin 
        loop
            Ta.mayorA(flg);
            if flg >= 0 then 
                Tb.terminaB;
                put("C"); 
                numBC := numBC + 1;
                accept terminaC;
            end if;
        end loop;
    end Tc;
begin
    null;
end abc;