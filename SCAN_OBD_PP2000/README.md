<h1>Arduino UNO K-Line protocol (OBD)<h1>
<p>   </p>
  
<h2>Hardware</h2>
  <p> <b>Materiel utilise:</b> Arduino UNO + MC33290 (ISO K Line Serial Link Interface)
    <br>
      <img src="RESOURCE/img/ArduinoUno_MC33290_Hardware.png">
    <br>
  </p>
  
<h2>Initialisation de la communication</h2>
 <p>
  <center><img src="RESOURCE/img/5_BAUD_INITIALIZATION.png"></center>
  <center><img src="RESOURCE/img/Initialisation_communication.png"></center>
</p>
  
  
<h2>Software</h2>

  <p>
  Pour me connecter au BSI de la voiture via la prise OBD je utilise le boitier diagnostic de peugeot ( PP2000 ) et sont logiciel ( peugeot        planet office ) il permet de lire tout les informations technique sur les equipements de la voiture.
  </p>
  
  <p align="center">
   <img src="RESOURCE/img/Menu_identification_PP2000.png" height=400 >
  </p>
  <br>
  <p>Un essais avec ARDUINO UNO qui envoye une demande d'information au BSI sur ca version et ca date de fabrication. </p>
  
  <p align="center">
    <img src="RESOURCE/img/Arduino_read_OBD_v1.01.png" height=300 >
  </p>
  
  <a href= "Software/Arduino_read_OBD_v1.01/Arduino_read_OBD_v1.01.ino" rel="nofollow"> =>Software Arduino_read_OBD_v1.01<= </a>
  
<h2>Documentation</h2>
  <p>Datasheet : 
    <a href= "RESOURCE/DataSheet/MC33290.pdf" rel="nofollow">MC33290</a>
  <br>
  &emsp;&emsp;&emsp;&emsp;&emsp;
  <a href= "RESOURCE/DataSheet/IRJET-V4I7181.pdf" rel="nofollow">Protocol for Diagnostic Application ISO 9141</a>

 
 </p>
