with Ada.Text_IO; use Ada.Text_IO; 

procedure barberSleep is 


    nWaiting : Integer := 0;

    task barber is 
        entry cutHair;
    end barber;     

    task customer is 
        entry getHairCut;
    end customer;
    

    task body barber is  
    begin
        for i in 1..10 loop 
            customer.getHairCut; -- espera a un curtomer 
            accept cutHair do 
                put_line("cortando cabeio");
                nWaiting := nWaiting - 1;
            end cutHair; -- espera a que lo despierte
        end loop; 
    end barber; 

    task body customer is 
    begin 
        loop
            select 
            when nWaiting < 3 =>
                accept getHairCut do
                    barber.cutHair; -- despierta a berber
                end getHairCut; 
                nWaiting := nWaiting + 1;
            or 
                null;
            end select; 
        end loop;
    end customer;



    --barber1 : barber;
    --curtomers : array (1..10) of customer;
begin 
    null;
end barberSleep; 