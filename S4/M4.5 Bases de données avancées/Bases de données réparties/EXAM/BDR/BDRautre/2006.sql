create or replace view ProduitsGlobal(NumProduit,Nom,Prix,Marque) as
select (NumProduit,Nom,Prix,'Sony') from Produit@site1
union
select (NumProduit,Nom,Prix,'Philips') from Produit@site2
union
select (NumProduit,Nom,Prix,Marque) from Produit@site3;

create sequence seq_Produit;
create or replace trigger insertIntoProduitsGlobal
INSTEAD of insert on ProduitsGlobal
for each row 
BEGIN
	if :new.Marque='Sony' and :new.Prix>=5000 then
		insert into Produit@site1 values
		(seq_Produit@bd_central.nextval,:new.Nom,:new.Prix);
	elsif :new.Marque='Philips' then
		insert into Produit@site2 values
		(seq_Produit@bd_central.nextval,:new.Nom,:new.Prix);
	else
		insert into Produit@site3 values
		(seq_Produit@bd_central.nextval,:new.Nom,:new.Prix,:new.Marque);
	end if;
END;
/

create or replace trigger deleteFromProduitsGlobal
INSTEAD of delete,update on ProduitsGlobal
for each row 
BEGIN
	if DELETING then 
		if :old.Marque='Sony' and :old.Prix>=5000 then
			delete from Produit@site1
			where NumProduit=:old.NumProduit;
		elsif :old.Marque='Philips' then
			delete from Produit@site2
			where NumProduit=:old.NumProduit;
		else
			delete from Produit@site3
			where NumProduit=:old.NumProduit;
		end if;
	end if;
	if UPDATING then
		if :old.Marque='Sony' and :old.Prix>=5000 then
			if :new.Marque='Sony' and :new.Prix>=5000 then
				update Produit@site1
				set
				NumProduit=:new.NumProduit,
				Nom=:new.Nom,
				Prix=:new.Prix
				where NumProduit=:old.NumProduit;
			elsif :new.Marque='Philips'
				delete from Produit@site1
				where NumProduit=:old.NumProduit;
				insert into Produit@site2 values
				(:new.NumProduit,:new.Nom,:new.Prix);
			else
				delete from Produit@site1
				where NumProduit=:old.NumProduit;
				insert into Produit@site3 values
				(:new.NumProduit,:new.Nom,:new.Prix,:new.Marque);
			end if;
		elsif :old.Marque='Philips'
			if :new.Marque='Sony' and :new.Prix>=5000 then
				delete from Produit@site2
				where NumProduit=:old.NumProduit;
				insert into Produit@site1 values
				(:new.NumProduit,:new.Nom,:new.Prix);
				
			elsif :new.Marque='Philips'
				update Produit@site2
				set
				NumProduit=:new.NumProduit,
				Nom=:new.Nom,
				Prix=:new.Prix
				where NumProduit=:old.NumProduit;
			else
				delete from Produit@site2
				where NumProduit=:old.NumProduit;
				insert into Produit@site3 values
				(:new.NumProduit,:new.Nom,:new.Prix,:new.Marque);
			end if;
		else
			if :new.Marque='Sony' and :new.Prix>=5000 then
				delete from Produit@site3
				where NumProduit=:old.NumProduit;
				insert into Produit@site1 values
				(:new.NumProduit,:new.Nom,:new.Prix);
				
			elsif :new.Marque='Philips'
				delete from Produit@site3
				where NumProduit=:old.NumProduit;
				insert into Produit@site2 values
				(:new.NumProduit,:new.Nom,:new.Prix);
				
			else
				update Produit@site3
				set
				NumProduit=:new.NumProduit,
				Nom=:new.Nom,
				Prix=:new.Prix,
				Marque=:new.Marque
				where NumProduit=:old.NumProduit;		
			end if;
		end if;
	end if;
END;
/

