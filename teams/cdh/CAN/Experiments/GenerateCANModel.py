import datetime
import calendar

def main():
    f=open("CANModelTemplate.c", "r")
    date = datetime.datetime.now()
    header = '''//////////////////////////////////////////////////////////////////////
//DO NOT MODIFY THIS FILE DIRECTLY
//Instead, change DubSat1/teams/cdh/CAN/Experiments/CANModelTemplate.*
//////////////////////////////////////////////////////////////////////

/*
* canwrap.c
*
*  Created on: ''' +  calendar.month_abbr[date.month] + " " + str(date.day) + ", " + str(date.year) + '''
*      Author: Nathan Wacker
*/'''
    templateString = header + f.read();
    #print(templateString)
    print(header)

    startingString = "some text"
    print("startingString: " + startingString);
    startingString = startingString.replace("some", "replaced");
    print("new String: " + startingString);

main()
