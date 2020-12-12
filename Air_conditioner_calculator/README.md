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

<p><img src="https://github.com/roma6868/Peugeot_406_coupe_automatisation/blob/master/Air_conditioner_calculator/extra/connecteur_6V_GR_et_elmos17010B.jpg" alt="" style="float: right;" width="450" height="322" /></p>

### ELMOS14010B  

Il est utilise comme convertiseur DC/DC 12V => 5v , mais aussi comme ADC les different capteur sont relier a lui  
<p><img src="https://github.com/roma6868/Peugeot_406_coupe_automatisation/blob/master/Air_conditioner_calculator/extra/Connection_electrique_ELMOS_14010B.jpg" alt="" style="float: right;" width="624" height="382" /></p>

----



----

<strong>8030 : thermistance air habitacle</strong>
En fonction des valeurs des résistances j’obtenait la température suivante.
D’apres le PP2000 la valeur doit rester entre 0,25 < U < 4,6 V entre les bornes 2 et 4 du 6V GR 

__Unité C°__

 - 1 kOhm = 80 C° <strong>MAX</strong>
 - 5 kOhm = 40 C°
 - 10 kOhm = 24 C°
 - 15 kOhm = 15 C°
 - 20 kOhm = 10 C°
 - 33 kOhm = 0 C°
 - 47 kOhm = -1 C°
 - 90 kOhm = -19 C°
 - 100 kOhm = -20 C°
 - 133 kOhm = -25 C°
 - 144 kOhm = -27 C° <strong>MIN</strong>

<p><img src="https://github.com/roma6868/Peugeot_406_coupe_automatisation/blob/master/Air_conditioner_calculator/extra/8030.jpg" alt="" style="float: right;" width="548" height="308" /></p>

----

<strong>8033 : thermistance d‘ensoleillement</strong>
On fonction des valeurs des résistances j’obtenait des valeur d’ensoleillement suivant . 

__Unité W/m2__

 - 110 kOhm = 1000 w/m2
 - 100 kOhm = 1180 w/m2
 - 82 kOhm = 1590 w/m2
 - 51 kOhm = 2770 w/m2
 - 33 kOhm = 3950 w/m2
 - 22 kOhm = 5170 w/m2
 - 15 kOhm = 6050 w/m2
 - 14 kOhm = 6270 w/m2
 - 10,3 kOhm = 7000 w/m2 <strong>MAX</strong>

<p><img src="https://github.com/roma6868/Peugeot_406_coupe_automatisation/blob/master/Air_conditioner_calculator/extra/8033.jpg" alt="" style="float: right;" width="361" height="220" /></p>

----

<strong>8031 : thermistance d'eau</strong>
On fonction des valeurs des résistances j’obtenait la température suivante.
D’apres le PP2000 la valeur doit rester entre 0,25 < U < 4,6 v entre les bornes 2 et 7 du 26V JN 

__Unité C°__

 - 1 kOhm = 80 C° <strong>MAX</strong>
 - 5 kOhm = 40 C°
 - 10 kOhm = 24 C°
 - 15 kOhm = 15 C°
 - 20 kOhm = 10 C°
 - 33 kOhm = 0 C°
 - 47 kOhm = -1 C°
 - 90 kOhm = -19 C°
 - 100 kOhm = -20 C°
 - 133 kOhm = -25 C°
 - 144 kOhm = -27 C° <strong>MIN</strong>

<p><img src="https://github.com/roma6868/Peugeot_406_coupe_automatisation/blob/master/Air_conditioner_calculator/extra/8031.jpg" alt="" style="float: right;" width="318" height="248" /></p>

----

<strong>8006 : thermistance évaporateur</strong>   
Pour faire fonctionne le calculateur climatisation sur le stend il faut absolument relier une résistance 22 kOhm (par exemple) a la place de la thermistance évaporateur sur le 
connecteur BSI 12V MR du BSI.  

<p><img src="https://github.com/roma6868/Peugeot_406_coupe_automatisation/blob/master/Air_conditioner_calculator/extra/8006.jpg" alt="" style="float: right;" width="370" height="350" /></p>

----
<strong>8045 : module commande pulseur</strong>  
Nombre de vitesse de ventilation : 7 

<p><img src="https://github.com/roma6868/Peugeot_406_coupe_automatisation/blob/master/Air_conditioner_calculator/extra/module_commande_pulseur_air.jpg" alt="" style="float: right;" width="359" height="257" /></p>

<strong>Commande du pulseur:</strong>  

Le signal envoyer par le calculateur climatisation au module 8045 qui commande la vitesse du pulseur  et de type PWM.
Fréquence : 1,966kHz
Il varie de 0% a 100 %
De 0V au 12V

 - Vitesse 0 = 0V = 0%
 - Vitesse 1 = 4,86V = 17,8% 
 - Vitesse 2 = 5,27V = 20,7%
 - Vitesse 3 = 6,69V = 29,8%
 - Vitesse 4 = 7,70V = 38,8%
 - Vitesse 5 = 8,72V = 48,0%
 - Vitesse 6 = 9,73V = 57,9%
 - Vitesse 7 = 12V = 100%


<p><img src="https://github.com/roma6868/Peugeot_406_coupe_automatisation/blob/master/Air_conditioner_calculator/extra/PWN_signal_pulseur_air.jpg" alt="" style="float: right;" width="1296" height="300" /></p>