create or replace trigger updateProduitsGlobal
INSTEAD of update on ProduitsGlobal
for each row
BEGIN
	if :old.Marque='Sony' and :old.Prix>=5000 then
		if :new.Marque='Sony' and :new.Prix>=5000 then
			update Produit@site1
			set
			NumProduit=:new.NumProduit,
			Nom=:new.Nom,
			Prix=:new.Prix
			where NumProduit=:old.NumProduit;
		elsif :new.Marque='Philips'
			delete from Produit@site1
			where NumProduit=:old.NumProduit;
			insert into Produit@site2 values
			(:new.NumProduit,:new.Nom,:new.Prix);
		else
			delete from Produit@site1
			where NumProduit=:old.NumProduit;
			insert into Produit@site3 values
			(:new.NumProduit,:new.Nom,:new.Prix,:new.Marque);
		end if;
	elsif :old.Marque='Philips'
		if :new.Marque='Sony' and :new.Prix>=5000 then
			delete from Produit@site2
			where NumProduit=:old.NumProduit;
			insert into Produit@site1 values
			(:new.NumProduit,:new.Nom,:new.Prix);
			
		elsif :new.Marque='Philips'
			update Produit@site2
			set
			NumProduit=:new.NumProduit,
			Nom=:new.Nom,
			Prix=:new.Prix
			where NumProduit=:old.NumProduit;
		else
			delete from Produit@site2
			where NumProduit=:old.NumProduit;
			insert into Produit@site3 values
			(:new.NumProduit,:new.Nom,:new.Prix,:new.Marque);
		end if;
	else
		if :new.Marque='Sony' and :new.Prix>=5000 then
			delete from Produit@site3
			where NumProduit=:old.NumProduit;
			insert into Produit@site1 values
			(:new.NumProduit,:new.Nom,:new.Prix);
			
		elsif :new.Marque='Philips'
			delete from Produit@site3
			where NumProduit=:old.NumProduit;
			insert into Produit@site2 values
			(:new.NumProduit,:new.Nom,:new.Prix);
			
		else
			update Produit@site3
			set
			NumProduit=:new.NumProduit,
			Nom=:new.Nom,
			Prix=:new.Prix,
			Marque=:new.Marque
			where NumProduit=:old.NumProduit;		
		end if;
	end if;
END;
/

Copy from bd_casa to bd_casa
	REPLACE EmployéCasa(NoEmployé, Nom, Prénom, Adresse, iDTitre, Salaire)
	USING select NoEmployé, Nom, Prénom, Adresse, iDTitre, Salaire
	FROM Employé e,Filiale f
	WHERE e.NoFiliale=f.NoFiliale and f.Ville='Casablanca';

Copy from bd_casa to bd_rabat
	REPLACE EmployéRabat(NoEmployé, Nom, Prénom, Adresse, iDTitre, Salaire)
	USING select NoEmployé, Nom, Prénom, Adresse, iDTitre, Salaire
	FROM Employé e,Filiale f
	WHERE e.NoFiliale=f.NoFiliale and f.Ville='Rabat';

Copy from bd_casa to bd_marrakech
	REPLACE EmployéRabat(NoEmployé, Nom, Prénom, Adresse, iDTitre, Salaire)
	USING select NoEmployé, Nom, Prénom, Adresse, iDTitre, Salaire
	FROM Employé e,Filiale f
	WHERE e.NoFiliale=f.NoFiliale and f.Ville='Marrakech';

drop table Employé@bd_casa;

create or replace trigger fk_Titre_Employé
before delete or update of iDTitre on Titre
for each row
DECLARE
x_casa number;
x_rabat number;
x_marrakech number;
BEGIN
select count(*) into x_casa from EmployéCasa@bd_casa where iDTitre=:old.iDTitre;
select count(*) into x_rabat from EmployéRabat@bd_rabat where iDTitre=:old.iDTitre;
select count(*) into x_marrakech from EmployéMarrakech@bd_marrakech where iDTitre=:old.iDTitre;
	if(x_casa+x_rabat+x_marrakech<>0) Raise_Application_ERROR(-20175,'Impossible de supprimer|updater ce Titre car réferencé sur des sites distants');
	end if;
END;

create or replace trigger fk_Employé_Titre
before insert on update of iDTitre on Employé 
for each row
DECLARE
x_Titre number;
BEGIN
select count(*) into x_Titre from Titre@bd_casa where iDTitre=:new.iDTitre;
	if x_Titre=0 then Raise_Application_ERROR(-20175,'Opération impossible car le tite réferencé est  inexistant!');
	end if;
END;

create or replace trigger MAJ_Titre
after insert or update or delete on Titre
for each row
BEGIN
	if inserting then
		insert into Titre@bd_rabat values
		(:new.iDTitre,:new.NomTitre);
		insert into Titre@bd_marrakech values
		(:new.iDTitre,:new.NomTitre);
	elsif updating then
		update Titre@bd_rabat
		set
		iDTitre=:new.iDTitre,
		NomTitre=:new.NomTitre
		where iDTitre=:old.iDTitre;
		update Titre@bd_marrakech
		set
		iDTitre=:new.iDTitre,
		NomTitre=:new.NomTitre
		where iDTitre=:old.iDTitre;
	else
		delete from Titre@bd_rabat where iDTitre=:old.iDTitre;
		delete from Titre@bd_marrakech where iDTitre=:old.iDTitre;
	end if;
END;
/
