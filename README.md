Summary

The purpose of this project was to build a home security system that detects intrusion through a door or a window and alerts the homeowner.

The project began on January 16th, 2023, and was completed on March 29th, 2023. The project was a success as the security system detected intrusions from the door or window in under one minute and alerted the homeowner by blinking the correct LED’s and sounding the siren. The best outcome during testing was intruder detection in under one second.

The major problems faced were:
1.	Not having enough pins on the Arduino.
2.	PIR sensor and ultrasonic sensor’s impractical implementations.
 
Solutions to the above problems:
1.	To solve this problem, the RX/D0 pin was used as a standard input pin.
2.	Since the PIR sensor and ultrasonic led to issues during implementation, another hall effect sensor was used instead, and it worked perfectly. Also, using only hall effect sensors resulted in consistent and improved results.

Along the way, some changes were made in the schedule as seen in Appendix D (initial schedule) and Appendix E (final schedule). Time allocated to brainstorming, logbook and final report was reduced, and more time was given to work on LCD, keypad, LED, speaker, and sensors.

Work that was completed in the latter half of the project included, implementing a matrix keypad to enter passcode to gain access to house, ultrasonic sensor testing, PIR motion sensor implementation, implementing a hall effect sensor used in the final design, implementing an LCD screen to display the passcode entered and current state of house, and an audio system to alert homeowner when the system is triggered/intrusion is detected. In this part, final implementation of the system and all components with firmware was completed since the last report.


1.	Introduction

This report presents the design and implementation of a home security system that aims to protect homes from intruders. The project began on January 16th, 2023, and was completed on March 29th, 2023. This report provides a comprehensive overview of the design, development, and implementation of the home security system. Following the executive summary, this report is structured into several sections, including the introduction, discussion, and conclusion. 

The team encountered several challenges, particularly during the design phase, where they had to determine the most effective combination of sensors and techniques to use. They also had to familiarize themselves with components such as LEDs, speakers, matrix keypads, and hall effect sensors. The system was tested and evaluated by running several tests to ensure that the system was working as intended, testing the accuracy of the sensors, and simulating break-in attempts to observe if the system responded accordingly. 

The project is officially completed, and after much deliberation, it is composed of an Arduino Nano, two hall effect sensors, a speaker, three colored LEDs, two 555 timers, an LCD display, and a matrix keypad. The report discusses the remainder of the design process since the last report. Covered in this report are the completed design phases, implementation and testing, and some valuable insights for future projects.
 

2.	Discussion

2.1	Final design
Since the last report, much work has been completed on the security system. The project was completed in a timely fashion as required by clients for the demonstration date of Wednesday March 29th, 2023. The midterm progress report outlined multiple details of the final integration that have been altered in the final design due to several factors.

The midterm vision of the overengineered system consisted of six sensors (two PIR sensors, two hall effect sensors, and two ultrasonic sensors), a speaker, an LCD screen, three LEDs, and a keypad. This design was streamlined in the finalization process due to decisions regarding pin assignments, sensor practicality, and overall system practicality.

The final design consists of only two sensors, three LEDs, a speaker, an LCD screen, and a keypad. After thorough integration testing, which began roughly halfway through March, it was decided that only one type of sensor would be used. The circuit diagram for the final design is shown below.


