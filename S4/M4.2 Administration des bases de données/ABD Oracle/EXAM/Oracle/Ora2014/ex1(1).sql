select identification_compte.*,count(NumCompte)
from Identification_compte join operation using (NumCompte)
group by identification_compte.*;


select Identification.* from Identification_compte cl

where (select count(*) from operation a where 
cl.numCompte=a.numcompte ) > 

(select count(*) from Identification_compte join operation using (numpcompte) ) ;