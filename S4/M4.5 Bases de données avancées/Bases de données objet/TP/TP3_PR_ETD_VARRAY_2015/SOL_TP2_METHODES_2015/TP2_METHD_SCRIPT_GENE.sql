--script general

--     @E:\THOR\BDOO\TP_ROO_2015\TP2_ROO_METHODES_2015\SOL_TP2_METHODES_2015\TP2_METHD_SCRIPT_GENE.sql
--- tracage
set echo on


set serveroutput on;

spool E:\THOR\BDOO\TP_ROO_2015\TP2_ROO_METHODES_2015\SOL_TP2_METHODES_2015\TP2_METHD_TRACES.txt


--nettopyage
drop table tab_pers;
drop type t_pers FORCE ;
drop table tab_adrs;
drop type t_adr FORCE ;

--1. creation du type T_ADR
@E:\THOR\BDOO\TP_ROO_2015\TP2_ROO_METHODES_2015\SOL_TP2_METHODES_2015\CREA_T_ADR.SQL
show error

--2. creation de la tables des adresses
@E:\THOR\BDOO\TP_ROO_2015\TP2_ROO_METHODES_2015\SOL_TP2_METHODES_2015\CREA_TAB_ADRS.SQL
show error

--3. creation de la tables des adresses
@E:\THOR\BDOO\TP_ROO_2015\TP2_ROO_METHODES_2015\SOL_TP2_METHODES_2015\CREA_T_PERS.SQL
show error

--4. creation de la table des personnes
@E:\THOR\BDOO\TP_ROO_2015\TP2_ROO_METHODES_2015\SOL_TP2_METHODES_2015\CREA_TAB_PERS.SQL
show error


--5. codage des m�thodes de T_ADR
@E:\THOR\BDOO\TP_ROO_2015\TP2_ROO_METHODES_2015\SOL_TP2_METHODES_2015\CREA_METH_T_ADR.SQL
show error

--6. codage des m�thodes de T_PERS
@E:\THOR\BDOO\TP_ROO_2015\TP2_ROO_METHODES_2015\SOL_TP2_METHODES_2015\CREA_METH_T_PERS.SQL
show error


-- FIN DE TRACAGE
SPOOL OFF;

