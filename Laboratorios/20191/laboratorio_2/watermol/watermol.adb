with Ada.Text_IO, Ada.Integer_Text_IO;
use  Ada.Text_IO, Ada.Integer_Text_IO;

procedure watermol is
    
    task Molecula is 
        entry Oxigeno; 
        entry Hidrogeno; 
    end Molecula; 

    task type H;
    task type O;

    protected Water is 
        entry Hidrogeno;
        entry Oxigeno; 
        entry Molecula; 
    private 
        O,H : Integer := 0; 
    end Water; 

    task body Molecula is 
    begin 
        for i in 1..10 loop 
            Water.Molecula; -- hay molecula? si hay reinicio
            accept Oxigeno; --aea
            accept Hidrogeno; 
            accept Hidrogeno;
        end loop;
    end Molecula;

    task body H is 
    begin 
        Water.Hidrogeno; -- hay 2 hidrogeno? los creo 
        Molecula.Hidrogeno; -- ya cree 1 hidrogeno papi
    end H;

    task body O is 
    begin 
        Water.Oxigeno;-- hay 1 de oxigeno? lo creo
        Molecula.Oxigeno; -- ya cree oxigeno pa
    end O;

    protected body Water is 
        entry Hidrogeno when H<2 is 
        begin 
            H := H+1; 
            put('H');
        end Hidrogeno;

        entry Oxigeno when O<1 is 
        begin 
            O := O+1;
            put('O');
        end Oxigeno;

        entry Molecula when H=2 and O=1 is
        begin 
            put_line("");
            put_line("Mol agua");
            H:=0;
            O:=0;
        end Molecula;
    end Water; 

    Oxigeno: array(1..10) of O; 
    Hidrogeno: array(1..20) of H;
begin
    null;
end watermol;