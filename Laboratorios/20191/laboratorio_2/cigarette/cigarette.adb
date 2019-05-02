with Ada.Text_IO, Ada.Integer_Text_IO;
use  Ada.Text_IO, Ada.Integer_Text_IO;

procedure cigarette is 
    task agente is
        entry agenteSem;
    end;

    task tiene_tabaco is
        entry tabacoSem;
    end;

    task tiene_cerillos is
        entry cerillosSem;
    end;

    task tiene_papel is
        entry papelSem;
    end;

    task body agente is
        subtype Procesos is Integer range 1..3;
        package Random_Proc is new Ada.Numerics.Discrete_Random(Procesos);
        use Random_proc;
        G:Generator;
        D:Procesos;

    hayPapel,hayCerillos,hayTabaco: BOOLEAN := False;
    protected colocaEnMesa is
        procedure Tabaco;
        procedure Papel;
        procedure Cerillos;
    end;



    protected body colocaEnMesa is
        procedure Tabaco is
        begin
            put_line("colocando ---> Tabaco");
            if hayPapel then
                hayPapel := False;
                tiene_cerillos.cerillosSem;
            elsif hayCerillos then
                hayCerillos := False;
                tiene_papel.papelSem;
            else
                hayTabaco := True;
            end if; 
        end Tabaco;
        procedure Cerillos is
        begin
            put_line("colocando ---> Cerillos");
            if hayPapel then
                hayPapel := False;
                tiene_tabaco.tabacoSem;
            elsif hayTabaco then
                hayTabaco := False;
                tiene_papel.papelSem;
            else
                hayCerillos := True;
            end if;
        end Cerillos;
        procedure Papel is
        begin
            put_line("colocando ---> Papel");
            if hayTabaco then
                hayTabaco := False;
                tiene_cerillos.cerillosSem;
            elsif hayCerillos then
                hayCerillos := False;
                tiene_tabaco.tabacoSem;
            else
                hayPapel := True;
            end if;
        end Papel; 
    end colocaEnMesa;
    
    begin
        Reset(G);
        loop
            D := Random(G);
            accept agenteSem do
                null;
            end;
            case D is
                when 1  =>  colocaEnMesa.Cerillos;
                            colocaEnMesa.Papel;
                when 2  =>  colocaEnMesa.Tabaco;
                            colocaEnMesa.Papel;
                when 3  =>  colocaEnMesa.Tabaco;
                            colocaEnMesa.Cerillos;
            end case;
        end loop;
    end agente;


    task body tiene_tabaco is
    begin
        loop
            accept tabacoSem do
                null;
            end;
            put_line("              Tengo Tabaco  =>  Armando cigarro");
            agente.agenteSem;
            put_line("Uff buen cigarro:Tenia tabaco");
            put_line("");
        end loop;
    end tiene_tabaco;

    task body tiene_cerillos is
    begin
        loop
            accept cerillosSem do
                null;
            end;
            put_line("              Tengo Cerillos  =>  Armando cigarro");
            agente.agenteSem;
            put_line("Uff buen cigarro:Tenia cerillos");
            put_line("");
        end loop;
    end tiene_cerillos;

    task body tiene_papel is
    begin
        loop
            accept papelSem do
                null;
            end;
            put_line("              Tengo Papel  =>  Armando cigarro");
            agente.agenteSem;
            put_line("Uff buen cigarro:Tenia papel");
            put_line("");
        end loop;
    end tiene_papel;
begin
    agente.agenteSem;
end cigarette;