# Stopwatch System with Multiplexed 7-Segment Display using ATmega32

#### Control a multiplexed 7-segment display stopwatch with start, stop, resume, and reset functions using external push buttons. Utilizes Timer1 in ATmega32 for accurate timekeeping.

## Features:

- **User Interaction:** Users interact with the system through external push buttons to start, stop, resume, and reset the stopwatch.
  
- **Stopwatch Functionality:** The system provides precise stopwatch functionality using Timer1 in ATmega32, ensuring accurate timekeeping.
  
- **Multiplexed 7-Segment Display:** Six Common Anode 7-segment displays are multiplexed using a 7447 decoder and NPN BJT transistors, controlled by the microcontroller.
  
- **External Interrupts:** Configures external interrupts INT0, INT1, and INT2 to handle start, stop, resume, and reset functionalities through push buttons.
  
- **Dynamic Display:** Utilizes the persistence of vision effect to display time on multiple 7-segment displays, creating a seamless visual experience.
  
- **Hardware Setup:** Provides detailed instructions for hardware setup, including connections and components required for proper functioning.

## Implementation:

- The project is implemented in C programming language, suitable for ATmega32 microcontroller development.
  
- Utilizes Timer1 and external interrupts to manage stopwatch functionality and user interactions.
  
- Employs multiplexing technique for efficient utilization of 7-segment display resources.

## Usage:

- Clone the repository and upload the provided code to an ATmega32 microcontroller.
  
- Follow hardware setup instructions and connect external push buttons and 7-segment displays as specified.
  
- Power up the microcontroller to start using the stopwatch functionalities.

**Additional Resources:**

- [LinkedIn Post](https://www.linkedin.com/posts/omar-sherif-rizk_embeddedsystems-avr-project-activity-7041577596328796160-Z3su?utm_source=share&utm_medium=member_desktop) demonstrates the hardware setup and functionality of the Stopwatch System through a video.
**Contributing:**

- Contributors are encouraged to suggest improvements, report issues, and submit pull requests to enhance the project's functionality and usability.

**License:**

- This project is licensed under the MIT License, allowing for modification, distribution, and commercial use with proper attribution.