![image](https://user-images.githubusercontent.com/114720268/230543258-9cbe61d2-876f-4fb6-819e-8e29645f14bc.png)

Figure 1:Circuit diagram for final design.

2.2 Integration/testing process
As mentioned earlier, multiple issues encountered caused the midterm report design to be altered. These included the sensitivity of the PIR sensor, and the large number of pins used by the ultrasonic sensor.

The PIR sensor was tested multiple times individually and in the final integration design using the model houses provided by the clients, shown in Appendix C. It worked as intended, but often gave false positives when stray thermal radiation was detected outside of the houses. This led to the decision of removing it from the final integration. 

The ultrasonic sensor was the third type of sensor initially intended to be included in the final design but was removed due to its resource requirements. The system is supposed to be as efficient and effective as possible, in the smallest form factor possible. The single Arduino Nano provided as a constraint by the clients led to integration issues with the ultrasonic sensor, as it required two pins to return a reading to the system, while the other two kinds of sensors only required one pin. This caused the decision to remove it from the final design. The integration of the ultrasonic sensor would have also caused a much more complex firmware, as the sensor requires a very specific interface with the Arduino to operate correctly.

While testing the ultrasonic sensor and the PIR sensor, multiple 3D designs for mounting the sensors were created and even 3D printed. These mounts are unfortunately not on the final system as the sensors were removed from the design. The PIR sensor mount is pictured below. The hall effect sensors were not mounted with 3D modelled brackets in the final design due to time constraints, and fine tolerances required for such a small part. They were mounted using tape for client demonstrations.

![image](https://user-images.githubusercontent.com/114720268/230543337-6920e174-5d45-4214-9a62-774abacd0873.png)

Figure 2: PIR sensor mount.

The other components such as the two 555 timers, the LEDs, the keypad, and the LCD screen all worked on the first attempt of integration into the final design, which was due to the extensive time spent working on their individual circuits throughout the year.

The hall effect sensor yielded near-instantaneous results in individual testing and client demonstration tests and proved to integrate with the rest of the components and firmware efficiently and easily, as shown by the concise function for sensor input in Appendix F.2. Thus, it was decided to place two hall effect sensors in the system. 

At the end of the testing and finalization phase, Group 3 came up with a design for the system that consistently recorded results of near instantaneous intruder detection through multiple tests. The final design also had important safety features such as brute force protection that activates after 5 incorrect password entry attempts, and requires a hardware reset to rearm the system. The system also had an intuitive user interface that allows users to clearly obtain the required information about the system on the LCD screen, such as the state, the password characters being entered, and instructions on what to do to interact with the system. The LEDs also relayed the same state information as the LCD screen, to allow users to get a quick reading on the system without a detailed inspection. The yellow and red LEDs also flash during their respective states as requested by the client by making use of the 555 timers provided by the client in the material constraints. The final circuit design, and pictures of the final working design are included below in Appendix A and Appendix B respectively. These functions were all implemented in the firmware using while loops that repeatedly call back to the main function when a specific condition is met, such as the one shown in Appendix F.2. This allows the main function to be modular and concise and allows the program to run indefinitely without added complexity causing possible snags in the firmware.

Overall, the components were combined into a working system in under 500 lines of code, utilizing an efficient switch function in the main as shown below in Appendix F.1. Below is a flow chart detailing the switching function and loops in the state functions in the system firmware

![image](https://user-images.githubusercontent.com/114720268/230543408-bd696afc-7a9e-4d93-8730-c6b41c0ff970.png)

Figure 3: Firmware flowchart.

2.3 Minor issues in the final implementation

As with most projects, the integration of the final design was one of the most challenging parts of the finalization. Multiple issues encountered hindered progress on prototype testing and finalization including speaker integration and pin management issues. 

As mentioned earlier, the pins were rationed to fit within the customer requirements, leading to the removal of the ultrasonic sensor and PIR sensor from the final design. Regular data input/output pins were also taken up on the Nano as more components were added onto the board, and at the last component, which was the second hall effect sensor, all regular data pins were exhausted.

This led to research about usage of the serial transmit and receive pins (RX, TX) on the digital port D of the Arduino to get one last pin for another sensor that was crucial for the final design. The research yielded that the receive pin (RX) could be used as an input pin but had to be unplugged when firmware was being flashed to the Arduino’s onboard storage. With this, the shortage of pins was resolved. The speaker had minor integration issues while being included in the main firmware file, and after more research about the Arduino’s registers, the issues were resolved, and it was integrated successfully into the system. 

2.4 Schedule changes and future revisions
The schedule set out for the project in the early stages and midterm report was not followed exactly, however the final design was completed on time as anticipated, due to the large amount of time allocated to research and familiarization with components at the beginning of the year. The project integration took place about a week later than originally planned, and the entire project came together within about a week.

These small changes in scheduling were due to external reasons but were kept in mind to avoid overrunning the deadline unintentionally.

Overall, the overengineered system design described in the last report was chopped down to a smaller system, but no performance and safety features were lost in the process, thus qualifying this project as a success. The current final design can be called a streamlined version of the “minimalist” design described in the last report. It works as required by the provided client constraints, with a very high accuracy and efficiency, and is more cost effective than the overengineered design while matching, or possibly even beating its effectiveness.

For future versions, further software development to include a password change system would benefit the user experience with the system. Including a stand-alone power supply module and 9V battery as shown below can also add extra capability to the system, allowing it to be deployed without a nearby USB power source. A 3D printed container for the entire system would also aid in the mobility and ease of employment of the system, along with some 3D printed mounts for the sensors.

![image](https://user-images.githubusercontent.com/114720268/230543503-45d33bdc-0587-48d2-803b-bd319d026771.png)

Figure 4: External power module with 9V and USB power capability.

3.	Conclusion

In conclusion, the home security system project was successful in detecting trespass through a door or window and notifying the homeowner, which was its main goal. The project, which began on January 16th, 2023, and ended on March 29th, 2023, was completed on schedule.

The project team encountered several difficulties, including problems implementing the PIR and ultrasonic sensors and inadequate pins on the Arduino board. The team overcame these difficulties by utilising the RX/D0 pin as a typical input/output pin and switching the PIR and ultrasonic sensors for a hall effect sensor, which led to better and more reliable findings.

The integration/testing of numerous components, including the matrix keypad, ultrasonic, PIR motion sensor, and hall effect sensors to detect intrusion, an LCD to show the passcode entered and the current state of the house, and an audio system to notify the homeowner when the system is triggered, was part of the project's second half. The final design of the project was completed as a result of the team's major integration of components and codes.

In the security system's final configuration, an Arduino Nano, two hall effect sensors, a speaker, an LCD screen, and a matrix keypad were all incorporated. Due to its propensity to report false positives when stray thermal radiation was found outside the dwellings, the PIR sensor was eliminated from the final design. Due to its resource needs, the ultrasonic sensor was also eliminated.

With the hall effect sensors producing nearly immediate results in individual testing and interacting with the other components and firmware effectively and simply, the final design was effective and efficient. The system was created with a friendly user interface, safety features, and LED lights to communicate state information to be as efficient and effective as feasible.

Overall, the project met its goals and offered helpful advice for future initiatives involving home security systems. The project team overcame obstacles and produced a useful and efficient home security system by working well together and addressing problems.
 

4.	Appendices

Appendix A: Final Working Design Circuit Diagram

![image](https://user-images.githubusercontent.com/114720268/230543549-aa9b9a24-13ae-4fa0-a711-536f32385a35.png)

Appendix B: Final Working Design Image

![image](https://user-images.githubusercontent.com/114720268/230543567-1181a4bb-05c3-4f60-8f47-22be44524803.png)

Appendix C: Model Houses Provided by Clients for Testing
![image](https://user-images.githubusercontent.com/114720268/230543610-62d15416-0876-489d-b941-d07aa0e9de4b.png)

Appendix D: Initial Schedule
![image](https://user-images.githubusercontent.com/114720268/230543636-e8c1a5b8-e3b8-495c-b5cd-055ffd2d44fc.png)

Appendix E: Final schedule
![image](https://user-images.githubusercontent.com/114720268/230543680-c07b65a1-96ca-4166-b680-d771fdaee943.png)

Appendix F: Relevant Firmware Snippets

F.1: Main function

int main(void){
	//initiates hardware
	init_keypad();
	LCD_init();
	init_Sense_LED();
	
//main loop that can be returned to from other loops via break statements under specific conditions
//contains switch statement that depends on int: state that can be set to 1, 2, or 3, depending on the state of the system.
	while(1){
		switch (state){
			case 1:
			disarmed();
			break;
			case 2:
			armed();
			break;
			case 3:
			triggered();
			break;
			default:
			LCD_Send_A_String("Reboot Hardware");
		}
	}
	return 0;
}

F.2: Sensor input function (armed function)

void armed(void){
	//setting LEDs to respectively required colors for state
	PORTD |= (1<<YELLOW);
	PORTC &= ~(1<<RED);
	PORTB &= ~(1<<GREEN);
	//displaying LCD message
	LCD_command(0x01);
	LCD_Send_A_String("System armed");
	LCD_command(0xC0);
	LCD_Send_A_String("Polling intruder");
	
//beginning main armed function loop, which only exits if the hall effect sensors no longer detect a magnet
	while(1){
		if(((PINC & (1<<PINC5))!=0) | ((PIND & (1<<HALL2)) != 0)){
//set state to the triggered state so that the switch function in the main loop routes the program to the triggered state function
			state = 3;
			break;
		}
	}
}

