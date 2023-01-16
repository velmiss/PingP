#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <ctime>
#include <windows.h>

#include "pingfunctions.h";
#include "mainfunction.h";


using namespace std;

int main()
{
    int pingnum; //how many times we want to ping the IP
    int resetcounter = 0; //resetcounter does the ping command two times after a settings reset 
    string run; //run is the string to use for user iput like settings, help and run ping
    bool timeron = false;
    int time2;
    int repeat;

    /* try to open file to read */
    ifstream ifile;
    ifile.open("pingsettings.bat");//checks if pingsettings.bat file exists
    if (ifile)      
    {               // tries to open file. if succesful closes file
        ifile.close();
    }

    cout << "Help to see commands" << endl;
    cout << "Press enter to run ping" << endl;

    getline(cin, run);      //input from user
    while (run != "stop")
    {
        while (run != "" && run != "help" && run != "reset" && run != "settings" && run != "stop" && run!= "timer")//when user input is wrong
        {
            cout << "----------------------ERROR----------------------" << endl;
            cout << "Something went wrong!" << endl;
            cout << "Help to see commands" << endl;
            cout << "Press enter to run ping" << endl;
            getline(cin, run);
        }
        while (run == ""|| resetcounter>0)//when user input is empty run ping. resetcounter goes to 2 after a reset so this will run after reset
        {
            string aptemp1, aptemp2;
            ifstream pingisthere;
            pingisthere.open("pingsettings.bat");
            if (pingisthere)// checks if pingsettings.bat is open. else error
            {
                string ping = "", memorystr;
                string settingsstr;


                int average, minimum, maximum;
                int oldaverage, oldminimum, oldmaximum;
                int averageap, minimumap=INT_MAX, maximumap =INT_MIN;
                
                getline(pingisthere, settingsstr);      //takes info from pingsettings.bat and copies into settingsstr string
                pingnum = ExtractInt(settingsstr, 1);   //extract the first int from the string and puts it into pingnum

                ifstream pingini1("ping.txt");  //tries to open or create the ping.txt file
                if (pingini1.is_open())     //file succesfully open
                {
                    int counter = 0;
                    while (!pingini1.eof())//while pingini1 is not endof
                    {
                        //reads one line at a time and puts it into line
                        string line;
                        counter++;
                        getline(pingini1, line);
                        if (counter > (pingnum + 6))//puts one line into the string ping pingnum + 6 chooses the line to the min max average line
                        {
                            ping += line;//ping is now the min max average line from ping command
                        }
                    }
                    pingini1.close();
                }
                else
                {
                    cout << "Something went wrong.\nTry to restart program or delete the ping.txt to resolve.\nOr just run reset." << endl;
                }
                //previous info in the string ping
                oldminimum = ExtractInt(ping, 1);//extract first int from the string ping
                oldmaximum = ExtractInt(ping, 2);//extract second int from the string ping
                oldaverage = ExtractInt(ping, 3);//extract third int from the string ping
                //Minimum = 1ms, Maximum = 4ms, Average = 2ms

                ping = "";//empties the string ping after we have extracted the old numbers
                system("pingsettings.bat&&CLS");//runs pingsettings.bat the .bat file sends the results to ping.txt
                int is = 0;
           
                ifstream pingini2("ping.txt");
                if (pingini2.is_open())//does the same thing as previously line 57 in code. if ping.txt is open
                {
                    int counter = 0;
                    while (!pingini2.eof())
                    {
                        //reads one line at a time and saves it in the string line
                        string line;
                        counter++;
                        getline(pingini2, line);
                        if (counter > (pingnum + 6))//when we reach the right line (min max average line) we save it to string ping
                        {
                            ping += line;
                        }
                        if (counter > 2 && counter < pingnum+3)//finding the lines for individual pings
                        {
                            //program should read how long it took for every ping from ping.txt and store it in memory.txt
                            string individualping;

                            individualping = line;
                            //individualping.erase(0, 24);
                            aptemp1 = ExtractWord(individualping, 5);//extract the fifth word (as time=25ms)
                            aptemp1.erase(0, 5);//remove the first five char
                            aptemp1.erase(aptemp1.size() - 2);//remove last two char
                            aptemp1 += "; ";// write semicolon and new line


                            time_t now = time(0);
                            tm* ltm = localtime(&now);
                         
                            ofstream memorywrite;
                            memorywrite.open("memory.txt", ios_base::app);
                            
                            memorywrite << aptemp1 << ltm->tm_hour << ":" << ltm->tm_min << ';' <<'\n';
                                //write to memory.txt

                            memorywrite.close();
                            aptemp1 = "";

                        }
                    }
                    pingini2.close();
                }
                else
                {
                    cout << "Something went wrong.\nTry to restart program or delete the ping.txt to resolve.\nOr just run reset." << endl;
                }
                
                //new info in the string ping
                minimum = ExtractInt(ping, 1);//extract first int from the string ping
                maximum = ExtractInt(ping, 2);//extract second int from the string ping
                average = ExtractInt(ping, 3);//extract third int from the string ping
                //Minimum = 1ms, Maximum = 4ms, Average = 2ms


                //should read every number in memory.txt and use them as minimum maximum and count average
                
                ifstream memstr("memory.txt");
                int counter2 = 1;
                while (!memstr.eof())
                {
                    string temp1;
                    getline(memstr, temp1, ';');
                    if (counter2 % 2 != 0)
                    {
                        memorystr += ' ';
                        memorystr += temp1;
                    }
                    counter2++;
                }
                
               


                //read numbers seperatly from memorystr and use them in averageap, minimumap, maximumap
                int wordnum = WordsInString(memorystr);

                int counter1 = 1,inttemp,sumtemp=0;
                while (wordnum > counter1-1)
                {
                    inttemp = ExtractInt(memorystr, counter1);

                    if (inttemp > maximumap)maximumap = inttemp;
                    if (inttemp > 0 && inttemp < minimumap)minimumap = inttemp;
                    sumtemp += inttemp;

                    counter1++;
                }
                averageap = sumtemp / wordnum;
               
                //old system to calculate


                cout << "--------------------LAST PING--------------------" << endl;
                cout << "Minimum: " << oldminimum << "ms" << endl;
                cout << "Maximum: " << oldmaximum << "ms" << endl;
                cout << "Average: " << oldaverage << "ms" << endl;

                cout << "--------------------THIS PING--------------------" << endl;
                cout << "Minimum: " << minimum << "ms" << endl;
                cout << "Maximum: " << maximum << "ms" << endl;
                cout << "Average: " << average << "ms" << endl;

                cout << "----------------AVERAGE AFTER PING---------------" << endl;
                cout << "Minimum: " << minimumap << "ms" << endl;
                cout << "Maximum: " << maximumap << "ms" << endl;
                cout << "Average: " << averageap << "ms" << endl;
                cout << "-------------------------------------------------" << endl;
                
                if (resetcounter > 0)resetcounter--;
                if (timeron == true)
                {
                    run = "timer";
                }
                else if (resetcounter == 0)
                {
                    cout << "Help to see commands" << endl;
                    cout << "Press enter to run ping" << endl;
                    getline(cin, run);
                    pingisthere.close();
                }

            }
            else    //if pingsettings.bat cant open
            {
                cout << "Help to see commands" << endl;
                cout << "You need to add new settings with the settings command" << endl;
                getline(cin, run);
            }
            system("CLS");
        }
        while (run == "help")
        {
            //help just prints instructions for the program
            cout << "----------------------HELP-----------------------" << endl;
            cout << "stop...............Stops program" << endl;
            cout << "reset..............Resets program (ping will be run twice afer reset)" << endl;
            cout << "settings...........Changes the ping settings" << endl;
            cout << "timer..............Sets a timer to run ping at intervalls" << endl;
            cout << "-------------------------------------------------" << endl;
            cout << "Press enter to run ping" << endl;
            getline(cin, run);
            system("CLS");
        }
        while (run == "reset")
        {
            cout << "----------------------RESET----------------------" << endl;
            cout << "Are you sure you want to reset? Y/N" << endl;
            string ync;
            int yni;
            getline(cin, ync);
            yni = YesOrNo(ync);//the YesOrNo checks if the string ync is "Y" or "N" or "yes" or "no" and outputs and setts the int yni to 0,1 or 2
            while (yni == 2) //when ync is not yes or no
            {
                cout << "---------------SOMETHING WENT WRONG--------------" << endl;
                cout << "Are you sure you want to reset? Y/N" << endl;
                getline(cin, ync);
                yni = YesOrNo(ync);
            }
            if (yni == 1)//if ync is yes
            {   //removes old info from ping.txt and memory.txt
                ofstream memoryreset1("ping.txt");
                memoryreset1 << "";
                memoryreset1.close();

                ofstream memoryreset2("memory.txt");
                memoryreset2 << "";
                memoryreset2.close();

                ofstream memoryreset3("time.txt");
                memoryreset3 << "";
                memoryreset3.close();

                run = "";
                resetcounter = 2;
            }
            else
            {
                cout << "Reset has been aborted" << endl;
                cout << "Help to see commands" << endl;
                cout << "Press enter to run ping" << endl;
                getline(cin, run);
            }
            system("CLS");
        }
        while (run == "settings")
        {
            string ip, pstr;
            cout << "How many times would you like to ping the selected ip?" << endl;
            cin >> pingnum;
            while (cin.fail() || pingnum < 1 || pingnum >200)
            {
                cin.clear();
                cin.ignore(999, '\n');
                cout << "You need to enter a whole number (1-200)" << endl;
                cout << "How many times would you like to ping the selected ip?" << endl;
                cin >> pingnum;
            }
            cin.clear();
            cin.ignore(999, '\n');
            cout << "\nWhat is the IP or webpage you would like to ping?" << endl;
            getline(cin, ip);

            //makes the int pingnum into a string
            stringstream pingtostr;
            pingtostr << pingnum;
            string pingstr = pingtostr.str();

            //makes the ping settings stream
            pstr = "ping -n ";
            pstr += pingstr;
            pstr += " ";
            pstr += ip;
            pstr += ">ping.txt";


            cout << "The new settings will ping " << ip << "   " << pingstr << " times. Are you sure? (Y/N)" << endl;
            string ync;
            int yni;
            getline(cin, ync);
            yni = YesOrNo(ync);//yni--> 0=no, 1=yes, 2=error
            while (yni == 2)//ync is not yes or no
            {
                cout << "The new settings will ping " << ip << "   " << pingstr << " times. Are you sure? (Y/N)" << endl;
                cout << "You need to enter Y or N, YES or NO." << endl;
                getline(cin, ync);
                yni = YesOrNo(ync);
            }
            if (yni == 1)//ync is Y or yes 
            {
                string settingscheck;
                ofstream newsettings;
                newsettings.open("pingsettings.bat");
                newsettings << pstr << endl;//writes pstr into pingsettings.bat
                newsettings.close();
                system("CLS");
                cout << "Your new settings are done.\nThe reset command will be run automatically." << endl;
                run = "reset";//runs reset command
            }
            else
            {
                cout << "New settings has been aborted!" << endl;
                cout << "help to see commands" << endl;
                cout << "press enter to run ping" << endl;
                getline(cin, run);
                system("CLS");
            }

        }
        while (run == "timer")
        {

            if(timeron == false)
            {
                cout << "How many minutes do you want between pings?" << endl;
                cin >> time2;
                while (cin.fail() || time2 < 1 || time2 >240)
                {
                    system("CLS");
                    cin.clear();
                    cin.ignore(INT_MAX, '\n');
                    cout << "This needs to be an integer between 1 and 240!" << endl;
                    cout << "How many minutes do you want between pings?" << endl;
                    cin >> time2;
                }
                cin.clear();
                cin.ignore(INT_MAX, '\n');
                system("CLS");
                cout << "How many times do you want to repeat the ping?" << endl;
                cin >> repeat;
                while (cin.fail() || repeat < 1 || repeat >500)
                {
                    system("CLS");
                    cin.clear();
                    cin.ignore(INT_MAX, '\n');
                    cout << "This needs to be an integer between 1 and 500!" << endl;
                    cout << "How many times do you want to repeat the ping?" << endl;
                    cin >> repeat;

                }
                cin.clear();
                cin.ignore(INT_MAX, '\n');

                timeron = true;
            }
            if (timeron == true)
            {
                system("CLS");
                cout << "Pinging in " << time2 << " minutes. " << endl;
                cout << "There are " << repeat << " pings left to do." << endl;
                Sleep(time2*60000);
                if (repeat == 0)
                {
                    cout << "Help to see commands" << endl;
                    cout << "Press enter to run ping" << endl;

                    getline(cin, run);      //input from user
                    system("CLS");
                }
                else run = "";
                repeat--;
                system("CLS");
            }


            
            
        }
    }

    system("pause");
    return 0;
}
