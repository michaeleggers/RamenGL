#include <glad/glad.h>

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <vector>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_sdl3.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

#include "ramen/rgl_camera.h"
#include "ramen/rgl_defines.h"
#include "ramen/rgl_math.h"
#include "ramen/rgl_model.h"
#include "ramen/rgl_shader.h"

static SDL_Window*   g_pWindow;
static SDL_Renderer* g_pRenderer;
static SDL_GLContext g_glContext;

static std::vector<Vertex> g_Cone;
static std::vector<Vertex> g_Cap;
static GLuint              g_ConeVBO;
static GLuint              g_CapVBO;

void CreateGeometry()
{
    // Triangle Fan mit 18 Vertices anlegen
    // Die Spitze des Konus ist ein Vertex, den alle Triangles gemeinsam haben;
    // um einen Konus anstatt einen Kreis zu produzieren muss der Vertex einen positiven z-Wert haben
    g_Cone.push_back(Vertex{ .position = Vec3f{ 0.0f, 0.0f, 75.0f }, .normal = Vec3f{}, .color = Vec3f{ 0, 1, 0 } });
    // Kreise um den Mittelpunkt und spezifiziere Vertices entlang des Kreises
    // um einen Triangle_Fan zu erzeugen
    int iPivot = 1;
    for ( float angle = 0.0f; angle < (2.0f * RAMEN_PI); angle += (RAMEN_PI / 8.0f) )
    {
        // Berechne x und y Positionen des naechsten Vertex
        float x = 50.0f * sinf(angle);
        float y = 50.0f * cosf(angle);

        Vertex v{};

        // Alterniere die Farbe
        if ( (iPivot % 2) == 0 )
            v.color = Vec3f{ 0.235f, 0.235f, 0.235f };
        else
            v.color = Vec3f{ 0.0f, 0.6f, 1.0f };

        // Inkrementiere iPivot um die Farbe beim naechsten mal zu wechseln
        iPivot++;

        // Spezifiziere den naechsten Vertex des Triangle_Fans
        v.position = Vec3f{ x, y, 0.0f };
        g_Cone.push_back(v);
    }

    // Fertig mit dem Konus
    /*
    // Erzeuge einen weiteren Triangle_Fan um den Boden zu bedecken
    boden.Begin(GL_TRIANGLE_FAN, 18);
    // Das Zentrum des Triangle_Fans ist im Ursprung
    boden.Vertex3f(0.0f, 0.0f, 0.0f);
    for ( float angle = 0.0f; angle < (2.0f * GL_PI); angle += (GL_PI / 8.0f) )
    {
        // Berechne x und y Positionen des naechsten Vertex
        float x = 50.0f * sin(angle);
        float y = 50.0f * cos(angle);

        // Alterniere die Farbe
        if ( (iPivot % 2) == 0 )
            boden.Color4f(1, 0.8, 0.2, 1);
        else
            boden.Color4f(0, 0.8, 0, 1);

        // Inkrementiere iPivot um die Farbe beim naechsten mal zu wechseln
        iPivot++;

        // Spezifiziere den naechsten Vertex des Triangle_Fans
        boden.Vertex3f(x, y, 0);
    }

    // Fertig mit dem Bodens
    boden.End();
    */

    glCreateBuffers(1, &g_ConeVBO);
    glNamedBufferData(g_ConeVBO, g_Cone.size() * sizeof(Vertex), g_Cone.data(), GL_STATIC_DRAW);
}

int main(int argc, char** argv)
{
    Mat4f M1    = Mat4f::Identity();
    Mat4f M2    = Mat4f::Identity();
    Mat4f M1xM2 = M1 * M2;
    printf("M1xM2:\n%s\n", M1xM2.ToString());
    Vec4f v4    = Vec4f{ 1, 2, 3, 4 };
    Vec4f M1xV4 = M1 * v4;
    printf("M1xV4: \n%s\n", M1xV4.ToString());

    if ( !SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) )
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not initialize SDL. Reason: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    g_pWindow = SDL_CreateWindow("Task01", 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if ( !g_pWindow )
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window. Reason: %s\n", SDL_GetError());
        exit(1);
    }

    g_glContext = SDL_GL_CreateContext(g_pWindow);
    if ( !g_glContext )
    {
        SDL_LogError(
            SDL_LOG_CATEGORY_ERROR, "Could not create OpenGL context for window. Reason: %s\n", SDL_GetError());
        exit(1);
    }

    if ( !gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress) )
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not load OpenGL extensions via GLAD.");
        exit(1);
    }

    if ( !SDL_GL_MakeCurrent(g_pWindow, g_glContext) )
    {
        SDL_LogError(
            SDL_LOG_CATEGORY_ERROR, "Could not create OpenGL context for window. Reason: %s\n", SDL_GetError());
        exit(1);
    }

    /* Print GL version */
    printf("Got OpenGL Version: %s\n", glGetString(GL_VERSION));
    printf("Got OpenGL Renderer: %s\n", glGetString(GL_RENDERER));

    /* Init Dear ImGUI */
    /* ImGUI context */
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

    /* Setup Dear ImGui style */
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    /* ImGUI scaling */
    float       mainScale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
    ImGuiStyle& style     = ImGui::GetStyle();
    style.ScaleAllSizes(
        mainScale); // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
    style.FontScaleDpi
        = mainScale; // Set initial font scale. (in docking branch: using io.ConfigDpiScaleFonts=true automatically overrides this for every window depending on the current monitor)

    /* Setup Platform/Renderer backends */
    ImGui_ImplSDL3_InitForOpenGL(g_pWindow, g_glContext);
    ImGui_ImplOpenGL3_Init("#version 460");

    /* Load shaders. */
    Shader shader{};
    if ( !shader.Load("./shaders/task01.vert", "./shaders/task01.frag") )
    {
        fprintf(stderr, "Could not load shader.\n");
    }

    /* Create a model on GPU */
    Model model{};
    if ( !model.Load("./models/stormtrooper.obj") )
    {
        fprintf(stderr, "Could not load model file.\n");
    }

    /* Create Cylinder and upload to GPU */
    CreateGeometry();

    /* Create camera */
    Camera camera(Vec3f{ 0.0f, 0.0f, 30.0f }, Vec3f{ 0.0f, 0.0f, 0.0f });
    camera.RotateAroundUp(-20.0f);

    /* Model mat*/
    Mat4f modelMat = Scale(Vec3f{ 3.0f, 3.0f, 3.0f });
    Translate(modelMat, Vec3f{ 0.0f, 0.0f, -20.0f }); //Mat4f::Identity();

