/***************************************************************************
* COSC2131 - Programming Using C++
* Semester 2013A Assignment #2
* Full Name        : 
	Luong Duc Tu - s3343706
	Nguyen Quoc Trong Nghia - s3343711
****************************************************************************/

-----------------------------------------------------------------------------
If selected, do you grant permission for your assignment to be released as an
anonymous student sample solution?
-----------------------------------------------------------------------------
Yes


Known bugs:
-----------
Delayed time is increasing if the MAX_DEPTH of DFS (Depth-First Search) 
algorithm increases, current MAX_DEPTH is set to 27.


Incomplete functionalities:
-------------------------
Additional feature 2	


Assumptions:
------------
"SIX" character is Shadow character (The name we got from json is diffrent
from the name on the WEB)


Any other notes for the marker:
-------------------------------
First, install 3 libraries: boost, libcurl and jsoncpp.

Installing Boost library by terminal: 
sudo apt-get update
wget -c 'http://sourceforge.net/projects/boost/files/boost/1.53.0/boost_1_53_0.tar.bz2/download'
tar xf download
cd boost_1_50_0
./bootstrap.sh
./b2 install

Installing libcurl by terminal:
download link : http://curl.haxx.se/download.html
install instruction link: http://linuxmoz.com/ubuntu-install-curl-command/

Installing jsoncpp: please follow the link for download and installation instruction
link: http://garajeando.blogspot.com/2011/06/how-to-install-jsoncpp-in-ubuntu-and.html

For compiling, please open makefile and change to the correct json version:
For example: jsoncpp linux-gcc-4.6 
	Append "ljson_linux-gcc-4.6_libmt" in this line:
	$(EXEC): $(EXEC).cpp $(OBJS) -lcurl					 

Final compile: you just type "make" and wait for system compiles all the files. 
Then type "./main" to run the program.

In run-time:

BASIC: 
1. Connect to the Path of Exile website:
--> We use libcurl to connect and get data from host. 
	Time for connecting to host may vary depending on the connection quality.

2. Read the source code from the passive skill tree webpage
--> We use jsoncpp to read and parse data to our data structure.

3. Interpret the source code and populate an appropriate data structure
--> In this iteration, we use 4 structures to manipulate data: 
	stack, queue, map and vector depending on the need of each data and algorithm.

4. Present a command line UI to the user allowing them to:
	a. Select the start position: type name of characters correctly (case ignored)
		such as: witch, RanEr, TEMPLAR,...
	
	b. Choose a final node: input the ID of the end node exactly as on graph otherwise
		error occurs
	
	c. Choose an optimization strategy: type 'sp' for shortest path, 'os' optimize 
		strength 'od' for optimize dexterity and 'oi' for optimize intelligence.
	
	d. Enter a total number of points available: input an integer indicating number of
		points available
5. Program runs: calculating time is dependent on the quality of each computer

ADDITIONAL:
1. Present a report of the total amount of passive bonuses (strength, dexterity, 
	intelligence and other skills if exist on route) of the route.

