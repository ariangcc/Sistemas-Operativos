with Ada.Text_IO; use Ada.Text_IO; 
with Ada.Integer_Text_IO; use Ada.Integer_Text_IO; 


procedure prod_cons is 
    task producer is
        --entry send(ms : in Integer); 
        entry receive(ms : in Integer);
    end producer;

    task consumer is 
        entry send(ms : in Integer); 
        --entry receive(ms : in Integer);
    end consumer;

    task body producer is 
        newms : Integer;
    begin
        for i in 1..10 loop 
            accept receive(ms: in Integer) do
                newms := ms + 1; 
            end receive;
            put_line("ms enviado" & Integer'Image(newms));
            consumer.send(newms);
        end loop;
    end producer;

    task body consumer is 
        msCons : Integer := 10;        
    begin 

        for i in 1..10 loop
            producer.receive(msCons);
            accept send(ms : in Integer) do
                msCons := ms;
            end send;
            put_line("ms recib" & Integer'Image(msCons));
        end loop;    
    end consumer; 

begin 
    null;
end prod_cons;