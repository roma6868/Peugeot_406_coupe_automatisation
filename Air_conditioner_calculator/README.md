# Calculateur climatisation automatique

Ref: PSA 9640045680  
Software version: 0802  

<p><img src="https://raw.githubusercontent.com/roma6868/Peugeot_406_coupe_automatisation/master/Air_conditioner_calculator/photo_component_extern_air_conditioning/8080/element_8060%20(1).jpg" alt="" style="float: right;" width="487" height="213" /></p>

----  
<br>
### Schéma électrique de la climatisation 

 - 8030 : thermistance air habitacle  
 - 8033 : thermistance d'ensoleillement  
 - 8031 : thermistance d'eau  
 - 8070 : motoréducteur volet entrée air  
 - 8065 : motoréducteur volet de mixage  
 - 8071 : motoréducteur volet de distribution  
 - 8045 : module commande pulseur  
 - 8050 : moteur pulseur  
 - 8080 : calculateur climatisation  

<p><img src="https://github.com/roma6868/Peugeot_406_coupe_automatisation/blob/master/Air_conditioner_calculator/extra/Wiring_diagram.jpg" alt="" style="float: right;" width="477" height="356" /></p>

----
### PCB Calculateur climatisation  

 - ELMOS 14010B : 12v => 5v convertisseur , entre analogique (Pas de datasheet trouve)  
 - TSS461C : VAN DATA link controller  
 - ALCATEL REM0339 : VAN line drive  
 - NEC DSF-75076335 : Microcontrôleur pas d’information trouve   
 - ELMOS 91001A : Low side driver 8 channel , serial interface  
 - TLE4207G : Dual Half Bridge Driver DC motor management  
 - HEF4013B : Dual D-type flip-flop  
 - M95160 : 16-Kbit serial SPI  bus EEPROM  

<p><img src="https://github.com/roma6868/Peugeot_406_coupe_automatisation/blob/master/Air_conditioner_calculator/extra/PCB_Calculateur_climatisation.jpg" alt="" style="float: right;" width="487" height="213" /></p>



