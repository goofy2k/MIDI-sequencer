I have an application that uses a class MyNotifier that captures events in the application and composes log messages based on the event data. By default it sends the log messages to std::cout, but the constructor of MyNotifier accepts a variable of type std::ostream& to overide this. I am trying to construct a class of that type which should send the logs to an different output channel, e.g. via an MQTT client. I have MQTT up and running well. My question is about the creation of the custom ostream class.

I found quite some info on output redirection, creation of streambuffers and ostream classes, but I did not manage to apply this succesfully yet

Here is the code that should use the new class (see the commented lines in app_main) and it's output when I use std::cout. For testing, the events are generated by calling MyNotifier::Notify directly. 


class MyNotifier {    
    public:
        //constructor
        MyNotifier(std::ostream& os = std::cout) : ost(os) {}
        //takes eventdata as a code for the event
        //composes some output string based on the input and outputs it to the customizable output stream ost
        virtual void Notify( unsigned long eventdata);
    protected:
        std::ostream& ost;        
}; //class MyNotifier

void MyNotifier::Notify(unsigned long eventdata) {
    //takes eventdata as dummy for an event
    //composes some output string based on the input and outputs it to the customizable output stream ost
    char s[200];
    int wr = sprintf(s, "RECEIVED EVENT %s ", "of type 1 ");
    sprintf( s + wr , "with number %lu\n", eventdata);
    std::cout << "MyNotifier::Notify" << s << std::endl;                     //this shows up
    ost << "dummy custom_ostream output: " << eventdata << std::endl;
    //trial to send over MQTT,  in the end ost should generate MQTT output
    esp_mqtt_client_publish(mqtt_client, "/fckx_seq/GUI", "value", 0, 1, 0); //works fine
} //MyNotifier::Notify


void app_main(void) {
    MyNotifier notifier;                    //instantiate with default output stream (std::cout)
    //MyNotifier notifier(std::cout);       //instantiate with default output stream explicitly, also works with the same result
    //MyNotifier notifier(custom_ostream)   //desired way to send logs over a Custom_ostream object
    notifier.Notify(3142727);  //notify some event
}    

Output to screen:

RECEIVED EVENT of type 1  with number 3142727

In my first step  I only customize the streambuf class (OutStreamBuf). It is used by a "plain" ostream class. 


    class OutStreamBuf : public std::streambuf {       
      protected:      
        /* central output function
         * - print characters in uppercase mode
        */          
        //override the existing overflow function with a function that converts each character to uppercase
        virtual int_type overflow (int_type c) {
            if (c != EOF) {
            // convert lowercase to uppercase
            c = std::toupper(static_cast<char>(c),getloc());
            //output to standard output
            putchar(c);

            }
            return c;
        }
   
        // write multiple characters  MUST USE CONST CHAR* ?
        virtual std::streamsize xsputn (char* s, std::streamsize num) {  
            std::cout << "**size: " << num << std::endl;    
            std::cout << "OutStreamBuf contents: " << s << std::endl;
            return 1;
        }    
    }; //OutStreamBuf




    OutStreamBuf outStreamBuf;                                    
    std::ostream custom_ostream(&outStreamBuf);
    MyNotifier notifier(custom_ostream);         //instantiate with customized output stream , 
    notifier.Notify(314132345);  //notify some event  
    custom_ostream << "Test << operator" << std::endl;

    Output:


MyNotifier::Notify direct: RECEIVED EVENT of type 1  with number 314132345
DUMMY CUSTOM_OSTREAM OUTPUT: 314132345    <------ THIS IS THE DESIRED OUTPUT
TEST << OPERATOR

The desired output is probably generated character by character by the overflow function in OutStreamBuf. As I have to get hold on the complete buffer to compose an MQTT message I thought that I have to get hold on the complete buffer, so I need a customized ostream object. In the second trial I therefore created a customized ostream class (OutStream) with an embedded customised streambuf class:

class OutStream : public std::ostream {
 
    private:
        //private local Outbuf for OutStream
        class Outbuf : public std::streambuf { 
      public:
   
      protected:
	/* central output function
	 * - print characters in uppercase mode
	 */     
     
        //override the existing overflow function with a function that converts each character to uppercase
        virtual int_type overflow (int_type c) {
            if (c != EOF) {
            // convert lowercase to uppercase
            c = std::toupper(static_cast<char>(c),getloc());
            //output to standard output
            putchar(c);

            }
            return c;
        }
   
        // write multiple characters  MUST USE CONST CHAR* (?)
        virtual std::streamsize xsputn (char* s, std::streamsize num) {  
            std::cout << "**size: " << num << std::endl;    
            std::cout << "OUTBUF contents: " << s << std::endl;
            return 1;
        }    
    }; //Outbuf

        Outbuf outbuf;
        std::streambuf * buf;
     public:
        //constructor
        OutStream() {
        //buf = this->rdbuf();  //compiles OK, but what does it do ?
         buf = rdbuf();         //compiles OK, but what does it do ?
         std::cout << "SOME MESSAGE FROM OutStream constructor" <<std::endl;                         //shows OK
         esp_mqtt_client_publish(mqtt_client, "/fckx_seq/GUI", "OutStream constructor", 0, 1, 0);    //shows OK
        };
        
        // << operator
        //https://www.geeksforgeeks.org/overloading-stream-insertion-operators-c/
        //have a good look on what parameters the operator should take , see the above article       
        friend std::ostream & operator << (std::ostream &stream, const OutStream& outStream){
            esp_mqtt_client_publish(mqtt_client, "/fckx_seq/GUI", "OutStream << operator", 0, 1, 0); //doesn't show
            stream << "Test << operator inside " << std::endl;                                       //doesn't show
            return stream; //return the stream              
        };     
}; //OutStream


    OutStream custom_ostream;   //using a composite ostream/streambuf object    
     
    MyNotifier notifier(custom_ostream);  //instantiate with customized output stream
    notifier.Notify(314132345);  //notify some event  
    custom_ostream << "Test << operator" << std::endl; 

This properly generates a log from it's constructor but it's customised output and that of it's << operator are not shown:

SOME MESSAGE FROM OutStream constructor
MyNotifier::Notify direct: RECEIVED EVENT of type 1  with number 314132345


I added the test on the << operator because of the missing response. As this also fails I think that something is wrong with the constructor of the ostream object and/or it's binding with the streambuf. I simply do no understand how to use this....











Now create a stream object custom_Out of class Custom_Out that is derived from std::ostream that can be passed a a parameter to notifier like:

MyNotifier notifier(Custom_Out);       //instantiate with custom output stream

111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111