
#Choix du dossier de travail
rm(list = ls())
getwd()
setwd("E:/EnSiaS/courses/analyse de donn�es/Workspace/TP4")
#Lecture des donn�es
tab <- read.csv2("pHconductiviteRiviere.csv", header=TRUE, sep=";", dec=",")
#Propri�ty�s des Donn�es
str(tab)
dim(tab)
#Statistiques des Donn�es
install.packages("psych")
library(psych)
describe(tab)
#Calcul du coefficient de variation
cv<-describe(tab)$sd/describe(tab)$mean
Des<-cbind(cv,describe(tab))
Des
#Centrage et reduction des Donn�es
tab.cr <- scale(tab)
View(tab.cr)
describe(tab.cr)
#Calcul des corr�lations entre variables
install.packages("Hmisc")
library(Hmisc)
cor <- rcorr(as.matrix(tab), type="pearson")$r
cor
cor.arr <- round(cor, 3) #valeurs arrondies
cor.arr
#Test de sph�ricit� de Bartlett
bartlett.test(tab)
#Calcul de l'indice KMO et des MSAi 
KMO(cor)
install.packages("FactoMineR")
library(FactoMineR)
#calcul des corr�lations entre variables
cor(tab)
#Application de l'ACP
res<-PCA(tab,ncp=5,axes=c(1,2))
attributes(res)
#Calcul des valeurs propres de la matrice de corr�lation
res$eig
#Graphique des valeurs propres
plot(1:10,res$eig[,1],type="b",ylab="Valeurs propres",xlab="Composantes",main="graphique des valeurs propres")
barplot(res$eig[,1],main="Eigenvalues", names.arg=paste("dim",1:nrow(res$eig)))
#Calcul de la somme des valeurs propres
sum(res$eig[,1])
#Nuages des variables
#Calcul de : coord, cor, cos2, contrib
res$var
res.arr <- round(as.data.frame(res$var), 3) #valeurs arrondies
res.arr
#Cumul des cos2
print(t(apply(res$var$cos2,1,cumsum)),digit=2)
#Nuage des individus
#Calcul de : coord, cor, cos2, contrib
res$ind
#Repr�sentation simultann�e des variables et des individus
fit <- princomp(tab, cor=T, scores=T)
biplot(fit,cex=0.75)

