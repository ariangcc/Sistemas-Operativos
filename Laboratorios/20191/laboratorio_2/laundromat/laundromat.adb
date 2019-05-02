with Ada.Text_IO; use Ada.Text_IO;
with Ada.Integer_Text_IO; use Ada.Integer_Text_IO;

procedure laundromat is 
    -- un cliente a la vez usa las maquinas 
    -- arreglo de n maquinas, todas inicializadas en 1
    -- 
    N_MAQUINAS : Integer := 5;
    maquinas : array(1..N_MAQUINAS) of Integer := (others => 1);

    --obj protegido lavanderia 
    protected lavanderia is 
        entry allocate(index: in out Integer); 
        entry release (index: in out Integer);
    private 
        nfree : Integer := N_MAQUINAS;
    end lavanderia; 

    protected body lavanderia is 
        entry allocate(index : in out Integer) when nfree > 0 is 
            i : Integer := 1;
        begin 
            nfree := nfree - 1;
            while i<=N_MAQUINAS and maquinas(i)=0 loop
                i := i + 1;
            end loop; 
            if i<=N_MAQUINAS then
                maquinas(i) := 0;
                put_line("Maquina " & Integer'Image(i) & " en uso");
                --se avisa qué maquina se agarró
                index := i; 
            end if;
        end allocate;

        entry release(index : in out Integer) when nfree < N_MAQUINAS is 
        begin 
            maquinas(index) := 1;
            nfree := nfree + 1;  
            put_line("Maquina " & Integer'Image(index) & " libre");        
        end release; 
    end lavanderia;

    task type cliente;

    task body cliente is 
        i : Integer := 0;
    begin 
        lavanderia.allocate(i); 
        -- comienza a lavar
        delay Duration(3);
        lavanderia.release(i);
    end;

    clientes : array(1..10) of cliente;

begin 
    null;
end laundromat;