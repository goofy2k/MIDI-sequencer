/******************************************************************************************
*flexUI.h

* IN DEVELOPMENT class FlexAPI   by FCKX
* inspired by JSONUI in the the Faust DSP libs
* 
* classes for a flexible UI, API and helpers
*
* flexible: 
*   - the UI can be defined by entering an object that contains a UI structure/tree
*   - the object can be entered as a C++ object or as a Json string
*   - the same object can be used to deply a GUI as it's stucture also reflects the structure of the GUI
*
* implementation:
*   - the class deploys the UI and thereby generates at least two ordered lists/vectors:
*               -uiPath (string)
*               -uiVal OR uiElement (for variable types of uiVal)
*   -these lists can be used to access (set or get) uiVal's or uiElement.val's by index in the list or
*    by uiPath
*   -the same lists can be used to define a list of API handlers
*   -the format of the uiPath is such that it can be used as a MQTT topic (or a part of it)for direct 
*    external control
*   -implementation will be done in steps, starting with hard coded lists and the API. Automatic deployment
*    of the lists via parsing of an input UI object will be done later
*             
*******************************************************************************************/

GUIelement_t enum {
    
    strentry,
    nentry,
    hslider,
    vslider,
    button
};


//vector with UIelement 's
//ElementVec

//vector with UIpath 's
//PathVec

struct UIelement {
    
    Valtype_t :  valType;  
    <valType> :  value;  //see if you also need initial value, this could be retrieved by calling a "setup" equivalent
    <valType> :  min;    //only if <valType> numeric
    <valType> :  max;    //only if <valType> numeric
    GUIelement_t : guiElement;
    //opschrift  //caption
    //flex HTML deployment parameters    
    
};




class FlexUI {
    
    public:
    //constructor
        // init empty vectors
    
    //destructor
    
    //initalize (UIobject)
    //initalize (UIjson)
    
    //getUIjson //output UIjson, e.g. before sending to GUI handling client
                //this can be used with object containing initial values or with actual status
    
    //getter helpers
    //getIdByPath
    //getPathById
    //getElementValueById
    //getElementById
    
    //setter helpers
    //setIdByPath
    //setPathById
    //setElementValueById
    //setElementById  


  
    private:
        //ElementVec
        //PathVec
    protected:
    
    
};