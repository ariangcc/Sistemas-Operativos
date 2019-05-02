with Ada.Text_IO; use Ada.Text_IO;
with Ada.Integer_Text_IO; use  Ada.Integer_Text_IO;

procedure Main is
   function esPrimo (numero : in Integer) return Integer is
      dev : Integer;
   begin
      dev := 1;
      for i in Integer range 2 .. (numero-1) loop
         if ((numero rem i) = 0) then
            dev := 0;
         end if;
      end loop;
      return dev;
   end esPrimo;
   num : Integer;
begin


   Ada.Text_IO.Put_Line("Ingrese un limite: ");
   get(num);

   for i in Integer range 2 .. (num) loop
      if ((esPrimo(i)=1)) then
         Ada.Text_IO.Put_Line(Integer'Image(i));
      end if;
   end loop;


end Main;
