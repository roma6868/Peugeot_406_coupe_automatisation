# Calculateur climatisation automatique

Ref: PSA 9640045680  
Software version: 0802  

<p><img src="https://raw.githubusercontent.com/roma6868/Peugeot_406_coupe_automatisation/master/Air_conditioner_calculator/photo_component_extern_air_conditioning/8080/element_8060%20(1).jpg" alt="" style="float: right;" width="487" height="213" /></p>

----  

### Schéma électrique de la climatisation 

 - <strong>8030</strong> : thermistance air habitacle  
 - <strong>8033</strong> : thermistance d'ensoleillement  
 - <strong>8031</strong> : thermistance d'eau  
 - <strong>8070</strong> : motoréducteur volet entrée air  
 - <strong>8065</strong> : motoréducteur volet de mixage  
 - <strong>8071</strong> : motoréducteur volet de distribution  
 - <strong>8045</strong> : module commande pulseur  
 - <strong>8050</strong> : moteur pulseur  
 - <strong>8080</strong> : calculateur climatisation  

<p><img src="https://github.com/roma6868/Peugeot_406_coupe_automatisation/blob/master/Air_conditioner_calculator/extra/Wiring_diagram.jpg" alt="" style="float: right;" width="477" height="356" /></p>

----
### PCB Calculateur climatisation  

 - <strong>ELMOS 14010B</strong> : 12v => 5v convertisseur , entre analogique (Pas de datasheet trouve)  
 - <strong>TSS461C</strong> : VAN DATA link controller [datasheet](https://github.com/roma6868/Peugeot_406_coupe_automatisation/blob/master/Air_conditioner_calculator/datasheet/TSS461C.pdf)  
 - <strong>ALCATEL</strong> REM0339 : VAN line drive  
 - <strong>NEC DSF-75076335</strong> : Microcontrôleur (pas d’information trouve)   
 - <strong>ELMOS 91001A</strong> : Low side driver 8 channel , serial interface [datasheet](https://github.com/roma6868/Peugeot_406_coupe_automatisation/blob/master/Air_conditioner_calculator/datasheet/ELMOS%20910.01.PDF)  
 - <strong>TLE4207G</strong> : Dual Half Bridge Driver DC motor management [datasheet](https://github.com/roma6868/Peugeot_406_coupe_automatisation/blob/master/Air_conditioner_calculator/datasheet/TLE4207G.pdf)  
 - <strong>HEF4013B</strong> : Dual D-type flip-flop [datasheet](https://github.com/roma6868/Peugeot_406_coupe_automatisation/blob/master/Air_conditioner_calculator/datasheet/HEF4013B.pdf)  
 - <strong>M95160</strong> : 16-Kbit serial SPI  bus EEPROM [datasheet](https://github.com/roma6868/Peugeot_406_coupe_automatisation/blob/master/Air_conditioner_calculator/datasheet/ST%20M95160.pdf)  

<p><img src="https://github.com/roma6868/Peugeot_406_coupe_automatisation/blob/master/Air_conditioner_calculator/extra/PCB_Calculateur_climatisation.jpg" alt="" style="float: right;" width="487" height="213" /></p>

----
### Connecteur 26V JN   

Voici les 2 composant qui s’occupe de contrôler les diffèrent actionneur de la climatisation automatique.  
Leur broche de sortie sont directement relier au connecteur jaune ( 26V JN ).  


<p><img src="https://github.com/roma6868/Peugeot_406_coupe_automatisation/blob/master/Air_conditioner_calculator/extra/26V_JN.jpg" alt="" style="float: right;" width="1331" height="410" /></p>

<p><img src="https://github.com/roma6868/Peugeot_406_coupe_automatisation/blob/master/Air_conditioner_calculator/extra/ce_qui_est_connnecter_26V_JN.jpg" alt="" style="float: right;" width="984" height="331" /></p>

INPUT capteur de luminosite, temperature eau moteur, module pulseur air

<p><img src="https://github.com/roma6868/Peugeot_406_coupe_automatisation/blob/master/Air_conditioner_calculator/extra/26V_JN_input.jpg" alt="" style="float: right;" width="413" height="355" /></p>

----
### Connecteur 6J NR  


