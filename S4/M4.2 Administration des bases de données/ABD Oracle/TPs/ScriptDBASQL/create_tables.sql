CONNECT system/manager


CREATE TABLE customers
 ( cust_code VARCHAR2(3),
 name VARCHAR2(50),
 region VARCHAR2(5) )
 TABLESPACE users;

 CREATE TABLE orders
( ord_id NUMBER(3),
ord_date DATE,
cust_code VARCHAR2(3),
date_of_dely DATE );



un profile est un system de limetation de resources et strat�gie de mdp 

cad est que le mdp expire , conssomation de cpu

ROle : creation un role avec des privli�grses sp�ifiques , utilisation de quota etc ...puis l'appler

aapr�s grant 'nomrole " to user1 ;
