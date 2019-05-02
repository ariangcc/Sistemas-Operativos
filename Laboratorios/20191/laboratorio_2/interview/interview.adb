with Ada.Text_IO; use Ada.Text_IO;
with Ada.Integer_Text_IO; use Ada.Integer_Text_IO;
with Ada.Task_Identification; use Ada.Task_Identification;
with Text_IO, Ada.Strings.Fixed, Ada.Strings.Bounded, Ada.Strings.Unbounded;
use Text_IO, Ada.Strings.Fixed, Ada.Strings.Bounded, Ada.Strings.Unbounded;

procedure interview is 
    protected Mutex is
        entry QuestionStart;
        procedure QuestionDone;
    private
        puertaAbierta: Boolean:= True;
    end Mutex;

    protected body Mutex is
        entry QuestionStart when puertaAbierta is
        begin
            puertaAbierta:= False;
        end;
        procedure QuestionDone is
        begin
            puertaAbierta:= True;
        end;
    end Mutex;

    task  Teacher is
        entry WaitQuestion(N:String:="Students(__)");
    end Teacher;

    task type Student;

    task body Teacher is
        Name: String:="Students(__)";
    begin
        for i in 1..10 loop
            accept WaitQuestion(N: String:="Students(__)") do
                Name := N;
            end WaitQuestion; -- espera pregunta , luego guarda nombre del allumno
            Put_Line("Respuesta a:" & Name); 
        end loop;
    end Teacher;

    task body Student is
        Name: String:="Students(__)";
    begin
        Mutex.QuestionStart;--en la entrada question start entra y cierra la puerta con flag
        Name := Slice(to_unbounded_string(Image(Current_Task)),1,12);
        Put_Line("Pregunta:" & Name);
        Teacher.WaitQuestion(Name);-- realiza la prefunta al teacher dando su nombre
        Mutex.QuestionDone;-- ciando acaba poone flag a 1 pueden entrar mas alumnos
    end Student;

    students: array(1..10) of Student;
begin 
    null;
end interview;
