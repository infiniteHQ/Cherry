future usages :

(using CherryKit = Cherry::Kit;)

//////////////////////////////////////////////

// Simple and anonymous approach

CherryKit::Button("SampleButton"); 

Summary : 
    - Anonymous button.
    - Cannot access to the button properties.


//////////////////////////////////////////////

// Advanced and identified approach

CherryKit::Button(Cherry::Identifier("SuperButton"), "SampleButton"); 

Summary :
    - Identified button.
    - Can have access to the button properties (IsCliked, LastCliked, BackgroundColor, etc...)
    - Can dynamicly modify the button properties (Change a color, a logo or other)

//////////////////////////////////////////////

Cherry::GetComponent(Cherry::Identifier("SuperButton")).SetProperty(key, val);
Cherry::GetComponent(Cherry::Identifier("SuperButton")).GetProperty(key);

Todo : 
The Cherry Component Stack
 