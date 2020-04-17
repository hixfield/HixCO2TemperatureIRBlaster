# HixCO2TemperatureIRBlaster - Hardware
The respository contains the EagleCAD file + JLCPCB zip export (in the `/hardware` folder)

| Top                                 | Bottom                                 |
|-------------------------------------|----------------------------------------|
| ![PCB top view](images/pcb_top.png) | ![PCB bottom view](images/pcb_bot.png) |

![3D PCB](images/3d_pcb.png)

# BOM
| ID                                 | Part                                                                |
|------------------------------------|----------------------------------------------------------------------
|B1                                  | BUZZER(12MM)     
|C1,C2                               | 47uF (SMD 1206)             
|C3,C5                               | 100nF (SMD 1206)            
|C4                                  | 4.7uF (SMD 1206) 
|C6                                  | 680uF/6.3V 
|F1                                  | Fuse 800mA 250V       
|J1                                  | Header female 6 pin
|J2                                  | IEC320C8 mains connector         
|J3                                  | Header female 4 pin or XH4P             
|L1,L2,L3                            | IR LED 100 mA.    -> only assemble the one that points in the direction right for you!
|Q1,Q2                               | Mosfet AO3400           
|R1,R2,R6 68                         | 68 Ohm (SMD 1206) -> only assemble for the LED that points in the direction right for you!                
|R2,R5,R7,R10,R11,R15                | 10K (SMD 1206)             
|R8                                  | 100 Ohm (SMD 1206)          
|R9                                  | 4.7K (SMD 1206)
|R12                                 | 2.2K (SMD 1206)  
|R14                                 | S10K275          
|S3,S4                               | SMD Push button     
|U$1                                 | MHZ-19_CO2       
|U$2                                 | ESP8266(ESP12E)V2
|U$3                                 | RGB LED 5mm          
|U1                                  | TSOP2238      
|U2                                  | LM3940 (SMD)         
|U3                                  | HLK-PM01 (mains to 5v powerbrick)     
|U4                                  | DS18B20   

# Schematic
The repository contains the EagleCAD (in the `/hardware` folder)

![Schematic](images/schematic.png)

# 3D printed case
(the repository contains the stl files (`/hardware/case folder`)

| base                                  | lid                                     |
|---------------------------------------|-----------------------------------------|
| ![PCB top view](images/case_base.png) | ![PCB bottom view](images/case_lid.png) |

![PCB top view](images/case_assembled.png)
|