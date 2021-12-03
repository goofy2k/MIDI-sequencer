/***********************************************
*definitions to make NimBLE driver varaiables globally accessible
*see: https://stackoverflow.com/questions/19929681/c-global-variable-declaration/19929727
*answer by Wolf, but learn more!
*/

//extern int globaltestPointer = 456; //extern to make it globally accessible????

/*******************
*nimBLEdriver.h
********************/
class NimBLEGluer {
    
    public:
    //constructor
    //NimBLEGluer(); //causes a: ../main/main.cpp:564: undefined reference to `NimBLEGluer::setPointer(int)'
                   //in that line there is NO reference to this class at all!
    //destructor
    //virtual ~NimBLEGluer();  

    int testPointer = 123;  //initial value for testing
    /*
    void setPointer( int inp);
    int getPointer( void ){
        return testPointer;
        };
    */        
    }; 

//NimBLEGluer::NimBLEGluer() {}  //causes a: error: definition of implicitly-declared 'constexpr NimBLEGluer::NimBLEGluer()'

//NimBLEGluer::~NimBLEGluer() {}

extern NimBLEGluer NimBLEData;  //to make it globally accessible without intantiating it again ?? 
/***************************************************
// in main() :

// in driver.cpp :

****************************************************/

    //static int testPointer; //static to make it globally accessible ????
    //friend void printTestPointer(NimBLEGluer x);