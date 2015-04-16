#include "Process.h"
#include <iostream>
#include <vector>
#include <sys/sysinfo.h>
#include <linux/kernel.h>
#include <string>
#include <iomanip> 

const long KB = 1000;
const long MB = 1000000;
const long GB = 1000000000;
const long TB = 1000000000000;

void displayDashes(){ //displays dashes to the user
  std::cout << "---------------------------------------------------------------------"  << std::endl;
}

//displays the bytes with the proper units behind them
void displayBytes(long val){  
  if(val / TB > 0){
    std::cout << val/ TB << " TB"; 
  }else if (val / GB > 0){
    std::cout << val / GB << " MB";
  }else if (val / MB > 0){
    std::cout << val / MB << " MB";
  }else if (val / KB > 0){
    std::cout << val / KB << " KB";
  }else{
    std::cout << val << " bytes";
  }
}

//Displays the system information, including uptime, total RAM, and total memory
void displaySysInfo(){
  displayDashes();
  struct sysinfo si;
  sysinfo (&si);
  std::cout << std::fixed << std::setprecision(2);  //set precision
  std::cout << "Uptime: " << si.uptime / 3600 << ":" <<si.uptime % 3600 / 60 << ":" << si.uptime % 3600 % 60
	    << ". Load averages: 1 min=" << si.loads[0]/65536.0f <<"; 5 min=" << si.loads[1]/65536.0f
            << "; 15 min=" << si.loads[2]/65536.0f << "\nSystem memory : Total RAM: ";
  displayBytes(si.totalram); 
  std::cout << ", Free RAM: ";
  displayBytes(si.freeram); 
  std::cout << ", Shared RAM: ";
  displayBytes(si.sharedram);
  std::cout << "\n" <<  "Virtual memory: Total Swap: ";
  displayBytes(si.totalswap);
  std::cout <<  ", Free Swap: ";
  displayBytes(si.freeswap);
  std::cout << std::endl;
  displayDashes();
}

//Runs the ps command, then sorts it.  
//After that, displays the list of users running processes
//Acheives this by piping 
void runPS(){
    //processes to be run.
    Process ps = Process::create(true, "ps -feaL");
    Process sort = Process::create(true, "sort -k6 -n -r");
    std::vector<std::string> cutComm = {"cut","-d"," ","-f","1"};  //vecotor for commands so white space is accepted as a command.
    Process cut = Process::create(true, cutComm);
    Process sort2 = Process::create(true, "sort -u");
    ps.close(STDIN_FILENO);
    std::string line;
    getline(ps.getStdout(), line);
    std::cout << line << std::endl;
    while(getline(ps.getStdout(), line)){
      sort.getStdin() << line <<std::endl;
      cut.getStdin() << line <<std::endl;
    }
    sort.close(STDIN_FILENO);
    cut.close(STDIN_FILENO);
    while(getline(sort.getStdout(), line)){
      std::cout << line << std::endl;
    }
    displayDashes();
    //display usernames
    while(getline(cut.getStdout(),line)){
      sort2.getStdin() << line << std::endl;
    }
    sort2.close(STDIN_FILENO);
    std::cout << "Current users: ";
    while(getline(sort2.getStdout(), line)){
      std::cout << line << " ";
    }
    int exitCode;
    ps.wait(exitCode);
    sort.wait(exitCode);
    sort2.wait(exitCode);
    cut.wait(exitCode);
}

//runs the program
int main(){
  displaySysInfo();
  runPS();
}
