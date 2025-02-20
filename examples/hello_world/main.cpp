//
//
//    Welcome in Cherry,
//    In this example, we will see a really simple example to do your first build.
//    🍒 Cherry is a complete & modern UI/UX framework for low level native apps. 
//    Minimal, but complete & advanced. Including backends, a render engine & ui 
//    components(ImGui/Vulkan/SDL) 
//
//    What we want to do for this hello world ? 
//    For this example, we will do a really simple window with components in it. 
//    With a simple render function.
//
//


// This is the single include, this file inclue all of the Cherry framework,
// just include it, and start simply !
#include "../../cherry.hpp"
#include "../../platform/engine/components.hpp"
#include "../../platform/engine/app.hpp"

static std::vector<std::shared_ptr<Cherry::Component>> m_TestComponents;

// The main Render function ! For this sample example, we will put all of our
// components in it.
void Render()
{
  // This is how we call simple inline component. We can handle it here
  // but not really outside, this is the most simple approach. This component
  // is anonymous, not object oriented and cannot be implemnted outside 
  // this simple call.

  Cherry::AddOneTimeProperty("color_border", "#B1FF31FF");
  if(CherryKit::ButtonTextDropdown("My first button"))
  {
    // When the button is clicked, we can add condition and handle it.
    std::cout << "\"My first button\" pressed !!!" << std::endl;
  }

  if(CherryKit::ButtonTextDropdown("My second button"))
  {
    // When the button is clicked, we can add condition and handle it.
    std::cout << "\"My second button\" pressed !!!" << std::endl;
  }


  CherryKit::TitleOne(CherryID("title"),"Test");
  CherryKit::TitleFour(CherryID("titgle"),"Test");
  CherryKit::TextSimple("Hello incredible world");
  CherryKit::TextLeft("Hello incredible world");
  CherryKit::TextRight("Hello incredible world");


  // Cherry?::AddPadding(7.0f);
  ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7.0f);


  CherryKit::TextWrapped("Hello incredible worlello incredible worello incredible worello incredible worello incredible worello incredible worello incredible worello incredible word");

  // Cherry?::AddPadding(7.0f);
  ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7.0f);

  static int valeurTEst = 5;
  static std::string valeurphrase = "Le wagon";

  static float x = 2.5f;
  static float y = 2.5f;
  static float z = 5.5f;
  static float w = 5.5f;

  static double doubleprecisionnumber = 5.5;

  Cherry::AddOneTimeProperty("step", "8");
  CherryKit::KeyValInteger(CherryID("TEst"),"Valeur TEst", &valeurTEst);

  CherryKit::TableSimple("Test", {
    Cherry::Application::GetComponent(CherryID("TEst")),
    CherryKit::KeyValComboString("11", {"Test 1", "Test2", "Test2kl"}, 0),
    CherryKit::KeyValString("La phrase de TEst", &valeurphrase),
    CherryKit::KeyValFloat("Float input", &x),
    CherryKit::KeyValDouble("Double input", &doubleprecisionnumber),
    CherryKit::KeyValVector1("11", &x),
    CherryKit::KeyValVector2("22", &x, &y),
    CherryKit::KeyValVector3("La phrase de TEst", &x, &y, &z),
    CherryKit::KeyValVector4("La LMe super Vector4 olann", &x, &y, &z, &w)
    });

CherryKit::HeaderText("456", [](){
  CherryKit::TextSimple("Hello incredible world");
});

CherryKit::HeaderTextButton("3123", Cherry::GetPath("resources/base/y.png"),[](){
  CherryKit::TextSimple("Hello incredible world 2 ");
});

CherryKit::HeaderImageTextButton("456", Cherry::GetPath("resources/base/w.png"),[](){
  CherryKit::TextSimple("Hello incredible world 2 ");
});

CherryKit::HeaderImageText("0456", Cherry::GetPath("resources/base/w.png"),[](){
  CherryKit::TextSimple("Hello incredible world 2 ");
});



    CherryKit::ButtonTextDropdown(CherryID(&m_TestComponents, "super_button"),"My first button");

CherryKit::ComboString("Test", {"Suepr Nombre 1", "Test 2", "Test 3"});

    // Cherry::CreateStaticRoutine
    static std::once_flag flag;
        std::call_once(flag, [&](){
        std::thread([&]() {
          while(true)
          {
    // Cherry::SetComponentProperty(ID, key, val)
            Cherry::Application::Get().GetComponent(CherryID("title"))->SetProperty("label", "OK"); 
            Cherry::Application::Get().GetComponent(CherryID("title"))->SetProperty("label", "Super !");
          }
        }).detach();
    });
    
  // This is how we call registered component. This text button can be 
  // initialized with a "CherryID" that we can use to get data, update
  // properties and add a lot of logics here, and outside this function! 
  // This approach will create a component registered in the Cherry Render 
  // engine.

}

// This is the entry point of Cherry. We need to add it to add the main
// configurations, default parameters dans all the modes we want
// in our application. We will do here the minimal example. But we can
// bootstrapp more advanced features in this application, like multicontextes,
// docking window, load-from-save, etc !!
Cherry::Application *Cherry::CreateApplication(int argc, char **argv)
{
  // Firstly, let's create a specification configuration, then, we can
  // configure the general behaviors, names of window,
  // sizes, scales, default themes, fonts, locales etc... 
  Cherry::ApplicationSpecification config;
  config.SetName("Hello Cherry");     // The name of the window
  config.SetMinimumWidth(250);        // The minimum width
  config.SetMinimumHeight(250);       // The minimum height
  config.SetDefaultWidth(400);        // The default width
  config.SetDefaultHeight(300);       // The default height
  config.SetGlobalScale(0.84f);       // The flobal scale of components and windows
  config.SetFontGlobalScale(0.84f);   // The flobal scale of fonts
  config.SetFavIconPath(Cherry::GetPath("resources/imgs/icon.png")); // Icon of the window. (for folder in [project]/assets)

  // Render mode
  // We can specify here the main behavior of the UI app (we can chose 
  // for docking, simple render of subwindow approach)
  // Here, we need a simple render, so let's select SimpleRender.
  config.SetRenderMode(WindowRenderingMethod::SimpleRender);

  // Now, let's create the UI app, and attach the specification.
  Cherry::Application *app = new Cherry::Application(config);

  // We previously choose for a SimpleRender, we need to attach a render 
  // fonction to the "MainRenderCallback". We put here the previous "Render"
  // function we made with all of our UI components !
  app->SetMainRenderCallback([](){ Render(); });

  return app;
}

// Main function of our executable
int main(int argc, char *argv[])
{
  // Starting the UI app on a different thread (simple approach of managing backends and frontend)
  std::thread([&](){ Cherry::Main(argc, argv); }).detach();

  // Sample loop of a programm
  while (true)
  {
    // If the UI app turn g_ApplicationRunning off, it's seem the user closed it.
    // So, when the UI is off, we want to close the main program correctly 
    if (!g_ApplicationRunning)
    {
      return 0;
    }

    // If Cherry not loaded all UI components and resources, we don't want
    // to access to empty or null components
    if (!Cherry::IsReady())
    {
      continue;
    }

    // In this example we retrieve our previous button object "button.test"
    // We want to check if the button is currently pressed.
   // std::cout << Cherry::GetData(CherryID("button.test"), "isPressed") << std::endl;

  
    /* Your program loop... */
  } 
}