with Ada.Text_IO; use Ada.Text_IO; 
with Ada.Integer_Text_IO; use Ada.Integer_Text_IO; 
with Ada.Task_Identification; use Ada.Task_Identification;

procedure dining is 
    
    protected forks is 
        entry take_forks(nphil : in Integer);
        procedure put_forks(nphil : in Integer);
    private
    forksArr : array (1..5) of Integer := (others => 1);
    end forks;

    protected body forks is 
        entry take_forks(nphil : in Integer) when forksArr(nphil)=1 and forksArr(nphil+1 mod 5)=1 is
        begin 
            forksArr(nphil) := 0;
            forksArr(nphil+1 mod 5) := 0;
        end take_forks;  

        procedure put_forks(nphil : in Integer) is 
        begin 
            forksArr(nphil) := 1;
            forksArr(nphil+1 mod 5) := 1;
        end put_forks;
    end forks; 

    task type philosopher; 

    task body philosopher is 
    begin 
        forks.take_forks(Integer'Value(Current_Task));
        put_line("Philosopher "& Integer'Image(Current_Task)&" Eating");
        forks.put_forks(Integer'Value(Current_Task));
    end philosopher;

    philosophers : array (1..5) of Philosopher;
begin 
    null;
end dining; 