#if 0
    printf("viewMat: \n%s\n", viewMat.ToString());
    const float* viewMatData = viewMat.Data();
    printf("viewMatData: %f\n", viewMatData[ 0 ]);
    printf("viewMatData: %f\n", viewMatData[ 1 ]);
    printf("viewMatData: %f\n", viewMatData[ 2 ]);
    printf("viewMatData: %f\n", viewMatData[ 3 ]);
    printf("viewMatData: %f\n", viewMatData[ 4 ]);
    printf("viewMatData: %f\n", viewMatData[ 5 ]);
    printf("viewMatData: %f\n", viewMatData[ 6 ]);
    printf("viewMatData: %f\n", viewMatData[ 7 ]);
    printf("viewMatData: %f\n", viewMatData[ 8 ]);
    printf("viewMatData: %f\n", viewMatData[ 9 ]);
    printf("viewMatData: %f\n", viewMatData[ 10 ]);
    printf("viewMatData: %f\n", viewMatData[ 11 ]);
    printf("viewMatData: %f\n", viewMatData[ 12 ]);
    printf("viewMatData: %f\n", viewMatData[ 13 ]);
    printf("viewMatData: %f\n", viewMatData[ 14 ]);
    printf("viewMatData: %f\n", viewMatData[ 15 ]);
#endif

    /* VAO. */
    GLuint VAO;
    glCreateVertexArrays(1, &VAO);
    glVertexArrayVertexBuffer(VAO, 0, model.GetBuffer(), 0, sizeof(Vertex));
    glVertexArrayAttribBinding(VAO, 0, 0);
    glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexArrayAttrib(VAO, 0);

    /* Some global GL states */
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    // glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    /* Main loop */
    bool isRunning = true;
    SDL_GL_SetSwapInterval(1); /* 1 = VSync enabled; 0 = VSync disabled */
    glPointSize(20.0f);        // TODO: Delete later.
    while ( isRunning )
    {
        SDL_Event e;
        while ( SDL_PollEvent(&e) )
        {
            ImGui_ImplSDL3_ProcessEvent(&e);

            if ( e.type == SDL_EVENT_QUIT )
            {
                isRunning = false;
            }

            if ( e.type == SDL_EVENT_KEY_DOWN )
            {
                switch ( e.key.key )
                {
                case SDLK_ESCAPE:
                {
                    isRunning = false;
                }
                break;

                default:
                {
                }
                }
            }
        }

        /* Query new frame dimensions */
        int windowWidth, windowHeight;
        SDL_GetWindowSize(g_pWindow, &windowWidth, &windowHeight);

        /* Adjust viewport and perspective projection accordingly. */
        glViewport(0, 0, windowWidth, windowHeight);

        /* View mat */
        Mat4f viewMat = LookAt(
            camera.GetPosition(), camera.GetPosition() + camera.GetForward(), camera.GetUp()); // Mat4f::Identity();

        /* Projection mat */
        float aspect  = (float)windowWidth / (float)windowHeight;
        Mat4f projMat = PerspectiveProjection(TO_RAD(70.0f), aspect, 0.01f, 500.0f);

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        ImGui::ShowDemoWindow();

        /* ImGUI Rendering */
        ImGui::Render();

        /* Rendering */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(1.0f, 0.95f, 0.0f, 1.0f);

        shader.Use();
        glBindVertexArray(VAO);
        glUniformMatrix4fv(0, 1, GL_FALSE, modelMat.Data());
        glUniformMatrix4fv(1, 1, GL_FALSE, viewMat.Data());
        glUniformMatrix4fv(2, 1, GL_FALSE, projMat.Data());
        glDrawArrays(GL_TRIANGLES, 0, model.NumVertices());

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(g_pWindow);
    }

    /* GL Resources shutdown. */
    shader.Delete();
    glDeleteVertexArrays(1, &VAO);

    /* ImGUI Shutdown */
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
    /* SDL Shutdown */
    SDL_GL_DestroyContext(g_glContext);
    SDL_DestroyWindow(g_pWindow);
    SDL_Quit();

    return 0;
}
