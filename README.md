# bluetooth
HC-05 C Project

Para este proyecto se conecta un cristal de 16 MHz en XTAL1 y XTAL2 como muestra la pagina 31 del manual del ATmega164p
se debe tomar en cuanta la configuracion de los fuse en el micro para ello se modifica el LOW Fuse 0xEF, segun recomendaciones del mismo manual
CKSEL 3..0=1111 y SUT = 10 Ext Crystal Osc
