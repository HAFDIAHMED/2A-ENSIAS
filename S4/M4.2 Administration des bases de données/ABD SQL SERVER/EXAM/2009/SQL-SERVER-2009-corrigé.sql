USE [Lassina]
GO
/****** Object:  StoredProcedure [dbo].[bulletin]    Script Date: 12/22/2009 23:59:17 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
ALTER procedure [dbo].[bulletin]
@cin char(7)
as

declare @nom char(20)
declare @date_naiss char(20)
declare @adresse char(20)
declare @mois int
declare @annee int
declare @taux decimal(8,2)
declare @nbreheure int
declare @mensuel int

declare curseur CURSOR FOR
select nom, date_naiss, adresse, mois, annee, taux, [nbre-heure],SUM([nbre-heure]*taux) [total des salaires]
from Lassina.dbo.employees e, Lassina.dbo.paie p
where e.cin=p.cin
and e.cin=@cin
group by mois, annee, nom, date_naiss,adresse, taux, [nbre-heure]

open curseur

FETCH curseur INTO @nom, @date_naiss, @adresse, @mois, @annee, @taux, @nbreheure, @mensuel

WHILE @@FETCH_STATUS=0

begin

PRINT 'CIN :'+@cin

PRINT 'Nom et Prenom : '+@nom

PRINT 'Date de naissance :'+@date_naiss

PRINT 'Adresse :'+@adresse


PRINT 'Mois / Annee : '+CONVERT(VARCHAR,@mois)+'/'+CONVERT(VARCHAR,@annee)


PRINT 'Taux horaire :'+CONVERT(VARCHAR,@taux) +'                 Nombre heure:'+CONVERT(VARCHAR,@nbreheure)

PRINT 'Salaire mensuel :'+CONVERT(VARCHAR,@mensuel)

PRINT 'Signature :'

FETCH curseur INTO @nom, @date_naiss, @adresse, @mois, @annee, @taux, @nbreheure, @mensuel

end


