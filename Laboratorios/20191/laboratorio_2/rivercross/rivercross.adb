with Ada.Text_IO; use Ada.Text_IO;
with Ada.Integer_Text_IO; use Ada.Integer_Text_IO;


procedure rivercross is 
    protected mutex is 
        entry hackers(isCap : out Boolean); 
        entry serfs(isCap : out Boolean);
    private 
        nH, nS : Integer := 0;
    end mutex;

    protected body mutex is 
        entry hackers(isCap : out Boolean) when nH <= 4 is 
        begin 
            nH := nH + 1;
            if nH = 4 then 
                nH := 0;
                put_line("4 hackers sarparon");
                isCap := True;
            elsif nH = 2 and nS >= 2 then 
                nH := 0; nS := nS - 2;
                put_line("2 hackers y 2 nerfs sarparon");
                isCap := True;
            end if;
        end hackers; 

        entry serfs(isCap : out Boolean) when nS <= 4 is 
        begin 
            nS := nS + 1;
            if nS = 4 then 
                nS := 0;
                put_line("4 hackers sarparon");
                isCap := True;
            elsif nS = 2 and nH >= 2 then 
                nS := 0; nH := nH - 2;
                put_line("2 hackers y 2 nerfs sarparon");
                isCap := True;
            end if;
        end serfs; 
    end mutex;

    task type hacker; 
    task type serf; 

    task body hacker is
        isCap : Boolean := False; 
    begin 
        mutex.hackers(isCap);
        if isCap then 
            put_line("Capitan Hacker");
        end if; 
    end hacker;

    task body serf is
        isCap : Boolean := False; 
    begin 
        mutex.serfs(isCap);
        if isCap then 
            put_line("Capitan Serf");
        end if; 
    end serf;

    hackers: array(1..8) of hacker;
    serfs: array(1..8) of serf;
begin 
    null;
end rivercross;