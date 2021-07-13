# OBD Digital Speedometer
This project sparked from not having a digital speed readout on the dashboard of my car (Alfa Romeo Giulietta) when driving in 'Normal' and 'Dynamic' mode. This uses the OBD-II port to read the vehicle speed and then print the speed onto the radio message. This radio message is sent from the radio module to the dashboard and requires the 'Radio Repetition' setting in the dashboard to be enabled for the message to be displayed. This message is sent only when the car is on and is sent at a 2hz rate.

## Vehicle information
The CAN network used is a 50Kbps network and is defined as the CAN-B network on the car (pins 1 and 9 on the OBD-2 port).

The car it has been tested on has 16" wheels and is a 2011 model. I'm not sure if the wheel diameter affect the speed value provided by CAN so the variable `SPEED_DIVIDER_MPH` might need to be changed to suit other vehicles and wheel diameters.

## Hardware
The hardware used is centered around an ATmega16M1 because it has an inbuilt CAN transceiver and works with the Arduino framework. It also helps keep the PCB as small as possible and keeps components count down.

There is a power switch for powering via either the car through the OBD port or via the ICSP header. This isn't really needed but added to make firmware development slightly easier.

The PCB is designed using Kicad 5.99 and is made to fit an [OBD-2 case](https://www.ebay.co.uk/itm/153652371926?_trkparms=ispr%3D1&hash=item23c66521d6:g:HHwAAOSwdQNdhnn7&amdata=enc%3AAQAGAAACgPYe5NmHp%252B2JMhMi7yxGiTJkPrKr5t53CooMSQt2orsStEKTPzZMfQmny3knR97t0P2xzOKwBnh460pCrowN0M2fdP1LrK2%252FlNNXwtBKkiyZgjnCfzohq7MmQZypsYZpGqb9iNp5bq3fPy86SU1ee1yX74yc28mBzZUldK%252FCKxlgLK%252BB0g2j0TXnxrujV%252BNjCKNVNu4POi%252BM7tg64JcsyxT%252BDZ4j%252BQTTMGst%252FpcTfGZ9AuVyziiUPG%252BPkG01deZ0BstfmsMCeEMNMR%252FcdPJJVM8Yla4EB%252FeAbM0FmlV9pD7Z19x5WS5PXT8xPHjpPLJJHLwvE0QloPH0ZvfmXkEVXXiO2pr67Ouw8As8%252FMWk6AZR1HlrHGkANnat2gKBLz%252BfdnoOoZ%252B9niK8YwDs6%252BjhJO%252FNx8f76ua%252FHvCwO2zI4Z5YXUm2lvHOKHnH2d0nlEEX%252Bzdm3KjvGHFynrN1IeXWBirVJkkNPPTIWtJh8wMwqr7hpvjRkKSJ6OksxHCQ%252FQ7bmDfcvUmL077RbBg875t6qsdSevrjHpyNWWpVv8FnrjNinAa5QxGfb7mkiDhNxtuFMRh8c6LPI0K73nWU5VZ17z5aV0rL89SEhwb6CNMYefBNpO3ivp%252B3gAzhEzc16%252F8bPzsvM9snGWhEB2ga3j2xaZ%252FxOXMEDk7QpJSgr9RZHcZKI2xOiqFsRLF2mqLBeLKj%252BZuILXlFBhURKpjydik8dR5T9%252Bga2H7A75C2luX0nX0fDN7NfBtcBCRJDhU2mkKHx5Bf8NR5TVa%252FPcpA8Fphq4HrLQr6pMq03eZ%252FLBe6U6zjURzgT7Fms0erm%252BviSGHRDDIqr4JI5SZSvUKgLE7R5ls%253D%7Campid%3APL_CLK%7Cclp%3A2334524) found on eBay.

![OBD-2 Plug Picture](https://raw.githubusercontent.com/FletcherJ1/CANSpeedo/main/Pictures/obd-2-plug.jpg)

The PCB is a 4 layer design because it didn't cost any more than a 2 layer design from JLCPCB.


Hardware used.
Picture of results
Open about current issues

## Data dumps
I've also included some data dumps (under the /Dumps folder) containing can information when the car is in various states. Feel free to use this information as desired to decode additional parts of the cars CAN data.

I also included a file in the dumps with additional information I found and some more decoded CAN messages. This might be confusing to read through but should give some ideas.

I found plotting the data with graphs, I used Flow CSV Viewer, quite helpful to find multi-byte variables as well as what the variable may represent based on how the data changes.

## Libraries Used
- [ATMegaxxM1-C1 Arduino IDE Support](https://github.com/thomasonw/ATmegaxxM1-C1)
- [ATMegaxxM1-C1 CAN Library](https://github.com/thomasonw/avr_can)

## Credits
- [FIAT Character Map](https://gist.github.com/fmntf/c4b2744bad3908ef10fc9a5d377f2823)